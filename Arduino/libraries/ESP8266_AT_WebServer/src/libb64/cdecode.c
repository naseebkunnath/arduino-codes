/****************************************************************************************************************************
   cdecoder.c - c source to a base64 decoding algorithm implementation
   For ESP8266 AT-command running shields

   This is part of the libb64 project, and has been placed in the public domain.
   For details, see http://sourceforge.net/projects/libb64

   ESP8266_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
   Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
   Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
   Licensed under MIT license
   Version: 1.0.6

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
    1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
    1.0.1   K Hoang      17/02/2020 Add support to server's lambda function calls
    1.0.2   K Hoang      22/02/2020 Add support to SAMD (DUE, ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit, etc) boards
    1.0.3   K Hoang      03/03/2020 Add support to STM32 (STM32,F0,F1, F2, F3, F4, F7, etc) boards
    1.0.4   K Hoang      19/03/2020 Fix bug. Sync with ESP8266WebServer library of core v2.6.3
    1.0.5   K Hoang      17/04/2020 Add support to SAMD51 and SAM DUE boards
    1.0.6   K Hoang      11/06/2020 Add support to nRF52 boards, such as AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, 
                                    Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc. 
 *****************************************************************************************************************************/

#include "cdecode.h"

int base64_decode_value(char value_in) {
  static const char decoding[] = {62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};
  static const char decoding_size = sizeof(decoding);
  value_in -= 43;
  if (value_in < 0 || value_in > decoding_size) return -1;
  return decoding[(int)value_in];
}

void base64_init_decodestate(base64_decodestate* state_in) {
  state_in->step = step_a;
  state_in->plainchar = 0;
}

int base64_decode_block(const char* code_in, const int length_in, char* plaintext_out, base64_decodestate* state_in) {
  const char* codechar = code_in;
  char* plainchar = plaintext_out;
  char fragment;

  *plainchar = state_in->plainchar;

  switch (state_in->step) {
      while (1) {
      case step_a:
        do {
          if (codechar == code_in + length_in) {
            state_in->step = step_a;
            state_in->plainchar = *plainchar;
            return plainchar - plaintext_out;
          }
          fragment = (char)base64_decode_value(*codechar++);
        } while (fragment < 0);
        *plainchar    = (fragment & 0x03f) << 2;
      case step_b:
        do {
          if (codechar == code_in + length_in) {
            state_in->step = step_b;
            state_in->plainchar = *plainchar;
            return plainchar - plaintext_out;
          }
          fragment = (char)base64_decode_value(*codechar++);
        } while (fragment < 0);
        *plainchar++ |= (fragment & 0x030) >> 4;
        *plainchar    = (fragment & 0x00f) << 4;
      case step_c:
        do {
          if (codechar == code_in + length_in) {
            state_in->step = step_c;
            state_in->plainchar = *plainchar;
            return plainchar - plaintext_out;
          }
          fragment = (char)base64_decode_value(*codechar++);
        } while (fragment < 0);
        *plainchar++ |= (fragment & 0x03c) >> 2;
        *plainchar    = (fragment & 0x003) << 6;
      case step_d:
        do {
          if (codechar == code_in + length_in) {
            state_in->step = step_d;
            state_in->plainchar = *plainchar;
            return plainchar - plaintext_out;
          }
          fragment = (char)base64_decode_value(*codechar++);
        } while (fragment < 0);
        *plainchar++   |= (fragment & 0x03f);
      }
  }
  /* control should not reach here */
  return plainchar - plaintext_out;
}

int base64_decode_chars(const char* code_in, const int length_in, char* plaintext_out) {
  base64_decodestate _state;
  base64_init_decodestate(&_state);
  int len = base64_decode_block(code_in, length_in, plaintext_out, &_state);
  if (len > 0) plaintext_out[len] = 0;
  return len;
}
