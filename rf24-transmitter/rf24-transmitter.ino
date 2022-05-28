/* 1 ch NRF 24 TRANSMITTER example.
/* Tutorial link: http://electronoobs.com/eng_arduino_tut95.php
 * Code: http://electronoobs.com/eng_arduino_tut95_code1.php
 * Scheamtic: http://electronoobs.com/eng_arduino_tut95_sch1.php
 * Youtube Channel: http://www.youtube/c/electronoobs
 *
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
#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h> //Downlaod it here: https://www.electronoobs.com/eng_arduino_NRF24.php
#include <RF24.h>
/*//////////////////////////////////////////////////////*/

#define ATTINYMH88
#define VARX_PIN 0
#define VARY_PIN 1
#define SWITCH_PIN 2
// #define UNO

#ifdef ATTINYMH88
SoftwareSerial mySerial(4, 5); // RX, TX
#endif

/*Create a unique pipe out. The receiver has to
  wear the same unique code*/
uint8_t pipeOut[6] = "1Node"; // IMPORTANT: The same as in the receiver!!!
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

void setup()
{
#ifdef UNO
    Serial.println("UNO");
#elif defined(ATTINYMH88)
    mySerial.begin(9600); // Set the speed to 9600 bauds if you want.
#endif
    pinMode(SWITCH_PIN, INPUT_PULLUP);
    // Start everything up
    radio.begin();
    radio.setAutoAck(false);
    radio.setDataRate(RF24_250KBPS);
    radio.openWritingPipe(pipeOut);
    resetData();
}

/**************************************************/

// Returns a corrected value for a potentiometer analog read
// It will map the value from 0 to 1024 to 1 to 255
int mapPotentiometers(int val, int lower, int middle, int upper, bool reverse)
{
    val = constrain(val, lower, upper);
    if (val < middle)
        val = map(val, lower, middle, 0, 128);
    else
        val = map(val, middle, upper, 128, 255);
    return (reverse ? 255 - val : val);
}

void loop()
{
    data.varx = mapPotentiometers(analogRead(VARX_PIN), 0, 512, 1024, true);
    data.vary = mapPotentiometers(analogRead(VARY_PIN), 0, 512, 1024, true);
    data.button = digitalRead(SWITCH_PIN);

    mySerial.print(data.varx);
    mySerial.print(" ");
    mySerial.print(data.vary);
    mySerial.print(" ");
    mySerial.println(data.button);

    radio.write(&data, 3);
}