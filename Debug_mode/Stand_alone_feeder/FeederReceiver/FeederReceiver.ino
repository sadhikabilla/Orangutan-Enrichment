//Triggers the state of the screw motor to either HIGH or LOW
#include "nRF905Tranceiver.cpp"

nRF905Tranceiver receiver(0x000000F1, 0x000000F0);
int count = 0;
const int motorPin = 6;
float currentTime = 0;
//const int ledPin = 13;
int state = 0;



void setup() {
 Serial.begin(9600);
  receiver.setup();
  receiver.onResponse(onResponse);
  pinMode(motorPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void onResponse(String command) {

    if(command == "TURN 90"){
        //indicates screw is turning        
        state = HIGH;
        Serial.println("State is HIGH and screw is turning");
        digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level) to indicate screw is being turned
    }else if (command == "TURN 0"){
        //indicates screw is not turning
        state = LOW;
        Serial.println("State is LOW and screw is not turning");
        digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW to indicate screw is not being turned
    }
}

void loop() {
  receiver.tryReceive();
  digitalWrite(motorPin,state);
  

}

void NRF905_CB_RXCOMPLETE(void) {
  receiver.onComplete();
}

void NRF905_CB_RXINVALID(void) {
  receiver.onInvalid();
}

