/****************************************************************************************************************************
  defines.h
  For ESP32, ESP8266, Teensy, SAMD, SAM DUE using W5x00 Ethernet shields
  
  BlynkEthernet_WM is a library for Teensy, ESP, SAM DUE and SAMD boards, with Ethernet W5X00 or ENC28J60 shields,
  to enable easy configuration/reconfiguration and autoconnect/autoreconnect of Ethernet/Blynk
  AVR Mega and W5100 is not supported.
  Library modified from Blynk library v0.6.1 https://github.com/blynkkk/blynk-library/releases
  Built by Khoi Hoang https://github.com/khoih-prog/BlynkEthernet_WM
  Licensed under MIT license
 *****************************************************************************************************************************/

#ifndef defines_h
#define defines_h

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT         Serial

//#define BLYNK_DEBUG         true
//#define BLYNK_DEBUG_ALL     true

#if ( defined(ESP32) || defined(ESP8266) )
  #define DOUBLERESETDETECTOR_DEBUG     true
#else
  #define DRD_GENERIC_DEBUG             true
#endif

#define BLYNK_WM_DEBUG                  1

#if ( defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || defined(ARDUINO_GENERIC_RP2040) )
  #if defined(ETHERNET_USE_RPIPICO)
    #undef ETHERNET_USE_RPIPICO
  #endif
  #define ETHERNET_USE_RPIPICO      true
#else
  #error This code is designed to run on RP2040 platform! Please check your Tools->Board setting.  
#endif
 
// Default pin 5 (in Mbed) or 17 to SS/CS
#if defined(ARDUINO_ARCH_MBED)
  // For RPI Pico using Arduino Mbed RP2040 core
  // SCK: GPIO2,  MOSI: GPIO3, MISO: GPIO4, SS/CS: GPIO5

  // Warning: Be careful !!  Use true only to clear all data in LittleFS to start over
  #define FORCE_REFORMAT            false
  
  #define USE_THIS_SS_PIN       5

  #if defined(BOARD_NAME)
    #undef BOARD_NAME
  #endif

  #if defined(ARDUINO_RASPBERRY_PI_PICO) 
    #define BOARD_TYPE      "MBED RASPBERRY_PI_PICO"
  #elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040)
    #define BOARD_TYPE      "MBED ADAFRUIT_FEATHER_RP2040"
  #elif defined(ARDUINO_GENERIC_RP2040)
    #define BOARD_TYPE      "MBED GENERIC_RP2040"
  #else
    #define BOARD_TYPE      "MBED Unknown RP2040"
  #endif
  
#else
  // For RPI Pico using E. Philhower RP2040 core
  // SCK: GPIO18,  MOSI: GPIO19, MISO: GPIO16, SS/CS: GPIO17
  #define USE_THIS_SS_PIN       17

#endif
  
#define SS_PIN_DEFAULT        USE_THIS_SS_PIN

// For RPI Pico
#warning Use RPI-Pico RP2040 architecture  

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

#define USE_BLYNK_WM      true

#define USE_SSL   false

