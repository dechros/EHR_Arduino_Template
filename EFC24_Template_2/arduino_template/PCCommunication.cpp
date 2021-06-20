/**
  ******************************************************************************
  * @file    	PCCommunication.cpp
  * @authors  	Özkan ÞÝMÞEK
  * @version 	V1
  * @date    	12/01/2020
  * @brief
  * @copyright	Copyright (c) 2018, EHR Elektronik, All rights reserved.
  ******************************************************************************
  * @attention
  ******************************************************************************
  * @revision list
  *				V1-01/01/2000: Ýlk versiyon.
  ******************************************************************************
  */
/* Includes *******************************************************************/
#include "PCCommunication.h"
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
  * @param[in]		<param name> <description>
  * @param[out]		<param name> <description>
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  
  *****************************************************************************/
cPCCommunication::cPCCommunication(tPCCOMMUNICATION_CommunicationType CommunicationType,uint32_t StartID, Uart *Serial, MCP_CAN *CAN)
{
	/* Haberleþme tipine göre kurulum yapýlýr. Can ya da Serial */ 
	if(CommunicationType == PCCOMMUNICATION_CAN){
		/* Eðer CAN seçilmiþ ve adres atýlmýþsa kurulum yapýlýr */
		if(CAN != NULL){
			this->CAN = CAN;
			this->CommunicationType = CommunicationType;
			/* Can için bazý IDler StartID'ye göre ayarlanýr. */
			this->Settings.CommunicationSettings.ConnectionStatusID = StartID + 1;
			this->Settings.CommunicationSettings.ParametresSendID = StartID + 2;
			this->Settings.CommunicationSettings.VariablesStartID = StartID + 3;
			this->Settings.CommunicationSettings.ParametresRequestStartID = StartID + 4;
		}
	}
	else {
		/* Eðer serial seçilmiþse seriale özgü paket alýcý kütüphane kurulur */
		if(Serial != NULL){
			this->Serial = Serial;
			this->CommunicationType = CommunicationType;
			/* Paket alýcý kütüphanenin ayarlarý girilir ve kurulur */
			tPACKAGEPICKER_Settings PackagePickerSettings;
			PackagePickerSettings.DiscartingKeysArraySize = 0;
			PackagePickerSettings.MaxPackageHoldingCount = 20;
			PackagePickerSettings.MaxPackageSize = PACKAGEPICKER_MAX_RXTX_PACKAGE_SIZE;
			PackagePickerSettings.PackageStartKey = '>';
			PackagePickerSettings.PackageEndKey = '<';
			PackagePickerSettings.PackageReceivedFunction = NULL;
			this->PackagePickerHandle = PACKAGEPICKER_SetupUnit(&PackagePickerSettings);

			/* Serialde ID sýralamasý farklý iþler */
			this->Settings.CommunicationSettings.ConnectionStatusID = StartID+1;
			this->Settings.CommunicationSettings.ParametresSendID = StartID + 2;
			this->Settings.CommunicationSettings.VariablesStartID = StartID + 52;
			this->Settings.CommunicationSettings.ParametresRequestStartID = StartID + 102;

		}
	}
	/* Parametrelerin ve Deðiþkenlerin Sayfa ve Deðer numaralarýna ilk deðer atýlýr */
	this->Settings.Parametres.LastPageNumber = 0;
	this->Settings.Parametres.LastValueNumber = 0;
	this->Settings.Variables.LastPageNumber = 0;
	this->Settings.Variables.LastValueNumber = 0;
	this->ParametresAdressField.LastIndex = 0;
	this->VariablesAdressField.LastIndex = 0;
}
/*******************************************************************************
  * @brief      	
  * @param[in]		<param name> <description>
  * @param[out]		<param name> <description>
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  
  *****************************************************************************/
void cPCCommunication::Executer(void)
{
	/* Burasýný Debug ile kontrol et  */
	if(this->CommunicationType == PCCOMMUNICATION_SERIAL){
		uint16_t ReadableBytes = this->Serial->available();
		if(ReadableBytes > 0) {
			char Temp;
			for (uint16_t i = 0; i<ReadableBytes; i++){
				this->Serial->readBytes(&Temp,1);
				PACKAGEPICKER_ADataReceived(this->PackagePickerHandle,Temp);
			}
		}
		/* Alýnmýþ bir paket varsa iþlenir */
		while(PackagePickerHandle->Public.ReceivedPackageCount > 0){
			if (PACKAGEPICKER_GetPackage(this->PackagePickerHandle,(uint8_t*) this->Settings.SerialSettings.PackageBuffer,&this->Settings.SerialSettings.LastReceivedPackageSize)){
				this->AnalyzeReceivedPackage();
			}
		}
	}
	if(this->CommunicationType == PCCOMMUNICATION_CAN){
		if(CAN_MSGAVAIL == this->CAN->checkReceive())
		{
			this->AnalyzeReceivedPackage();
			
		}
	}
	
}
/*******************************************************************************
  * @brief      	
  * @param[in]		<param name> <description>
  * @param[out]		<param name> <description>
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  
  *****************************************************************************/
