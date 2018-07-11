#include <XBee.h>
#include <AltSoftSerial.h>

AltSoftSerial SoftSerial;
#define DebugSerial Serial
#define XBeeSerial SoftSerial

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

int LedAlert = 12; // red led
int LedSafe = 13; // green led
int Buzzer = 10; 

bool Safe;


void setup()
{
  // put your setup code here, to run once:


  
  pinMode(LedAlert, OUTPUT);
  pinMode(LedSafe, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  
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
      for (int i=0; i<rx.getDataLength(); i++)
      {
        DebugSerial.print(char(rx.getData(i)));
      }
      if (rx.getData(0) == 'T')
      {
        if (rx.getData(1) == '0')
        { Safe = true; }
        if (rx.getData(1) == '1')
        { Safe = false; }
        
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

  
   if (Safe == true)
   {
    digitalWrite(LedSafe, HIGH);
   }
   if (Safe == false)
   {
    digitalWrite(LedSafe, LOW);
    digitalWrite(LedAlert, HIGH);  
    delay(50);              
    digitalWrite(LedAlert, LOW);   
    delay(50);

   play('B', 2);      
   play('E', 2);      

   }
   
 
}

void play(char note, int beats) {
  int numNotes = 2; 
  char notes[] = { 'E', 'B', ' '};
  int frequencies[] = {330, 495, 0};
  int currentFrequency = 0;
  int beatLength = 150;

  for (int i = 0; i < numNotes; i++)  // check each value in notes from 0 to 14
    {
      if (notes[i] == note)             // does the letter passed to the play function match the letter in the array?
      {
       currentFrequency = frequencies[i];   // Yes! Set the current frequency to match that note
     }
   }

  //play the frequency that matched our letter for the number of beats passed to the play function
   tone(Buzzer, currentFrequency, beats * beatLength);   
   delay(beats* beatLength);   //wait for the length of the tone so that it has time to play
   delay(50);   //a little delay between the notes makes the song sound more natural
}