#if USE_BLYNK_WM

  #define USE_DYNAMIC_PARAMETERS     true
 
  // To use faster 25MHz clock instead of defaulf 14MHz. Only for W5200 and W5500. W5100 also tested OK.
  //#define USE_W5100     false
  
  //#define USE_ETHERNET_WRAPPER    true
  #define USE_ETHERNET_WRAPPER    false
   
  #define USE_CUSTOM_ETHERNET     false //true
  
  // Note: To rename ESP628266 Ethernet lib files to Ethernet_ESP8266.h and Ethernet_ESP8266.cpp
  // In order to USE_ETHERNET_ESP8266
  
  // To override the default CS/SS pin. Don't use unless you know exactly which pin to use
  // You can define here or customize for each board at same place with BOARD_TYPE
  // Check @ defined(SEEED_XIAO_M0)
  //#define USE_THIS_SS_PIN   22  //21  //5 //4 //2 //15
  
  // Only one if the following to be true
  #define USE_ETHERNET          false
  #define USE_ETHERNET2         true
  #define USE_ETHERNET3         false
  #define USE_ETHERNET_LARGE    false
  #define USE_ETHERNET_ESP8266  false 
  #define USE_CUSTOM_ETHERNET   false
  
  #if !USE_ETHERNET_WRAPPER
    
    #if ( USE_ETHERNET2 || USE_ETHERNET3 || USE_ETHERNET_LARGE || USE_ETHERNET_ESP8266 || USE_ETHERNET_ENC || USE_NATIVE_ETHERNET )
      #ifdef USE_CUSTOM_ETHERNET
        #undef USE_CUSTOM_ETHERNET
      #endif
      #define USE_CUSTOM_ETHERNET   false
    #endif
  
    #if USE_NATIVE_ETHERNET
      #include "NativeEthernet.h"
      #warning Using NativeEthernet lib for Teensy 4.1. Must also use Teensy Packages Patch or error
      #define SHIELD_TYPE           "Custom Ethernet using Teensy 4.1 NativeEthernet Library"
    #elif USE_ETHERNET3
      #include "Ethernet3.h"
      #warning Using Ethernet3 lib
      #define SHIELD_TYPE           "W5x00 using Ethernet3 Library"
    #elif USE_ETHERNET2
      #include "Ethernet2.h"
      #warning Using Ethernet2 lib
      #define SHIELD_TYPE           "W5x00 using Ethernet2 Library"
    #elif USE_ETHERNET_LARGE
      #include "EthernetLarge.h"
      #warning Using EthernetLarge lib
      #define SHIELD_TYPE           "W5x00 using EthernetLarge Library"
    #elif USE_ETHERNET_ESP8266
      #include "Ethernet_ESP8266.h"
      #warning Using Ethernet_ESP8266 lib 
      #define SHIELD_TYPE           "W5x00 using Ethernet_ESP8266 Library" 
     #elif USE_CUSTOM_ETHERNET
      //#include "Ethernet_XYZ.h"
      #include "EthernetENC.h"
      #warning Using Custom Ethernet library. You must include a library and initialize.
      #define SHIELD_TYPE           "Custom Ethernet using Ethernet_XYZ Library"
    #else
      #ifdef USE_ETHERNET
        #undef USE_ETHERNET
      #endif
      #define USE_ETHERNET   true
      #include "Ethernet.h"
      #warning Using Ethernet lib
      #define SHIELD_TYPE           "W5x00 using Ethernet Library"
    #endif
  
    // Ethernet_Shield_W5200, EtherCard, EtherSia not supported
    // Select just 1 of the following #include if uncomment #define USE_CUSTOM_ETHERNET
    // Otherwise, standard Ethernet library will be used for W5x00
  
  #endif    //USE_ETHERNET_WRAPPER
   
  #if USE_SSL
    // Need ArduinoECCX08 and ArduinoBearSSL libraries
    // Currently, error not enough memory for UNO, Mega2560. Don't use
    #include <BlynkSimpleEthernetSSL_WM.h>
  #else
    #include <BlynkSimpleEthernet_WM.h>
  #endif

#else   ////USE_BLYNK_WM

  // Only one if the following to be true
  #define USE_ETHERNET          true
  #define USE_ETHERNET2         false
 
  #if USE_SSL
    // Need ArduinoECCX08 and ArduinoBearSSL libraries
    // Currently, error not enough memory for UNO, Mega2560. Don't use
    #include <BlynkSimpleEthernetSSL.h>
    #warning Using Ethernet lib
    #define SHIELD_TYPE           "W5x00 using Ethernet Library"
      
    #define BLYNK_SERVER_HARDWARE_PORT    9443
  #else
    #if USE_ETHERNET
      #include <BlynkSimpleEthernet.h>
      #warning Using Ethernet lib
      #define SHIELD_TYPE           "W5x00 using Ethernet Library"
    #else
      #include <BlynkSimpleEthernet2.h>
      #warning Using Ethernet2 lib
      #define SHIELD_TYPE           "W5x00 using Ethernet2 Library"
    #endif
    
    #define BLYNK_SERVER_HARDWARE_PORT    8080
  #endif

    #if defined(BLYNK_INFO_DEVICE)
    #undef BLYNK_INFO_DEVICE
  #endif
  
  #define BLYNK_INFO_DEVICE       BOARD_TYPE
  
#endif    //USE_BLYNK_WM

#define W5100_CS        10
#define SDCARD_CS       4

#define BLYNK_HOST_NAME   "W5500-Master-Controller"

#endif      //defines_h
