/*Sends signal to the hose receiver. If the value is above the treshold (both the wires being touched)
a turn 360 message is sent and turn 0 otherwise.*/    
#include "nRF905Transceiver.cpp"
#include <ADCTouch.h>
#include <Time.h>

nRF905Tranceiver transmitter(0x000000F0,0x000000F1);

int ref0; 
int ref1;
int capThresholdval = 35; //This value can be readjusted depending on the sensitivity of the device 
int movingAverage1 = 0;
int movingAverage0 = 0;
int value0;
int value1;


void setup() {
  ref0 = ADCTouch.read(A0, 500);
  ref1 = ADCTouch.read(A2, 500);
  Serial.begin(9600);
  transmitter.setup();
  transmitter.onResponse(onResponse);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  value0 = ADCTouch.read(A0);
  value1 = ADCTouch.read(A2);
  value0 -= ref0;       //remove offset
  value1 -= ref1;       //remove offset

//Adjusting moving average
if(value0 < (movingAverage0 + capThresholdval)){
  movingAverage0 = value0 * 0.5 + movingAverage0 * 0.5;
}

if(value1 < (movingAverage1 + capThresholdval)){
   movingAverage1 = value1 * 0.5 + movingAverage1 * 0.5;
}

if((value0 > (movingAverage0 + capThresholdval))  && (value1 > (movingAverage1 + capThresholdval)) ){

    //If value above threshold, turn the hose
    Serial.println("TURN HOSE 360");
    transmitter.send("TURN HOSE 360");
 
  }else {

    //If value below threshold, do not turn the hose
    Serial.println("TURN HOSE 0");
    transmitter.send("TURN HOSE 0");
    
   }

   
  transmitter.trySend();

}

/* DO NOT MODIFY */
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