void cPCCommunication::FillAdressField(tPCCOMMUNICATION_AdressType AdressType, int16_t *ValueStartAdress, uint16_t Length)
{

	int16_t *ValueAdress = ValueStartAdress;
	if (AdressType == PCCOMMUNICATION_PARAMETRE){
		this->ParametresAdressField.Adresses[this->ParametresAdressField.LastIndex] = ValueAdress;
		this->ParametresAdressField.AdressEndPoints[this->ParametresAdressField.LastIndex] = Length/2;
		this->ParametresAdressField.LastIndex++;
		this->Settings.Parametres.LastValueNumber += (Length/2) % 20;
		this->Settings.Parametres.LastPageNumber += (Length/2) / 20;
	}
	else{
		this->VariablesAdressField.Adresses[this->VariablesAdressField.LastIndex] = ValueAdress;
		this->VariablesAdressField.AdressEndPoints[this->VariablesAdressField.LastIndex] = Length/2;
		this->VariablesAdressField.LastIndex++;
		this->Settings.Variables.LastValueNumber += (Length/2) % 20;
		this->Settings.Variables.LastPageNumber += (Length/2) / 20;
	}
	
}
/*******************************************************************************
  * @brief      	
  * @param[in]		<param name> <description>
  * @param[out]		<param name> <description>
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  
  *****************************************************************************/
void cPCCommunication::AnalyzeReceivedPackage(void)
{
	if(this->CommunicationType == PCCOMMUNICATION_SERIAL){
		uint16_t ParameterSize;
		uint8_t MsgIDText[9];
		uint32_t MsgID;
		bool Result;

		Result = PACKAGEPARSER_GetParameter((uint8_t*) this->Settings.SerialSettings.PackageBuffer, this->Settings.SerialSettings.LastReceivedPackageSize,1, ',',MsgIDText, &ParameterSize, NULL, NULL);
		if(!Result) {
			/* Parametre hatasÄ±*/
			return;
		}
		MsgID = strtoul((char*) MsgIDText,NULL,16);
		if (MsgID ==  this->Settings.CommunicationSettings.ConnectionStatusID){
			this->CommunicationCheck();
		}
	}
	if(this->CommunicationType == PCCOMMUNICATION_CAN){
		if(this->Settings.CommunicationSettings.CANCommunicationStarted){
			
		}
		this->CAN->readMsgBuf(&this->Settings.CANSettings.Length,this->Settings.CANSettings.CANRxBuffer);
		this->Settings.CANSettings.CanID = this->CAN->getCanId();
		if(this->Settings.CANSettings.CanID == this->Settings.CommunicationSettings.ConnectionStatusID){
			this->CommunicationCheck();
		}
	}

}
/*******************************************************************************
  * @brief      	
  * @param[in]		<param name> <description>
  * @param[out]		<param name> <description>
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  
  *****************************************************************************/
