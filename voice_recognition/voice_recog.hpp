#ifndef VOICE_RECOG_H_
#define VOICE_RECOG_H_
#include <iostream>
//jconfs
#define MINEJCONF 0
#define FASTJCONF 1

typedef void (*FUNCTYPE1)(std::string);

//Forward declaration
struct voicecode;
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
  int Init(); //Initialize julius and setup
  std::string Wait_One_Sentence(int seconds);  //if not recognize in arg, int seconds, return null  //else return one sentence.
  int Wait_One_Code(int seconds, struct voicecode & vc);
  void Register_Callback(FUNCTYPE1 f); //Register callback function returns result sentences.
  void Finalize();   //Finalize Julius
  int ChangeMode(int); //Change dictionaries and grammers //arg is jconfs. 
  std::string getString(void); //Get the last recognized sentence
private:
  Jconf *jconf;
  Recog *recog;
  void ReturnOneString_Instance(std::string s);
  int Convert_String_to_Code(std::string s, struct voicecode & vc);
  int Init(int filenum);
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
