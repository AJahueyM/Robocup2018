#include <Wire.h> 
#include <Arduino.h>
#include "subsystems/Dispenser.h"

Dispenser*servoDispenser;

void setup(){
  servoDispenser = new Dispenser(11,12);
}

void loop(){
  servoDispenser->dispenseDirection(right,2);

   delay(3000);

   servoDispenser->dispenseDirection(left,2);

   delay(3000);

}

// #include <i2cmaster.h>

// void setup(){
//     Serial.begin(9600);
//     Serial.println("Setup...");
//     Serial.println(PORTC4);
//     Serial.println(" ");
//     Serial.println(PORTC5);

//     i2c_init(); //Initialise the i2c bus
//     PORTC = (1 << PORTC4) | (1 << PORTC5);//enable pullups
// }

// void loop(){
//   Serial.println("entro");
//   int dev = 0x5A<<1;
//   int data_low = 0;
//   int data_high = 0;
//   int pec = 0;
  
//   i2c_start_wait(dev+I2C_WRITE);
//       Serial.println("PASO ANTES DE WRITE");

//   i2c_write(0x07);
//     Serial.println("PASO ANTES DE FUNCIONES");

//   // read
//   i2c_rep_start(dev+I2C_READ);
//   data_low = i2c_readAck(); //Read 1 byte and then send ack
//   Serial.println("PASO 1");
//   data_high = i2c_readAck(); //Read 1 byte and then send ack
//     Serial.println("PASO 1");

//   pec = i2c_readNak();
//   i2c_stop();
  
//   //This converts high and low bytes together and processes temperature, MSB is a error bit and is ignored for temps
//   double tempFactor = 0.02; // 0.02 degrees per LSB (measurement resolution of the MLX90614)
//   double tempData = 0x0000; // zero out the data
//   int frac; // data past the decimal point
  
//   // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
//   tempData = (double)(((data_high & 0x007F) << 8) + data_low);
//   tempData = (tempData * tempFactor)-0.01;
  
//   float celcius = tempData - 273.15;
//   float fahrenheit = (celcius*1.8) + 32;

//   Serial.print("Celcius: ");
//   Serial.println(celcius);

//   Serial.print("Fahrenheit: ");
//   Serial.println(fahrenheit);

//   delay(1000); // wait a second before printing again
// }




//   // // Written by Nick Gammon
//   // // February 2011

//   // #include <SPI.h>

//   // char buf [100];
//   // volatile byte pos;
//   // volatile bool process_it;

//   // void setup ()
//   // {
//   //   Serial.begin (115200);   // debugging
    
//   //   // turn on SPI in slave mode
//   //   SPCR |= bit (SPE);

//   //   // have to send on master in, *slave out*
//   //   pinMode(MISO, OUTPUT);
    
//   //   // get ready for an interrupt 
//   //   pos = 0;   // buffer empty
//   //   process_it = false;

//   //   // now turn on interrupts
//   //   SPI.attachInterrupt();

//   // }  // end of setup


//   // // SPI interrupt routine
//   // ISR (SPI_STC_vect)
//   // {
//   // byte c = SPDR;  // grab byte from SPI Data Register
    
//   //   // add to buffer if room
//   //   if (pos < (sizeof (buf) - 1))
//   //     buf [pos++] = c;
      
//   //   // example: newline means time to process buffer
//   //   if (c == '\n'){
//   //         process_it = true;
//   //         SPDR = 'p';
//   //   }
        
//   // }  // end of interrupt routine SPI_STC_vect

//   // // main loop - wait for flag set in interrupt routine
//   // void loop (void)
//   // {
//   //   if (process_it)
//   //     {
//   //     buf [pos] = 0;  
//   //     Serial.println (buf);
//   //     pos = 0;
//   //     process_it = false;
//   //     }  // end of flag set
      
//   // }  // end of loop