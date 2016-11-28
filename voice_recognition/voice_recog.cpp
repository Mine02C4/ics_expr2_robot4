#include <iostream>
#include <string>
#include <julius/juliuslib.h>
#include <iconv.h>
#include "voice_recog.h"
#define STR_OUT 100
static FUNCTYPE1 fp1;
static Jconf *jconf;
static Recog *recog;
static int ret;
class charconv
{
 private:
  iconv_t ic;
 public:
  charconv();
  void convert(char *str_in, char *str_out, size_t str_out_size);
  ~charconv();
};

charconv::charconv()
{
  ic = iconv_open("UTF-8", "EUC-JP");
}
charconv::~charconv()
{
  iconv_close(ic);
}
void charconv::convert(char *str_in, char *str_out, size_t str_out_size)
{
  char *ptr_in  = str_in;
  char *ptr_out = str_out;
  size_t inbufsz = strlen(str_in)+1;
  size_t outbufsz = str_out_size-1;
  iconv(ic, &ptr_in, &inbufsz, &ptr_out, &outbufsz);
}
int vcrec_init(int argc, char *argv[]) {
  //Initialize
  jlog_set_output(NULL);
  if (argc == 1) {    // -Cオプションで指定する.jconfファイルは必要なので確認する
    fprintf(stderr, "Try `-help' for more information.\n");
    return -1;
  }

  // 指定した.jconfファイルから設定を読み込む
  jconf = j_config_load_args_new(argc, argv);
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
    printf("recognition init miss\n");
    /* error */
    return -1;
  }

  // 入力デバイスがちゃんと開けてるか確認
  switch(j_open_stream(recog, NULL)) {
  case 0:     /* succeeded */
    printf("input device ok\n");
    break;
  case -1:          /* error */
    fprintf(stderr, "error in input stream\n");
    return 0;
  case -2:      /* end of recognition process */
    fprintf(stderr, "failed to begin input stream\n");
    return 0;
  }
  callback_add(recog, CALLBACK_RESULT, output_result, NULL);
  ret = j_recognize_stream(recog);
  if (ret == -1) return -1;
  return 0;
}

void output_result(Recog * recog, void * dummy) {
  // use function pointer
  //and operate host program by callback function.
  charconv *cc = new charconv;
  printf("ok111\n");
  WORD_INFO *winfo;
  WORD_ID *seq;
  int seqnum;
  int n,i;
  Sentence *s;
  RecogProcess *r;
  char    str_out[STR_OUT];
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
	cc->convert(winfo->woutput[seq[i]], str_out, (size_t)STR_OUT); // ワードをutf8に変換してstr_outに入れる
	fp1(str_out);
      }
      printf("\n");
    }
  }
  delete cc;
  fflush(stdout);
}
void register_callback(FUNCTYPE1 f) {
  //Register callback
  fp1 = f;
}
void vcrec_finalize () {
  j_close_stream(recog);
  j_recog_free(recog);
}
