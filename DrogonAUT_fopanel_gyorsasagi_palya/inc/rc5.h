/*
 * rc5.h
 *
 *  Created on: 2018. jan. 23.
 *      Author: Roland
 */

#ifndef RC5_H_
#define RC5_H_


#define RISING_EDGE 1
#define FALLING_EDGE 2
#define SHORT_MIN 444
#define SHORT_MAX 1333
#define LONG_MIN 1334
#define LONG_MAX 2222


typedef enum {
    RC5EVENT_SHORTSPACE = 0,
    RC5EVENT_SHORTPULSE = 2,
    RC5EVENT_LONGSPACE  = 4,
    RC5EVENT_LONGPULSE  = 6,
    RC5EVENT_ERROR      = 8
} RC5EVENT;


typedef enum {
    RC5STATE_START1 = 0,
    RC5STATE_MID1   = 1,
    RC5STATE_MID0   = 2,
    RC5STATE_START0 = 3
} RC5STATE;

const unsigned char trans[] = {0x01,
                               0x91,
                               0x9B,
                               0xFB};



#endif /* RC5_H_ */
