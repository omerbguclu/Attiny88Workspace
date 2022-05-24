/*
 * See documentation at https://nRF24.github.io/RF24
 * See License information at root directory of this library
 * Author: Brendan Doherty (2bndy5)
 */

/**
 * A simple example of sending data from 1 nRF24L01 transceiver to another.
 *
 * This example was written to be used on 2 devices acting as "nodes".
 * Use the Serial Monitor to change each node's behavior.
 */
#include <SPI.h>
#include "RF24.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX
// instantiate an object for the nRF24L01 transceiver
RF24 radio(16, 10); // using pin 7 for the CE pin, and pin 8 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[][6] = {"1Node", "2Node"};
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber = 1; // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// Used to control whether this node is sending or receiving
bool role = true; // true = TX role, false = RX role

// For this example, we'll be using a payload containing
// a single float number that will be incremented
// on every successful transmission
float payload = 0.0;

void setup()
{

    //   Serial.begin(115200);
    //   while (!Serial) {
    //     // some boards need to wait to ensure access to serial over USB
    //   }
    mySerial.begin(9600);
    // initialize the transceiver on the SPI bus
    if (!radio.begin())
    {
        while (1)
        {
            delay(1000);
            mySerial.println(F("nok"));

        } // hold in infinite loop
    }

    // print example's introductory prompt
    // mySerial.println(F("RF24/examples/GettingStarted"));

    // To set the radioNumber via the Serial monitor on startup
    // mySerial.println(F("Which radio is this? Enter '0' or '1'. Defaults to '0'"));
    //   while (!Serial.available()) {
    // wait for user input
    //   }
    //   char input = Serial.parseInt();
    radioNumber = 1;
    //   mySerial.print(F("rn = "));
    //   mySerial.println((int)radioNumber);

    // // role variable is hardcoded to RX behavior, inform the user of this
    //   mySerial.println(F("PRESS 'T'"));

    // Set the PA Level low to try preventing power supply related problems
    // because these examples are likely run with nodes in close proximity to
    // each other.
    radio.setPALevel(RF24_PA_LOW); // RF24_PA_MAX is default.

    // save on transmission time by setting the radio to only transmit the
    // number of bytes we need to transmit a float
    radio.setPayloadSize(sizeof(payload)); // float datatype occupies 4 bytes

    // set the TX address of the RX node into the TX pipe
    radio.openWritingPipe(address[radioNumber]); // always uses pipe 0

    // set the RX address of the TX node into a RX pipe
    radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1

    // additional setup specific to the node's role
    if (role)
    {
        radio.stopListening(); // put radio in TX mode
    }
    else
    {
        radio.startListening(); // put radio in RX mode
    }

    // For debugging info
    // printf_begin();             // needed only once for printing details
    // radio.printDetails();       // (smaller) function that prints raw register values
    // radio.printPrettyDetails(); // (larger) function that prints human readable data

} // setup

void loop()
{

    if (role)
    {
        // This device is a TX node

        unsigned long start_timer = micros();               // start the timer
        bool report = radio.write(&payload, sizeof(float)); // transmit & save the report
        unsigned long end_timer = micros();                 // end the timer

        if (report)
        {
            mySerial.print(F("T OK! ")); // payload was delivered
            //   mySerial.print(F("Time to T = "));
            //   mySerial.print(end_timer - start_timer);                 // print the timer result
            //   mySerial.print(F(" us. Sent: "));
            //   mySerial.println(payload);                               // print payload sent
            payload += 0.01; // increment float payload
        }
        else
        {
              mySerial.println(F("failed")); // payload was not delivered
        }

        // to make this example readable in the serial monitor
        delay(1000); // slow transmissions down by 1 second
    }
    else
    {
        // This device is a RX node

        uint8_t pipe;
        // if (radio.available(&pipe))
        // {                                           // is there a payload? get the pipe number that recieved it
        //     uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
        //     radio.read(&payload, bytes);            // fetch payload from FIFO
        //       mySerial.print(F("R "));
        //       mySerial.print(bytes);                    // print the size of the payload
        //       mySerial.print(F(" bytes on pipe "));
        //       mySerial.print(pipe);                     // print the pipe number
        //       mySerial.print(F(": "));
        //       mySerial.println(payload);                // print the payload's value
        // }
    } // role

    // if (mySerial.available())
    // {
    //     // change the role via the serial monitor

    //     char c = toupper(Serial.read());
    //     if (c == 'T' && !role)
    //     {
    //         // Become the TX node

    //         role = true;
    //         //   mySerial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
    //         radio.stopListening();
    //     }
    //     else if (c == 'R' && role)
    //     {
    //         // Become the RX node

    //         role = false;
    //         //   mySerial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
    //         radio.startListening();
    //     }
    // }

} // loop
