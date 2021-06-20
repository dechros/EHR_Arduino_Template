/**
  ******************************************************************************
  * @file    	.c
  * @authors  	Özkan ÞÝMÞEK
  * @version 	V1
  * @date    	01/01/2000
  * @brief
  * @copyright	Copyright (c) 2018, ETÜM Elektronik, All rights reserved.
  ******************************************************************************
  * @attention
  ******************************************************************************
  * @revision list
  *				V1-01/01/2000: Ýlk versiyon.
  ******************************************************************************
  */
/* Includes *******************************************************************/
#include "PCCommunicationCan.h"
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
cPCCommunicationCan::cPCCommunicationCan(MCP_CAN *CAN,uint32_t StartID)
{
	this->Settings.CAN = CAN;
	this->Settings.CommunicationIDs.ConnectionStatusID = StartID;
	this->Settings.CommunicationIDs.ParametresID = StartID + 2;
	this->Settings.CommunicationIDs.VariablesID = StartID + 3;
	this->Settings.CommunicationIDs.ParametresReceiveID = StartID + 4;
	
	this->Settings.AdressFields.ParametresAdressField.AdressEndIndex= 0;
	this->Settings.AdressFields.ParametresAdressField.LastValueNumber = 0;
	this->Settings.AdressFields.ParametresAdressField.LastPageNumber = 0;
	this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber = 0;
	this->Settings.AdressFields.ParametresAdressField.CurrentValueNumber = 0;
	
	this->Settings.AdressFields.VariablesAdressField.AdressEndIndex= 0;
	this->Settings.AdressFields.VariablesAdressField.LastValueNumber = 0;
	this->Settings.AdressFields.VariablesAdressField.LastPageNumber = 0;
	this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber = 0;
	this->Settings.AdressFields.VariablesAdressField.CurrentValueNumber = 0;
	
	this->Settings.CommunicationSettings.ParametreSendTimer = new TON(20);
	this->Settings.CommunicationSettings.VariableSendTimer = new TON(50);
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
void cPCCommunicationCan::Executer(void)
{

	if(CAN_MSGAVAIL == this->Settings.CAN->checkReceive())
	{
		this->Settings.CAN->readMsgBuf(&this->Settings.CANSettings.Length,this->Settings.CANSettings.RxBuffer);
		this->Settings.CANSettings.ID = this->Settings.CAN->getCanId();
		if(this->Settings.CANSettings.ID == this->Settings.CommunicationIDs.ConnectionStatusID){
			this->CommunicationCheck();
		}
		if(this->Settings.CANSettings.ID == this->Settings.CommunicationIDs.ParametresReceiveID){
			this->CheckReceivedParameters();
		}
		
	}
	if(this->Settings.CommunicationSettings.CommunicationStarted){
		if(this->Settings.CommunicationSettings.SendParametres){
			this->Settings.CommunicationSettings.ParametreSendTimer->process(true);
			if(this->Settings.CommunicationSettings.ParametreSendTimer->Q){
				this->SendParametres();
				this->Settings.CommunicationSettings.ParametreSendTimer->process(false);
			}
		}
		else{
			if(this->Settings.CommunicationSettings.ParametersRequested){
				this->Settings.CommunicationSettings.SendParametres = true;
				this->Settings.CommunicationSettings.ParametersRequested = false;
			}
		}
		// Deðiþkenler sürekli gönderilir.
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
void cPCCommunicationCan::FillAdressField(tPCCOMMUNICATIONCAN_AdressType AdressType, int16_t *ValueStartAdress, uint16_t Length)
{
	int16_t *ValueAdress = ValueStartAdress;
	if (AdressType == PCCOMMUNICATIONCAN_PARAMETRE){
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
void cPCCommunicationCan::CommunicationCheck(void)
{
	uint16_t CRCResult = this->Settings.CANSettings.RxBuffer[7] * 256 + this->Settings.CANSettings.RxBuffer[6];
	uint16_t CRCCalculated = this->CalculateCRC16Arc(this->Settings.CANSettings.RxBuffer,6);
	
	if(CRCResult != CRCCalculated) {
		/* CRC tutmamýþ */
		return;
	}
	else{
		if(strncmp((char*)this->Settings.CANSettings.RxBuffer, "CNNT", 4) == 0){
			memset(this->Settings.CANSettings.TxBuffer,NULL,sizeof(this->Settings.CANSettings.TxBuffer));
			this->Settings.CANSettings.TxBuffer[0] = this->Settings.AdressFields.ParametresAdressField.LastPageNumber;
			this->Settings.CANSettings.TxBuffer[1] = this->Settings.AdressFields.ParametresAdressField.LastValueNumber;
			this->Settings.CANSettings.TxBuffer[2] = this->Settings.AdressFields.VariablesAdressField.LastPageNumber;
			this->Settings.CANSettings.TxBuffer[3] = this->Settings.AdressFields.VariablesAdressField.LastValueNumber;
			
			CRCCalculated = this->CalculateCRC16Arc(this->Settings.CANSettings.TxBuffer,6);
			
			this->Settings.CANSettings.TxBuffer[6] = CRCCalculated;
			this->Settings.CANSettings.TxBuffer[7] = CRCCalculated>>8;
			this->Settings.CAN->sendMsgBuf(this->Settings.CommunicationIDs.ConnectionStatusID+1,0,8,this->Settings.CANSettings.TxBuffer);
			
		}
		else if(strncmp((char*)this->Settings.CANSettings.RxBuffer, "CNOK", 4) == 0){
			
			this->Settings.CommunicationSettings.CommunicationStarted = true;
			this->Settings.CommunicationSettings.SendParametres = true;
		}
		else if(strncmp((char*)this->Settings.CANSettings.RxBuffer, "READ", 4) == 0){
			this->Settings.CommunicationSettings.ParametersRequested = true;
		}
		else if(strncmp((char*)this->Settings.CANSettings.RxBuffer, "DCNT", 4) == 0){
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
void cPCCommunicationCan::SendParametres(void)
{
	memset(this->Settings.CANSettings.TxBuffer,NULL,sizeof(this->Settings.CANSettings.TxBuffer));
	if(this->Settings.AdressFields.ParametresAdressField.SendCrc){
		this->Settings.CANSettings.TxBuffer[0] = this->Settings.AdressFields.ParametresAdressField.CrcForThisPage;
		uint32_t CrcCalculated;
		int16_t *ValueStart;
		if(this->Settings.AdressFields.ParametresAdressField.CrcForThisPage != this->Settings.AdressFields.ParametresAdressField.LastPageNumber){
			this->Settings.CANSettings.TxBuffer[1] = 212; // ilk iki bit (11) set edilir geriye kalanlara toplam boyut (20) girilir.
			ValueStart = this->Settings.AdressFields.ParametresAdressField.Adress + (20 * this->Settings.AdressFields.ParametresAdressField.CrcForThisPage);
			CrcCalculated = this->CalculateCRC32Mpeg2( (uint8_t*) ValueStart,40);
		}
		else{
			this->Settings.CommunicationSettings.SendParametres = false;
			this->Settings.CANSettings.TxBuffer[1] = 193 + this->Settings.AdressFields.ParametresAdressField.LastValueNumber;
			ValueStart = this->Settings.AdressFields.ParametresAdressField.Adress + (20 * this->Settings.AdressFields.ParametresAdressField.CrcForThisPage);
			CrcCalculated = this->CalculateCRC32Mpeg2( (uint8_t*) ValueStart,((this->Settings.AdressFields.ParametresAdressField.LastValueNumber+1) * 2));
		}
		this->Settings.CANSettings.TxBuffer[2] = CrcCalculated;
		this->Settings.CANSettings.TxBuffer[3] = CrcCalculated>>8;
		this->Settings.CANSettings.TxBuffer[4] = CrcCalculated>>16;
		this->Settings.CANSettings.TxBuffer[5] = CrcCalculated>>24;
		uint16_t CrcCalculatedForThisMessage = this->CalculateCRC16Arc(this->Settings.CANSettings.TxBuffer,6);
		this->Settings.CANSettings.TxBuffer[6] = CrcCalculatedForThisMessage;
		this->Settings.CANSettings.TxBuffer[7] = CrcCalculatedForThisMessage>>8;
		this->Settings.CAN->sendMsgBuf(this->Settings.CommunicationIDs.ParametresID,0,8,this->Settings.CANSettings.TxBuffer);
		this->Settings.AdressFields.ParametresAdressField.SendCrc = false;
		return;
	}
	
	
	this->Settings.CANSettings.TxBuffer[0] = this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber;
	this->Settings.CANSettings.TxBuffer[1] = this->Settings.AdressFields.ParametresAdressField.CurrentValueNumber;
	uint16_t CurrentValueIndex = this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber * 20 + this->Settings.AdressFields.ParametresAdressField.CurrentValueNumber;
	
	uint8_t i;
	for (i = 0; i < 3; i++){
		this->Settings.CANSettings.TxBuffer[2*i+2] = *(this->Settings.AdressFields.ParametresAdressField.Adress + CurrentValueIndex);
		this->Settings.CANSettings.TxBuffer[2*i+3] = *(this->Settings.AdressFields.ParametresAdressField.Adress + CurrentValueIndex)>>8;
		
		if (CurrentValueIndex >= this->Settings.AdressFields.ParametresAdressField.AdressEndIndex){
			this->Settings.AdressFields.ParametresAdressField.SendCrc = true;
			this->Settings.AdressFields.ParametresAdressField.CrcForThisPage = this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber;
			this->Settings.AdressFields.ParametresAdressField.CurrentValueNumber= 0;
			this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber = 0;
			break;
			
		}
		
		if(this->Settings.AdressFields.ParametresAdressField.CurrentValueNumber >= 19){
			this->Settings.AdressFields.ParametresAdressField.SendCrc = true;
			this->Settings.AdressFields.ParametresAdressField.CrcForThisPage = this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber;
			this->Settings.AdressFields.ParametresAdressField.CurrentValueNumber= 0;
			this->Settings.AdressFields.ParametresAdressField.CurrentPageNumber++;
			break;
		}
		CurrentValueIndex++;
		this->Settings.AdressFields.ParametresAdressField.CurrentValueNumber++;
	}
	if(i==1){
		this->Settings.CANSettings.TxBuffer[1] = this->Settings.CANSettings.TxBuffer[1] + 64;
		
	}
	else if (i == 0){
		this->Settings.CANSettings.TxBuffer[1] = this->Settings.CANSettings.TxBuffer[1] + 128;
	}
	this->Settings.CAN->sendMsgBuf(this->Settings.CommunicationIDs.ParametresID,0,8,this->Settings.CANSettings.TxBuffer);
}
/*******************************************************************************
  * @brief      	
  * @param[in]		<param name> <description>
  * @param[out]		<param name> <description>
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  
  *****************************************************************************/
void cPCCommunicationCan::SendVariables(void)
{
	memset(this->Settings.CANSettings.TxBuffer,NULL,sizeof(this->Settings.CANSettings.TxBuffer));
	if(this->Settings.AdressFields.VariablesAdressField.SendCrc){
		this->Settings.CANSettings.TxBuffer[0] = this->Settings.AdressFields.VariablesAdressField.CrcForThisPage;
		uint32_t CrcCalculated;
		//int16_t *ValueStart;
		if(this->Settings.AdressFields.VariablesAdressField.CrcForThisPage != this->Settings.AdressFields.VariablesAdressField.LastPageNumber){
			this->Settings.CANSettings.TxBuffer[1] = 212; // ilk iki bit (11) set edilir geriye kalanlara toplam boyut (20) girilir.
			//ValueStart = &this->Settings.CANSettings.LastSendedVarDatas;
			CrcCalculated = this->CalculateCRC32Mpeg2( (uint8_t*) this->Settings.CANSettings.LastSendedVarDatas,40);
		}
		else{
			this->Settings.CANSettings.TxBuffer[1] = 193 + this->Settings.AdressFields.VariablesAdressField.LastValueNumber;
			
			CrcCalculated = this->CalculateCRC32Mpeg2( (uint8_t*) this->Settings.CANSettings.LastSendedVarDatas,((this->Settings.AdressFields.VariablesAdressField.LastValueNumber+1) * 2));
		}
		this->Settings.CANSettings.TxBuffer[2] = CrcCalculated;
		this->Settings.CANSettings.TxBuffer[3] = CrcCalculated>>8;
		this->Settings.CANSettings.TxBuffer[4] = CrcCalculated>>16;
		this->Settings.CANSettings.TxBuffer[5] = CrcCalculated>>24;
		uint16_t CrcCalculatedForThisMessage = this->CalculateCRC16Arc(this->Settings.CANSettings.TxBuffer,6);
		this->Settings.CANSettings.TxBuffer[6] = CrcCalculatedForThisMessage;
		this->Settings.CANSettings.TxBuffer[7] = CrcCalculatedForThisMessage>>8;
		this->Settings.CAN->sendMsgBuf(this->Settings.CommunicationIDs.VariablesID,0,8,this->Settings.CANSettings.TxBuffer);
		this->Settings.AdressFields.VariablesAdressField.SendCrc = false;
		return;
	}
	
	if(this->Settings.AdressFields.VariablesAdressField.CurrentValueNumber == 0){
		if(this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber != this->Settings.AdressFields.VariablesAdressField.LastPageNumber){
			for(uint8_t i = 0 ;i< 20 ; i++){
				this->Settings.CANSettings.LastSendedVarDatas[i] = *(this->Settings.AdressFields.VariablesAdressField.Adress + (this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber * 20) + i);
			}
		}
		else{
			for(uint8_t i = 0; i<this->Settings.AdressFields.VariablesAdressField.LastValueNumber+1;i++){
				this->Settings.CANSettings.LastSendedVarDatas[i] = *(this->Settings.AdressFields.VariablesAdressField.Adress + (this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber * 20) + i);
			}
		}
		
	}
	this->Settings.CANSettings.TxBuffer[0] = this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber;
	this->Settings.CANSettings.TxBuffer[1] = this->Settings.AdressFields.VariablesAdressField.CurrentValueNumber;
	uint16_t CurrentValueIndex = this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber * 20 + this->Settings.AdressFields.VariablesAdressField.CurrentValueNumber;
	
	uint8_t i;
	for (i = 0; i < 3; i++){
		this->Settings.CANSettings.TxBuffer[2*i+2] = this->Settings.CANSettings.LastSendedVarDatas[this->Settings.AdressFields.VariablesAdressField.CurrentValueNumber];
		this->Settings.CANSettings.TxBuffer[2*i+3] = this->Settings.CANSettings.LastSendedVarDatas[this->Settings.AdressFields.VariablesAdressField.CurrentValueNumber]>>8;
		
		if (CurrentValueIndex >= this->Settings.AdressFields.VariablesAdressField.AdressEndIndex){
			this->Settings.AdressFields.VariablesAdressField.SendCrc = true;
			this->Settings.AdressFields.VariablesAdressField.CrcForThisPage = this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber;
			this->Settings.AdressFields.VariablesAdressField.CurrentValueNumber= 0;
			this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber = 0;
			break;
			
		}
		
		if(this->Settings.AdressFields.VariablesAdressField.CurrentValueNumber >= 19){
			this->Settings.AdressFields.VariablesAdressField.SendCrc = true;
			this->Settings.AdressFields.VariablesAdressField.CrcForThisPage = this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber;
			this->Settings.AdressFields.VariablesAdressField.CurrentValueNumber= 0;
			this->Settings.AdressFields.VariablesAdressField.CurrentPageNumber++;
			break;
		}
		CurrentValueIndex++;
		this->Settings.AdressFields.VariablesAdressField.CurrentValueNumber++;
	}
	if(i==1){
		this->Settings.CANSettings.TxBuffer[1] = this->Settings.CANSettings.TxBuffer[1] + 64;
		
	}
	else if (i == 0){
		this->Settings.CANSettings.TxBuffer[1] = this->Settings.CANSettings.TxBuffer[1] + 128;
	}
	this->Settings.CAN->sendMsgBuf(this->Settings.CommunicationIDs.VariablesID,0,8,this->Settings.CANSettings.TxBuffer);
}
/*******************************************************************************
  * @brief      	
  * @param[in]		<param name> <description>
  * @param[out]		<param name> <description>
  * @param[in,out]	<param name> <description>
  * @retval     	None
  * @attention  
  *****************************************************************************/
void cPCCommunicationCan::CheckReceivedParameters(void){
	if(this->Settings.CANSettings.RxBuffer[1]>192){
		//Crc Gelmiþ.
		int Page = this->Settings.CANSettings.RxBuffer[0];
		int Count = this->Settings.CANSettings.RxBuffer[1];
		Count -= 192;
		// Boyut?
		if(Count == this->Settings.CANSettings.LastReceivedParamSize){
			//Msjýn Crcsi
			uint16_t CRCResult = this->Settings.CANSettings.RxBuffer[7] * 256 + this->Settings.CANSettings.RxBuffer[6];
			uint16_t CRCCalculated = this->CalculateCRC16Arc(this->Settings.CANSettings.RxBuffer,6);
			
			if(CRCResult == CRCCalculated) {
				//Alýnan tüm mesaj crcsi tutmuþ mu?
				uint32_t CRC32Result = this->Settings.CANSettings.RxBuffer[2] + this->Settings.CANSettings.RxBuffer[3] * 256 + this->Settings.CANSettings.RxBuffer[4] * 4096 + this->Settings.CANSettings.RxBuffer[5] * 65536;
				int16_t *ValueStart;
				ValueStart = &this->Settings.CANSettings.LastReceivedParamDatas[0];
				uint32_t CRC32Calculated = this->CalculateCRC32Mpeg2((uint8_t*)ValueStart,(2*Count));
				if(CRC32Result = CRC32Calculated){
					for(int i = 0; i<Count;i++){
						*(this->Settings.AdressFields.ParametresAdressField.Adress + (Page * 20 + i)) = this->Settings.CANSettings.LastReceivedParamDatas[i];
					}
					
				}
			}
		}
		
	}
	else if(this->Settings.CANSettings.RxBuffer[1]>128){
		// 1 data gelmiþ.
		int Page = this->Settings.CANSettings.RxBuffer[0];
		int Count = this->Settings.CANSettings.RxBuffer[1];
		Count -= 128;
		int16_t Value = this->Settings.CANSettings.RxBuffer[2] + this->Settings.CANSettings.RxBuffer[3] * 256;
		this->Settings.CANSettings.LastReceivedParamDatas[Count] = Value;
		
		Count++;
		
		this->Settings.CANSettings.LastReceivedParamPage = Page;
		this->Settings.CANSettings.LastReceivedParamSize = Count;
		
	}
	else if(this->Settings.CANSettings.RxBuffer[1]>64){
		int Page = this->Settings.CANSettings.RxBuffer[0];
		int Count = this->Settings.CANSettings.RxBuffer[1];
		Count -= 64;
		for(uint8_t i = 0; i<2; i++){
			int16_t Value = this->Settings.CANSettings.RxBuffer[2*i + 2] + this->Settings.CANSettings.RxBuffer[2*i + 3] * 256;
			this->Settings.CANSettings.LastReceivedParamDatas[Count] = Value;
			
			Count++;
		}
		
		
		this->Settings.CANSettings.LastReceivedParamPage = Page;
		this->Settings.CANSettings.LastReceivedParamSize = Count;
	}
	else{
		//3 data gelmiþ.
		int Page = this->Settings.CANSettings.RxBuffer[0];
		int Count = this->Settings.CANSettings.RxBuffer[1];
		
		for(uint8_t i = 0; i<3; i++){
			int16_t Value = this->Settings.CANSettings.RxBuffer[2*i + 2] + this->Settings.CANSettings.RxBuffer[2*i + 3] * 256;
			this->Settings.CANSettings.LastReceivedParamDatas[Count] = Value;
			Count++;
		}
		
		
		this->Settings.CANSettings.LastReceivedParamPage = Page;
		this->Settings.CANSettings.LastReceivedParamSize = Count;
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
uint32_t cPCCommunicationCan::CalculateCRC32Mpeg2(unsigned char *Message, size_t Length)
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
uint16_t cPCCommunicationCan::CalculateCRC16Arc (const uint8_t *Message, uint16_t Length)
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
