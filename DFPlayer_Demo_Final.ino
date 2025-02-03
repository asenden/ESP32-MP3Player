#include <DFPlayerMini_Fast.h>
#include "SoftwareSerial.h"

static const uint8_t PIN_MP3_TX = 26; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 27; // Connects to module's TX 
const int LED_PIN = 2;

// SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
EspSoftwareSerial::UART softwareSerial;

DFPlayerMini_Fast myDFPlayer;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // starting with LOW blue LED
  delay(5000);
  Serial.begin(115200);
  delay(1000);
  Serial.println("Hello world");
  delay(1000);
  softwareSerial.begin(9600, SWSERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX);
  delay(1000);
  if (!softwareSerial) { // If the object did not initialize, then its configuration is invalid
    Serial.println("Invalid EspSoftwareSerial pin configuration, check config"); 
    while (1) { // Don't continue with invalid configuration
      delay (1000);
    }
  }else{
    Serial.println("Software Serial started :-)");
  }
  
  Serial.println("...Start Player...");
  myDFPlayer.begin(softwareSerial, false);
  
  Serial.println("DFPlayer Instance initiated");
  delay(500);
  //myDFPlayer.reset();
  //delay(5000);
  //Serial.println("Player reset");
  myDFPlayer.flush();
  Serial.println("Serial Buffer flushed");
  delay(1500);

  myDFPlayer.pause();
  delay(100);
  myDFPlayer.volume(2);
  delay(100);
  myDFPlayer.play(1);
  delay(1500);
}

void loop() {
  static unsigned long timer = millis();
  if (millis() - timer > 5000) {
 
    if (myDFPlayer.isPlaying()){
      Serial.println("Playing MP3 Nr.: "+String(myDFPlayer.currentSdTrack())+" with Volume: "+String(myDFPlayer.currentVolume()));
      digitalWrite(LED_PIN, HIGH); // blue LED will obviously go HIGH with a delay due to delayed execution of this IF statement
    }else{
      Serial.println("Not Playing");
      digitalWrite(LED_PIN, LOW); // blue LED will obviously go LOW with a delay due to delayed execution of this IF statement
    }

    delay(100);

    timer = millis();
  }
}
