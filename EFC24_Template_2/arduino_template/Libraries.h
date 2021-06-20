/**
  ******************************************************************************
  * @attention 	Bu k�t�phanenin tek amac� Libraries teki di�er k�t�phanelerin
  * 			�a��raca�� i�lemciye ba�l� k�t�phaneleri �a��rmas�d�r. STM32Fx
  * 			ailesine g�re otomatik �a��r�r.
  ******************************************************************************
  */
#ifndef __LIBRARIES_H__
#define __LIBRARIES_H__
/* Includes *******************************************************************/
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <ustring.h>
#include "stdint.h"
#include "stdbool.h"
/* Public macro ***************************************************************/
#define LIBRARIES_CHECK_IS_AREA_ALLOCATED(PTR)				{if(PTR == 0){while(1);}}
/* Public define **************************************************************/
/* Public typedef *************************************************************/
/* Public variables ***********************************************************/
/* Public function prototypes *************************************************/
/******************************************************************************/
#endif /* __LIBRARIES_H__ */
