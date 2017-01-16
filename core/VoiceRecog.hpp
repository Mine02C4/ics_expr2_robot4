#ifndef ROBOT_CORE_VRECOG_H_
#define ROBOT_CORE_VRECOG_H_

/* Wrapper class for Voice Recognition */
class Vrecog : public Voicerec {
	
	public:
		static Vrecog& getInstance(void) {
			static Vrecog singletron;
			return singletron;
		}
		void Init();
		void GetVoiceCommand();
		void Finalize();
	private:
		Vrecog();
};
Vrecog::Vrecog() {
	// constructor
}

#endif	// ROBOT_CORE_VRECOG_H_
