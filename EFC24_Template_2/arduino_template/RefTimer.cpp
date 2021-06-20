/**
  ******************************************************************************
    @file    	RefTimer.cpp
    @authors  	Onur KESKÝN
    @version 	V1
    @date    	01/01/2017
    @brief		Süre sayma iþlemleri için oluþturulmuþtur. Arduino kütüphaneleri 
				ile uyumlu yazýlmýþtýr. System millis() ile zaman bilgisini alýr.
    @copyright	Copyright (c) 2017, ElektroHidrolik Tic. ve Ltd. Þti., All rights reserved.
  ******************************************************************************
    @attention
  ******************************************************************************
    @revision list
  				V1-01/01/2017: ilk versiyon.
  ******************************************************************************
*/
/* Includes *******************************************************************/
#include "RefTimer.h"
/* Public variables ***********************************************************/
/* Private macro **************************************************************/
/* Private define *************************************************************/
/* Private typedef ************************************************************/
/* Private variables **********************************************************/
struct sREFTIMER_TimeSlots {
  uint16_t RefValue;
} REFTIMER_TimeSlots[REFTIMER_SLOTCOUNT];
uint16_t LastCheckTickTimer = 0;
/* Private function prototypes ************************************************/
/* Externs ********************************************************************/
/******************************************************************************/
/*******************************************************************************
    @brief      	Kurulum i?in gereklidir. ??lemci ?al??maya ba?lad???nda en
  					az bir kere ?a?r?lmal?d?r.
    @param			None
    @retval     	None
    @attention
  *****************************************************************************/
void REFTIMER_SetupUnit(void) {
  uint8_t i;
  for (i = 0; i < REFTIMER_SLOTCOUNT; i++) {
    REFTIMER_TimeSlots[i].RefValue = 0;
  }
}
/*******************************************************************************
    @brief      	Mainde while i?inde ?a?r?lmal?d?r.
    @param			None
    @retval     	None
    @attention
  *****************************************************************************/
void REFTIMER_Executer(void) {
  if (LastCheckTickTimer != millis()) {
    uint16_t TempTick;
    uint8_t i;
    uint8_t Fark;
    TempTick = millis();
    Fark = TempTick - LastCheckTickTimer;
    LastCheckTickTimer = TempTick;

    for (i = 0; i < REFTIMER_SLOTCOUNT; i++) {
      REFTIMER_TimeSlots[i].RefValue += Fark;
    }
  }
}
/*******************************************************************************
    @brief      	Ka? referans s?resi ge?ti?inin bilgisini d?nd?r?r.
    @param[in]		SlotNumber: h dosyas?ndaki REFTIMER_X parametresi girilir.
    @retval     	Ka? referans s?resi ge?ti?ini d?nd?r?r.
    @attention
  *****************************************************************************/
uint16_t REFTIMER_GetTimeDifference(uint8_t SlotNumber) {
  if (SlotNumber >= REFTIMER_SLOTCOUNT)
    return 0;
  return REFTIMER_TimeSlots[SlotNumber].RefValue;
}
/*******************************************************************************
    @brief      	Sayac? s?f?rlar.
    @param[in]		SlotNumber: h dosyas?ndaki REFTIMER_X parametresi girilir.
    @retval     	None
    @attention
  *****************************************************************************/
void REFTIMER_ResetTimeDifference(uint8_t SlotNumber) {
  if (SlotNumber >= REFTIMER_SLOTCOUNT)
    return;
  REFTIMER_TimeSlots[SlotNumber].RefValue = 0;
}
/******************************************************************************/
