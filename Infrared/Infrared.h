#ifndef INFRARED_H_
    #define INFRARED_H_

    #ifndef DRIVERLIB_H
        #define DRIVERLIB_H
        #include "driverlib.h"
    #endif

    #define TIMER_PERIOD 65535

    /* Standard Includes */
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdio.h>

    /* Statics */
    static volatile uint16_t curADCResult;
    static volatile float normalizedADCRes;
    extern int curr;
    extern int prev;
    extern int barCounter;
    extern int thickBar;
    extern int i;
    extern int mulCount;
    extern bool inBarCode;
    extern bool isBlackBar;
    extern bool isWhiteBar;
    extern bool match;
    extern int timerValues[11];
    extern int multiplyCount[11];
    extern int resultCount[11];
    char code[11];
    extern char *decode[];

    int Infraredmain(void);
    void TA1_0_IRQHandler(void);
    void ADC14_IRQHandler(void);
    void calculateTime();
#endif
