/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.

 03/17/2013 : Charles-Henri Hallard (http://hallard.me)
              Modified to use with Arduipi board http://hallard.me/arduipi
						  Changed to use modified bcm2835 and RF24 library

 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <time.h>
#include "RF24.h"

//#define SENID_CTR	1
//#define SENID_SW01	2
//#define SENID_VCC	3
//#define SENID_SW02  4

typedef enum {
	SENID_NONE = 0,
	SENID_CTR,
	SENID_SW1_NC_PC,
	SENID_VCC,
	SENID_TEMP,
	SENID_SW1_NC_POLL,
	SENID_SW1_NO_PC,
	SENID_SW1_NO_POLL,
	SENID_SW2_NC_PC,
	SENID_SW2_NC_POLL,
	SENID_SW2_NO_PC,
	SENID_SW2_NO_POLL
} senId_t;


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
RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_4MHZ);

// RPi generic:
//RF24 radio(22,0);

/*** RPi Alternate ***/
//Note: Specify SPI BUS 0 or 1 instead of CS pin number.
// See http://tmrh20.github.io/RF24/RPi.html for more information on usage

//RPi Alternate, with MRAA
//RF24 radio(15,0);

//RPi Alternate, with SPIDEV - Note: Edit RF24/arch/BBB/spi.cpp and  set 'this->device = "/dev/spidev0.0";;' or as listed in /dev
//RF24 radio(22,0);


unsigned char payload[8];
int longStr = 0;
int printPayload = 0;
char *pgmName = NULL;


//************  Forward Declarations
int parse_payload(unsigned char *payload);



/********************************/

// Radio pipe addresses for the 2 nodes to communicate.
//const uint8_t pipes[][6] = {"1Node","2Node"};



int Usage(void)
{
	fprintf(stderr, "Usage: %s [-l] [-p]\n", pgmName);
	return 0;
}


int main(int argc, char *argv[])
{
	int opt;

#if 0
	struct timespec ts;

	printf("lofi_rx\n");
	printf("sizeof(long int): %d\n", sizeof(long int));
	printf("sizeof(time_t): %d\n", sizeof(time_t));
	clock_getres(CLOCK_REALTIME, &ts);
	printf("CLOCK_REALTIME:         tv_sec: 0x%08X  tv_nsec: 0x%08X\n", (unsigned int)ts.tv_sec, (unsigned int)ts.tv_nsec);
	clock_getres(CLOCK_REALTIME_COARSE, &ts);
	printf("CLOCK_REALTIME_COARSE:  tv_sec: 0x%08X  tv_nsec: 0x%08X\n", (unsigned int)ts.tv_sec, (unsigned int)ts.tv_nsec);
	clock_getres(CLOCK_MONOTONIC, &ts);
	printf("CLOCK_MONOTONIC:        tv_sec: 0x%08X  tv_nsec: 0x%08X\n", (unsigned int)ts.tv_sec, (unsigned int)ts.tv_nsec);
	clock_getres(CLOCK_MONOTONIC_COARSE, &ts);
	printf("CLOCK_MONOTONIC_COARSE: tv_sec: 0x%08X  tv_nsec: 0x%08X\n", (unsigned int)ts.tv_sec, (unsigned int)ts.tv_nsec);
	clock_getres(CLOCK_MONOTONIC_RAW, &ts);
	printf("CLOCK_MONOTONIC_RAW:    tv_sec: 0x%08X  tv_nsec: 0x%08X\n", (unsigned int)ts.tv_sec, (unsigned int)ts.tv_nsec);
	clock_getres(CLOCK_BOOTTIME, &ts);
	printf("CLOCK_BOOTTIME:         tv_sec: 0x%08X  tv_nsec: 0x%08X\n", (unsigned int)ts.tv_sec, (unsigned int)ts.tv_nsec);

	clock_gettime(CLOCK_REALTIME, &ts);
	printf("CLOCK_REALTIME:         tv_sec: 0x%08X  tv_nsec: 0x%08X\n", (unsigned int)ts.tv_sec, (unsigned int)ts.tv_nsec);
#endif	

	pgmName = argv[0];

	while ((opt = getopt(argc, argv, "lp")) != -1) {
		switch (opt) {
		case 'l':
			longStr = 1;
			break;
		case 'p':
			printPayload = 1;
			break;
		default:
			Usage();
			exit(-1);
			break;
		}
	}

  // Setup and configure rf radio
  radio.begin();

  radio.setChannel(2);
  radio.setDataRate(RF24_2MBPS);
//  radio.setPayloadSize(8);

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
//		if ( radio.available(0) )
//		{
			// Dump the payloads until we've gotten everything
//			unsigned long got_time;

			// Fetch the payload, and see if this was the last one.
			while (radio.available(0)) {
				radio.read( payload, 8 );
				// Spew it
				parse_payload(payload);
			}
//			radio.stopListening();
				
//			radio.write( &got_time, sizeof(unsigned long) );

			// Now, resume listening so we catch the next packets.
//			radio.startListening();

//		}
		usleep(50000);

	} // forever loop

  return 0;
}


