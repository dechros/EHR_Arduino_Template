 /*************************************************************************************************
    @file    	EF_Control_Library.cpp
    @authors  	Oktay KARAKAYA
    @version 	V1
    @date    	24/11/2018
    @brief		Kontrol ile ilgili genel fonksiyonlarýn bulunduðu library dosyasý
    @copyright	Copyright (c) 2017, EHR Elektronik Hidrolik ve Otomasyon A.Þ.
  ************************************************************************************************
    @attention
  ************************************************************************************************
    @revision list
  				V1-24/11/2018: ilk versiyon.
  ************************************************************************************************
*/
 
 /*  Include Files	 */
 #include "EF_Control_Library.h"		
 #include "RefTimer.h"
 #include "Global_Variables.h"
 
 /* Define Variables */
  #define VARIABLE_RANGE_ERROR 								1
  #define INPUT_RANGE_ERROR 								2
  #define REFERENCE_ERROR		 							3
  #define JOYSTICK_ERROR		 							4
  #define DEADMAN_PROCESS_WRONG								5
  #define EMERGENCY_STOP_PUSHED								6
  #define STATE_OK()										0
  #define DIRECTION_FORWARD									1
  #define DIRECTION_BACK									2
  #define STOP												0
  #define MIN_INTERVAL										20
  #define STEP_NUMBER										1
 /* Define Functions */
  #define CLR(x,y) (x&=(~(1<<y))) //DigitalWrite yerine portu direk 0 yapmak istersek kullanabiliriz. Daha hýzlýdýr. Örnek:CLR(PORTB, 0);
  #define SET(x,y) (x|=(1<<y)) //DigitalWrite yerine portu direk 1 yapmak istersek kullanabiliriz. Daha hýzlýdýr. Örnek:SET(PORTB, 0);
 /* Global Variables */

 /* Local Variables */
