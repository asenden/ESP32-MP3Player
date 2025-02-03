#include "DFPlayerMini_Fast.h"
#include "SoftwareSerial.h"

// SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
EspSoftwareSerial::UART softwareSerial;
DFPlayerMini_Fast myDFPlayer;

static const uint8_t PIN_MP3_TX = 26; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 27; // Connects to module's TX 
const int LED_PIN = 2;

  String line;
  char command;
  int pausing = 0;
  int repeat = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // starting with LOW blue LED
  delay(5000);
  Serial.begin(57600);
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

/*
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
*/

void loop() {

  // Waits for data entry via serial
  while (Serial.available() > 0) {
    command = Serial.peek();
    line = Serial.readStringUntil('\n');

    // Play from first 9 files
    if ((command >= '1') && (command <= '9')) {
      Serial.print("Music reproduction ");
      Serial.println(command);
      command = command - 48;
      myDFPlayer.play(command);
      menu_options();
    }

    //Play from specific folder
    if (command == 'f') {
      int indexF = line.indexOf('f');
      int indexS = line.indexOf('s');
      if (indexF != -1 && indexS != -1 && indexF < indexS) {
        int folder = line.substring(indexF + 1, indexS).toInt();
        int song = line.substring(indexS + 1).toInt();
        Serial.print("From folder: ");
        Serial.print(folder);
        Serial.print(", playing song: ");
        Serial.println(song);
        myDFPlayer.playFolder(folder, song);  //play specific mp3 in SD:/folder/song.mp3; Folder Name(1~99); File Name(1~255)
      } else {
        Serial.println("Incomplete 'f' command. Specify both folder and song numbers.");
      }
      menu_options();
    }

    // Reproduction
    // Stop
    if (command == 's') {
      myDFPlayer.stop();
      Serial.println("Music Stopped!");
      menu_options();
    }

    // Pause/Continue the music
    if (command == 'p') {
      pausing = !pausing;
      if (pausing == 0) {
        Serial.println("Continue...");
        myDFPlayer.resume();
      }
      if (pausing == 1) {
        Serial.println("Music Paused!");
        myDFPlayer.pause();
      }
      menu_options();
    }

    // Toggle repeat mode
    if (command == 'r') {
      repeat = !repeat;
      if (repeat == 1) {
        myDFPlayer.startRepeat();
        Serial.println("Repeat mode enabled.");
      } else {
        myDFPlayer.stopRepeat();
        Serial.println("Repeat mode disabled.");
      }
      menu_options();
    }

    // Set volume
    if (command == 'v') {
      int myVolume = line.substring(1).toInt();
      if (myVolume >= 0 && myVolume <= 30) {
        myDFPlayer.volume(myVolume);
        Serial.print("Current Volume: ");
        Serial.println(myDFPlayer.currentVolume());
      } else {
        Serial.println("Invalid volume level, choose a number between 0-30.");
      }
      menu_options();
    }

    // Increases volume
    if (command == '+') {
      myDFPlayer.incVolume();
      Serial.print("Current Volume: ");
      Serial.println(myDFPlayer.currentVolume());
      menu_options();
    }
    // Decreases volume
    if (command == '-') {
      myDFPlayer.decVolume();
      Serial.print("Current Volume: ");
      Serial.println(myDFPlayer.currentVolume());
      menu_options();
    }

    // Play previouse
    if (command == '<') {
      myDFPlayer.playNext();
      Serial.println("Previous:");
      Serial.print("Current track: ");
      Serial.println(myDFPlayer.currentSdTrack() - 1);
      menu_options();
    }

    // Play next
    if (command == '>') {
      myDFPlayer.playPrevious();
      Serial.println("Next:");
      Serial.print("Current track: ");
      Serial.println(myDFPlayer.currentSdTrack() + 1);
      menu_options();
    }
  }
}

void menu_options() {
  Serial.println();
  Serial.println(F("=================================================================================================================================="));
  Serial.println(F("Commands:"));
  Serial.println(F(" [1-9] To select the MP3 file"));
  Serial.println(F(" [fXsY] Play song from folder X, song Y"));
  Serial.println(F(" [s] stopping reproduction"));
  Serial.println(F(" [p] pause/continue music"));
  Serial.println(F(" [r] toggle repeat mode"));
  Serial.println(F(" [vX] set volume to X"));
  Serial.println(F(" [+ or -] increases or decreases the volume"));
  Serial.println(F(" [< or >] forwards or backwards the track"));
  Serial.println();
  Serial.println(F("================================================================================================================================="));
}
