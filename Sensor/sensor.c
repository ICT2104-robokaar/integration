#include "sensor.h"
// -------------------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------------------

uint32_t nn=0;
uint32_t countPtr=0;

void Initalise_HCSR04(void)
{
    /* Timer_A UpMode Configuration Parameter */
    const Timer_A_UpModeConfig upConfig =
    {
            TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
            TIMER_A_CLOCKSOURCE_DIVIDER_3,          // SMCLK/3 = 1MHz
            TICKPERIOD,                             // 1000 tick period
            TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
            TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
            TIMER_A_DO_CLEAR                        // Clear value
    };



    int a = CS_getSMCLK();

    /* Configure P1.0 and set it to LOW */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);


    /* Configuring P3.6 as Output */
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6);//
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);//
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P3, GPIO_PIN7);

    /* Configuring P6.6 as Output */
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6);//
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);//
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P6, GPIO_PIN7);

    /* Configuring P2.6 as Output */
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN6);//
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);//
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P4, GPIO_PIN7);

    /* Configuring Timer_A0 for Up Mode */
    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);


    /* Enabling interrupts and starting the timer */
    Interrupt_enableInterrupt(INT_TA0_0);


    //Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    //Timer_A_stopT
}
void TA0_0_IRQHandler(void)
{
    /* Increment global variable (count number of interrupt occurred) */
    SR04IntTimes++;
    SR04IntTimes1++;
    SR04IntTimes2++;

    /* Clear interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2);
}


// -------------------------------------------------------------------------------------------------------------------

// 10000 counter, effective clock 1/1mhz = 1ms, smclk3mhz, div = 3 , effective clk = 1mhz
//get ticks
// can chang counter , clk and divider
static uint32_t getHCSR04Time(void)
{
    uint32_t pulsetime=0;

    /* Number of times the interrupt occurred (1 interrupt = 1000 ticks)    */
    pulsetime = SR04IntTimes * TICKPERIOD;

    /* Number of ticks (between 1 to 999) before the interrupt could occur */
    pulsetime += Timer_A_getCounterValue(TIMER_A0_BASE);

    /* Clear Timer */
    Timer_A_clearTimer(TIMER_A0_BASE);

 //   Delay(3000);

    return pulsetime;
}

static uint32_t getHCSR04Time1(void)
{
    uint32_t pulsetime=0;

    /* Number of times the interrupt occurred (1 interrupt = 1000 ticks)    */
    pulsetime = SR04IntTimes1 * TICKPERIOD;

    /* Number of ticks (between 1 to 999) before the interrupt could occur */
    pulsetime += Timer_A_getCounterValue(TIMER_A0_BASE);

    /* Clear Timer */
    Timer_A_clearTimer(TIMER_A0_BASE);

 //   Delay(3000);

    return pulsetime;
}

static uint32_t getHCSR04Time2(void)
{
    uint32_t pulsetime=0;

    /* Number of times the interrupt occurred (1 interrupt = 1000 ticks)    */
    pulsetime = SR04IntTimes2 * TICKPERIOD;

    /* Number of ticks (between 1 to 999) before the interrupt could occur */
    pulsetime += Timer_A_getCounterValue(TIMER_A0_BASE);

    /* Clear Timer */
    Timer_A_clearTimer(TIMER_A0_BASE);

 //   Delay(3000);

    return pulsetime;
}

// -------------------------------------------------------------------------------------------------------------------

float getHCSR04Distance(void)
{
    uint32_t pulseduration = 0;
    float calculateddistance = 0;

    /* Generate 10us pulse at P3.6 */
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);//--------
  //  Delay(30);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);  // ------_____



    /* Wait for positive-edge, 3.7 = 0 i will wait here, wait for input */
    /*improvement set interrupt low to high or something*/
    while(GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN7) == 0);

    /* Start Timer */
    SR04IntTimes = 0;
    Timer_A_clearTimer(TIMER_A0_BASE);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    /* Detects negative-edge */
    /*stop when high to low*/
    while(GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN7) == 1);
    /* Stop Timer */
    Timer_A_stopTimer(TIMER_A0_BASE);

    /* Obtain Pulse Width in microseconds */
    pulseduration = getHCSR04Time();


    /* Calculating distance in cm */
    calculateddistance = (float)pulseduration / 58.0f;





    return calculateddistance;
}