#if 1
int parse_payload(unsigned char *payload)
{
	struct timespec ts;
	int i;
	unsigned short val;
	uint8_t	sensorId;
	uint8_t nodeId;

	if (printPayload) {
		printf("Payload: %02X %02X %02X %02X %02X %02X %02X %02X\n",
			payload[0], payload[1], payload[2], payload[3],
			payload[4], payload[5], payload[6], payload[7]);
	}
			
	clock_gettime(CLOCK_REALTIME, &ts);
	nodeId = payload[0];
	if (longStr)
		printf("%d NodeId: %2d", (int)ts.tv_sec, nodeId);
	i = 1;

	while((sensorId = (payload[i]>>3) & 0x1F) != 0) {
		switch(sensorId) {
		case SENID_CTR:
			val = payload[i++] & 0x03;
			val <<= 8;
			val += payload[i++];
			if (longStr)
				printf("  Ctr: %4d", val);
			else
				printf("%d NodeId: %2d  Ctr: %4d\n", (unsigned int)ts.tv_sec, nodeId, val);
			break;
		case SENID_SW1_NC_PC:
//			if (payload[i] & 0x01)
//				printf(" toggled");
			if (longStr)
				printf("  SW1 %2d: %s", sensorId, (payload[i++] & 0x02) ? "OPEN  " : "CLOSED");
			else
				printf("%d NodeId: %2d  SW1 %2d: %s", (unsigned int)ts.tv_sec, nodeId, sensorId,  (payload[i++] & 0x02) ? " OPEN\n" : " CLOSED\n");
			break;
		case SENID_SW2_NC_PC:
//			if (payload[i] & 0x01)
//				printf(" toggled");
			if (longStr)
				printf("  SW2 %2d: %s", sensorId, (payload[i++] & 0x02) ? "OPEN  " : "CLOSED");
			else
				printf("%d NodeId: %2d  SW2 %2d: %s", (unsigned int)ts.tv_sec, nodeId, sensorId,  (payload[i++] & 0x02) ? " OPEN\n" : " CLOSED\n");
			break;
		case SENID_VCC:
			val = payload[i++] & 0x03;
			val <<= 8;
			val += payload[i++];
			if (longStr)
				printf("  Vcc: %4.2f",(1.1 * 1024.0)/(float)val);
			else
				printf("%d NodeId: %2d  Vcc: %4.2f\n", (unsigned int)ts.tv_sec, nodeId, (1.1 * 1024.0)/(float)val);
			break;
		case SENID_TEMP:
			val = payload[i++] & 0x03;
			val <<= 8;
			val += payload[i++];
			if (longStr)
				printf("  Temp: %4.2f",1.0 * (float)val - 260.0);
			else
				printf("%d NodeId: %2d  Vcc: %4.2f\n", (unsigned int)ts.tv_sec, nodeId, 1.0 * (float)val - 260.0);
			break;
		default:
			break;
		}
	}
	printf("\n");
				
	fflush(stdout);
	return 0;
}
#else
int parse_payload(unsigned char *payload)
{
	struct timespec ts;
	int i;
	unsigned short val;
	uint8_t	sensorId;
	uint8_t nodeId;

#if 0
	printf("Payload: %02X %02X %02X %02X %02X %02X %02X %02X\n",
		payload[0], payload[1], payload[2], payload[3],
		payload[4], payload[5], payload[6], payload[7]);
#endif
			
	clock_gettime(CLOCK_REALTIME, &ts);
//	printf("CLOCK_REALTIME:         tv_sec: 0x%08X  tv_nsec: 0x%08X\n", (unsigned int)ts.tv_sec, (unsigned int)ts.tv_nsec);
	nodeId = payload[0];
	printf("%d NodeId: %2d", ts.tv_sec, nodeId);
	i = 1;

	while((sensorId = (payload[i]>>3) & 0x1F) != 0) {
		switch(sensorId) {
		case SENID_CTR:
			val = payload[i++] & 0x03;
			val <<= 8;
			val += payload[i++];
			printf(" Ctr: %4d", val);
			break;
		case SENID_SW01:
			if (payload[i] & 0x01)
				printf(" toggled");
			if (payload[i++] & 0x02)
				printf(" SW1: OPEN");
			else
				printf(" SW1: CLOSED");
			break;
		case SENID_VCC:
			val = payload[i++] & 0x03;
			val <<= 8;
			val += payload[i++];
			printf(" Vcc: %4.2f", (1.1 * 1024.0)/(float)val);
			break;
		default:
			break;
		}
	}
	printf("\n");
				
	fflush(stdout);
	return 0;
}
#endif

