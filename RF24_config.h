
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
*/

#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__

  /*** USER DEFINES:  ***/  
  //#define FAILURE_HANDLING
  //#define SERIAL_DEBUG
  //#define MINIMAL
  //#define SPI_UART  // Requires library from https://github.com/TMRh20/Sketches/tree/master/SPI_UART
  //#define SOFTSPI   // Requires library from https://github.com/greiman/DigitalIO
  
  /**********************/
  #define rf24_max(a,b) (a>b?a:b)
  #define rf24_min(a,b) (a<b?a:b)

  #if defined SPI_HAS_TRANSACTION && !defined SPI_UART && !defined SOFTSPI
    #define RF24_SPI_TRANSACTIONS
  #endif
  

  // The Makefile checks for bcm2835 (RPi) and copies the correct includes.h file to /utility/includes.h (Default is spidev config)
  // This behavior can be overridden by calling 'make RF24_SPIDEV=1' or 'make RF24_MRAA=1'
  // The includes.h file defines either RF24_RPi, MRAA or RF24_BBB and includes the correct RF24_arch_config.h file
  #include "includes.h"




#endif // __RF24_CONFIG_H__