float getHCSR04Distance1(void)
{
    uint32_t pulseduration = 0;
    float calculateddistance = 0;

    /* Generate 10us pulse at P3.6 */
    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN6);//--------
  //  Delay(30);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);  // ------_____



    /* Wait for positive-edge, 3.7 = 0 i will wait here, wait for input */
    /*improvement set interrupt low to high or something*/
    while(GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN7) == 0);

    /* Start Timer */
    SR04IntTimes1 = 0;
    Timer_A_clearTimer(TIMER_A0_BASE);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    /* Detects negative-edge */
    /*stop when high to low*/
    while(GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN7) == 1);
    /* Stop Timer */
    Timer_A_stopTimer(TIMER_A0_BASE);

    /* Obtain Pulse Width in microseconds */
    pulseduration = getHCSR04Time1();


    /* Calculating distance in cm */
    calculateddistance = (float)pulseduration / 58.0f;





    return calculateddistance;
}

float getHCSR04Distance2(void)
{
    uint32_t pulseduration = 0;
    float calculateddistance = 0;

    /* Generate 10us pulse at P3.6 */
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);//--------
  //  Delay(30);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);  // ------_____



    /* Wait for positive-edge, 3.7 = 0 i will wait here, wait for input */
    /*improvement set interrupt low to high or something*/
    while(GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN7) == 0);

    /* Start Timer */
    SR04IntTimes2 = 0;
    Timer_A_clearTimer(TIMER_A0_BASE);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    /* Detects negative-edge */
    /*stop when high to low*/
    while(GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN7) == 1);
    /* Stop Timer */
    Timer_A_stopTimer(TIMER_A0_BASE);

    /* Obtain Pulse Width in microseconds */
    pulseduration = getHCSR04Time2();


    /* Calculating distance in cm */
    calculateddistance = (float)pulseduration / 58.0f;





    return calculateddistance;
}

// -------------------------------------------------------------------------------------------------------------------

uint32_t sensorMain(void)
{
    printf("Running Sensor Main\n");
    Initalise_HCSR04();

}
/**
 * Get 4 samples for each direction, get the average and determine if it is a 1  or 0
 */
void getBorder(bool *left, bool *front, bool *right) {
    /*printf("Please enter the left front right boundary: ");
    int a = 0;
    int b = 0;
    int c = 0;
    scanf("%d %d %d", &a, &b, &c);
    *left = a == 1;
    *front = b == 1;
    *right = c == 1;*/
    while (countPtr < 4) {
        int dist = getHCSR04Distance();
        int dist1 = getHCSR04Distance1();
        int dist2 = getHCSR04Distance2();

        countval0[countPtr] = dist;
        countval1[countPtr] = dist1;
        countval2[countPtr] = dist2;
        countPtr++;
    }
    countPtr = 0;
    countstat=(countval0[0]+countval0[1]+countval0[2]+countval0[3])>>2;
    countstat1=(countval1[0]+countval1[1]+countval1[2]+countval1[3])>>2;
    countstat2=(countval2[0]+countval2[1]+countval2[2]+countval2[3])>>2;
    printf("s1 %d cm %d\n", (int)(countstat), countstat < MIN_DISTANCE);
    printf("s2 %d cm %d\n", (int)(countstat1), countstat1 < MIN_DISTANCE_FRONT);
    printf("s3 %d cm %d\n", (int)(countstat2), countstat2 < MIN_DISTANCE);
    printf("%d, %d, %d, \n", (int)(countstat), (int)(countstat1), (int)(countstat2));

    *left = countstat < MIN_DISTANCE;
    *front = countstat1 < MIN_DISTANCE_FRONT;
    *right = countstat2 < MIN_DISTANCE;

}
