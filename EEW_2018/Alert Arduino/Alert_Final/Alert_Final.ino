#include <XBee.h>
#include <AltSoftSerial.h>

AltSoftSerial SoftSerial;
#define DebugSerial Serial
#define XBeeSerial SoftSerial

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

LedAlert = 12;
LedSafe = 13;

void setup()
{
  // put your setup code here, to run once:
  pinMode(LedAlert, OUTPUT);
  pinMode(LedSafe, OUTPUT);
  
  DebugSerial.begin(115200);
  XBeeSerial.begin(9600);
  xbee.begin(XBeeSerial);
}

void loop()
{
  // put your main code here, to run repeatedly:
  xbee.readPacket();

  if (xbee.getResponse().isAvailable())
  {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
    {
      xbee.getResponse().getZBRxResponse(rx);
      if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED)
      {
        DebugSerial.println("Packet received");
      }
      else
      {
        DebugSerial.println("Packet received, but no acknowledgement");
      }
      for (int i=0; i<rx.getDataLength(); i++){
        DebugSerial.print(char(rx.getData(i)));
      }
      DebugSerial.println();
    }
    else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE)
    {
      xbee.getResponse().getModemStatusResponse(msr);
    }
  }
  else if (xbee.getResponse().isError())
  {
    DebugSerial.println("Error reading packet.");
  }
}



