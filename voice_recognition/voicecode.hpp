#ifndef VOICE_RECOGNITION_VOICECODE_H_
#define VOICE_RECOGNITION_VOICECODE_H_
//move
struct voicecode{
	int code;
	int num;
};

#define VC_CODE_UNDEFINED -1
#define VC_CODE_FORWARD 2
#define VC_CODE_BACK 5
#define VC_CODE_RIGHT 8
#define VC_CODE_LEFT 11
#define VC_CODE_ROTATE 13
#define VC_CODE_STOP 14
//cannon
#define VC_CODE_CANNON_UP 15
#define VC_CODE_CANNON_DOWN 16
#define VC_CODE_CANNON_RIGHT 17
#define VC_CODE_CANNON_LEFT 18
//one word
#define VC_CODE_HOUTOU 12

#define VC_CODE_MODECHANGE 19
#define VC_CODE_UCHIKATA 20


#define VC_CODE_FIRE 21

#endif  // VOICE_RECOGNITION_VOICECODE_H_
