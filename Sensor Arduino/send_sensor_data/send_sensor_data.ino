#include <XBee.h>
#include <AltSoftSerial.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

AltSoftSerial SoftSerial;
#define DebugSerial Serial
#define XBeeSerial SoftSerial

Adafruit_BME280 bme;

XBee xbee = XBee();

uint8_t payload[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x4150EC4B);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

void setup() {
  // put your setup code here, to run once:
  DebugSerial.begin(115200);
  XBeeSerial.begin(9600);
  xbee.begin(XBeeSerial);

  bool status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  String dataValues = getValues();

  for(int i=0; i<sizeof(payload); i++) {
    payload[i] = dataValues[i];
  }

  DebugSerial.println(dataValues);
  xbee.send(zbTx);
  delay(5000);
}

String getValues() {
    String data = "";
    char delimiter = ',';
    
    data += String(bme.readTemperature()) + delimiter;  // Measured in *C
    data += String(bme.readPressure()/100.0F) + delimiter; // Measured in hPa
    data += String(bme.readHumidity());  // Measured in %

    return data;
}
