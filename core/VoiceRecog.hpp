#ifndef ROBOT_CORE_VOICERECOG_H_
#define ROBOT_CORE_VOICERECOG_H_

#include "../voice_recognition/voice_recog.hpp"

/* Wrapper class for Voice Recognition */
class Vrecog : public Voicerec {
	
	public:
		static Vrecog& getInstance(void) {
			static Vrecog singletron;
			return singletron;
		}
		//void Init();
		char* GetVoiceCommand();
		//VOID fINALIZE();
	private:
		Vrecog();
};


#endif	// ROBOT_CORE_VOICERECOG_H_

