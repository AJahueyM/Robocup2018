#ifndef __COMMS_HEADER_
#define __COMMS_HEADER_
#include <SPI.h>

enum Chip {
	Mega, 
	Rasp
};

class Comms{
public:
	static Comms &getInstance() {
		static Comms singletonInstance;
		return singletonInstance;
	}
	char sendMessage(Chip target, String message);
	char pullMessage(Chip target);
private:
	Comms();
	Comms &operator=(const Comms &);
	const uint8_t MEGA_SS_PIN = 53;
	const uint8_t RASP_SS_PIN = 49;
	char response;
};
#endif