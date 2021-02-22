/* -----------------------------------------------------------------------------
[FILE NAME]    :	main.c

[AUTHOR]       :	MOHANAD K. SAEED

[DATA CREATED] :	20/02/2021

[DESCRIPTION]  :	Mini Project 2 Control the motor speed through the
					potentiometer. The potentiometer is connected through
					the ADC driver and motor speed is controlled though the
					PWM signal. Also activate INT1 with raising it should
					revert the motor direction whatever the speed of the motor.

					ADC Driver uses interrupt mode
------------------------------------------------------------------------------*/
#define F_CPU 1000000UL
#include "DC Motor Driver/dc_motor.h"
#include "ADC/adc.h"
#include "External_Interrupt/external_interrupts.h"
#include "LCD Driver/lcd.h"

extern g_adcResult;

/*Call back Function for interrupt 1 which is used to revert the motor direction*/
void revertMotorDir(void){
	DCMOTOR_revertRotationDirection();
}

int main(void){
	//uint16 res_value;  /*In case we use polling mode for ADC*/
	uint8 percentage;
	Adc_ConfigType adc;
	Ei_ConfigType int1;
	Dcmotor_ConfigType motor;
	Timer0_ConfigType PWM;

	SET_BIT(SREG,7);
	/*Set the configurations of the ADC driver*/
	adc.clock=ADC_F_CPU_8;
	adc.channelnumber=ADC0;

	/*Set the configurations of the External Interrupt driver*/
	int1.interruptevent=INT_RISING_EDGE;
	int1.pullup=INT_DEACTIVATE;

	/*Set the configurations of the DC Motor driver*/
	motor.rotationDirection=CW;
	motor.speedPercentage=100;

	/*Initialize the External Interrupt 1 driver*/
	INT1_init(&int1);
	/*Set the call back function of interrupt 1*/
	INT1_setCallBack(revertMotorDir);

	/*Initialize the DC Motor driver*/
	DCMOTOR_init(&motor,&PWM);

	LCD_init(); /* initialize LCD driver */
	ADC_init(&adc); /* initialize ADC driver */
	LCD_clearScreen(); /* clear LCD at the beginning */
	/* display this string "ADC Value = " only once at LCD */
	LCD_displayString("ADC Value = ");
	LCD_goToRowColumn(1,0);
	LCD_displayString("MotorSpeed=");
	LCD_goToRowColumn(1,15);
	LCD_displayString("%");

	while(1){
		LCD_goToRowColumn(0,12); /* display the number every time at this position */
		ADC_readChannel(ADC0); /* read channel zero where the potentiometer is connect */
		LCD_intgerToString(g_adcResult ); /* display the ADC value on LCD screen */
		percentage=(float)g_adcResult/1023*100;
		LCD_goToRowColumn(1,12); /* display the number every time at this position */
		LCD_intgerToString(percentage); /* display the ADC value on LCD screen */
		/*Change the speed of the dc motor after calculating the percentage*/
		/*This function takes the percentage of maximum speed*/
		DCMOTOR_changeSpeed(percentage);
	}
	return 0;
}
