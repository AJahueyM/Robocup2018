#pragma once

#define M_PI 3.1415926535897
#define  converterToRads 3.1415926535897 / 180
#define converterToDegree 180 / 3.1415926535897

class Point3D{
public:
	Point3D() { x = 0; y = 0; z = 0; }
	Point3D(double x, double y, double z) { this->x = x; this->y = y; this->z = z; }
	
	void setX(double x) { this->x = x; }
	double getX() { return this->x; }

	void setY(double y) { this->y = y; }
	double getY() { return this->y; }

	void setZ(double z) { this->z = z; }
	double getZ() { return this->z; }

	Point3D operator+(Point3D v2) {
		Point3D buffer(this->x, this->y, this->z);
		double xSum, ySum, zSum;
		xSum = buffer.x + v2.x;
		ySum = buffer.y + v2.y;
		zSum = buffer.z + v2.z;

		buffer.setX(xSum);
		buffer.setY(ySum);
		buffer.setZ(zSum);

		return buffer;
	}

	friend Point3D operator+=(Point3D &v1, Point3D v2) {
		v1 = v1 + v2;
		return v1;
	}

	Point3D operator-(Point3D v2) {
		Point3D buffer(this->x, this->y, this->z);
		double xSub, ySub, zSub;
		xSub = buffer.x - v2.x;
		ySub = buffer.y - v2.y;
		zSub = buffer.z - v2.z;

		buffer.setX(xSub);
		buffer.setY(ySub);
		buffer.setZ(zSub);

		return buffer;

	}

	double getMagnitudeFromCoordinates() {
		double xMag = pow(this->x, 2), yMag = pow(this->y, 2), zMag = pow(this->z, 2);
		double sumMags = xMag + yMag + zMag;
		sumMags = sqrt(sumMags);
		return sumMags;
	}

	double getPhi() {
		double Pz = this->z,  P = this->getMagnitudeFromCoordinates(), param = Pz / P;
		double result = acos(param) * (converterToDegree);
		return result;
	}

	//double getAlpha() {
	//	// Alpha is the angle between x axis and the vector itself
	//	double Px = this->x, P = this->getMagnitudeFromCoordinates(), param = Px / P;
	//	double result = acos(param) * (converterToDegree);
	//	return result;
	//}

	double getTheta() {
		// Phi is the angle between x axis and the projection of the vector
		double Px = this->x, Pproy = sqrt(pow(this->x,2) + pow(this->y,2)), param = Px/Pproy;
		double result = acos(param) * (converterToDegree);
		return result;
	}

	//double getBeta() {
	//	// Beta is the angle between y axis and the the vector (kind of height)
	//	double Py = this->y, P = this->getMagnitudeFromCoordinates(), param = Py / P;
	//	double result = acos(param) * (converterToDegree);
	//	return result;
	//}

	static Point3D getVector(double magnitude, double pitch, double yaw) {
		double phi = 90 - pitch;  
		double y = magnitude * sin(phi * converterToRads) * sin(yaw * converterToRads);
		double x = magnitude * sin(phi * converterToRads) * cos(yaw * converterToRads);
		double z = magnitude * cos(phi * converterToRads);

		Point3D result(x, y, z);
		return result;
	}

	static Point3D addAndGetVector(double magInitial, double pitchInitial, double yawInitial, double magFinal,  double pitchFinal, double yawFinal) {

		Point3D result = getVector(magInitial, pitchInitial, yawInitial) + getVector(magFinal, pitchFinal, yawFinal);
		return result;
	}

private:
	double x, y, z;
};