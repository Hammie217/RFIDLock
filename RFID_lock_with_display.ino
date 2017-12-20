#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
//SCL-A5
//SDA-A4
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 
byte hamish[4] ={54, 173, 127, 172 };

void setup()   {                
  Serial.begin(9600);
   pinMode(8,OUTPUT);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
   pinMode(8,OUTPUT);

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
 
}

int on=-1;
void loop() {

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been read
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  if (rfid.uid.uidByte[0] == hamish[0] || rfid.uid.uidByte[1] == hamish[1] || rfid.uid.uidByte[2] == hamish[2] || rfid.uid.uidByte[3] == hamish[3] ) {
       display.setCursor(0,0);
    display.println("Hello Hamish!");
    on *= -1;
  delay(500);
  digitalWrite(8,((on+1)/2));
  display.print("Relay ");
  if(on==1){
    display.println("ON");
  }
  else{
     display.println("OFF");
  }
  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();
    }
  else{
    display.setCursor(0,0);
     display.println("Unknown card: ");
    for (byte i = 0; i < 4; i++) {
      display.print(rfid.uid.uidByte[i]);
      display.print(" ");
  }
  display.display();
  delay(1000);
   display.clearDisplay();
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  
}


