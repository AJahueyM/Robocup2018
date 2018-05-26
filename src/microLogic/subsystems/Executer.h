#ifndef __EXECUTER_HEADER_
#define __EXECUTER_HEADER_
#include "Comms.h"

enum Command {
	Drive30cms,
	Drive15cms,
	/// TODO= DEFINE ALL OF THIS
};

class Executer {
public:
	static Executer &getInstance(Comms& comms) {
		static Executer singletonInstance(comms);
		return singletonInstance;
	}
	void runPath(Path path);
	void runCommand(Command commands);
private:
	Executer(Comms& comms);
	Executer &operator=(const Executer &);
	Comms& comms;
};

#endif