 /*************************************************************************************************
    @file    	EF_Control_Library.h
    @authors  	Oktay KARAKAYA
    @version 	V1
    @date    	24/11/2018
    @brief		Kontrol ile ilgili genel fonksiyonların bulunduğu library dosyasına ait header
	dosyası. Detaylı açıklamalar EF_Control_Library.cpp içerisinde mevcuttur.
    @copyright	Copyright (c) 2017, EHR Elektronik Hidrolik ve Otomasyon A.Ş.
  ************************************************************************************************
    @attention
  ************************************************************************************************
    @revision list
  				V1-24/11/2018: ilk versiyon.
  ************************************************************************************************
*/
#include "Arduino.h"
#include "EF_EFC24_Library.h"
/* Global Variables */

 /* Integer Variables */


 /* Bool Variables */

	
 /* Local Variables */
 
 
void fun_moving_average_filter(uint16_t *Non_Filtered_Value, uint16_t *Filtered_Value,uint8_t Filter_Factor, uint8_t PLC_Analog_Pin);// Denenecek
void fun_average_filter(int16_t *Non_Filtered_Value,int16_t *Filtered_Value,uint8_t Filter_Factor, uint8_t PLC_Analog_Pin);//Denendi
long fun_linear(long x, long in_min, long in_max, long out_min, long out_max);
long tree_point_lineer(long adc_value, long adc_minus, long adc_center, long adc_plus, long angle_minus, long angle_plus, long zero_treshould);

void fb_low_pass_filter
(long Non_Filtered_Value, 
uint8_t Filter_Factor,
int16_t *Filtered_Value);

void fb_get_joystick_direction_and_percentage 
(boolean Emergency_Stop,
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
uint8_t *fault);


void fb_calculate_duty_out_with_ramp
(uint16_t percentage,
uint8_t Direction,
uint8_t Error,
uint8_t forward_start_ramp_step_time,
uint8_t forward_stop_ramp_step_time,
uint8_t backward_start_ramp_step_time,
uint8_t backward_stop_ramp_step_time,
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
uint8_t *current_backward_duty);

bool fun_digital_input_get_value (uint8_t PLC_Digital_Pin);
bool fun_analog_digital_input_get_value (uint8_t PLC_Analog_Pin);



