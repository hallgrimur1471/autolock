#include <bluefruit.h>
#include <ble_gap.h>

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

BLEUuid uuid = BLEUuid(CUSTOM_UUID);

void setup() {
  Serial.begin(115200);

  // initialize Bluefruit
  err_t err = Bluefruit.begin();

  // did we manage to initialize Bluefruit?
  if (err) {
    Serial.print("Unable to init Bluefruit (ERROR CODE: ");
    Serial.print(err);
    Serial.println(")");
    while (1) {
      digitalToggle(LED_RED);
      delay(100);
    }
  }

  // set transmit power in [dBm]
  // accepted values: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Car");

  // this flag means:
  // LE General Discoverable Mode, BR/EDR not supported.
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);

  // make Advertiser use Bluefruit's TX power
  Bluefruit.Advertising.addTxPower();

  // Add UUID to payload
  Bluefruit.Advertising.addUuid(uuid);

  // Adafruit suggested sending the name in scan response
  // ...but is it ever sent?
  Bluefruit.ScanResponse.addName();

  Bluefruit.Advertising.restartOnDisconnect(true);
  // pretty fast for now
  Bluefruit.Advertising.setInterval(32, 244);
  // decrease this later
  Bluefruit.Advertising.setFastTimeout(30);
  Bluefruit.Advertising.start();
}

// loop has to be defined because feather is Arduino based
void loop() {}