/*
 * app_RC522.h
 *
 *  Created on: 28/09/2017
 *      Author: uidj2522
 */

#ifndef APP_RC522_H_
#define APP_RC522_H_

#include "../Headers/stdtypedef.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../mcc_generated_files/eusart2.h"

#define APP_RC522_BUFFER_MAX_LENGTH	20U

// Return codes from the functions in this class. Remember to update GetStatusCodeName() if you add more.
// last value set to 0xff, then compiler uses less ram, it seems some optimisations are triggered

enum {
    STATUS_OK, // Success
    STATUS_ERROR, // Error in communication
    STATUS_COLLISION, // Collission detected
    STATUS_TIMEOUT, // Timeout in communication.
    STATUS_NO_ROOM, // A buffer is not big enough.
    STATUS_INTERNAL_ERROR, // Internal error in the code. Should not happen ;-)
    STATUS_INVALID, // Invalid argument.
    STATUS_CRC_WRONG, // The CRC_A does not match
    STATUS_MIFARE_NACK = 0xff // A MIFARE PICC responded with NAK.
};

// MFRC522 commands. Described in chapter 10 of the datasheet.

enum PCD_Command {
    PCD_Idle = 0x00, // no action, cancels current command execution
    PCD_Mem = 0x01, // stores 25 bytes into the internal buffer
    PCD_GenerateRandomID = 0x02, // generates a 10-byte random ID number
    PCD_CalcCRC = 0x03, // activates the CRC coprocessor or performs a self-test
    PCD_Transmit = 0x04, // transmits data from the FIFO buffer
    PCD_NoCmdChange = 0x07, // no command change, can be used to modify the CommandReg register bits without affecting the command, for example, the PowerDown bit
    PCD_Receive = 0x08, // activates the receiver circuits
    PCD_Transceive = 0x0C, // transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission
    PCD_MFAuthent = 0x0E, // performs the MIFARE standard authentication as a reader
    PCD_SoftReset = 0x0F // resets the MFRC522
};

// Commands sent to the PICC.

enum PICC_Command {
    // The commands used by the PCD to manage communication with several PICCs (ISO 14443-3, Type A, section 6.4)
    PICC_CMD_REQA = 0x26, // REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
    PICC_CMD_WUPA = 0x52, // Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anticollision or selection. 7 bit frame.
    PICC_CMD_CT = 0x88, // Cascade Tag. Not really a command, but used during anti collision.
    PICC_CMD_SEL_CL1 = 0x93, // Anti collision/Select, Cascade Level 1
    PICC_CMD_SEL_CL2 = 0x95, // Anti collision/Select, Cascade Level 2
    PICC_CMD_SEL_CL3 = 0x97, // Anti collision/Select, Cascade Level 3
    PICC_CMD_HLTA = 0x50, // HaLT command, Type A. Instructs an ACTIVE PICC to go to state HALT.
    PICC_CMD_RATS = 0xE0, // Request command for Answer To Reset.
    // The commands used for MIFARE Classic (from http://www.mouser.com/ds/2/302/MF1S503x-89574.pdf, Section 9)
    // Use PCD_MFAuthent to authenticate access to a sector, then use these commands to read/write/modify the blocks on the sector.
    // The read/write commands can also be used for MIFARE Ultralight.
    PICC_CMD_MF_AUTH_KEY_A = 0x60, // Perform authentication with Key A
    PICC_CMD_MF_AUTH_KEY_B = 0x61, // Perform authentication with Key B
    PICC_CMD_MF_READ = 0x30, // Reads one 16 byte block from the authenticated sector of the PICC. Also used for MIFARE Ultralight.
    PICC_CMD_MF_WRITE = 0xA0, // Writes one 16 byte block to the authenticated sector of the PICC. Called "COMPATIBILITY WRITE" for MIFARE Ultralight.
    PICC_CMD_MF_DECREMENT = 0xC0, // Decrements the contents of a block and stores the result in the internal data register.
    PICC_CMD_MF_INCREMENT = 0xC1, // Increments the contents of a block and stores the result in the internal data register.
    PICC_CMD_MF_RESTORE = 0xC2, // Reads the contents of a block into the internal data register.
    PICC_CMD_MF_TRANSFER = 0xB0, // Writes the contents of the internal data register to a block.
    // The commands used for MIFARE Ultralight (from http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf, Section 8.6)
    // The PICC_CMD_MF_READ and PICC_CMD_MF_WRITE can also be used for MIFARE Ultralight.
    PICC_CMD_UL_WRITE = 0xA2 // Writes one 4 byte page to the PICC.
};

