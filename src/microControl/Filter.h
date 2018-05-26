#include <math.h>
#include <Arduino.h>
class Filter{
public:
	Filter(float errorMeasurement,float estUncertainty,float variance);
	float kalmanFilter();
	void setSource(float* source);
	~Filter();

private:
	float previousSource;
	float* source;
	float previousEst;
	float errorMeasurement;
	float estUncertainty;
	float variance;
	float errEstimate;
};