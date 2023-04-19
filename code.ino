#include "pitches.h"
#include <ESP8266WiFi.h> 
#include <WiFiClient.h> 
#include "Ubidots.h"

#define TOKEN "your-token" // Assign your Ubidots TOKEN
#define DEVICE_LABEL "your-device-label" // Assign the unique device label
#define VARIABLE_LABEL "your-variable-label" // Assign the unique variable label to get the last value
char ssid[] = "your-ssid";
char pass[] = "your-pass";
#define RED D4
#define GREEN D6
#define BLUE D5
#define buz_pin D2


Ubidots client(TOKEN);

boolean buzzing_state = false;

int melody[] = {
  NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5,
  NOTE_G5, REST, NOTE_G4, REST, 
  NOTE_C5, NOTE_G4, REST, NOTE_E4,
  NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
};

int durations[] = {
  8, 8, 8, 8, 8, 8, 8,
  4, 4, 8, 4, 
  4, 8, 4, 4,
  4, 4, 8, 4,
};



void setup(void){ 

  Serial.begin(9600);        

  delay(10);

  pinMode(buz_pin, OUTPUT); 
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);

  Serial.print("\n\nConnecting Wifi... ");

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED){    
    delay(500);
  }

  Serial.println("OK!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());           
  Serial.println();
}

void loop(void){ 

  float value = client.get(DEVICE_LABEL,VARIABLE_LABEL);
  Serial.println(value);
  
  if (value != ERROR_VALUE) {
    Serial.print("Value:");
    Serial.println(value);
    if (value == 1){
      buzzing_state=true;
      int size = sizeof(durations) / sizeof(int);

      for (int note = 0; note < size && buzzing_state == true; note++) {
        int duration = 1000 / durations[note];

        tone(buz_pin, melody[note], duration);
        if(note%3==0){
          digitalWrite(RED, HIGH);
        }
        else if(note%3==1){
          digitalWrite(GREEN, HIGH);
        }
        else if(note%3==2){
          digitalWrite(BLUE, HIGH);
        }
        int pauseBetweenNotes = duration * 1.30;
        delay(pauseBetweenNotes);
        yield();

        noTone(buz_pin);
        if(note%3==0){
          digitalWrite(RED, LOW);
        }
        else if(note%3==1){
          digitalWrite(GREEN, LOW);
        }
        else if(note%3==2){
          digitalWrite(BLUE, LOW);
        }
        yield();
       }
    }
    else if (value == 0){
      buzzing_state=false;
      digitalWrite(buz_pin, LOW);
    } 

  }
}
