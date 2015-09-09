#include <T3Mac.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>   
#include <OSCMessage.h>

EthernetUDP Udp;

unsigned int localPort = 11999;  
const unsigned int resetPin = 9;

int led = 13;

int pot1 = A1;
int pot2 = A2;
int pot3 = A3;

int btn1 = 22;
int btn2 = 4;
int btn3 = 3;


int switchButtonState = 0;         
int lastSwitchButtonState = 0;     

int pushButtonState = 0;         
int lastPushButtonState = 0;    

const int numReadings = 5;

int readingsL[numReadings];      
int readingIndexL = 0;                  
int totalL = 0;                  
int averageL = 0;  

int readingsR[numReadings];      
int readingIndexR = 0;                  
int totalR = 0;                  
int averageR = 0;    

int readingsB[numReadings];      
int readingIndexB = 0;                  
int totalB = 0;                  
int averageB = 0;    

int prevB = 0;
int prevL = 0;
int prevR = 0; 

void setup() {
  
  pinMode(led, OUTPUT); 
  
  pinMode(btn1, INPUT);  
  pinMode(btn2, INPUT); 
  pinMode(btn3, INPUT);
  

  
  delay(2000);
  pinMode(resetPin, OUTPUT);
  delayMicroseconds(10);
  pinMode(resetPin, INPUT);
  Ethernet.begin(mac, IPAddress(192,168,2,13));
  delay(200);
  Udp.begin(localPort);
  
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readingsL[thisReading] = 0;  
  }
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readingsR[thisReading] = 0;  
  }
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readingsB[thisReading] = 0;  
  }
  Serial.begin(9600); 
}

void loop() {

  switchButtonState = digitalRead(btn1);
  if (switchButtonState != lastSwitchButtonState) {
    OSCMessage msg("/switch/");
    Udp.beginPacket(IPAddress(192,168,2,11), 2009);
    if (switchButtonState == HIGH) {
      Serial.println("piece off \n");
      msg.add(1);      
    } 
    else {
      Serial.println("piece on \n"); 
      msg.add(0);      
    }
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
  }
  lastSwitchButtonState = switchButtonState;

  pushButtonState = digitalRead(btn2);
  if (pushButtonState != lastPushButtonState) {
    if (pushButtonState == HIGH) {
      Serial.println("state change \n");
      OSCMessage msg("/state/");
      Udp.beginPacket(IPAddress(192,168,2,11), 2009);
      msg.add(0);         
      msg.send(Udp);
      Udp.endPacket();
      msg.empty();
    }
  }
  lastPushButtonState = pushButtonState;
  
  totalB = totalB - readingsB[readingIndexB];         
  readingsB[readingIndexB] = analogRead(pot1); 
  totalB += readingsB[readingIndexB]; 
  readingIndexB++;
  readingIndexB %= numReadings;               
  averageB = totalB / numReadings; 
  
  totalL = totalL - readingsL[readingIndexL];         
  readingsL[readingIndexL] = analogRead(pot2); 
  totalL += readingsL[readingIndexL]; 
  readingIndexL++;
  readingIndexL %= numReadings;               
  averageL = totalL / numReadings; 
  
  totalR = totalR - readingsR[readingIndexR];         
  readingsR[readingIndexR] = analogRead(pot3); 
  totalR += readingsR[readingIndexR]; 
  readingIndexR++;
  readingIndexR %= numReadings;               
  averageR = totalR / numReadings; 
  
  if (abs(averageB - prevB) >= 2) {
    Serial.println(averageB);
    Serial.println("");
    OSCMessage msg("/alpha/");
    Udp.beginPacket(IPAddress(192,168,2,11), 2009);
    msg.add(averageB);         
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
  }
  prevB = averageB;
  
  if (abs(averageL - prevL) >= 2) {
    Serial.println(averageL);
    Serial.println("");
    OSCMessage msg("/left/");
    Udp.beginPacket(IPAddress(192,168,2,11), 2009);
    msg.add(averageL);         
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
  }
  prevL = averageL; 
  
  if (abs(averageR - prevR) >= 2) {
    Serial.println(averageR);
    Serial.println("");
    OSCMessage msg("/right/");
    Udp.beginPacket(IPAddress(192,168,2,11), 2009);
    msg.add(averageR);         
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
  }
  prevR = averageR; 

  delay(10);
}


