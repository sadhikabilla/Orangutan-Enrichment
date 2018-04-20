#include <nRF905.h>

/*
 *      3.3v  VCC: Power Supply
 *         2  AM: Address matched
 *         3  DR: Rx and Tx Ready
 *         4  CD: Carrier Detect
 *         7  CE: Enable Tx and Rx
 *         8  PWR: Power up chip
 *         9  TXE: Mode Selecting
 *        10  CSN: SPI Enable
 *        11  MOSI: SPI Master arduinoOutput Slave Input
 *        12  MISO: SPI Master Input Slave arduinoOutput
 *        13  SCK: SPI Clock
 *       GND  GND: Ground
 *       GND  GND: Ground
 *            CLK: arduinoOutput clock
 */

#define PACKET_NONE 0
#define PACKET_OK   1
#define PACKET_INVALID  2

class nRF905Tranceiver {
  private:
    uint32_t rx_address;
    uint32_t tx_address;
    int ms_timeout;
    nRF905_pwr_t tx_power;
    nRF905_low_rx_t rx_power;
    String payload = "";
    volatile uint8_t packet_status;
    bool isDebug = true;
    
    typedef void (*EventCallback) (String);
    EventCallback onResponseCallback;
  public:
    nRF905Tranceiver(uint32_t rx_address, uint32_t tx_address, int ms_timeout = 100, nRF905_pwr_t tx_power = NRF905_PWR_10, nRF905_low_rx_t rx_power = NRF905_LOW_RX_ENABLE) :
      rx_address(rx_address),
      tx_address(tx_address),
      ms_timeout(ms_timeout),
      tx_power(tx_power),
      rx_power(rx_power) {}

    void onResponse(EventCallback callback) {
      onResponseCallback = callback;
    }
    
    void onComplete() {
      packet_status = PACKET_OK;
      nRF905_standby();
    }

    void debug() {
      isDebug = true;
    }
    
    void onInvalid() {
      packet_status = PACKET_INVALID;
      nRF905_standby();
    }
    
    void setup() {
      nRF905_init();
      nRF905_setListenAddress(rx_address);
      nRF905_setTransmitPower(tx_power);
      nRF905_setLowRxPower(rx_power);
      nRF905_RX();
      arduinoOutput("RECEIVER READY");
    }

    void send(String data) {
      payload = data;
    }

    void tryReceive() {
      arduinoOutput("----------");
      arduinoOutput("WAITING FOR PACKET...");
      while(packet_status == PACKET_NONE);

      if(packet_status != PACKET_OK) {
        arduinoOutput("INVALID PACKET");
        packet_status = PACKET_NONE;
        nRF905_RX();
      } else {
        packet_status = PACKET_NONE; 
        char packet[NRF905_MAX_PAYLOAD];
        nRF905_read(packet, sizeof(packet));
        arduinoOutput("GOT PACKET, SENDING ACKNOWLEDGEMENT...");
        int packetSize = sizeof(packet);
        char replyPacket[NRF905_MAX_PAYLOAD];
        String(packetSize).toCharArray(replyPacket, NRF905_MAX_PAYLOAD);
        while(!nRF905_TX(tx_address, replyPacket, sizeof(replyPacket), NRF905_NEXTMODE_RX));
        arduinoOutput("PACKET ACKNOWLEDGEMENT SENT");
        arduinoOutput("PACKET DATA: " + String(packet));
        onResponseCallback(packet);
      }
    }

    
    void trySend() {
      if(payload.length() <= 0) return;
      packet_status = PACKET_NONE;
      char packet[NRF905_MAX_PAYLOAD];
      payload.toCharArray(packet, NRF905_MAX_PAYLOAD);
      payload = payload.substring(NRF905_MAX_PAYLOAD);
      arduinoOutput("----------");
      arduinoOutput("SENDING DATA: " + String(packet));

      uint32_t startTime = millis();
      while(!nRF905_TX(tx_address, packet, sizeof(packet), NRF905_NEXTMODE_RX));
      arduinoOutput("DATA SENT, WAITING FOR ACKNOWLEDGEMENT...");
      
      uint32_t sendStartTime = millis();

      while(true) {
        if(packet_status != PACKET_NONE) {
          break;
        } else if (millis() - sendStartTime > ms_timeout) {
          break;
        }
      }

      if(packet_status == PACKET_NONE) {
        arduinoOutput("TIMED OUT");
        payload = packet + payload;
      } else if (packet_status == PACKET_INVALID) {
        arduinoOutput("INVALID PACKET");
        payload = packet + payload;
      } else {
        uint32_t totalTime = millis() - startTime;
        arduinoOutput("ACKNOWLEDGEMENT TIME: " + String(totalTime) + " ms");
        
        uint8_t acknowledgementData[NRF905_MAX_PAYLOAD];
        nRF905_read(acknowledgementData, sizeof(acknowledgementData));
        String acknowledgement = (char*)acknowledgementData;
        onResponseCallback(acknowledgement);
      }
      
      nRF905_powerDown();
    }
    void arduinoOutput(String string) {
      if(isDebug) Serial.println("[nRF905] " + string);
    }
};
