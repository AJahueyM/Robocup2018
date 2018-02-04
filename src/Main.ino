#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

using namespace std;

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup(){

	Serial.begin(9600);
	Serial.println("INICIAR PROG");

	if(!bno.begin()){
		Serial.print("NO BNO 055 DETECTED");
		while(1);
		}

	delay(5000);

	for(int i = 0; i < 6; )

	bno.setExtCrystalUse(true);
}


void loop(void){

	sensors_event_t event;
	bno.getEvent(&event);
	
	Serial.print("X ");
	Serial.print(event.orientation.x, 4);
	Serial.print("\tY ");
	Serial.print(event.orientation.y, 4);
	Serial.print("\tZ ");
	Serial.print(event.orientation.z, 4);
	Serial.println("");

	delay(100);

}