long fun_linear(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



long tree_point_lineer(long adc_value, long adc_minus, long adc_center, long adc_plus, long angle_minus, long angle_plus, long zero_treshould)
{
	
	
	if ((adc_value < (adc_center + zero_treshould)) && (adc_value > (adc_center - zero_treshould)))
	{
		return 0 ;
	}
	
	
	if (adc_plus > adc_minus)
	{
		if (adc_value == adc_center)
		{
			return 0 ;
		}
		else if (adc_value > adc_center)
		{
			return fun_linear( adc_value, adc_center, adc_plus, 0,angle_plus);
		}
		else
		{
			return fun_linear( adc_value, adc_center, adc_minus, 0,angle_minus);
		}
	}
	else
	{
		if (adc_value == adc_center)
		{
			return 0 ;
		}
		else if (adc_value > adc_center)
		{
			return fun_linear( adc_value, adc_center, adc_minus, 0,angle_minus);
		}
		else
		{
			return fun_linear( adc_value, adc_center, adc_plus, 0,angle_plus);
			
		}
	}
	
	
}

void Average_Filter(uint16_t *Non_Filtered_Value, uint16_t *Filtered_Value,uint8_t Filter_Factor, uint8_t PLC_Analog_Pin)
{
	uint16_t adc_value=0;
	uint16_t avg_value=0;
	for (int i=0;i<FILTER_FACTOR;i++)
	{
		uint8_t buffer[2];
		adc_value = analogRead(PLC_Analog_Pin);
		avg_value+=adc_value;
	}
	
	adc_value=0;
	adc_value=avg_value/FILTER_FACTOR;
	*Non_Filtered_Value=adc_value;
	// convert it to temp
	*Filtered_Value=(*Filtered_Value*0.8)+(adc_value*0.2); //*0.32;//(adc_value/1023)*3.3*100
	// Fraction=(*Filtered_Value*100)-((uint8_t)*Filtered_Value*100);
}	

void fb_get_joystick_direction_and_percentage 
(
boolean	 Emergency_Stop,
uint16_t AnalogValue, 
uint16_t AdcMin, 
uint16_t AdcCenter, 
uint16_t AdcMax,
uint16_t AdcCenterDead,
uint16_t AdcMinAlarm, 
uint16_t AdcMaxAlarm,
uint8_t JoystickReverse, 
boolean DeadmanInput,
boolean *dead_man_enable,
uint16_t *percentage, 
uint8_t *direction,
uint8_t *fault)

{
	
	
	if (( DeadmanInput == true 	) && ( *percentage == 0 ))  {
		*dead_man_enable = true ;
	}
	else if	( DeadmanInput == false ){
		*dead_man_enable = false ;
	}
	if (Emergency_Stop==false)
	{
		*fault = EMERGENCY_STOP_PUSHED;
		*direction = STOP;
		*percentage = 0;
	}	
	else if (!(AdcMaxAlarm > AdcMax && AdcMax > AdcCenter && AdcCenter > AdcMin && AdcMin > AdcMinAlarm))
	{
		*fault = VARIABLE_RANGE_ERROR;
		*direction = STOP;
		*percentage = 0;
	}
	
	else if ((AdcCenterDead + AdcCenter) >= AdcMax)
	{
		*fault = VARIABLE_RANGE_ERROR;
		*direction = STOP;
		*percentage = 0;
	}
	
	else if ((AdcMin + AdcCenterDead) >= AdcCenter)
	{
		*fault = VARIABLE_RANGE_ERROR;
		*direction = STOP;
		*percentage = 0;
	}
	
	else if (AnalogValue > AdcMaxAlarm || AnalogValue < AdcMinAlarm)
	{
		*fault = INPUT_RANGE_ERROR;
		*direction = STOP;
		*percentage = 0;
	}
	
	else if (AnalogValue <= 0)
	{
		*fault = JOYSTICK_ERROR;
		*direction = STOP;
		*percentage = 0;
	}
	else if ((*dead_man_enable == false) && (AnalogValue > (AdcCenterDead+AdcCenter)|| AnalogValue < (AdcCenter-AdcCenterDead)))
	{
		*fault = DEADMAN_PROCESS_WRONG;
		*direction = STOP;
		*percentage = 0;
	}
	
    else{
	*fault = STATE_OK();
	}
	
	if (*fault == STATE_OK())
	{
		if (AnalogValue > AdcMax) {
			*direction = DIRECTION_FORWARD ;
			*percentage = 100 ;
		}
		else if (AnalogValue < AdcMin) {
			*direction = DIRECTION_BACK ;
			*percentage = 100 ;
			
		}
		else if (AnalogValue > (AdcCenter + AdcCenterDead) && AnalogValue <= AdcMax) {
			*direction = DIRECTION_FORWARD ;
			*percentage = map(AnalogValue, (AdcCenter + AdcCenterDead), AdcMax, 0, 100);
		}
		
		else if (AnalogValue < (AdcCenter - AdcCenterDead) && AnalogValue > AdcMin) {
			*direction = DIRECTION_BACK;
			*percentage = map(AnalogValue, (AdcCenter - AdcCenterDead), AdcMin, 0, 100);
		}
		
		else if (AnalogValue <= (AdcCenter + AdcCenterDead) && AnalogValue >= (AdcCenter - AdcCenterDead)) {
			*direction = STOP;
			*percentage = 0;
		}
		
		else {
			*direction = STOP ;
			*percentage = 0;
		}

		
		if (JoystickReverse) {
			if (*direction == DIRECTION_FORWARD) {
				*direction = DIRECTION_BACK;
			}
			else if (*direction == DIRECTION_BACK) {
				*direction = DIRECTION_FORWARD;
			}
		}
	}
}

void fb_calculate_duty_out_with_ramp (uint16_t percentage,
									uint8_t Direction,
									uint8_t Error,
									uint8_t backward_start_ramp_step_time,
									uint8_t backward_stop_ramp_step_time,
									uint8_t forward_start_ramp_step_time,
									uint8_t forward_stop_ramp_step_time,
									uint8_t TimeSlotForwardUp,
									uint8_t TimeSlotForwardDown,
									uint8_t TimeSlotBackUp,
									uint8_t TimeSlotBackDown,
									uint8_t ForwardMinPwm,
									uint8_t ForwardMaxPwm,
									uint8_t BackwardMinPwm,
									uint8_t BackwardMaxPwm,
									uint8_t pwm_ramp_step,
									uint8_t *current_forward_duty,
									uint8_t *current_backward_duty) {				
	if (!Error)	{
		uint8_t target_forward_duty;
		uint8_t target_backward_duty;	
		
		target_forward_duty	 = constrain(map(percentage,0,100,ForwardMinPwm,ForwardMaxPwm),0,255);
		target_backward_duty = constrain(map(percentage,0,100,BackwardMinPwm,BackwardMaxPwm),0,255);
				
		if(Direction == DIRECTION_FORWARD) {
				if (*current_backward_duty > 0){
					
					if (REFTIMER_GetTimeDifference(TimeSlotBackDown) > backward_stop_ramp_step_time)
					{
						*current_backward_duty = *current_backward_duty - pwm_ramp_step;
						REFTIMER_ResetTimeDifference(TimeSlotBackDown);
					}
				}
				else {
					*current_backward_duty = 0; 
					if (*current_forward_duty < ForwardMinPwm) {
						*current_forward_duty = ForwardMinPwm;
					}
					
					if(target_forward_duty > *current_forward_duty) {
						
						if (REFTIMER_GetTimeDifference(TimeSlotForwardUp) > forward_start_ramp_step_time)
						{
							*current_forward_duty = *current_forward_duty + pwm_ramp_step;
							REFTIMER_ResetTimeDifference(TimeSlotForwardUp);
						}
					}
					else if(target_forward_duty < *current_forward_duty){
						
						if (REFTIMER_GetTimeDifference(TimeSlotForwardDown) > forward_stop_ramp_step_time)
						{
							*current_forward_duty = *current_forward_duty-pwm_ramp_step;
							REFTIMER_ResetTimeDifference(TimeSlotForwardDown);
						}
					}
					else {
						*current_forward_duty = target_forward_duty; 
					}
				}
			}
		/* Geri yönde hareket rapmasý. */
		else if(Direction == DIRECTION_BACK) {
			if (*current_forward_duty > 0){
				
				if (REFTIMER_GetTimeDifference(TimeSlotForwardDown) > forward_stop_ramp_step_time) {
					*current_forward_duty = *current_forward_duty - pwm_ramp_step;
					REFTIMER_ResetTimeDifference(TimeSlotForwardDown);
				}
			}
			else {
				*current_forward_duty = 0; 
				if(target_backward_duty > *current_backward_duty) {
					if (*current_backward_duty < BackwardMinPwm) {
						*current_backward_duty = BackwardMinPwm;
					}
					if (REFTIMER_GetTimeDifference(TimeSlotBackUp) > backward_start_ramp_step_time) {
	
						*current_backward_duty = *current_backward_duty + pwm_ramp_step;
						REFTIMER_ResetTimeDifference(TimeSlotBackUp);
					}
				}
				else if(target_backward_duty < *current_backward_duty){
					if (REFTIMER_GetTimeDifference(TimeSlotBackDown) > backward_stop_ramp_step_time) {
						*current_backward_duty = *current_backward_duty - pwm_ramp_step;
						REFTIMER_ResetTimeDifference(TimeSlotBackDown);
					}
				}
				else {
					*current_backward_duty = target_backward_duty ; 
				}
			}
		}
		/* Joystick verilerinin mid de olduðu durumda tüm çýkýþlar sýfýrlanýr. */
		else if (Direction ==STOP)
		{
			if(*current_forward_duty > 0) {
				if (REFTIMER_GetTimeDifference(TimeSlotForwardDown) > forward_stop_ramp_step_time) {				
					*current_forward_duty = *current_forward_duty - pwm_ramp_step;
					REFTIMER_ResetTimeDifference(TimeSlotForwardDown);
				}
			}
			else {
				*current_forward_duty = 0;
			}
			if (*current_backward_duty > 0){
				if (REFTIMER_GetTimeDifference(TimeSlotBackDown) > backward_stop_ramp_step_time) {
					*current_backward_duty = *current_backward_duty - pwm_ramp_step;
					REFTIMER_ResetTimeDifference(TimeSlotBackDown);
				}
			}
			else {
				*current_backward_duty = 0;// analogWrite(PWMOutPinDown,0);
			}
		}
		else
		{
			*current_forward_duty = 0;
			*current_backward_duty = 0;
		}
	}
	//Eðer herhani bir hata durumu oluþursa systemi durduran kýsýmdýr.
	else {
		*current_forward_duty = 0;
		*current_backward_duty = 0;
	}
}


/************************************************************************/
/*                        Filter Functions                              */
/************************************************************************/


void fb_low_pass_filter(long Non_Filtered_Value, uint8_t Filter_Factor, int16_t *Filtered_Value)
{
	long Result = Non_Filtered_Value;
	for(uint8_t i=0; i<FILTER_FACTOR; i++) {
		Result = Result + (*Filtered_Value);
		Result = Result / 2;
	}
	*Filtered_Value = Result;
}


bool fun_digital_input_get_value (uint8_t PLC_Digital_Pin){
	return digitalRead(PLC_Digital_Pin);
}

bool fun_analog_digital_input_get_value (uint8_t PLC_Analog_Pin){
	return (analogRead(PLC_Analog_Pin)>MIN_ADC_FOR_ANALOG_TO_DIGITAL);
	
}
