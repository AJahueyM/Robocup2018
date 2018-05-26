#ifndef __TILEBUMP_HEADER_
#define __TILEBUMP_HEADER_

class TileBump {
public:
	TileBump(double minIncline = 0.0, double maxIncline = 0.0);
	double getWeight();
private:
	double minIncline, maxIncline, weight;
};

#endif