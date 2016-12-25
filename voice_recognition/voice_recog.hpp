#ifndef VOICE_RECOG_H_
#define VOICE_RECOG_H_

//jconfs
#define MINEJCONF 0
#define FASTJCONF 1

typedef void (*FUNCTYPE1)(std::string);

//Forward declaration
struct __Jconf__;
struct __Recog__;
typedef __Jconf__ Jconf;
typedef __Recog__ Recog;

class Voicerec
{
public:
  static Voicerec& getInstance() {
    static Voicerec singleton;
    return singleton;
  }
  void Register_Callback(FUNCTYPE1 f);
  int Init();
  void Finalize();
  int ChangeMode(int); //arg is jconfs.
private:
  Jconf *jconf;
  Recog *recog;
  static void Output_Result(Recog *recog, void * dummy);
  Voicerec();
};

#endif //VOICE_RECOG_H_