typedef enum {
    RC522_STATE_INIT, // Init State. Used for Driver initialization
    RC522_STATE_CARD_SEARCH, // Card Search Request.
    RC522_STATE_GET_ID_CARD, // Get ID from Card present
    RC522_STATE_SELECT_CARD, // Select CARD to operate
    RC522_STATE_AUTHENTICATION, // Authentication State
    RC522_STATE_READ_PAGE1,
    RC522_STATE_READ_WRITE, //Write State
    RC522_N_STATES
} E_RC522_STATES;

/***************************************
 * Definitions						   *
 ***************************************/
/*RESET PIN*/
#define APP_RC_522_RESET_ENABLE				FALSE
#define APP_RC_522_RESET_DISABLE			TRUE
#define APP_RC_522_RESET_SET_PIN()          IO_RD5_SetLow()
#define APP_RC_522_RESET_CLEAR_PIN()        IO_RD5_SetHigh()

/*MX PIN*/

/*DTRQ PIN*/

/* RX PIN */

/* TX PIN */

/* Communication Interface */
#define APP_RC522_COMM_INTERFACE_SEND(dataToSend)	INTERRUPT_GlobalInterruptHighDisable();\
EUSART2_Write((T_UBYTE)dataToSend);\
INTERRUPT_GlobalInterruptHighEnable()

#define APP_RC522_COMM_INTERFACE_RECEIVE()          EUSART2_Read()

/* Timeout Definitions */
#define APP_RC522_TIMER_VALUE				(T_UBYTE)200U //100ms (100 times for 1ms)
#define APP_RC522_TIMER_STOP(timer)			timer = 0;
#define APP_RC522_TIMER_IS_STOPPED(timer)	(T_UBYTE)(timer == 0)
#define APP_RC522_TIMER_LOAD(timer)			timer = APP_RC522_TIMER_VALUE; \
                                            rub_RC522_TimeoutFlag = FALSE

/* Error Manager */
#define APP_RC522_COUNT_FOR_ERROR           ((T_UBYTE)10U)
#define APP_RC522_MODULE_IS_FAIL()          ((T_UBYTE)(rub_ErrorMatureCounter >= APP_RC522_COUNT_FOR_ERROR))

/******************************************************************************
 * Definitions
 ******************************************************************************/

//------------------ MFRC522 registers---------------
//Page 0:Command and Status
#define     Reserved00            0x00
#define     CommandReg            0x01
#define     CommIEnReg            0x02
#define     DivlEnReg             0x03
#define     CommIrqReg            0x04
#define     DivIrqReg             0x05
#define     ErrorReg              0x06
#define     Status1Reg            0x07
#define     Status2Reg            0x08
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     Reserved01            0x0F
//Page 1:Command
#define     Reserved10            0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxASKReg              0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     Reserved11            0x1A
#define     Reserved12            0x1B
#define     MifareReg             0x1C
#define     Reserved13            0x1D
#define     Reserved14            0x1E
#define     SerialSpeedReg        0x1F
//Page 2:CFG
#define     Reserved20            0x20
#define     CRCResultRegH         0x21
#define     CRCResultRegL         0x22
#define     Reserved21            0x23
#define     ModWidthReg           0x24
#define     Reserved22            0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsPReg	          0x28
#define     ModGsPReg             0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
//Page 3:TestRegister
#define     Reserved30            0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39
#define     TestDAC2Reg           0x3A
#define     TestADCReg            0x3B
#define     Reserved31            0x3C
#define     Reserved32            0x3D
#define     Reserved33            0x3E
#define     Reserved34			  0x3F
//-----------------------------------------------

/***************************************
 * Variables						   *
 ***************************************/
extern T_UBYTE raub_RC522_FIFOData[APP_RC522_BUFFER_MAX_LENGTH];
extern T_UWORD ruw_RC522_FIFOReceivedLength;

/***************************************
 * Prototypes						   *
 ***************************************/
extern void app_RC522_Init(void);
extern T_UBYTE app_RC522_ReadRegister(T_UBYTE lub_Address);
extern T_UBYTE app_RC522_WriteRegister(T_UBYTE lub_Address, T_UBYTE lub_Value);
extern T_UBYTE app_RC522_IsANewCardPresent(void);
extern void app_RC522_TaskMng(void);
extern void app_RC522_TimeoutTask(void);
extern T_UBYTE app_RC522_ModuleIsFail(void);
extern void app_RC522_WriteRequest(void);
extern void app_RC522_EraseRequest(void);

#endif /* APP_RC522_H_ */
