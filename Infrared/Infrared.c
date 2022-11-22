#include "Infrared.h"


int curr = 0;
int prev = 0;
int barCounter = 0;
int thickBar = 0;
int i = 0;
int mulCount = 0;
bool inBarCode = false;
bool isBlackBar = false;
bool isWhiteBar = false;
bool match = false;
int timerValues[11] = {0};
int multiplyCount[11] = {0};
int resultCount[11] = {0};
char *decode[] = {"A", "1000010010", "B", "0010010010", "C", "1010010000", "D", "0000110010",
                  "E", "1000110000", "F", "0010110000", "G", "0000011010", "H", "1000011000",
                  "I", "0010011000", "J", "0000111000", "K", "1000000110", "L", "0010000110",
                  "M", "1010000100", "N", "0000100110", "O", "1000100100", "P", "0010100100",
                  "Q", "0000001110", "R", "1000001100", "S", "0010001100", "T", "0000101100",
                  "U", "1100000010", "V", "0110000010", "W", "1110000000", "X", "0100100010",
                  "Y", "1100100000", "Z","0110100000"};

/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_64,          // SMCLK/1 = 3MHz
        TIMER_PERIOD,                           // 5000 tick period
//        TIMER_A_CLOCKSOURCE_ACLK,
//        TIMER_A_CLOCKSOURCE_DIVIDER_10,
//        32768,
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Disable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};

int Infraredmain(void)
{
    /* Halting the Watchdog  */
    MAP_WDT_A_holdTimer();

    /* Initializing Variables */
    curADCResult = 0;

    /*Configure Timer*/
    MAP_Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);
    Interrupt_enableInterrupt(INT_TA1_0);
    Interrupt_enableMaster();

    /* Setting Flash wait state */
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Setting DCO to 48MHz  */
    MAP_PCM_setPowerState(PCM_AM_LDO_VCORE1);
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

    /* Enabling the FPU for floating point operation */
    MAP_FPU_enableModule();
    MAP_FPU_enableLazyStacking();

    /* Initializing ADC (MCLK/1/4) */
    MAP_ADC14_enableModule();
//    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_4, 0);
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);
//    MAP_ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);



    /* Configuring P1.0 as output */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configuring GPIOs (5.5 A0) */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configuring ADC Memory */
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
    ADC_INPUT_A0, false);

    /* Configuring Sample Timer */
    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    /* Enabling/Toggling Conversion */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();

    /* Enabling interrupts */
    MAP_ADC14_enableInterrupt(ADC_INT0);
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();

    while (1)
    {
//        printf("Testing");
        MAP_PCM_gotoLPM0();
    }

}

