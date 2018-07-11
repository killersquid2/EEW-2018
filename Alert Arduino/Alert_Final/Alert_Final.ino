#include <XBee.h>
#include <AltSoftSerial.h>

AltSoftSerial SoftSerial;
#define XBeeSerial SoftSerial

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

int LedAlertT = 0; // Red LED for Temperature 
int LedSafeT = 1; // Green LED for Temperature

int LedAlertH = 5; // Red LED for Humidity
int LedSafeH = 6; // Green LED for Humidity

int Buzzer = 10; // Buzzer

bool SafeT; // Whether Temperature at Safe Levels
bool SafeH; // Whether Humidity at Safe Levels


void setup()
{
  // put your setup code here, to run once:
  
  pinMode(LedAlertT, OUTPUT);
  pinMode(LedSafeT, OUTPUT);
  pinMode(LedAlertH, OUTPUT);
  pinMode(LedSafeH, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  
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
      
      if (rx.getData(0) == 'T')
      {
        if (rx.getData(1) == '0')
        { SafeT = true; }
        if (rx.getData(1) == '1')
        { SafeT = false; }
      }
      else if (rx.getData(0) == 'H')
      {
        if (rx.getData(1) == '0')
        { SafeH = true; }
        if (rx.getData(1) == '1')
        { SafeH = false; }
      }
    }
    else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE)
    {
      xbee.getResponse().getModemStatusResponse(msr);
    }
  }
   
  if (SafeT == true)
  {
    digitalWrite(LedSafeT, HIGH);
    if (SafeH == true)
    {
      digitalWrite(LedSafeH, HIGH);
      delay(200);
    }
    else
    {
      digitalWrite(LedSafeH, LOW);
      digitalWrite(LedAlertH, HIGH);
      play('B', 2);
      digitalWrite(LedAlertH, LOW);
      play('E', 2);
    }
  }
  else
  {
    digitalWrite(LedSafeT, LOW);
    digitalWrite(LedAlertT, HIGH);
    if (SafeH == true)
    {
      digitalWrite(LedSafeH, HIGH);
    }
    else
    {
      digitalWrite(LedSafeH, LOW);
      digitalWrite(LedAlertH, HIGH);
    }
    play('B', 2);
    digitalWrite(LedAlertT, LOW);
    digitalWrite(LedAlertH, LOW); 
    play('E', 2);  
  }

/*   if (SafeH == true)
   {
    digitalWrite(LedSafeH, HIGH);
   }
   else
   {
    digitalWrite(LedSafeH, LOW);
    digitalWrite(LedAlertH, HIGH);  
    delay(50);              
    digitalWrite(LedAlertH, LOW);   
    delay(50);

    play('B', 2);      
    play('E', 2);      
   }
   */
 
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
   delay(100);   //a little delay between the notes makes the song sound more natural
}




