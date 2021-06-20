/**
  ******************************************************************************
  * @file    	.c
  * @authors  	÷zkan ﬁ›MﬁEK
  * @version 	V1
  * @date    	01/01/2000
  * @brief
  * @copyright	Copyright (c) 2018, ET‹M Elektronik, All rights reserved.
  ******************************************************************************
  * @attention
  ******************************************************************************
  * @revision list
  *				V1-01/01/2000: ›lk versiyon.
  ******************************************************************************
  */
/* Includes *******************************************************************/
#include "PCCommunicationSerial.h"
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
cPCCommunicationSerial::cPCCommunicationSerial(Uart *Serial,uint32_t StartID)
{
	this->Settings.Serial = Serial;
	
	

	/* Serialde ID s˝ralamas˝ farkl˝ i˛ler */
	this->Settings.CommunicationIDs.ConnectionStatusID = StartID;
	this->Settings.CommunicationIDs.ParametresID = StartID + 2;
	this->Settings.CommunicationIDs.VariablesID = StartID + 52;
	this->Settings.CommunicationIDs.ParametresReceiveID = StartID + 102;
		
	this->Settings.AdressFields.VariablesAdressField.AdressEndIndex= 0;
	this->Settings.AdressFields.VariablesAdressField.LastValueNumber = 0;
	this->Settings.AdressFields.VariablesAdressField.LastPageNumber = 0;
	this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber = 0;
	this->Settings.AdressFields.VariablesAdressField.CurrentValueNumber = 0;
	
	this->Settings.AdressFields.ParametresAdressField.AdressEndIndex= 0;
	this->Settings.AdressFields.ParametresAdressField.LastValueNumber = 0;
	this->Settings.AdressFields.ParametresAdressField.LastPageNumber = 0;
	this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber = 0;
	this->Settings.AdressFields.ParametresAdressField.CurrentValueNumber = 0;
		
	this->Settings.CommunicationSettings.ParametreSendTimer = new TON(200);
	this->Settings.CommunicationSettings.VariableSendTimer = new TON(200);
	this->Settings.CommunicationSettings.CommunicationStarted = false;
	this->Settings.CommunicationSettings.SendParametres = false;
	this->Settings.CommunicationSettings.ParametersRequested = false;
	
	

	
}
/*******************************************************************************
  * @brief      	
  * @param[in]		<param name> <description>
  * @param[out]		<param name> <description>
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  
  *****************************************************************************/
