/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.

 03/17/2013 : Charles-Henri Hallard (http://hallard.me)
              Modified to use with Arduipi board http://hallard.me/arduipi
						  Changed to use modified bcm2835 and RF24 library
TMRh20 2014 - Updated to work with optimized RF24 Arduino library

 */

/**
 * Example RF Radio Ping Pair
 *
 * This is an example of how to use the RF24 class on RPi, communicating to an Arduino running
 * the GettingStarted sketch.
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>

using namespace std;
//
// Hardware configuration
// Configure the appropriate pins for your connections

/****************** Raspberry Pi ***********************/

// Radio CE Pin, CSN Pin, SPI Speed
// See http://www.airspayce.com/mikem/bcm2835/group__constants.html#ga63c029bd6500167152db4e57736d0939 and the related enumerations for pin information.

// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 4Mhz
//RF24 radio(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ);

// NEW: Setup for RPi B+
//RF24 radio(RPI_BPLUS_GPIO_J8_15,RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ);

// Setup for GPIO 15 CE and CE0 CSN with SPI Speed @ 8Mhz
RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

// RPi generic:
//RF24 radio(22,0);

/*** RPi Alternate ***/
//Note: Specify SPI BUS 0 or 1 instead of CS pin number.
// See http://tmrh20.github.io/RF24/RPi.html for more information on usage

//RPi Alternate, with MRAA
//RF24 radio(15,0);

//RPi Alternate, with SPIDEV - Note: Edit RF24/arch/BBB/spi.cpp and  set 'this->device = "/dev/spidev0.0";;' or as listed in /dev
//RF24 radio(22,0);


/****************** Linux (BBB,x86,etc) ***********************/

// See http://tmrh20.github.io/RF24/pages.html for more information on usage
// See http://iotdk.intel.com/docs/master/mraa/ for more information on MRAA
// See https://www.kernel.org/doc/Documentation/spi/spidev for more information on SPIDEV

// Setup for ARM(Linux) devices like BBB using spidev (default is "/dev/spidev1.0" )
//RF24 radio(115,0);

//BBB Alternate, with mraa
// CE pin = (Header P9, Pin 13) = 59 = 13 + 46 
//Note: Specify SPI BUS 0 or 1 instead of CS pin number. 
//RF24 radio(59,0);

/********** User Config *********/
// Assign a unique identifier for this node, 0 or 1
//bool radioNumber = 1;

/********************************/

// Radio pipe addresses for the 2 nodes to communicate.
//const uint8_t pipes[][6] = {"1Node","2Node"};


int main(int argc, char** argv){

//  bool role_ping_out = true, role_pong_back = false;
//  bool role = role_pong_back;

  printf("RF24/examples/GettingStarted/\n");

  // Setup and configure rf radio
  radio.begin();

  radio.setChannel(2);
  radio.setDataRate(RF24_2MBPS);
  radio.setPayloadSize(8);

  // optionally, increase the delay between retries & # of retries
//  radio.setRetries(15,15);
  // Dump the configuration of the rf unit for debugging
  radio.printDetails();


/***********************************/
	radio.startListening();
	
	// forever loop
	while (1)
	{

		// if there is data ready
		if ( radio.available() )
		{
			// Dump the payloads until we've gotten everything
			unsigned long got_time;

			// Fetch the payload, and see if this was the last one.
			while(radio.available()){
				radio.read( &got_time, sizeof(unsigned long) );
			}
//			radio.stopListening();
				
//			radio.write( &got_time, sizeof(unsigned long) );

			// Now, resume listening so we catch the next packets.
//			radio.startListening();

			// Spew it
			printf("Got payload(%d) %lu...\n",sizeof(unsigned long), got_time);
			
				
		}
		delay(925); //Delay after payload responded to, minimize RPi CPU time
		printf("."); fflush(stdout);
//  radio.printDetails();
		

	} // forever loop

  return 0;
}

