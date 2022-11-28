
#ifndef _MOTOR_H_
    #define _MOTOR_H_

    int motorMain(void);
    float getPIDOutputLeft();
    float getPIDOutputRight();
    void TA1_0_IRQHandler(void);
    void PORT2_IRQHandler(void);
#endif /* MOTOR_MOTOR_H_ */
