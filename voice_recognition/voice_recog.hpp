#ifndef VOICE_RECOG_H_
#define VOICE_RECOG_H_
typedef void (*FUNCTYPE1)(std::string);

//init api
extern int vcrec_init(int argc, char *argv[]);


extern void register_callback(FUNCTYPE1 f); //TODO:コールバック登録　文字列を受け取る関数を登録

//Finalize
extern void vcrec_finalize (void);

#endif