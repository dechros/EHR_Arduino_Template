/**
  ******************************************************************************
  * @attention 	Detaylı açıklama c dosyasındadır.
  ******************************************************************************
  */
#ifndef __PCCOMMUNICATIONSERIAL_H__
#define __PCCOMMUNICATIONSERIAL_H__
/* Includes *******************************************************************/
//#include "Libraries.h"
#include "Uart.h"
#include "PackageParser.h"
#include "EF_Standart_Library.h"
/* Public macro ***************************************************************/
/* Public define **************************************************************/
/* Public typedef *************************************************************/
typedef struct tsPCCOMMUNICATIONSERIAL_ParametresAdressField{
	int16_t *Adress;
	uint16_t AdressEndIndex;
	uint8_t CurrentPageNumber;
	uint8_t CurrentValueNumber;
	uint8_t LastPageNumber;
	uint8_t LastValueNumber;
	bool SendCrc;
	uint8_t CrcForThisPage;
}tPCCOMMUNICATIONSERIAL_ParametresAdressField;
typedef struct tsPCCOMMUNICATIONSERIAL_VariablesAdressField{
	int16_t *Adress;
	uint16_t AdressEndIndex;
	uint8_t CurrentPageNumber;
	uint8_t CurrentValueNumber;
	uint8_t LastPageNumber;
	uint8_t LastValueNumber;
	bool SendCrc;
	uint8_t CrcForThisPage;
}tPCCOMMUNICATIONSERIAL_VariablesAdressField;
typedef enum tePCCOMMUNICATIONSERIAL_AdressType{
	PCCOMMUNICATIONSERIAL_PARAMETRE = 0,
	PCCOMMUNICATIONSERIAL_VARIABLE  = 1,
}tPCCOMMUNICATIONSERIAL_AdressType;
/* Public variables ***********************************************************/
/* Classes ********************************************************************/
class cPCCommunicationSerial{
	private:
	struct{
		struct{
			char RxBuffer[256];
			char TxBuffer[128];
			bool PackageStarted;
			uint16_t LastReceivedPackageSize;
		}SerialSettings;
		struct{
			uint32_t ConnectionStatusID;
			uint32_t ParametresID;
			uint32_t VariablesID;
			uint32_t ParametresReceiveID;
		}CommunicationIDs;
		struct{
			bool CommunicationStarted;
			bool SendParametres;
			bool ParametersRequested;
			TON *ParametreSendTimer;
			TON *VariableSendTimer;
		}CommunicationSettings;
		struct{
			tPCCOMMUNICATIONSERIAL_ParametresAdressField ParametresAdressField;
			tPCCOMMUNICATIONSERIAL_VariablesAdressField VariablesAdressField;
		}AdressFields;
		Uart *Serial;
	}Settings;
	void CommunicationCheck(void);
	uint32_t CalculateCRC32Mpeg2 (unsigned char *Message, size_t Length);
	void SendParameters(void);
	void SendVariables(void);
	void CheckReceivedParameters(uint32_t ID);
	void AnalyzeReceivedPackage(void);
	void AddRawDataToTransmitBuffer(uint8_t *RawData, uint16_t RawDataSize);
	void HexRawDataToBinRawData(uint8_t *HexRawData, uint8_t *BinRawData, uint16_t BinRawDataSize);
	public:
	cPCCommunicationSerial(Uart *Serial,uint32_t StartID);
	void Executer(void);
	void FillAdressField(tPCCOMMUNICATIONSERIAL_AdressType AdressType, int16_t *ValueStartAdress, uint16_t Length);
	void NewDataReceived(uint8_t Data);
};
/******************************************************************************/
#endif /* __PCCOMMUNICATIONSERIAL_H__ */
