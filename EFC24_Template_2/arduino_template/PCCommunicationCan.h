/**
  ******************************************************************************
  * @attention 	Detaylı açıklama c dosyasındadır.
  ******************************************************************************
  */
#ifndef __PCCOMMUNICATIONCAN_H__
#define __PCCOMMUNICATIONCAN_H__
/* Includes *******************************************************************/
//#include "Libraries.h"
#include "mcp_can.h"
#include "EF_Standart_Library.h"
/* Public macro ***************************************************************/
/* Public define **************************************************************/
/* Public typedef *************************************************************/
typedef struct tsPCCOMMUNICATIONCAN_ParametresAdressField{
	int16_t *Adress;
	uint16_t AdressEndIndex;
	uint8_t CurrentPageNumber;
	uint8_t CurrentValueNumber;
	uint8_t LastPageNumber;
	uint8_t LastValueNumber;
	bool SendCrc;
	uint8_t CrcForThisPage;
}tPCCOMMUNICATIONCAN_ParametresAdressField;
typedef struct tsPCCOMMUNICATIONCAN_VariablesAdressField{
	int16_t *Adress;
	uint16_t AdressEndIndex;
	uint8_t CurrentPageNumber;
	uint8_t CurrentValueNumber;
	uint8_t LastPageNumber;
	uint8_t LastValueNumber;
	bool SendCrc;
	uint8_t CrcForThisPage;
}tPCCOMMUNICATIONCAN_VariablesAdressField;
typedef enum tePCCOMMUNICATIONCAN_AdressType{
	PCCOMMUNICATIONCAN_PARAMETRE = 0,
	PCCOMMUNICATIONCAN_VARIABLE  = 1,
}tPCCOMMUNICATIONCAN_AdressType;
/* Public variables ***********************************************************/
/* Classes ********************************************************************/
class cPCCommunicationCan{
	private:
	struct{
		MCP_CAN *CAN;
		struct{
			INT8U RxBuffer[8];
			INT8U TxBuffer[8];
			INT8U Length;
			unsigned long ID;
			int16_t LastReceivedParamDatas[20];
			int16_t LastSendedVarDatas[20];
			int16_t LastSendedParamDatas[20];
			uint16_t LastReceivedParamSize;
			uint8_t LastReceivedParamPage;
		}CANSettings;
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
		tPCCOMMUNICATIONCAN_ParametresAdressField ParametresAdressField;
		tPCCOMMUNICATIONCAN_VariablesAdressField VariablesAdressField;
		}AdressFields;
	}Settings;
	
	void CommunicationCheck(void);
	uint16_t CalculateCRC16Arc (const uint8_t *Message, uint16_t Length);
	uint32_t CalculateCRC32Mpeg2 (unsigned char *Message, size_t Length);
	void SendParametres(void);
	void SendVariables(void);
	void CheckReceivedParameters(void);
	
	public:
	
	cPCCommunicationCan(MCP_CAN *CAN,uint32_t StartID);
	void Executer(void);
	void FillAdressField(tPCCOMMUNICATIONCAN_AdressType AdressType, int16_t *ValueStartAdress, uint16_t Length);
};
/******************************************************************************/
#endif /* __PCCOMMUNICATIONCAN_H__ */