void cPCCommunication::CommunicationCheck(void)
{
		if(this->CommunicationType == PCCOMMUNICATION_SERIAL){
			bool Result;
			uint8_t CRCText[9];
			uint16_t CRCTextSize;
			uint16_t CRCStartIndex;
			uint32_t CRCResult;
			uint8_t Message[5];
			uint16_t MessageSize;

			/* CRC Ã§ekiliyor */
			Result = PACKAGEPARSER_GetParameter((uint8_t*) this->Settings.SerialSettings.PackageBuffer,this->Settings.SerialSettings.LastReceivedPackageSize, 3, ',', CRCText, &CRCTextSize, &CRCStartIndex, NULL);

			/* double check yapiliyor */
			if((!Result) || (CRCTextSize > 8)) {
				/* Beklenmeyen bir durum */
				return;
			}
			
			
			CRCResult = strtoul((char*)CRCText, NULL, 16);

			uint32_t CRCCalculated = this->CalculateCRC32Mpeg2((uint8_t*) this->Settings.SerialSettings.PackageBuffer, CRCStartIndex);

			/* CRC tutuyor mu */
			if(CRCResult != CRCCalculated) {
				/* CRC tutmamýþ */
				return;
			}
			else{
				/* CRC Tutmuþ Message alýnýr */
				Result = PACKAGEPARSER_GetParameter((uint8_t*) this->Settings.SerialSettings.PackageBuffer,this->Settings.SerialSettings.LastReceivedPackageSize, 2, ',', Message, &MessageSize, NULL, NULL);
				if((!Result) || (MessageSize > 4)) {
					/* Beklenmeyen bir durum */
					return;
				}
				/* Eðer CNNT gelmiþ ve doðrulanmýþsa PC yazýlýmýna bilgiler gönderilir. */
				if(!memcmp(Message, "CNNT", strlen("CNNT"))) {
					
					/* Strlen fonksiyonu hata yapmamasý için Transmit Bufferýn tamamýna NULL atanýr */
					memset(this->Settings.SerialSettings.TransmitBuffer,NULL,sizeof(this->Settings.SerialSettings.TransmitBuffer));
					
					/*PCCOMMUNICATION_CONNECTION_STATUS_ID ile  cevap verilir ve Yazýlýmda kullanýlacak olan sayfa ve deðerin son kaldýðý yer gönderilir */
					sprintf((char*)this->Settings.SerialSettings.TransmitBuffer, ">%x,%x,%x,%x,%x", (this->Settings.CommunicationSettings.ConnectionStatusID),
					this->Settings.Parametres.LastPageNumber,this->Settings.Parametres.LastValueNumber,
					this->Settings.Variables.LastPageNumber,this->Settings.Variables.LastValueNumber);
					
					/* Mesajýn sonuna CRC yerleþtirilir. */
					uint16_t Length = strlen(this->Settings.SerialSettings.TransmitBuffer);
					uint32_t CrcValue = CalculateCRC32Mpeg2((uint8_t*) &this->Settings.SerialSettings.TransmitBuffer[1],Length-1);
					sprintf(&this->Settings.SerialSettings.TransmitBuffer[Length],"%x<",CrcValue);
					
					/*Serialden basýlýr */
					this->Serial->write((char*)&this->Settings.SerialSettings.TransmitBuffer,strlen(this->Settings.SerialSettings.TransmitBuffer));
				}
				/* Eðer CNOK gelmiþ ve doðrulanmýþsa Haberleþme baþlamaya hazýrdýr. */
				else if(!memcmp(Message, "CNOK", strlen("CNOK"))) {
					
					this->Settings.CommunicationSettings.SerialCommunicationStarted = true;
				}
				
				/* Herþey doðru ancak message yanlýþ gelmiþ CNRQ komutu gönderilir */
				else {
					
				}
				
			}
		}
		if(this->CommunicationType == PCCOMMUNICATION_CAN){
			
			uint16_t CRCResult = this->Settings.CANSettings.CANRxBuffer[6] * 256 + this->Settings.CANSettings.CANRxBuffer[7];
			uint16_t CRCCalculated = this->CalculateCRC16Arc(this->Settings.CANSettings.CANRxBuffer,6);
			
			if(CRCResult != CRCCalculated) {
				/* CRC tutmamýþ */
				return;
			}
			else{
				if(strncmp((char*)this->Settings.CANSettings.CANRxBuffer, "CNNT", 4) == 0){
					memset(this->Settings.CANSettings.CANTxBuffer,NULL,sizeof(this->Settings.CANSettings.CANTxBuffer));
					this->Settings.CANSettings.CANTxBuffer[0] = this->Settings.Parametres.LastPageNumber;
					this->Settings.CANSettings.CANTxBuffer[1] = this->Settings.Parametres.LastValueNumber;
					this->Settings.CANSettings.CANTxBuffer[2] = this->Settings.Variables.LastPageNumber;
					this->Settings.CANSettings.CANTxBuffer[3] = this->Settings.Variables.LastValueNumber;
					
					CRCCalculated = this->CalculateCRC16Arc(this->Settings.CANSettings.CANTxBuffer,6);
					
					this->Settings.CANSettings.CANTxBuffer[6] = CRCCalculated>>8;
					this->Settings.CANSettings.CANTxBuffer[7] = CRCCalculated;
					this->CAN->sendMsgBuf(this->Settings.CommunicationSettings.ConnectionStatusID,0,8,this->Settings.CANSettings.CANTxBuffer);
					
				}
				else if(strncmp((char*)this->Settings.CANSettings.CANRxBuffer, "CNOK", 4) == 0){
					
					this->Settings.CommunicationSettings.CANCommunicationStarted = true; 
				}
			}
		}
}
/*******************************************************************************
  * @brief      	
  * @param[in]		<param name> <description>
  * @param[out]		<param name> <description>
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  
  *****************************************************************************/
