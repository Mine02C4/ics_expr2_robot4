#include <iostream>
#include <string>
#include <julius/juliuslib.h>
#include "voice_recog.hpp"
//#include <iconv.h>
//#define STR_OUT 100
//typedef void (*FUNCTYPE1)(std::string);
FUNCTYPE1 fp1;
//Jconf *jconf;
//Recog *recog;
//static int ret;
static char * mine_jconf = {(char *)"dictation-kit-v4.2.3/mine.jconf"};
static char * fast_jconf = {(char *)"dictation-kit-v4.2.3/fast.jconf"};

int Voicerec::Init() {
  //Initialize
  jlog_set_output(NULL);
  /*
  if (argc == 1) {    // -Cオプションで指定する.jconfファイルは必要なので確認する
    fprintf(stderr, "Try `-help' for more information.\n");
    return -1;
    }*/

  // 指定した.jconfファイルから設定を読み込む
  jconf  = j_config_load_file_new(fast_jconf);
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
  int ret = j_recognize_stream(recog);
  if (ret == -1) return -1;
  return 0;
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
  //charconv *cc = new charconv;

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
      for(i=0;i<seqnum;i++) { // ワードの数だけ回す
	//cc->convert(winfo->woutput[seq[i]], str_out, (size_t)STR_OUT); // ワードをutf8に変換してstr_outに入れる
	fp1(winfo->woutput[seq[i]]);
      }
      printf("\n");
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
Voicerec::Voicerec() {
}
