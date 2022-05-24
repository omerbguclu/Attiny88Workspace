#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX

void setup() {

  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
  mySerial.println("Hello, world?");
}

void loop() { // run over and over
  if(mySerial.available())
  {
    mySerial.write(mySerial.read());
  }
    //mySerial.write("hello\n");
    //delay(1000);
}
