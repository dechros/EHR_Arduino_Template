/**
  ******************************************************************************
    @attention 	Detaylı açıklamalar .cpp dosyasındadır.
  ******************************************************************************
*/
#ifndef __REFTIMER_H__
#define	__REFTIMER_H__
/* Includes *******************************************************************/
#include "stdint.h"
#include "stdbool.h"
#include "Arduino.h"
/* Public macro ***************************************************************/
/* Public define **************************************************************/
#define REFTIMER_LIS331_USING_TIME							0
#define REFTIMER_EXTERNAL_INT_TIME							1
#define REFTIMER_CALLBACK_TIME								2
#define REFTIMER_ANIMATION_TIME								3

/* Yukarıda kaç adet tanımlama varsa onun değeri yazılır. */
#define REFTIMER_SLOTCOUNT                                  12
/* Public typedef *************************************************************/
/* Public variables ***********************************************************/
/* Public function prototypes *************************************************/
void REFTIMER_SetupUnit(void);
void REFTIMER_Executer(void);
uint16_t REFTIMER_GetTimeDifference(uint8_t SlotNumber);
void REFTIMER_ResetTimeDifference(uint8_t SlotNumber);
/******************************************************************************/
#endif	/* __REFTIMER_H__ */

