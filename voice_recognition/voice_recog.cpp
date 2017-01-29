#include "voice_recog.hpp"
#include "voicecode.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <ctime>
#include <cstring>
#include <utility>
#include <julius/juliuslib.h>

#ifdef MOD_MODE
const std::string dictation_prefix = "voice_recognition/dictation-kit-v4.2.3/";
#else
const std::string dictation_prefix = "dictation-kit-v4.2.3/";
#endif

// Global variable
FUNCTYPE1 fp1 = NULL;
const std::string mine_jconf = dictation_prefix + "mine.jconf";
const std::string fast_jconf = dictation_prefix + "fast.jconf";

int Voicerec::Init() {
  return Init(0);
}
int Voicerec::Init(int filenum) {
  //Initialize
  jlog_set_output(NULL);
  // 指定した.jconfファイルから設定を読み込む
switch(filenum) {
  case MINEJCONF:
    {
      char *cstr = new char[mine_jconf.size() + 1];
      strcpy(cstr, mine_jconf.c_str());
      jconf = j_config_load_file_new(cstr);
      delete cstr;
    }
    break;
  case FASTJCONF:
    {
      char *cstr = new char[fast_jconf.size() + 1];
      strcpy(cstr, fast_jconf.c_str());
      jconf =  j_config_load_file_new(cstr);
      delete cstr;
    }
    break;
 default:
   {
      char *cstr = new char[mine_jconf.size() + 1];
      strcpy(cstr, mine_jconf.c_str());
      jconf = j_config_load_file_new(cstr);
      delete cstr;
    }
   break;
 }
   if (jconf == NULL) {
    fprintf(stderr, "jconf load miss\n");
    return -1;
  }
  // 読み込んだjconfから認識器を作成する
  recog = j_create_instance_from_jconf(jconf);
  if (recog == NULL) {
    fprintf(stderr, "Error in startup\n");
    return -1;
  }
  if (j_adin_init(recog) == FALSE) {
    fprintf(stderr,"recognition init miss\n");
    /* error */
    return -1;
  }
  // 入力デバイスがちゃんと開けてるか確認
  switch(j_open_stream(recog, NULL)) {
  case 0:     /* succeeded */
    fprintf(stderr ,"input device ok\n");
    break;
  case -1:          /* error */
    fprintf(stderr, "error in input stream\n");
    return 0;
  case -2:      /* end of recognition process */
    fprintf(stderr, "failed to begin input stream\n");
    return 0;
  }
  callback_add(recog, CALLBACK_RESULT, Output_Result, NULL);
  auto th = std::thread([this] {
  int ret = j_recognize_stream(recog);
  if (ret == -1) return -1;
    });
  th.detach();
  return 0;
}
int Voicerec::ChangeMode(int status) {
  j_close_stream(recog);
  return Init(status);
}

void Voicerec::Output_Result(Recog * recog, void * dummy) {
  // use function pointer
  //and operate host program by callback function.
  WORD_INFO *winfo;
  WORD_ID *seq;
  int seqnum;
  int n,i;
  Sentence *s;
  RecogProcess *r;
  for(r=recog->process_list;r;r=r->next) {
    // 処理が生きてないときは飛ばす
    if (! r->live) continue;
    // 処理結果が得られない時も飛ばす
    if (r->result.status < 0) continue;
    // 全てのセンテンスをアウトプットする
    winfo = r->lm->winfo;
    for(n = 0; n < r->result.sentnum; n++) { // センテンスの数だけループ
      s = &(r->result.sent[n]);      // センテンスの構造体のアドレス入手
      seq = s->word;           // ワード（文を品詞レベルまで分解したもの）の集まりのIDを取得
      seqnum = s->word_num;        // ワードの数
      //printf("結果:", n+1);
      std::string tmp = "";
      for(i=0;i<seqnum;i++) {
	std::string word = winfo->woutput[seq[i]];
       // ワードの数だけ回す
	if (word != "silB" && word != "silE")
	  tmp += word;
        if (fp1 != NULL) fp1(winfo->woutput[seq[i]]);
	       //fp1(winfo->woutput[seq[i]]);
      }
      Voicerec::Return_One_String(tmp);
    }
  }
  fflush(stdout);
}
void Voicerec::Register_Callback(FUNCTYPE1 f) {
  //Register callback
  fp1 = f;
}
void Voicerec::Finalize () {
  j_close_stream(recog);
  j_recog_free(recog);
}
void Voicerec::ReturnOneString_Instance (std::string s) {
  result = s;
  flag = 1;
}
std::string Voicerec::Wait_One_Sentence(int seconds) {
  flag = 0;
  time_t t1, t2;
  t1 = time(NULL);
  t2 = time(NULL);
  while (flag == 0 && (int)(t2-t1) <= seconds) {
    t2 = time(NULL);
  }//waitcallback
  if (flag == 0) return "";
  else return Voicerec::getString();
}
std::string Voicerec::getString (void) {
  return result;
}
int Voicerec::Wait_One_Code(int seconds) {
  return Convert_String_to_Code(Wait_One_Sentence(seconds));
}
int Voicerec::Convert_String_to_Code(std::string s) {
  if (s == "前") return VC_CODE_FORWARD;
  else if (s == "後ろ")  return VC_CODE_BACK;
  else if (s == "右")    return VC_CODE_RIGHT;
  else if (s == "左")    return VC_CODE_LEFT;
  else if (s == "進め")  return VC_CODE_FORWARD;
  else if (s == "行け")  return VC_CODE_FORWARD;
  else if (s == "まわれ") return VC_CODE_ROTATE;   
  else if (s == "とまれ") return VC_CODE_STOP;
  else if (s == "さがれ") return VC_CODE_BACK;
  else if (s == "うて")   return VC_CODE_FIRE;
  else  return -1;
}
Voicerec::Voicerec() :flag(0), result("") {
}
