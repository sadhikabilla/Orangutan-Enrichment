#include "tranceiver.cpp"
#include <Servo.h>

nRF905Tranceiver receiver(0x000000F1, 0x000000F0);
int count = 0;

#define SERVO_PIN 5
Servo valve;
int angle = 0;

void setup() {
  Serial.begin(115200);
  receiver.setup();
  receiver.onResponse(onResponse);
  valve.attach(SERVO_PIN);
}

void onResponse(String command) {

  if(command == "TURN HOSE 120"){
    angle = 120;
  }else if(command == "TURN HOSE 0"){
    angle = 0;
  }
}

void loop() {
  receiver.tryReceive();
  valve.write(angle);
}

void NRF905_CB_RXCOMPLETE(void) {
  receiver.onComplete();
}

void NRF905_CB_RXINVALID(void) {
  receiver.onInvalid();
}