void TA1_0_IRQHandler(void)
{
//    printf("Timmer Ended\n");
    //Count the number of times the timer ends to calculate the timerValue
    mulCount++;
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

/* ADC Interrupt Handler. This handler is called whenever there is a conversion that is finished for ADC_MEM0. */
void ADC14_IRQHandler(void)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);
//    int z = CS_getACLK();
//            printf("ACLK -> %d\n", z);
//            z = CS_getMCLK();
//            printf("MCLK -> %d\n", z);
//            z = CS_getSMCLK();
//            printf("SMCLK -> %d\n", z);

    if (ADC_INT0 & status)
    {
        curADCResult = MAP_ADC14_getResult(ADC_MEM0);
        normalizedADCRes = (curADCResult * 3.3) / 16384;

        /*Check if an transition occured*/
        prev = curr;
        /* If white is detected, turn on P1.0 */
        if(curADCResult < 8000){
            /*Remove this*/
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
//            printf("On White\n");
            isWhiteBar = true;
            isBlackBar = false;
            curr = 0;
        }else{
            /*If Black*/
            /*Remove this*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
//            printf("On Black\n");
            isBlackBar = true;
            isWhiteBar = false;
            curr = 1;
        }
        printf("%d\n", curADCResult);
        MAP_ADC14_toggleConversionTrigger();
    }
//    printf("testing barCounter: ");

    //Checking if its the begining of the barcode
    if(isBlackBar && barCounter == 0){
//        barCounter = 1;
        //Clearing the String
        code[11] = '\0';
        //Indicator that currently still part of the barcode
        inBarCode = true;
//        printf("Barcode Start\n");
    }

    //Check if is reading a barcode
    if(inBarCode){
        //Check if transition occurred transitions
        if(prev != curr){
//            printf("Transition, multiplier Count -> %d\n", mulCount);
            //Increase count on transitions
            barCounter++;
            //Get the length of a Thick Bar -> 2nd Bar is always a thick bar
            if(isWhiteBar && barCounter == 2){
//                printf("Bar 2 - White -> Start Timer");
                //Clear and Start Timer
                Timer_A_clearTimer(TIMER_A1_BASE);
                MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
            }
            if(isBlackBar && barCounter == 3){
                //Stop timer and store value of thick bar in Array timerValues
//                printf("Get Thick Bar value -> StopTimer & Store Value -> ");
                MAP_Timer_A_stopTimer(TIMER_A1_BASE);
                thickBar = Timer_A_getCounterValue(TIMER_A1_BASE);
                timerValues[10] = thickBar;
                multiplyCount[10] = mulCount;
//                Timer_A_clearTimer(TIMER_A1_BASE);
//                printf("Thickbar: %d\n", thickBar);
            }

            //Get length of each bar between the asterix
            if(barCounter >= 11 && barCounter < 22){
                if(isBlackBar && barCounter == 11){
//                    printf("Bar 11 (Start of A) -> Start Timer");
                    Timer_A_clearTimer(TIMER_A1_BASE);
                    MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
                }
                else if(isBlackBar && barCounter == 21){
//                    printf("Bar 19 (End of A) -> End Timer, Store Value");
                    MAP_Timer_A_stopTimer(TIMER_A1_BASE);
                    timerValues[barCounter - 12] = Timer_A_getCounterValue(TIMER_A1_BASE);
                    multiplyCount[barCounter - 12] = mulCount;
                }else{
                    MAP_Timer_A_stopTimer(TIMER_A1_BASE);
                    timerValues[barCounter - 12] = Timer_A_getCounterValue(TIMER_A1_BASE);
                    multiplyCount[barCounter - 12] = mulCount;
//                    printf("All other Bars -> End -> Store value %d at timerValues[%d] -> Start\n", Timer_A_getCounterValue(TIMER_A1_BASE), barCounter-12);
                    Timer_A_clearTimer(TIMER_A1_BASE);
                    i = Timer_A_getCounterValue(TIMER_A1_BASE);
//                    printf("timer value after clear: %d\n", i);
                    MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
                }
            }
            mulCount = 0;
        }
        else if(barCounter >= 30){
            barCounter = 0;
//            printf("Barcode Ends\n");
            inBarCode = false;
            //Calculate timer values
            int x,y;
            for(x = 0; x < 11; x++){
                resultCount[x] = (TIMER_PERIOD * multiplyCount[x]) + timerValues[x];
//                if(x%2 == 0){       //if is even -> means blackbar
            }
            //Convert Values to Code (1s and 0s)
            for(y = 0; y < 10; y++){
                if(resultCount[y] > resultCount[10]){       //Meaning Thickbar
                    strcat(code, "1");
                }
                else{
                    strcat(code, "0");
                }
            }
            printf("Code: %s\n", code);
            //match code
            i = 0;
                while (match == false)
                {
//                     printf("%s\n", decode[i]);
                    if (i % 2 == 1)
                    {
                        int res = strcmp(code, decode[i]);
                        if (res == 0)
                        {
                            printf("Decoded message: %s\n", decode[i - 1]);
                            memset(code,0,sizeof(code));
                            match = true;
                        }
                        if(i > 52){
                            printf("No Match !!\n");
                            memset(code,0,sizeof(code));
                            match = true;
                        }
                    }
                    i++;
                }
        }
//        printf("Counter: %d\n", barCounter);
    }
}

void calculateTime(){

}