void cPCCommunicationSerial::Executer(void)
{

	if(this->Settings.CommunicationSettings.CommunicationStarted){
		if(this->Settings.CommunicationSettings.SendParametres){
			this->Settings.CommunicationSettings.ParametreSendTimer->process(true);
			if(this->Settings.CommunicationSettings.ParametreSendTimer->Q){
				this->SendParameters();
				this->Settings.CommunicationSettings.ParametreSendTimer->process(false);
			}
		}
		else{
			if(this->Settings.CommunicationSettings.ParametersRequested){
				this->Settings.CommunicationSettings.SendParametres = true;
				this->Settings.CommunicationSettings.ParametersRequested = false;
			}
		}
		// Dei˛kenler s¸rekli gˆnderilir.
		this->Settings.CommunicationSettings.VariableSendTimer->process(true);
		if(this->Settings.CommunicationSettings.VariableSendTimer->Q){
			this->SendVariables();
			this->Settings.CommunicationSettings.VariableSendTimer->process(false);
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
void cPCCommunicationSerial::FillAdressField(tPCCOMMUNICATIONSERIAL_AdressType AdressType, int16_t *ValueStartAdress, uint16_t Length)
{
	int16_t *ValueAdress = ValueStartAdress;
	if (AdressType == PCCOMMUNICATIONSERIAL_PARAMETRE){
		this->Settings.AdressFields.ParametresAdressField.Adress = ValueAdress;
		this->Settings.AdressFields.ParametresAdressField.AdressEndIndex = Length/2 - 1 ;
		this->Settings.AdressFields.ParametresAdressField.LastValueNumber = this->Settings.AdressFields.ParametresAdressField.AdressEndIndex % 20;
		this->Settings.AdressFields.ParametresAdressField.LastPageNumber = this->Settings.AdressFields.ParametresAdressField.AdressEndIndex / 20;
		
	}
	else{
		this->Settings.AdressFields.VariablesAdressField.Adress = ValueAdress;
		this->Settings.AdressFields.VariablesAdressField.AdressEndIndex = Length/2 - 1;
		this->Settings.AdressFields.VariablesAdressField.LastValueNumber = this->Settings.AdressFields.VariablesAdressField.AdressEndIndex % 20;
		this->Settings.AdressFields.VariablesAdressField.LastPageNumber = this->Settings.AdressFields.VariablesAdressField.AdressEndIndex / 20;
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
void cPCCommunicationSerial::NewDataReceived(uint8_t Data)
{
	if(Data == '>'){
		this->Settings.SerialSettings.PackageStarted = true;
		memset(this->Settings.SerialSettings.RxBuffer,0,sizeof(this->Settings.SerialSettings.RxBuffer));
		this->Settings.SerialSettings.LastReceivedPackageSize = 0;
	}
	else if(Data == '<')
	{
		this->Settings.SerialSettings.PackageStarted = false;
		
		// al˝nan ˆge kontrol : this->Settings.Serial->write(this->Settings.SerialSettings.RxBuffer,this->Settings.SerialSettings.LastReceivedPackageSize);
		AnalyzeReceivedPackage();
	}
	else if(this->Settings.SerialSettings.PackageStarted){
		this->Settings.SerialSettings.RxBuffer[this->Settings.SerialSettings.LastReceivedPackageSize] = Data;
		this->Settings.SerialSettings.LastReceivedPackageSize++;
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
void cPCCommunicationSerial::AnalyzeReceivedPackage(void)
{
	uint16_t ParameterSize;
	uint8_t MsgIDText[9];
	uint32_t MsgID;
	bool Result;

	Result = PACKAGEPARSER_GetParameter((uint8_t*) this->Settings.SerialSettings.RxBuffer, this->Settings.SerialSettings.LastReceivedPackageSize,1, ',',MsgIDText, &ParameterSize, NULL, NULL);
	if(!Result) {
		/* Parametre hatasƒ±*/
		return;
	}
	MsgID = strtoul((char*) MsgIDText,NULL,16);
	if (MsgID ==  this->Settings.CommunicationIDs.ConnectionStatusID){
		this->CommunicationCheck();
	}
	if (MsgID >= this->Settings.CommunicationIDs.ParametresReceiveID){
		this->CheckReceivedParameters(MsgID);
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
void cPCCommunicationSerial::CommunicationCheck(void)
{
	bool Result;
	uint8_t CRCText[9];
	uint16_t CRCTextSize;
	uint16_t CRCStartIndex;
	uint32_t CRCResult;
	uint8_t Message[5];
	uint16_t MessageSize;

	/* CRC √ßekiliyor */
	Result = PACKAGEPARSER_GetParameter((uint8_t*) this->Settings.SerialSettings.RxBuffer,this->Settings.SerialSettings.LastReceivedPackageSize, 3, ',', CRCText, &CRCTextSize, &CRCStartIndex, NULL);

	/* double check yapiliyor */
	if((!Result) || (CRCTextSize > 8)) {
		/* Beklenmeyen bir durum */
		return;
	}
	
	
	CRCResult = strtoul((char*)CRCText, NULL, 16);

	uint32_t CRCCalculated = this->CalculateCRC32Mpeg2((uint8_t*) this->Settings.SerialSettings.RxBuffer, CRCStartIndex);

	/* CRC tutuyor mu */
	if(CRCResult != CRCCalculated) {
		/* CRC tutmam˝˛ */
		return;
	}
	else{
		/* CRC Tutmu˛ Message al˝n˝r */
		Result = PACKAGEPARSER_GetParameter((uint8_t*) this->Settings.SerialSettings.RxBuffer,this->Settings.SerialSettings.LastReceivedPackageSize, 2, ',', Message, &MessageSize, NULL, NULL);
		if((!Result) || (MessageSize > 4)) {
			/* Beklenmeyen bir durum */
			return;
		}
		/* Eer CNNT gelmi˛ ve dorulanm˝˛sa PC yaz˝l˝m˝na bilgiler gˆnderilir. */
		if(!memcmp(Message, "CNNT", strlen("CNNT"))) {
			
			/* Strlen fonksiyonu hata yapmamas˝ iÁin Transmit Buffer˝n tamam˝na NULL atan˝r */
			memset(this->Settings.SerialSettings.TxBuffer,NULL,sizeof(this->Settings.SerialSettings.TxBuffer));
			
			/*PCCOMMUNICATION_CONNECTION_STATUS_ID+1 ile  cevap verilir ve Yaz˝l˝mda kullan˝lacak olan sayfa ve deerin son kald˝˝ yer gˆnderilir */
			sprintf((char*)this->Settings.SerialSettings.TxBuffer, ">%x,%x,%x,%x,%x", (this->Settings.CommunicationIDs.ConnectionStatusID+1),
			this->Settings.AdressFields.ParametresAdressField.LastPageNumber,this->Settings.AdressFields.ParametresAdressField.LastValueNumber,
			this->Settings.AdressFields.VariablesAdressField.LastPageNumber,this->Settings.AdressFields.VariablesAdressField.LastValueNumber);
			
			/* Mesaj˝n sonuna CRC yerle˛tirilir. */
			uint16_t Length = strlen(this->Settings.SerialSettings.TxBuffer);
			this->Settings.SerialSettings.TxBuffer[Length] = ',';
			uint32_t CrcValue = this->CalculateCRC32Mpeg2((uint8_t*) &this->Settings.SerialSettings.TxBuffer[1],Length);
			sprintf(&this->Settings.SerialSettings.TxBuffer[Length+1],"%x<",CrcValue);
			
			/*Serialden bas˝l˝r */
			this->Settings.Serial->write((char*)&this->Settings.SerialSettings.TxBuffer,strlen(this->Settings.SerialSettings.TxBuffer));
		}
		/* Eer CNOK gelmi˛ ve dorulanm˝˛sa Haberle˛me ba˛lamaya haz˝rd˝r. */
		else if(!memcmp(Message, "CNOK", strlen("CNOK"))) {
			
			this->Settings.CommunicationSettings.CommunicationStarted = true;
			this->Settings.CommunicationSettings.SendParametres = true;
		}
		else if(!memcmp(Message, "READ", strlen("READ"))) {
			
			this->Settings.CommunicationSettings.ParametersRequested = true;
		}
		else if(!memcmp(Message, "DCNT", strlen("DCNT"))) {
			this->Settings.CommunicationSettings.CommunicationStarted = false;
			this->Settings.CommunicationSettings.SendParametres = false;
			this->Settings.CommunicationSettings.ParametersRequested = false;
			
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
void cPCCommunicationSerial::SendParameters(void)
{
	memset(this->Settings.SerialSettings.TxBuffer,NULL,sizeof(this->Settings.SerialSettings.TxBuffer));
	if(this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber < this->Settings.AdressFields.ParametresAdressField.LastPageNumber){
		sprintf((char*)this->Settings.SerialSettings.TxBuffer, ">%x,", this->Settings.CommunicationIDs.ParametresID+this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber);
		int16_t *ValueStart;
		ValueStart = this->Settings.AdressFields.ParametresAdressField.Adress + (20 * this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber);
		
		this->AddRawDataToTransmitBuffer((uint8_t*)ValueStart, 40);
		int Length = strlen(this->Settings.SerialSettings.TxBuffer);
		uint32_t CrcValue = CalculateCRC32Mpeg2((uint8_t*) &this->Settings.SerialSettings.TxBuffer[1],Length-1);
		sprintf(&this->Settings.SerialSettings.TxBuffer[Length],"%x<",CrcValue);
		this->Settings.Serial->write((char*)&this->Settings.SerialSettings.TxBuffer,strlen(this->Settings.SerialSettings.TxBuffer));
		this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber++;
	}
	else if(this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber == this->Settings.AdressFields.ParametresAdressField.LastPageNumber){
		sprintf((char*)this->Settings.SerialSettings.TxBuffer, ">%x,", this->Settings.CommunicationIDs.ParametresID+this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber);
		int16_t *ValueStart;
		ValueStart = this->Settings.AdressFields.ParametresAdressField.Adress + (20 * this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber);
		this->AddRawDataToTransmitBuffer((uint8_t*)ValueStart, (this->Settings.AdressFields.ParametresAdressField.LastValueNumber+1) * 2);
		int Length = strlen(this->Settings.SerialSettings.TxBuffer);
		uint32_t CrcValue = CalculateCRC32Mpeg2((uint8_t*)&this->Settings.SerialSettings.TxBuffer[1],Length-1);
		sprintf(&this->Settings.SerialSettings.TxBuffer[Length],"%x<",CrcValue);
		this->Settings.Serial->write((char*)&this->Settings.SerialSettings.TxBuffer,strlen(this->Settings.SerialSettings.TxBuffer));
		this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber = 0;
		this->Settings.CommunicationSettings.SendParametres = false;
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
void cPCCommunicationSerial::SendVariables(void)
{
	memset(this->Settings.SerialSettings.TxBuffer,NULL,sizeof(this->Settings.SerialSettings.TxBuffer));
	if(this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber < this->Settings.AdressFields.VariablesAdressField.LastPageNumber){
		sprintf((char*)this->Settings.SerialSettings.TxBuffer, ">%x,", this->Settings.CommunicationIDs.VariablesID+this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber);
		int16_t *ValueStart;
		ValueStart = this->Settings.AdressFields.VariablesAdressField.Adress + (20 * this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber);
		
		this->AddRawDataToTransmitBuffer((uint8_t*)ValueStart, 40);
		int Length = strlen(this->Settings.SerialSettings.TxBuffer);
		uint32_t CrcValue = CalculateCRC32Mpeg2((uint8_t*) &this->Settings.SerialSettings.TxBuffer[1],Length-1);
		sprintf(&this->Settings.SerialSettings.TxBuffer[Length],"%x<",CrcValue);
		this->Settings.Serial->write((char*)&this->Settings.SerialSettings.TxBuffer,strlen(this->Settings.SerialSettings.TxBuffer));
		this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber++;
	}
	else if(this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber == this->Settings.AdressFields.VariablesAdressField.LastPageNumber){
		sprintf((char*)this->Settings.SerialSettings.TxBuffer, ">%x,", this->Settings.CommunicationIDs.VariablesID+this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber);
		int16_t *ValueStart;
		ValueStart = this->Settings.AdressFields.VariablesAdressField.Adress + (20 * this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber);
		this->AddRawDataToTransmitBuffer((uint8_t*)ValueStart, (this->Settings.AdressFields.VariablesAdressField.LastValueNumber+1) * 2);
		int Length = strlen(this->Settings.SerialSettings.TxBuffer);
		uint32_t CrcValue = CalculateCRC32Mpeg2((uint8_t*)&this->Settings.SerialSettings.TxBuffer[1],Length-1);
		sprintf(&this->Settings.SerialSettings.TxBuffer[Length],"%x<",CrcValue);
		this->Settings.Serial->write((char*)&this->Settings.SerialSettings.TxBuffer,strlen(this->Settings.SerialSettings.TxBuffer));
		this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber = 0;
		
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
void cPCCommunicationSerial::CheckReceivedParameters(uint32_t ID){
	int PageNumber = ID - this->Settings.CommunicationIDs.ParametresReceiveID;
	
	bool Result;
	uint8_t CRCText[9];
	uint16_t CRCTextSize;
	uint16_t CRCStartIndex;
	uint32_t CRCResult;
	uint16_t DataIndex;
	uint16_t DataSize = 0;

	/* CRC √ßekiliyor */
	Result = PACKAGEPARSER_GetParameter((uint8_t*) this->Settings.SerialSettings.RxBuffer,this->Settings.SerialSettings.LastReceivedPackageSize, 3, ',', CRCText, &CRCTextSize, &CRCStartIndex, NULL);

	/* double check yapiliyor */
	if((!Result) || (CRCTextSize > 8)) {
		/* Beklenmeyen bir durum */
		return;
	}
	
	
	CRCResult = strtoul((char*)CRCText, NULL, 16);

	uint32_t CRCCalculated = this->CalculateCRC32Mpeg2((uint8_t*) this->Settings.SerialSettings.RxBuffer, CRCStartIndex);

	/* CRC tutuyor mu */
	if(CRCResult != CRCCalculated) {
		/* CRC tutmam˝˛ */
		return;
	}
	else{
		/* CRC Tutmu˛ Message al˝n˝r */
		Result = PACKAGEPARSER_GetParameter((uint8_t*) this->Settings.SerialSettings.RxBuffer,this->Settings.SerialSettings.LastReceivedPackageSize, 2, ',', NULL, &DataSize, &DataIndex, NULL);
		if((!Result) || (DataSize > 80)) {
			/* Beklenmeyen bir durum */
			return;
		}
		int16_t *ValueStart;
		uint8_t ValueText[5];
		
		for (int16_t i = 0; i < DataSize/4 ;i++){
			memset(ValueText,0,sizeof(ValueText));
			ValueText[0] = this->Settings.SerialSettings.RxBuffer[DataIndex + 4*i];
			ValueText[1] = this->Settings.SerialSettings.RxBuffer[DataIndex + 1 + 4*i];
			ValueText[2] = this->Settings.SerialSettings.RxBuffer[DataIndex + 2 + 4*i];
			ValueText[3] = this->Settings.SerialSettings.RxBuffer[DataIndex + 3 + 4*i];
			int16_t Value = strtoul((char*) ValueText,NULL,16);
			
			*(this->Settings.AdressFields.ParametresAdressField.Adress + (20 * PageNumber + i)) = Value; 
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
void cPCCommunicationSerial::AddRawDataToTransmitBuffer(uint8_t *RawData, uint16_t RawDataSize)
{
	uint16_t i;
	/* S√ºrekli yeniden hesaplamamk i√ßin yazmaya ba√ælanacak yer hesap√æan√Ωp de√∞i√ækene atan√Ωyor */
	uint16_t StartIndex = strlen((char*)this->Settings.SerialSettings.TxBuffer);

	/* 2 karakter olarak s√ºrekli hex hesaplan√Ωp yaz√Ωl√Ωr */
	for(i=0; i<RawDataSize; i++) {
		sprintf((char*)(&this->Settings.SerialSettings.TxBuffer[StartIndex]), "%02x", RawData[i]);
		StartIndex += 2;
	}
	/* en sonuna ',' eklenir. */
	this->Settings.SerialSettings.TxBuffer[StartIndex] = ',';
	this->Settings.SerialSettings.TxBuffer[StartIndex+1] = NULL;
}
/*******************************************************************************
  * @brief      	
  * @param[in]		<param name> <description>
  * @param[out]		<param name> <description>
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  
  *****************************************************************************/
void cPCCommunicationSerial::HexRawDataToBinRawData(uint8_t *HexRawData, uint8_t *BinRawData, uint16_t BinRawDataSize) {
	uint16_t i;
	uint8_t TempHex[3] = {0, 0, 0};
	for(i=0; i<BinRawDataSize; i++) {
		TempHex[0] = HexRawData[i*2];
		TempHex[1] = HexRawData[i*2 + 1];
		BinRawData[i] = strtoul((char*)TempHex, NULL, 16);
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
uint32_t cPCCommunicationSerial::CalculateCRC32Mpeg2(unsigned char *Message, size_t Length)
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