//Triggers the state of the screw motor to either HIGH or LOW
#include "nRF905Tranceiver.cpp"

nRF905Tranceiver receiver(0x000000F1, 0x000000F0);
int count = 0;
const int motorPin = 6;
float currentTime = 0;
int state = 0; //Indicates if the motor is turning or not (0 corresponds to LOW, 1 corresponds to HIGH) 


void setup() {
 Serial.begin(9600);
  receiver.setup();
  receiver.onResponse(onResponse);
  pinMode(motorPin, OUTPUT);
}

void onResponse(String command) {

    //Changes state based on signal from transmitter 
    if(command == "TURN 90"){
        state = HIGH;
    }else if (command == "TURN 0"){
        state = LOW;

    }
}

void loop() {
  receiver.tryReceive();
  digitalWrite(motorPin,state);
  

}

/* DO NOT MODIFY*/

void NRF905_CB_RXCOMPLETE(void) {
  receiver.onComplete();
}

void NRF905_CB_RXINVALID(void) {
  receiver.onInvalid();
}

