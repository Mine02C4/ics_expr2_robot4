#ifndef VOICE_RECOG_H_
#define VOICE_RECOG_H_
#include <iostream>
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
  //Initialize julius and setup
  int Init();
  //if not recognize in arg, int seconds, return null
  //else return one sentence.
  std::string Wait_One_Sentence(int seconds);
  //Register callback function returns result sentences.
  void Register_Callback(FUNCTYPE1 f);
  //Finalize Julius
  void Finalize();
  //Change dictionaries and grammers
  int ChangeMode(int); //arg is jconfs.
  //Get the last recognized sentence
  std::string getString(void);
private:
  Jconf *jconf;
  Recog *recog;
  void ReturnOneString_Instance(std::string s);
  static void Return_One_String(std::string s) {
    Voicerec::getInstance().ReturnOneString_Instance(s);
  }
  static void Output_Result(Recog *recog, void * dummy);
  int flag;
  std::string result;
protected:
  Voicerec();
};

#endif //VOICE_RECOG_H_
