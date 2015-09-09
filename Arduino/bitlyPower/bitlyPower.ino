int switches[5] = {2,3,4,5,6};

void setup() {
  
  pinMode(switches[0], OUTPUT); 
  pinMode(switches[1], OUTPUT); 
  pinMode(switches[2], OUTPUT); 
  pinMode(switches[3], OUTPUT); 
  pinMode(switches[4], OUTPUT); 

  Serial.begin(57600); 
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(switches[i], HIGH);
  }

}

void loop() {
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();
    for (int i = 0; i < 5; i++) {
      if (incomingByte == 'h') {
        digitalWrite(switches[i], HIGH);
      } else if (incomingByte == 'l') {
        digitalWrite(switches[i], LOW);
      }
    }
  }
  delay(100);
}
