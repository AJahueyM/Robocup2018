#ifndef __TILEWALL_HEADER_
#define __TILEWALL_HEADER_
enum VisionVictim {
	None, 
	Harmed, 
	Stable,
	Unharmed
};

class TileWall {
public:
	TileWall(bool exists = false, bool heatVictim = false, VisionVictim visionVictim = None);
	bool exists();
	bool hasHeatVictim();
	VisionVictim getVisionVictim();
private:
	bool exists, heatVictim;
	VisionVictim visionVictim;
};

#endif