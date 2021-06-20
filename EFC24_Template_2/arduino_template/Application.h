/*
 * IncFile1.h
 *
 * Created: 26.09.2018 21:15:49
 *  Author: Oktay Karakaya
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_


void project (void);
void prg_application_setup(void);

void prg_default_parameters(void);
void project_set_can_tx_buffer(void);

boolean fun_read_eeprom(boolean read_eeprom_enable);
boolean fun_write_eeprom(boolean write_eeprom_enable);

void fun_moving_average_filter(uint16_t *Non_Filtered_Value, uint16_t *Filtered_Value,uint8_t Filter_Factor, uint8_t PLC_Analog_Pin);// Denenecek
void fun_average_filter(int16_t *Non_Filtered_Value,int16_t *Filtered_Value,uint8_t Filter_Factor);//Denendi
long fun_linear(long x, long in_min, long in_max, long out_min, long out_max);
long tree_point_lineer(long adc_value, long adc_minus, long adc_center, long adc_plus, long angle_minus, long angle_plus, long zero_treshould);
#endif /* INCFILE1_H_ */