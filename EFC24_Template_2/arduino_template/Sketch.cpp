#include <Arduino.h>
#include "Global_Variables.h"
#include "EF_EFC24_Library.h"		/* Yazılımın yazıldığı ürüne ait library */
#include "EF_Standart_Library.h"	/* Bütün ürünlerde kullanılacak standart PLC fonksiyonlarının bulunduğu library */
//#include "Ef_Math_Library.h"		/* MAtematiksel fonksiyonların bulunduğu library */
#include "Application.h"			/* Bütün yazılımlarda ortak kullanılacak. Ana yazılım bu rutin içerisinde olacak */
//#include "ServiceTool.h"
#include "RefTimer.h"

TON Timer_Stabilization			(250);
TON Timer_eprom_reading_alarm	(250);
TON Timer_Serial_Port			(250);

void setup()
{
	prg_pinmode_setup();
	prg_application_setup();
}

void loop()
{
	
	//  /*************************  PROGRAM STEP 0  *********************************/
	//  System will wait until end of starting stabilization time for to be sure hardware stabilization. All outputs should be shut down for safety.
	//  Program donanımın stabil bir hale gelmesi için stabilizasyon zamanı kadar step0 da kalacak ve bu zaman süresince tüm çıkışlar kapatılacaktır.

	#pragma region program_step_0

	if ( g_.program_step == 0 ) {
		
		Timer_Stabilization.process(true);
		
		if ( Timer_Stabilization.Q == true ) {
			g_.program_step = 1;
			Timer_Stabilization.process(false);
			Serial1.println("Program Basladi");
		}
	}
	#pragma endregion program_step_0

	//  /***************************************************  PROGRAM STEP 1  **********************************************************************/
	//  Eprom Reading. All outputs should be shut down for safety.
	//  Epromdan programda kullanılan parametreler okunarak değişkenlere atanıyor. Bu süre içerisinde güvenlik içintüm çıkışlar kapatılacaktır.
	#pragma region program_step_1

	else if ( g_.program_step == 1 )
	{
		g_.eeprom_read_successfull= fun_read_eeprom(true);
		
		if ( Timer_eprom_reading_alarm.Q == false )
		{
			Timer_eprom_reading_alarm.process(true);
			if (g_.eeprom_read_successfull == true)
			{
				Serial1.println("Program Step 3");
				g_.program_step = 3; /* Go to main Loop */
				Timer_eprom_reading_alarm.process(false);
				prg_default_parameters();
			
			}
		}
		else
		{
			g_.program_step = 2;
			/* Go to Alarm Loop */
			// prg_alarm_occured   ( alarm_no := 3 ); ----Eprom reading alarm
		}
		
	}
	
	#pragma endregion program_step_1
	
	//  /****************************************************  PROGRAM STEP 2  ***************************************************************************/
	//   Alarm_Loop
	#pragma region program_step_2

	else if ( g_.program_step == 2 ){
		
	}
	#pragma endregion program_step_2
	
	//  /****************************************************  PROGRAM STEP 3  ***************************************************************************/
	//   Start Main Program
	#pragma region program_step_3

	else if ( g_.program_step == 3 )
	{
		// Send initial messages. Execute only once after reset.
	
		
		project();
	}
	
	
	else  {
		g_.program_step        = 0 ;
		
		//       prg_close_all_outputs();
		
	}
	
}
