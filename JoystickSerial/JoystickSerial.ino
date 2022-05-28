#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX

#define VARX_PIN 0
#define VARY_PIN 1
#define SWITCH_PIN 2

int varx = 0, vary = 0, switchpin = 0;
String message = "";

void setup() {

  // set the data rate for the SoftwareSerial port
  pinMode(SWITCH_PIN, INPUT);
  mySerial.begin(4800);
  mySerial.println("Hello, world?");
}

void loop() { // run over and over
  varx = analogRead(VARX_PIN);
  vary = analogRead(VARY_PIN);
  switchpin = digitalRead(SWITCH_PIN);

  message = "";
  message += "VARX: "; message += varx; message += " ";
  message += "VARY: "; message += vary; message += " ";
  message += "Switch: "; message += switchpin; message += " ";

  mySerial.println(message);
}
