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
	Executer(Comms& comms);
	void runPath(Path path);
	void runCommand(Command commands);
private:
	Comms& comms;
};

#endif