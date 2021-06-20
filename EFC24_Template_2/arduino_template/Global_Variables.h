/*
 * IncFile1.h
 *
 * Created: 3.07.2018 00:38:11
 *  Author: Oktay Karakaya
 */ 


#ifndef Global_Variables_H_
#define Global_Variables_H_
#include "Arduino.h"
#include "EF_EFC24_Library.h"


typedef struct tsPWM_Valve_Parameters {
	uint8_t min_duty;
	uint8_t max_duty;
	uint16_t upper_ramp_time;
	uint16_t lower_ramp_time;
}tPWM_Valve_Parameters;


typedef struct tsIO_Variables {
	
	
}tIO_Variables;





typedef struct tsEprom_Parameters {
							
		uint16_t	default_empty_parameter					;

} tEprom_Parameters;

typedef struct tsGlobal_Variables {

		
		boolean		parameter_alarm_flag;
		boolean		high_level_alarm_flag;
		boolean		eeprom_write_successfull;
		boolean		eeprom_read_successfull;
		boolean		alarm_array[128]		; /* Array for alarms */
		boolean		warning_array[128]		; /* Array for warnings */
		boolean		timer_input;	
		uint16_t		analog_input[5]	; /* Array for analog inputs */
		
		uint16_t	pwm_output[8]		; /* Array for pwm outputs */
		uint16_t			high_time;
		uint16_t			low_time;
		
		float		time_period;
		float		frequency;
		uint16_t	plc_cycle_time;
		uint16_t	previous_micros ;
		uint16_t	current_micros ;
		
		uint16_t	current_high_time_micros;
		uint16_t	current_low_time_micros;		
		uint16_t	previous_high_time;
		uint16_t	previous_low_time;	
		uint16_t	program_step;
	

		boolean		warning_occured_bool[64];
		
		
	tIO_Variables	io_					; /* Structure for All IO Variables */
	
	
}tGlobal_Variables;


extern tGlobal_Variables g_; /* All global variables are under the "g_" structure */
extern tEprom_Parameters p_; /* All parameters are under the "p_" structure */

#endif /* Global_Variables_H_ */