/**
  ******************************************************************************
  * @attention 	Detaylı açıklama c dosyasındadır.
  ******************************************************************************
  */
#ifndef __PCCOMMUNICATION_H__
#define __PCCOMMUNICATION_H__
/* Includes *******************************************************************/
//#include "Libraries.h"
#include "Uart.h"
#include "mcp_can.h"
#include "PackagePicker.h"
#include "PackageParser.h"
/* Public macro ***************************************************************/
/* Public define **************************************************************/
#define PCCOMMUNICATION_PARAMETRES_ADRESSFIELD_SIZE  10 // Bu Parametre Alanında kuyruk oluşturmak için.
#define PCCOMMUNICATION_VARIABLES_ADRESSFIELD_SIZE 10

//#define PCCOMMUNICATION_PARAMETRES_MAX_PAGE_NUMBER 1
//#define PCCOMMUNICATION_VARIABLES_MAX_PAGE_NUMBER 1
/* Public typedef *************************************************************/
typedef struct tsPCCOMMUNICATION_ParametresAdressField{
	int16_t * Adresses[PCCOMMUNICATION_PARAMETRES_ADRESSFIELD_SIZE];
	uint16_t AdressEndPoints[PCCOMMUNICATION_PARAMETRES_ADRESSFIELD_SIZE];
	uint8_t LastIndex;
	
	}tPCCOMMUNICATION_ParametresAdressField;

typedef struct tsPCCOMMUNICATION_VariablesAdressField{
	int16_t * Adresses[PCCOMMUNICATION_PARAMETRES_ADRESSFIELD_SIZE];
	uint16_t AdressEndPoints[PCCOMMUNICATION_PARAMETRES_ADRESSFIELD_SIZE];
	uint8_t LastIndex;
	}tPCCOMMUNICATION_VariablesAdressField;

typedef enum tePCCOMMUNICATION_AdressType{
	PCCOMMUNICATION_PARAMETRE = 0,
	PCCOMMUNICATION_VARIABLE  = 1,
}tPCCOMMUNICATION_AdressType;
typedef enum tePCCOMMUNICATION_CommunicationType{
PCCOMMUNICATION_CAN = 0,
PCCOMMUNICATION_SERIAL = 1,
}tPCCOMMUNICATION_CommunicationType;
/* Public variables ***********************************************************/
/* Classes ********************************************************************/
class cPCCommunication{
	private:
	void AnalyzeReceivedPackage(void);
	void CommunicationCheck(void);
	void SendParametres(void);
	unsigned int CalculateCRC32Mpeg2(unsigned char *Message, size_t Length);
	uint16_t CalculateCRC16Arc (const uint8_t *Message, uint16_t Length);
	tPCCOMMUNICATION_CommunicationType CommunicationType;
	Uart *Serial;
	MCP_CAN *CAN;
	tPACKAGEPICKER_Handle *PackagePickerHandle;
	struct sSettings{
		struct{
			uint8_t LastPageNumber;
			uint8_t LastValueNumber;
		}Parametres;
		struct{
			uint8_t LastPageNumber;
			uint8_t LastValueNumber;
		}Variables;
		struct{
			char PackageBuffer[256];
			char TransmitBuffer[128];
			uint16_t LastReceivedPackageSize;
			uint16_t AdressIndex;
			uint16_t AdressShiftIndex;
		}SerialSettings;
		struct{
			INT8U CANRxBuffer[8];
			INT8U CANTxBuffer[8];
			INT8U Length;
			unsigned long CanID;
			uint16_t AdressIndex;
			uint16_t AdressShiftIndex;
			struct{
				uint8_t CurrentPageNumber;
				uint8_t CurrentValueNumber;
			}ParametresIndexes;
			struct{
				uint8_t CurrentPageNumber;
				uint8_t CurrentValueNumber;
			}VariablesIndexes;
		}CANSettings;
		struct{
			uint32_t ParametresSendID;
			uint32_t VariablesStartID;
			uint32_t ParametresRequestStartID;
			uint32_t ConnectionStatusID;
			bool CANCommunicationStarted;
			bool SerialCommunicationStarted;
			}CommunicationSettings;
	}Settings;
	public:
	cPCCommunication(tPCCOMMUNICATION_CommunicationType CommunicationType, uint32_t StartID, Uart *Serial, MCP_CAN *CAN);
	void Executer(void);
	void FillAdressField(tPCCOMMUNICATION_AdressType AdressType, int16_t *ValueStartAdress, uint16_t Length);
	tPCCOMMUNICATION_ParametresAdressField ParametresAdressField;
	tPCCOMMUNICATION_VariablesAdressField VariablesAdressField;
	};
/******************************************************************************/
#endif /* __PCCOMMUNICATION_H__ */
