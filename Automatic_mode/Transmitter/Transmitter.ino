/*Sends signal to the receiver to trigger the hose or feeder turn at regular time intervals. Currently the feeder is set to  trigger every 6 seconds*/
#include "nRF905Transceiver.cpp"
#include <ADCTouch.h>
#include <Time.h>

nRF905Tranceiver transmitter(0x000000F0,0x000000F1);

int ref0; 
int ref1;
int capThresholdval = 35; 
int movingAverage1 = 0;
int movingAverage0 = 0;
int value0;
int value1;
float currentTime = 0;
boolean check90 = false;
float timeDiff = 0; //Controls the amount of time the screw rotates for - currently set to 3 seconds. The value can be modified accordingly. 
float timeInterval = 6000; //The time interval between subsequent turns of the feeder. The time interval is currently set to 6 seconds and can be modified accordingly

void setup() {
  ref0 = ADCTouch.read(A0, 500);
  ref1 = ADCTouch.read(A2, 500);
  Serial.begin(9600);
  transmitter.setup();
  transmitter.onResponse(onResponse);

}

void loop() {

  value0 = ADCTouch.read(A0);
  value1 = ADCTouch.read(A2);
  value0 -= ref0;       //remove offset
  value1 -= ref1;       //remove offset

// Readjust the moving average 
if(value0 < (movingAverage0 + capThresholdval)){
  movingAverage0 = value0 * 0.5 + movingAverage0 * 0.5;
}

if(value1 < (movingAverage1 + capThresholdval)){
   movingAverage1 = value1 * 0.5 + movingAverage1 * 0.5;
}

if((value0 > (movingAverage0 + capThresholdval))  && (value1 > (movingAverage1 + capThresholdval)) ){
  
    /*If the value is above the treshold and the time interval between two consecutive turns of the feeder exceeds 6 seconds
    the feeder is triggered, else the hose is triggered*/  
    if(millis() >= (currentTime + timeInterval)){
      transmitter.send("TURN 90");
      check90 = true;
      currentTime = millis();
    }else if (check90 == false){      
    transmitter.send("TURN HOSE 120");

    }   
  }else {
    
     transmitter.send("TURN HOSE 0");

   }

   if(check90 == true){
    timeDiff = millis() - currentTime;
    if(timeDiff > 3000){
      transmitter.send("TURN 0"); 
      check90 = false;
      currentTime = millis();
      
    }
   }
   
  transmitter.trySend();

}

/* DO NOT MODIFY*/

void onResponse(String command) {

}

void NRF905_CB_RXCOMPLETE(void) {
  transmitter.onComplete();
  nRF905_standby();
}

void NRF905_CB_RXINVALID(void) {
  transmitter.onInvalid();
  nRF905_standby();
}

