/**
  ******************************************************************************
  * @file    	PackageParser.c
  * @authors  	�zkan ��M�EK
  * @version 	V2
  * @date    	30/11/2019
  * @brief
  * @copyright	Copyright (c) 2019, EHR Elektronik, All rights reserved.
  ******************************************************************************
  * @attention
  ******************************************************************************
  * @revision list
  *				V1-30/11/2019: �lk versiyon.
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
  * @param[in]		Package: ba�lang�� ve biti� karakteri gibi karakterleri i�ermeyen
  * 				sadece bir SeperatorKey ile birbirinden ayr�lm�� par�alar i�eren
  * 				char dizisidir.
  * @param[in]		PackageSize: Package in boyutudur.
  * @param[in]		ParameterIndex: 1 den ba�lar. Ka��nc� parametrenin istendi�ini
  * 				belirtir.
  * @param[in]		SeperatorKey: Parametreleri birbirinden ay�ran ',' gibi bir
  * 				karakterdir.
  * @param[out]		Parameter: optional, NULL gelirse buraya bir�ey yaz�lmaz. E�er
  * 				parametre boyutu uzunsa ve alan ayr�lmak istenmiyorsa bu
  * 				de�i�kene NULL g�nderilebilir. Aksi halde Package i�inde parametre
  * 				bu alana yaz�l�r. Yazma string yap�s��nda olaca��ndan en sona
  * 				NULL eklenecek ve yeterli boyutun oldu�u kullan�c�n�n sorumlulu�undad�r.
  * @param[out]		ParameterSize: optional, NULL d���nda bir karakter gelirse
  * 				parameter �n boyutunu belirtir.
  * @param[out]		ParameterStartIndex: optional, E�er null d���nda bir karakter
  * 				gelirse istenen parametrenin Package i�indeki 0 a g�re ba�lang��
  * 				index de�erini verir.
  * @param[out]		ParameterEndIndex: optional, E�er null d���nda bir karakter
  * 				gelirse istenen parametrenin Package i�indeki 0 a g�re biti�
  * 				index de�erini verir.
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  	Parametrenin bo� olmas� durumu bir hata de�ildir. Fakat parametre
  * 				bo� oldu�unda da parametrede 1 karakter oldu�unda da ParameterStartIndex
  * 				ve ParameterEndIndex de�erleri ayn� olur. Fakat parametre bo� oldu�unda
  * 				ParameterSize de�eri 0, parametre 1 karakter oldu�unda ise ParameterSize
  * 				de�eri 1 olur.
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

	/* parametre de�eri 1 den ba�lar ama ba�lang�� yerini arama 0 dan ba�lar */
	Temp = ParameterIndex-1;

	/* Parametrenin ba�lad��� yer bulunuyor */
	if(Temp > 0) {
		for(i=0; i<PackageSize; i++) {
			if(Package[i] == SeperatorKey) {
				Temp--;
				if(Temp == 0) {
					break;
				}
			}
		}
		/* seperator keyden bir sonraki index ba�lang�� yeridir */
		i = i + 1;
	}
	else {
		i = 0;
	}

	/* Yeterli parametre var m�*/
	if(Temp > 0) {
		/* yeterli parametre yok */
		return false;
	}

	/* Ba�lang�� yeri bulunmu� */
	StartIndex = i;
	if(ParameterStartIndex != NULL) {
		(*ParameterStartIndex) = StartIndex;
	}

	/* Biti� yeri aran�yor. */
	for(i = StartIndex; i<PackageSize; i++) {
		if(Package[i] == SeperatorKey) {
			break;
		}
	}

	/* i start�ndex ise parametre bo� demektir. Bu durum hata de�ildir */

	if(i == StartIndex) {
		EndIndex = StartIndex;

		/* Size buada hesaplan�r. */
		PSize = 0;
	}
	else {
		/* Biti� indexi seperatorkeyin bulundu�u konumunun ya da en sona gitmi�se
		 * en son konumun bir gerisindedir. */
		EndIndex = i-1;

		/* Size buada hesaplan�r. */
		PSize = EndIndex - StartIndex + 1;
	}

	/* End indexe de�er atan�r*/
	if(ParameterEndIndex != NULL) {
		(*ParameterEndIndex) = EndIndex;
	}

	/* Parametre size a de�er atan�r */
	if(ParameterSize != NULL) {
		(*ParameterSize) = PSize;
	}

	/* Art�k parametrenin konumu tam olarak belli. e�er alan ayr�lm��sa kopyalama yap�l�r */
	if(Parameter != NULL) {
		memcpy(Parameter, &Package[StartIndex], PSize);
		Parameter[PSize] = NULL;
	}

	return true;
}
