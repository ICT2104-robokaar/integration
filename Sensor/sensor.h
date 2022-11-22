#ifndef SENSOR_H_
    #define SENSOR_H_

    #ifndef DRIVERLIB_H
        #define DRIVERLIB_H
        #include "driverlib.h"
    #endif

    #define MIN_DISTANCE    15.0f
    #define MIN_DISTANCE_FRONT    5.0f
    #define TICKPERIOD      1000

    uint32_t SR04IntTimes;
    uint32_t SR04IntTimes1;
    uint32_t SR04IntTimes2;
    uint32_t countval0[4];
    uint32_t countval1[4];
    uint32_t countval2[4];
    extern uint32_t countPtr;
    uint32_t countstat;
    uint32_t countstat1;
    uint32_t countstat2;
    uint32_t tval1, tval2, avgdist;
    extern uint32_t nn;

    void Initalise_HCSR04(void);
    void TA0_0_IRQHandler(void);
    static uint32_t getHCSR04Time(void);
    static uint32_t getHCSR04Time1(void);
    static uint32_t getHCSR04Time2(void);
    float getHCSR04Distance(void);
    float getHCSR04Distance1(void);
    float getHCSR04Distance2(void);
    uint32_t sensorMain(void);
    void getBorder(uint32_t *left, uint32_t *front, uint32_t *right);
#endif /* SENSOR_H_ */
