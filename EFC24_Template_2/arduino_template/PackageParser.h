/**
  ******************************************************************************
  * @attention 	Detaylý açýklama c dosyasýndadýr.
  ******************************************************************************
  */
#ifndef __PACKAGEPARSER_H__
#define __PACKAGEPARSER_H__
#ifdef __cplusplus
	extern "C" {
#endif
/* Includes *******************************************************************/
#include "Libraries.h"
/* Public macro ***************************************************************/
/* Public define **************************************************************/
/* Public typedef *************************************************************/
/* Public variables ***********************************************************/
/* Public function prototypes *************************************************/
bool PACKAGEPARSER_GetParameter(uint8_t *Package, uint16_t PackageSize, uint8_t ParameterIndex, uint8_t SeperatorKey,
		uint8_t *Parameter, uint16_t *ParameterSize, uint16_t *ParameterStartIndex, uint16_t *ParameterEndIndex);
/******************************************************************************/
#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif /* __PACKAGEPARSER_H__ */
