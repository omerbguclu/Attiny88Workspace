/* 1 ch NRF 24 TRANSMITTER example.
/* Tutorial link: http://electronoobs.com/eng_arduino_tut95.php
 * Code: http://electronoobs.com/eng_arduino_tut95_code2.php
 * Scheamtic: http://electronoobs.com/eng_arduino_tut95_sch1.php
 * Youtube Channel: http://www.youtube/c/electronoobs

  Module // Arduino UNO
    GND    ->   GND
    Vcc    ->   3.3V
    CE     ->   D9
    CSN    ->   D10
    CLK    ->   D13
    MOSI   ->   D11
    MISO   ->   D12
 */

/* First we include the libraries. Download it from
   my webpage if you donw have the NRF24 library */
#include <SPI.h>
#include <nRF24L01.h> //Downlaod it here: https://www.electronoobs.com/eng_arduino_NRF24.php
#include <RF24.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX

/*//////////////////////////////////////////////////////*/

/*Create a unique pipe out. The receiver has to
  wear the same unique code*/
uint8_t pipeIn[6] = "1Node"; // IMPORTANT: The same as in the receiver!!!
/*//////////////////////////////////////////////////////*/

/*Create the data struct we will send
  The sizeof this struct should not exceed 32 bytes
  This gives us up to 32 8 bits channals */
RF24 radio(16, 10); // select  CSN and CE  pins
struct MyData
{
    byte varx;
    byte vary;
    byte button;
};

MyData data;
/*//////////////////////////////////////////////////////*/

// This function will only set the value to  0 if the connection is lost...
void resetData()
{
    data.varx = 0;
    data.vary = 0;
    data.button = 0;
}

/**************************************************/

void setup()
{
    mySerial.begin(9600); // Set the speed to 9600 bauds if you want.
    // You should always have the same speed selected in the serial monitor
    resetData();
    radio.begin();
    radio.setAutoAck(false);
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(1, pipeIn);
    // we start the radio comunication
    radio.startListening();
}

/******Reset the received data to 0 if connection is lost******/
unsigned long lastRecvTime = 0;
void recvData()
{
    while (radio.available())
    {
        radio.read(&data, 3);
        mySerial.print(data.varx);
        mySerial.print(" ");
        mySerial.print(data.vary);
        mySerial.print(" ");
        mySerial.println(data.button);
        lastRecvTime = millis(); // here we receive the data
    }
}
/**************************************************************/

void loop()
{
    recvData();
    unsigned long now = millis();
    // Here we check if we've lost signal, if we did we reset the values
    if (now - lastRecvTime > 3000)
    {
        // Signal lost?
        lastRecvTime = now;
        resetData();
        mySerial.println("Reset");
    }
    else
    {
    }
}