void cPCCommunication::SendParametres(void){
	if(this->CommunicationType == PCCOMMUNICATION_SERIAL){
	}
	
	if(this->CommunicationType == PCCOMMUNICATION_CAN){
		memset(this->Settings.CANSettings.CANTxBuffer,NULL,sizeof(this->Settings.CANSettings.CANTxBuffer));
		int16_t Value = *(this->ParametresAdressField.Adresses[this->Settings.CANSettings.AdressIndex] + this->Settings.CANSettings.AdressShiftIndex);
		this->Settings.CANSettings.AdressShiftIndex++;
		if(this->Settings.CANSettings.AdressShiftIndex >= this->ParametresAdressField.AdressEndPoints[this->Settings.CANSettings.AdressIndex]){
			this->Settings.CANSettings.AdressIndex++;
			this->Settings.CANSettings.AdressShiftIndex = 0;
		}
		this->Settings.CANSettings.CANTxBuffer[0] = Value>>8;
		this->Settings.CANSettings.CANTxBuffer[1] = Value;
		
		Value = *(this->ParametresAdressField.Adresses[this->Settings.CANSettings.AdressIndex] + this->Settings.CANSettings.AdressShiftIndex);
		this->Settings.CANSettings.AdressShiftIndex++;
		if(this->Settings.CANSettings.AdressShiftIndex >= this->ParametresAdressField.AdressEndPoints[this->Settings.CANSettings.AdressIndex]){
			this->Settings.CANSettings.AdressIndex++;
			this->Settings.CANSettings.AdressShiftIndex = 0;
		}
		this->Settings.CANSettings.CANTxBuffer[2] = Value>>8;
		this->Settings.CANSettings.CANTxBuffer[3] = Value>>8;
		this->Settings.CANSettings.CANTxBuffer[4] = this->Settings.CANSettings.ParametresIndexes.CurrentPageNumber;
		this->Settings.CANSettings.CANTxBuffer[5] = this->Settings.CANSettings.ParametresIndexes.CurrentValueNumber;
		this->Settings.CANSettings.ParametresIndexes.CurrentValueNumber+2;
		if(this->Settings.CANSettings.ParametresIndexes.CurrentValueNumber>= 18){
			this->Settings.CANSettings.ParametresIndexes.CurrentPageNumber++;
			this->Settings.CANSettings.ParametresIndexes.CurrentValueNumber=0;
		}
		uint16_t CRCCalculated = this->CalculateCRC16Arc(this->Settings.CANSettings.CANTxBuffer,6);
		this->Settings.CANSettings.CANTxBuffer[6] = CRCCalculated>>8;
		this->Settings.CANSettings.CANTxBuffer[7] = CRCCalculated;
		this->CAN->sendMsgBuf(this->Settings.CommunicationSettings.ParametresSendID,0,8,this->Settings.CANSettings.CANTxBuffer);
	}
	
}
/*******************************************************************************
  * @brief      	
  * @param[in]		<param name> <description>
  * @param[out]		<param name> <description>
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  
  *****************************************************************************/
unsigned int cPCCommunication::CalculateCRC32Mpeg2(unsigned char *Message, size_t Length)
{
	size_t i, j;
	unsigned int crc, msb;

	crc = 0xFFFFFFFF;
	for(i = 0; i < Length; i++) {
		crc ^= (((unsigned int)Message[i])<<24);
		for (j = 0; j < 8; j++) {
			msb = crc>>31;
			crc <<= 1;
			crc ^= (0 - msb) & 0x04C11DB7;
		}
	}
	return crc;
	
}
/*******************************************************************************
  * @brief      	
  * @param[in]		<param name> <description>
  * @param[out]		<param name> <description>
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  
  *****************************************************************************/
uint16_t cPCCommunication::CalculateCRC16Arc (const uint8_t *Message, uint16_t Length)
{
	uint16_t out = 0;
	int bits_read = 0, bit_flag;
	uint16_t CRC16 = 0x8005;
	if(Message == NULL)
	return 0;

	while(Length > 0)
	{
		bit_flag = out >> 15;

		out <<= 1;
		out |= (*Message >> bits_read) & 1;

		bits_read++;
		if(bits_read > 7)
		{
			bits_read = 0;
			Message++;
			Length--;
		}

		if(bit_flag)
		out ^= CRC16;

	}

	int i;
	for (i = 0; i < 16; ++i) {
		bit_flag = out >> 15;
		out <<= 1;
		if(bit_flag)
		out ^= CRC16;
	}

	uint16_t crc = 0;
	i = 0x8000;
	int j = 0x0001;
	for (; i != 0; i >>=1, j <<= 1) {
		if (i & out) crc |= j;
	}

	return crc;
}
