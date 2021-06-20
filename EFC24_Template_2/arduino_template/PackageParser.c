/**
  ******************************************************************************
  * @file    	PackageParser.c
  * @authors  	Özkan ÞÝMÞEK
  * @version 	V2
  * @date    	30/11/2019
  * @brief
  * @copyright	Copyright (c) 2019, EHR Elektronik, All rights reserved.
  ******************************************************************************
  * @attention
  ******************************************************************************
  * @revision list
  *				V1-30/11/2019: Ýlk versiyon.
  ******************************************************************************
  */
/* Includes *******************************************************************/
#include "PackageParser.h"
/* Public variables ***********************************************************/
/* Private macro **************************************************************/
/* Private define *************************************************************/
/* Private typedef ************************************************************/
/* Private variables **********************************************************/
/* Private function prototypes ************************************************/
/* Externs ********************************************************************/
/******************************************************************************/
/*******************************************************************************
  * @brief      	
  * @param[in]		Package: baþlangýç ve bitiþ karakteri gibi karakterleri içermeyen
  * 				sadece bir SeperatorKey ile birbirinden ayrýlmýþ parçalar içeren
  * 				char dizisidir.
  * @param[in]		PackageSize: Package in boyutudur.
  * @param[in]		ParameterIndex: 1 den baþlar. Kaçýncý parametrenin istendiðini
  * 				belirtir.
  * @param[in]		SeperatorKey: Parametreleri birbirinden ayýran ',' gibi bir
  * 				karakterdir.
  * @param[out]		Parameter: optional, NULL gelirse buraya birþey yazýlmaz. Eðer
  * 				parametre boyutu uzunsa ve alan ayrýlmak istenmiyorsa bu
  * 				deðiþkene NULL gönderilebilir. Aksi halde Package içinde parametre
  * 				bu alana yazýlýr. Yazma string yapýsýýnda olacaðýndan en sona
  * 				NULL eklenecek ve yeterli boyutun olduðu kullanýcýnýn sorumluluðundadýr.
  * @param[out]		ParameterSize: optional, NULL dýþýnda bir karakter gelirse
  * 				parameter ýn boyutunu belirtir.
  * @param[out]		ParameterStartIndex: optional, Eðer null dýþýnda bir karakter
  * 				gelirse istenen parametrenin Package içindeki 0 a göre baþlangýç
  * 				index deðerini verir.
  * @param[out]		ParameterEndIndex: optional, Eðer null dýþýnda bir karakter
  * 				gelirse istenen parametrenin Package içindeki 0 a göre bitiþ
  * 				index deðerini verir.
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  	Parametrenin boþ olmasý durumu bir hata deðildir. Fakat parametre
  * 				boþ olduðunda da parametrede 1 karakter olduðunda da ParameterStartIndex
  * 				ve ParameterEndIndex deðerleri ayný olur. Fakat parametre boþ olduðunda
  * 				ParameterSize deðeri 0, parametre 1 karakter olduðunda ise ParameterSize
  * 				deðeri 1 olur.
  *****************************************************************************/
bool PACKAGEPARSER_GetParameter(uint8_t *Package, uint16_t PackageSize, uint8_t ParameterIndex, uint8_t SeperatorKey,
		uint8_t *Parameter, uint16_t *ParameterSize, uint16_t *ParameterStartIndex, uint16_t *ParameterEndIndex) {
	uint16_t i;
	uint8_t Temp;
	uint16_t StartIndex;
	uint16_t EndIndex;
	uint16_t PSize;
	/* ParameterIndex 0 olamaz. */
	if(ParameterIndex == 0) {
		return false;
	}

	/* parametre deðeri 1 den baþlar ama baþlangýç yerini arama 0 dan baþlar */
	Temp = ParameterIndex-1;

	/* Parametrenin baþladýðý yer bulunuyor */
	if(Temp > 0) {
		for(i=0; i<PackageSize; i++) {
			if(Package[i] == SeperatorKey) {
				Temp--;
				if(Temp == 0) {
					break;
				}
			}
		}
		/* seperator keyden bir sonraki index baþlangýç yeridir */
		i = i + 1;
	}
	else {
		i = 0;
	}

	/* Yeterli parametre var mý*/
	if(Temp > 0) {
		/* yeterli parametre yok */
		return false;
	}

	/* Baþlangýç yeri bulunmuþ */
	StartIndex = i;
	if(ParameterStartIndex != NULL) {
		(*ParameterStartIndex) = StartIndex;
	}

	/* Bitiþ yeri aranýyor. */
	for(i = StartIndex; i<PackageSize; i++) {
		if(Package[i] == SeperatorKey) {
			break;
		}
	}

	/* i startýndex ise parametre boþ demektir. Bu durum hata deðildir */

	if(i == StartIndex) {
		EndIndex = StartIndex;

		/* Size buada hesaplanýr. */
		PSize = 0;
	}
	else {
		/* Bitiþ indexi seperatorkeyin bulunduðu konumunun ya da en sona gitmiþse
		 * en son konumun bir gerisindedir. */
		EndIndex = i-1;

		/* Size buada hesaplanýr. */
		PSize = EndIndex - StartIndex + 1;
	}

	/* End indexe deðer atanýr*/
	if(ParameterEndIndex != NULL) {
		(*ParameterEndIndex) = EndIndex;
	}

	/* Parametre size a deðer atanýr */
	if(ParameterSize != NULL) {
		(*ParameterSize) = PSize;
	}

	/* Artýk parametrenin konumu tam olarak belli. eðer alan ayrýlmýþsa kopyalama yapýlýr */
	if(Parameter != NULL) {
		memcpy(Parameter, &Package[StartIndex], PSize);
		Parameter[PSize] = NULL;
	}

	return true;
}
