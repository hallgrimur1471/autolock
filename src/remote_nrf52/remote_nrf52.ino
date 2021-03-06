#include <bluefruit.h>
#include "car.h"

#define CONNECTION_TIMEOUT (2000) // ms
#define TOGGLE_GRACETIME (3000) // ms

// Custom UUID used to differentiate this device.
// Use any online UUID generator to generate a valid UUID.
// Note that the byte order is reversed ... CUSTOM_UUID
// below corresponds to the follow value:
// df67ff1a-718f-11e7-8cf7-a6006ad3dba0
const uint8_t CUSTOM_UUID[] =
{
    0xA0, 0xDB, 0xD3, 0x6A, 0x00, 0xA6, 0xF7, 0x8C,
    0xE7, 0x11, 0x8F, 0x71, 0x1A, 0xFF, 0x67, 0xDF
};

/* struct with info about detected packet */
typedef struct packet_t
{
  uint8_t addr[6]; // six byte device address
  int8_t rssi; // RSSI value in [dBm]
  int32_t timestamp;
};

packet_t packet;
Car car;

int32_t last_message_time = 0;
int32_t time_since_last_message = 0;

BLEUuid uuid = BLEUuid(CUSTOM_UUID);

void setup() {
  Serial.begin(115200);

  /* Enable both peripheral and central modes */
  err_t err = Bluefruit.begin(true, true);
  if (err)
  {
    Serial.print("Unable to init Bluefruit (ERROR CODE: ");
    Serial.print(err);
    Serial.println(")");
    while(1)
    {
      digitalToggle(LED_RED);
      delay(100);
    }
  }

  // set transmit power in [dBm]
  // accepted values: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Remote");

  // *** start central scanning ***
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  // only invoke callback if signal strength greater than the specified rssi
  Bluefruit.Scanner.filterRssi(-80);
  // only invoke callback for certain uuid
  Bluefruit.Scanner.filterUuid(uuid);
  // interval = 100 ms, window = 50 ms
  Bluefruit.Scanner.setInterval(160, 80); // in units of 0.625 ms
  // request scan response data
  Bluefruit.Scanner.useActiveScan(true);
  Bluefruit.Scanner.start(0); // scan forever (no timeout)

  // RED off = not connected
  // RED on  = connected
  ledOff(LED_RED);
}

// loop has to be defined because feather is Arduino based
void loop() {
  time_since_last_message = millis() - last_message_time;

  if (car.isLocked()) {
    //Serial.println("Locked");
    //Serial.printf("Time since last message: %d\n", time_since_last_message);
    if (time_since_last_message < CONNECTION_TIMEOUT &&
        car.get_time_since_toggle() > TOGGLE_GRACETIME) {
      car.unlock();
    }
  }
  else { // car is unlocked
    //Serial.println("Unlocked");
    if (time_since_last_message >= CONNECTION_TIMEOUT &&
        car.get_time_since_toggle() > TOGGLE_GRACETIME) {
      car.lock();
    }
  }
  //delay(200);
}

void scan_callback(ble_gap_evt_adv_report_t* report) {

  // add report data to our packet struct
  memcpy(packet.addr, report->peer_addr.addr, 6);
  packet.rssi = report->rssi;
  packet.timestamp = millis(); // (approximately)
  last_message_time = packet.timestamp;

  Serial.printf("%d,%d\n", packet.timestamp, packet.rssi);
}