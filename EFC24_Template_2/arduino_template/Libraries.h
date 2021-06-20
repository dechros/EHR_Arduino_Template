/**
  ******************************************************************************
  * @attention 	Bu kütüphanenin tek amacý Libraries teki diðer kütüphanelerin
  * 			çaðýracaðý iþlemciye baðlý kütüphaneleri çaðýrmasýdýr. STM32Fx
  * 			ailesine göre otomatik çaðýrýr.
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
