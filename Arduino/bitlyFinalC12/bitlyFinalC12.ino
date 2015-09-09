#include <T3Mac.h>
#include "FastLED.h"
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>   
EthernetUDP Udp;

unsigned int localPort = 11999;  
const unsigned int resetPin = 9;

const unsigned int ledPin0 = 2;
const unsigned int ledPin1 = 5;
const unsigned int ledPin2 = 6;
const unsigned int ledPin3 = 21;
const unsigned int ledPin4 = 7;
const unsigned int ledPin5 = 20;

const int NUM_LEDS= 648;
const int NUM_LEDS_PER_PIN = 108;
CRGB leds[NUM_LEDS];

const int IN_BUFFER_SIZE = 974;
char packetBuffer[974];

boolean starting = true;
int brightness = 255;

void setup() {
  delay(2000);

  //Ethernet Module Reset
  digitalWrite(resetPin, LOW);
  pinMode(resetPin, OUTPUT);
  delayMicroseconds(10);
  pinMode(resetPin, INPUT);
  
  Ethernet.begin(mac, IPAddress(192,168,2,3));
  delay(200);

  Udp.begin(localPort);
  
  Serial.begin(9600); 
  
  FastLED.addLeds<NEOPIXEL, ledPin0>(leds, 0, 108);
  FastLED.addLeds<NEOPIXEL, ledPin1>(leds, 108, 108);
  FastLED.addLeds<NEOPIXEL, ledPin2>(leds, 216, 108);
  FastLED.addLeds<NEOPIXEL, ledPin3>(leds, 324, 108);
  FastLED.addLeds<NEOPIXEL, ledPin4>(leds, 432, 108);
  FastLED.addLeds<NEOPIXEL, ledPin5>(leds, 540, 108);

  for (int i = 0; i < 108; i++) {
    leds[i] = CRGB::Red;
  }
  for (int i = 108; i < 216; i++) {
    leds[i] = CRGB::White;
  }
  for (int i = 216; i < 324; i++) {
    leds[i] = CRGB::Blue;
  }
  for (int i = 324; i < 432; i++) {
    leds[i] = CRGB::Yellow;
  }
  for (int i = 432; i < 540; i++) {
    leds[i] = CRGB::White;
  }
  for (int i = 540; i < 648; i++) {
    leds[i] = CRGB::Purple;
  }
  FastLED.show();
}

void loop() {
    if (starting) {
    for (int i = 0; i < 648; i++) {
      FastLED.setBrightness(brightness);
    }
    brightness--;
    if (brightness <= 0) {
      starting = false;
      brightness = 255;
      for (int i = 0; i < 648; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.setBrightness(brightness);
    }
    FastLED.show();
  } else {
    int packetSize = Udp.parsePacket();
    if (packetSize == IN_BUFFER_SIZE) {
      Serial.println("Showing!");
      Udp.read((char*)packetBuffer, IN_BUFFER_SIZE);
      if (packetBuffer[0] == 3 && packetBuffer[1] == 1) {
        memcpy(leds, packetBuffer+2, 972);
      } else if (packetBuffer[0] == 3 && packetBuffer[1] == 2) {
        memcpy(&leds[324], packetBuffer+2, 972);
      }
    } else if (packetSize == 1) {
      FastLED.show();
    }
  }
}


