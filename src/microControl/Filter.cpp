#include "Filter.h"



Filter::Filter( float errorMeasurement, float estUncertainty, float variance){
	this->errorMeasurement = errorMeasurement;
	this->estUncertainty = estUncertainty;
	this->variance = variance;
	errEstimate = estUncertainty;
}
void Filter::setSource(float* source) {
	this->source = source;
	previousEst = *source;
}
float Filter::kalmanFilter() {
	double kGain = errEstimate / (errEstimate + errorMeasurement);
	double estimate = previousEst + kGain * (*source - previousEst);
	errEstimate = (1.0 - kGain) * errEstimate + fabs(previousEst - estimate)*variance;
	previousEst = estimate;
	previousSource = *source;
	return estimate;
}
Filter::~Filter()
{
}
