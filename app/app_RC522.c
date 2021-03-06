/*
 * app_RC522.c
 *
 *  Created on: 28/09/2017
 *      Author: Omar Sevilla
 */

#include "../Headers/system.h"
#include "../Headers/stdtypedef.h"
#include "../mcc_generated_files/eusart2.h"
#include "../mcc_generated_files/interrupt_manager.h"
#include "app_RC522.h"
#include "app_UART.h"
#include "app_LED.h"


/***************************************
 * Constants						   *
 ***************************************/
#define APP_RC522_ID_SIZE	4U
#define	APP_RC522_KEY_SIZE	6U

const T_UBYTE caub_RC522_Key[APP_RC522_KEY_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const T_UBYTE caub_RC522_TagData[] = "SantaAnitaResidencial";
const T_UBYTE caub_RC522_OmarWriteTagData[] = "OmarWrt2412";
const T_UBYTE caub_RC522_OmarEraseTagData[] = "OmarErs1990";

/***************************************
 * Variables						   *
 ***************************************/
T_UBYTE raub_RC522_FIFOData[APP_RC522_BUFFER_MAX_LENGTH];
T_UBYTE raub_CardID[APP_RC522_ID_SIZE];
T_UWORD ruw_RC522_FIFOReceivedLength;
static T_UBYTE rub_RC522WatchDog;
static E_RC522_STATES re_RC522State = RC522_STATE_INIT;
static E_RC522_STATES re_RC522_NextState = RC522_STATE_CARD_SEARCH;
static T_UBYTE rub_WriteRequestFlag = FALSE;
static T_UBYTE rub_ErrorMatureCounter = 0U;
static T_UBYTE rub_RC522DematureCounter = 0U;
static T_UBYTE rub_FailureFlag = FALSE;
static T_UBYTE rub_RC522_TimeoutFlag = FALSE;
static T_UBYTE rub_EraseRequestFlag = FALSE;
static T_UBYTE rub_MasterTagFlag = FALSE;
static T_UBYTE rub_SearchOnGoingFlag = FALSE;

/***************************************
 * Prototypes						   *
 ***************************************/
static void app_RC522_AntennaOn(void);
static void app_RC522_SetRegisterBitMask(T_UBYTE lub_Address, T_UBYTE lub_mask);
static void app_RC522_ClearRegisterBitMask(T_UBYTE lub_Address, T_UBYTE lub_mask);
static T_UBYTE app_RC522_ToCard(T_UBYTE lub_command, T_UBYTE *lpub_sendData, T_UBYTE lub_sendLen, T_UBYTE *lpub_backData, T_UWORD *lpuw_backLen);
static T_UBYTE app_RC522_ReadBlock(T_UBYTE lub_BlockAddr, T_UBYTE *lpub_recvData, T_UWORD *luw_unLen);
static void app_RC522_CalculateCRC(T_UBYTE *lpub_Indata, T_UBYTE lub_len, T_UBYTE *lpub_OutData);
static T_UBYTE app_RC522_Anticoll(T_UBYTE *lpub_serNum);
static T_UBYTE app_RC522_SelectCard(T_UBYTE *lpub_CardID, T_UBYTE lub_IDSize);
static T_UBYTE app_RC522_Authenticate(T_UBYTE lub_Command, T_UBYTE lub_BlockAddress, const T_UBYTE *lpub_Key, T_UBYTE *lpub_CardID);
static T_UBYTE app_RC522_WriteBlock(T_UBYTE lub_BlockAddr, T_UBYTE *lpub_recvData, T_UWORD *luw_unLen);
static void app_RC522_FailCondition(void);
static T_UBYTE app_RC522_CompareData(T_UBYTE *lpub_A, T_UBYTE *lpub_B, T_UBYTE lub_Length);


/***************************************
 * Code								   *
 ***************************************/

/**********************************************************
 * Name: app_RC522_TaskMng
 * Description: This function manage the RC522 task
 **********************************************************/
void app_RC522_TaskMng(void) {
    T_UBYTE lub_BlockAddress;

    if (APP_RC522_MODULE_IS_FAIL() == FALSE) {
        switch (re_RC522State) {
            case RC522_STATE_INIT:
            {
                app_RC522_Init();
                re_RC522State = re_RC522_NextState;
            }
                break;
            case RC522_STATE_CARD_SEARCH:
            {
                if (app_RC522_IsANewCardPresent() == STATUS_OK) {
                    /* Go to next state after INIT */
                    re_RC522_NextState = RC522_STATE_CARD_SEARCH;
                    re_RC522State = RC522_STATE_GET_ID_CARD;

                    /* Set Search on Going Flag */
                    rub_SearchOnGoingFlag = FALSE;
                } else {
                    /* Go to next state after INIT */
                    re_RC522_NextState = RC522_STATE_CARD_SEARCH;
                    /* Reset the Transceiver */
                    re_RC522State = RC522_STATE_INIT;

                    /* Set Search on Going Flag */
                    rub_SearchOnGoingFlag = TRUE;

                    LED_GREEN_OFF();
                    LED_RED_OFF();
                }

            }
                break;

            case RC522_STATE_GET_ID_CARD:
            {
                if (app_RC522_Anticoll(raub_RC522_FIFOData) == STATUS_OK) {
                    for (T_UBYTE i = 0; i < ruw_RC522_FIFOReceivedLength; i++) {
                        if (i < APP_RC522_ID_SIZE) {
                            raub_CardID[i] = raub_RC522_FIFOData[i];
                        } else {
                            /* Do Nothing */
                        }
                    }
                    /* Go to next state after INIT */
                    re_RC522_NextState = RC522_STATE_CARD_SEARCH;
                    /* Reset the Transceiver */
                    re_RC522State = RC522_STATE_SELECT_CARD;
                } else {
                    /* Go to next state after INIT */
                    re_RC522_NextState = RC522_STATE_CARD_SEARCH;
                    /* Reset the Transceiver */
                    re_RC522State = RC522_STATE_CARD_SEARCH;
                }
            }
                break;

            case RC522_STATE_SELECT_CARD:
            {
                if (app_RC522_SelectCard(raub_CardID, APP_RC522_ID_SIZE) == STATUS_OK) {
                    /* Go to next state after INIT */
                    re_RC522_NextState = RC522_STATE_CARD_SEARCH;
                    /* Reset the Transceiver */
                    re_RC522State = RC522_STATE_AUTHENTICATION;
                } else {
                    /* Go to next state after INIT */
                    re_RC522_NextState = RC522_STATE_CARD_SEARCH;
                    /* Reset the Transceiver */
                    re_RC522State = RC522_STATE_INIT;
                }

            }
                break;

            case RC522_STATE_AUTHENTICATION:
            {

                T_UBYTE lub_TrailerBlockIndex = 7U; //Block Address 7U corresponds to Trailer Block for sector 1
                if (app_RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, lub_TrailerBlockIndex, caub_RC522_Key, raub_CardID) == STATUS_OK) {
                    /* Reset the Transceiver */
                    re_RC522State = RC522_STATE_READ_PAGE1;
                } else {
                    /* Reset the Transceiver */
                    re_RC522State = RC522_STATE_INIT;
                }
            }
                break;

            case RC522_STATE_READ_PAGE1:
            {
                lub_BlockAddress = 4U; //Block address 4 corresponds to Block 0 in sector 1
                ruw_RC522_FIFOReceivedLength = 18U;
                if (app_RC522_ReadBlock(lub_BlockAddress, raub_RC522_FIFOData, &ruw_RC522_FIFOReceivedLength) == STATUS_OK) {
                    /* Check if Tag Data is valid */
                    if (app_RC522_CompareData(&raub_RC522_FIFOData, &caub_RC522_TagData, ruw_RC522_FIFOReceivedLength) == TRUE) {
                        /* Clear Master Tag Flag */
                        rub_MasterTagFlag = FALSE;
                        LED_RED_OFF();
                        LED_BLUE_OFF();
                        LED_GREEN_ON();
                        /*TODO: Access Granted*/
                    }
                    else if (app_RC522_CompareData(&raub_RC522_FIFOData, &caub_RC522_OmarWriteTagData, ruw_RC522_FIFOReceivedLength) == TRUE) {
                        rub_MasterTagFlag = TRUE;
                        app_RC522_WriteRequest();
                        LED_BLUE_ON();
                    } else if (app_RC522_CompareData(&raub_RC522_FIFOData, &caub_RC522_OmarEraseTagData, ruw_RC522_FIFOReceivedLength) == TRUE) {
                        rub_MasterTagFlag = TRUE;
                        app_RC522_EraseRequest();
                        LED_RED_ON();
                        LED_BLUE_ON();
                    } else {
                        /* Clear Master Tag Flag */
                        rub_MasterTagFlag = FALSE;

                        /* Leds Off */
                        LED_GREEN_OFF();
                        LED_BLUE_OFF();
                        LED_RED_ON();
                    }


                } else {

                }

                /*Check if Write Request Exists and the TAG Selected s not a master tag*/
                if (rub_WriteRequestFlag == TRUE && rub_MasterTagFlag == FALSE) {
                    /* Go to next state after INIT */
                    re_RC522_NextState = RC522_STATE_CARD_SEARCH;
                    /* Reset the Transceiver */
                    re_RC522State = RC522_STATE_READ_WRITE;
                } else {
                    /* Go to next state after INIT */
                    re_RC522_NextState = RC522_STATE_CARD_SEARCH;
                    /* Reset the Transceiver */
                    re_RC522State = RC522_STATE_INIT;
                }
            }
                break;

            case RC522_STATE_READ_WRITE:
            {
                rub_WriteRequestFlag = FALSE;
                (void) app_RC522_WriteBlock(4U, raub_RC522_FIFOData, &ruw_RC522_FIFOReceivedLength);
                rub_EraseRequestFlag = FALSE;
                /* Go to next state after INIT */
                re_RC522_NextState = RC522_STATE_CARD_SEARCH;
                /* Reset the Transceiver */
                re_RC522State = RC522_STATE_INIT;
            }
                break;

            default:
            {
                re_RC522State = RC522_STATE_INIT;
                re_RC522_NextState = RC522_STATE_CARD_SEARCH;
            }
                break;
        }
    } else { //Module disconnected or with HW failure
        re_RC522State = RC522_STATE_INIT;
        re_RC522_NextState = RC522_STATE_CARD_SEARCH;
    }
}

/**********************************************************
 * Name: app_RC522_Init
 * Description: This function initializes the RC_522 module
 **********************************************************/
void app_RC522_Init(void) {
    /* Set HW configuration for TC522 assigned pines */

    //Config RST PIN
    APP_RC_522_RESET_SET_PIN(); //Reset Module

    //Config MX PIN

    //Config DTRQ PIN

    //Config RX

    //Config TX

    /* UART Configuration */

    //Get default config
    BAUD2CON = 0x00;
    RC2STA = 0x00;
    TX2STA = 0x00;

    //Init UART
    EUSART2_Initialize();

    //Enable Interrupts

    //Disable Reset (High is disable)
    APP_RC_522_RESET_CLEAR_PIN(); //Normal Mode

    T_UBYTE lub_Result = 0U;

    //Set RC522 baud rate to 19200
    lub_Result |= app_RC522_WriteRegister(SerialSpeedReg, 0x9AU);

    //Set UART baud rate to 19200
    app_UART_SetBaudRate();

    lub_Result = app_RC522_ReadRegister(SerialSpeedReg);

    // Reset baud rates
    app_RC522_WriteRegister(TxModeReg, 0x00);
    app_RC522_WriteRegister(RxModeReg, 0x00);
    // Reset ModWidthReg
    lub_Result = app_RC522_WriteRegister(ModWidthReg, 0x26);
    // When communicating with a PICC we need a timeout if something goes wrong.
    // f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
    // TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.
    lub_Result |= app_RC522_WriteRegister(TModeReg, 0x80); // TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
    lub_Result |= app_RC522_WriteRegister(TPrescalerReg, 0xA9); // TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25μs.
    lub_Result |= app_RC522_WriteRegister(TReloadRegH, 0x03); // Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
    lub_Result |= app_RC522_WriteRegister(TReloadRegL, 0xE8);

    lub_Result |= app_RC522_WriteRegister(TxASKReg, 0x40); // Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
    lub_Result |= app_RC522_WriteRegister(ModeReg, 0x3D); // Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
    app_RC522_AntennaOn(); // Enable the antenna driver pins TX1 and TX2 (they were disabled by the reset)
}

/**********************************************************
 * Name: app_RC522_RequestReadRegister
 * Description: This function read a register from RC522 chip and
 * return the value
 **********************************************************/
T_UBYTE app_RC522_ReadRegister(T_UBYTE lub_Address) {
    T_UBYTE lub_DataToSend;

    //Adjust data
    lub_DataToSend = ((lub_Address) | 0x80);

    APP_RC522_COMM_INTERFACE_SEND(lub_DataToSend);

    /*Start TImeout Timer*/
    APP_RC522_TIMER_LOAD(rub_RC522WatchDog);
    while (EUSART2_DataReady == 0U && APP_RC522_TIMER_IS_STOPPED(rub_RC522WatchDog) == FALSE) {
        /*Wait Data*/
    }
    /* Stop Timer */
    APP_RC522_TIMER_STOP(rub_RC522WatchDog);

    /* Check if there was a reception */
    if (EUSART2_DataReady != 0U) {
        return APP_RC522_COMM_INTERFACE_RECEIVE();
    } else {//Not RX Data
        app_RC522_FailCondition();
        return 0U;
    }
}

/**********************************************************
 * Name: app_RC522_RequestReadRegister
 * Description: This function writes a register from RC522 chip and
 * return the value
 **********************************************************/
T_UBYTE app_RC522_WriteRegister(T_UBYTE lub_Address, T_UBYTE lub_Value) {
    T_UBYTE lub_DataToSend;
    T_UBYTE lub_Return;

    lub_Return = STATUS_ERROR;

    //Adjust data
    lub_DataToSend = (lub_Address & ~(0x80U));

    //Start written

    APP_RC522_COMM_INTERFACE_SEND(lub_DataToSend);

    /*Start TImeout Timer*/
    APP_RC522_TIMER_LOAD(rub_RC522WatchDog);
    while (EUSART2_DataReady == 0U && APP_RC522_TIMER_IS_STOPPED(rub_RC522WatchDog) == FALSE) {
        /* Wait for reception */
    }
    /* Stop Timer */
    APP_RC522_TIMER_STOP(rub_RC522WatchDog);

    /* Check if there was a reception */
    if (EUSART2_DataReady > 0U) {
        //Confirm address
        if (APP_RC522_COMM_INTERFACE_RECEIVE() == lub_DataToSend) {
            APP_RC522_COMM_INTERFACE_SEND(lub_Value);
            while (IO_RD4_GetValue() == TRUE) {
                /* Wait Write */
            }

            lub_Return = STATUS_OK;
        } else {
            //Address Confirmation Failed
        }
    } else {//No RX. Timeout Passed

        app_RC522_FailCondition();

        lub_Return = STATUS_ERROR;
    }
    return lub_Return;
}

/**********************************************************
 * Name: app_RC522_AntennaOn
 * Description: This function turns on the RFID Antenna
 **********************************************************/
static void app_RC522_AntennaOn(void) {
    app_RC522_SetRegisterBitMask(TxControlReg, 0x03);
}

/**********************************************************
 * Name: app_RC522_IsANewCardPresent
 * Description: This function writes a register from RC522 chip and
 * return the value
 **********************************************************/
T_UBYTE app_RC522_IsANewCardPresent(void) {
    T_UBYTE status;
    T_UBYTE sendData;

    app_RC522_WriteRegister(BitFramingReg, 0x07); //TxLastBists = BitFramingReg[2..0]	???

    sendData = PICC_CMD_REQA;

    status = app_RC522_ToCard(PCD_Transceive, &sendData, 1U, raub_RC522_FIFOData, &ruw_RC522_FIFOReceivedLength);

    return status;
}

/**********************************************************
 * Name: app_RC522_ClearRegisterBitMask
 * Description: Clears the bits given in mask from register reg.
 **********************************************************/
static void app_RC522_ClearRegisterBitMask(
        T_UBYTE lub_Address, ///< The register to update. One of the PCD_Register enums.
        T_UBYTE lub_mask ///< The bits to clear.
        ) {
    T_UBYTE tmp;
    tmp = app_RC522_ReadRegister(lub_Address);
    app_RC522_WriteRegister(lub_Address, tmp & (~lub_mask)); // clear bit mask
} // End PCD_ClearRegisterBitMask()

/**********************************************************
 * Name: app_RC522_SetRegisterBitMask
 * Description: Sets the bits given in mask from register reg.
 **********************************************************/
static void app_RC522_SetRegisterBitMask(
        T_UBYTE lub_Address, ///< The register to update. One of the PCD_Register enums.
        T_UBYTE lub_mask ///< The bits to set.
        ) {
    T_UBYTE tmp;
    tmp = app_RC522_ReadRegister(lub_Address);
    app_RC522_WriteRegister(lub_Address, tmp | (lub_mask)); // clear bit mask
} // End PCD_ClearRegisterBitMask()

/**********************************************************
 * Name: app_RC522_ToCard
 * Description: TBD
 **********************************************************/
static T_UBYTE app_RC522_ToCard(T_UBYTE lub_command, T_UBYTE *lpub_sendData, T_UBYTE lub_sendLen, T_UBYTE *lpub_backData, T_UWORD *lpuw_backLen) {
    T_UBYTE lub_status = STATUS_ERROR;
    T_UBYTE lub_irqEn = 0x00;
    T_UBYTE lub_waitIRq = 0x00;
    T_UBYTE lub_n;
    T_UWORD lub_i;

    switch (lub_command) {
        case PCD_MFAuthent:
        {
            lub_irqEn = 0x12;
            lub_waitIRq = 0x10;
            break;
        }
        case PCD_Transceive:
        {
            lub_irqEn = 0x77;
            lub_waitIRq = 0x30;
            break;
        }
        default:
            break;
    }

    /* Set Interrupt Configuration */
    app_RC522_WriteRegister(CommIEnReg, lub_irqEn | 0x80);
    app_RC522_ClearRegisterBitMask(CommIrqReg, 0x80);
    app_RC522_SetRegisterBitMask(FIFOLevelReg, 0x80);

    app_RC522_WriteRegister(CommandReg, PCD_Idle);

    /* Store Data in FIFO Register */
    for (lub_i = 0; lub_i < lub_sendLen; lub_i++) {
        app_RC522_WriteRegister(FIFODataReg, *(lpub_sendData + lub_i));
    }

    app_RC522_ClearRegisterBitMask(CommIrqReg, 0x80);
    /* Command Execution */
    app_RC522_WriteRegister(CommandReg, lub_command);
    if (lub_command == PCD_Transceive) {
        app_RC522_SetRegisterBitMask(BitFramingReg, 0x80);
    } else {
        /* Do Nothing */
    }

    /* Wait for Data */
    APP_RC522_TIMER_LOAD(rub_RC522WatchDog); //Load WatchDog
    do {
        lub_n = app_RC522_ReadRegister(CommIrqReg);
    } while (((lub_n & 0x01) == 0U) &&
            ((lub_n & lub_waitIRq) != lub_waitIRq) &&
            (APP_RC522_TIMER_IS_STOPPED(rub_RC522WatchDog) == FALSE)); //Check watch dog

    APP_RC522_TIMER_STOP(rub_RC522WatchDog);

    /* Stop Transmission */
    app_RC522_ClearRegisterBitMask(BitFramingReg, 0x80);

    T_UBYTE lub_Error = 0;

    lub_Error = (app_RC522_ReadRegister(ErrorReg) & 0x1B); //BufferOvfl Collerr CRCErr ProtecolErr
    lub_Error |= (lub_n & 0x01);
    lub_Error |= rub_RC522_TimeoutFlag; //Timeout

    /* Check for errors */
    if (lub_Error == 0U) {//No errors

        /* Transceive Actions*/
        if (lub_command == PCD_Transceive) {
            lub_n = app_RC522_ReadRegister(FIFOLevelReg);

            *lpuw_backLen = lub_n;
            //			}

            if (lub_n > 0U) {
                lub_status = STATUS_OK;
            } else {
                /*Do Nothing*/
            }

            if (lub_n == 0) {
                //				lub_n = 1U;
                lub_status = STATUS_ERROR;
            } else if (lub_n > APP_RC522_BUFFER_MAX_LENGTH) {
                lub_n = APP_RC522_BUFFER_MAX_LENGTH;
            } else {
                /* Do Nothing */
            }

            /* Read FIFO Data */
            for (lub_i = 0; lub_i < lub_n; lub_i++) {
                *(lpub_backData + lub_i) = app_RC522_ReadRegister(FIFODataReg);
            }

        } else {
            lub_status = STATUS_OK;
        }
    } else {//Error found
        lub_status = STATUS_ERROR;
    }

    return lub_status;
}

/**********************************************************
 * Name: app_RC522_ReadBlock
 * Description: TBD
 **********************************************************/
static T_UBYTE app_RC522_ReadBlock(T_UBYTE lub_BlockAddr, T_UBYTE *lpub_recvData, T_UWORD *luw_unLen) {
    T_UBYTE lub_status;

    lpub_recvData[0] = PICC_CMD_MF_READ;
    lpub_recvData[1] = lub_BlockAddr;
    app_RC522_CalculateCRC(lpub_recvData, 2, &lpub_recvData[2]);
    lub_status = app_RC522_ToCard(PCD_Transceive, lpub_recvData, 4, lpub_recvData, luw_unLen);

    return lub_status;
}

/**********************************************************
 * Name: app_RC522_CalculateCRC
 * Description: TBD
 **********************************************************/
static void app_RC522_CalculateCRC(T_UBYTE *lpub_Indata, T_UBYTE lub_len, T_UBYTE *lpub_OutData) {
    T_UBYTE i, n;

    app_RC522_WriteRegister(CommandReg, PCD_Idle);
    app_RC522_ClearRegisterBitMask(DivIrqReg, 0x04); //CRCIrq = 0
    app_RC522_SetRegisterBitMask(FIFOLevelReg, 0x80); //Clear FIFO


    /* Write Input Data in FIFO Buffer */
    for (i = 0; i < lub_len; i++) {
        app_RC522_WriteRegister(FIFODataReg, *(lpub_Indata + i));
    }

    /* CRC Calculation Command */
    app_RC522_WriteRegister(CommandReg, PCD_CalcCRC);

    //APP_RC522_TIMER_LOAD(rub_RC522WatchDog);
    do {
        n = app_RC522_ReadRegister(DivIrqReg);
    } while (((n & 0x04) == 0));

    //Guarda el calculo del CRC
    lpub_OutData[0] = app_RC522_ReadRegister(CRCResultRegL);
    lpub_OutData[1] = app_RC522_ReadRegister(CRCResultRegH);
}

static T_UBYTE app_RC522_Anticoll(T_UBYTE *lpub_serNum) {
    T_UBYTE lub_status;
    T_UBYTE i;
    T_UBYTE lub_serNumCheck = 0;

    app_RC522_ClearRegisterBitMask(CollReg, 0x80); //ValuesAfterColl
    app_RC522_WriteRegister(BitFramingReg, 0x00); //TxLastBists = BitFramingReg[2..0]

    lpub_serNum[0] = PICC_CMD_SEL_CL1;
    lpub_serNum[1] = 0x20;
    lub_status = app_RC522_ToCard(PCD_Transceive, lpub_serNum, 2, lpub_serNum, &ruw_RC522_FIFOReceivedLength);

    if (lub_status == STATUS_OK) {
        //?????? Compruebe el nÃƒÂºmero de serie de la tarjeta
        for (i = 0; i < 4; i++) {
            lub_serNumCheck ^= lpub_serNum[i];
        }
        if (lub_serNumCheck != lpub_serNum[i]) {
            lub_status = STATUS_ERROR;
        }
    }

    return lub_status;
}

/**********************************************************
 * Name: app_RC522_SelectCard
 * Description: Select a CARD
 **********************************************************/
static T_UBYTE app_RC522_SelectCard(T_UBYTE *lpub_CardID, T_UBYTE lub_IDSize) {
    T_UBYTE lub_status = STATUS_ERROR;
    T_UBYTE laub_BufferToSend[18];

    //Prepare RC522
    app_RC522_ClearRegisterBitMask(CollReg, 0x80); // ValuesAfterColl=1 => Bits received after collision are cleared.

    laub_BufferToSend[0] = PICC_CMD_SEL_CL1; //PICC Command
    laub_BufferToSend[1] = 0x70; // NVB - Number of Valid Bits: Seven whole bytes
    /* Store ID */
    for (T_UBYTE i = 0; i < lub_IDSize; i++) {
        laub_BufferToSend[2 + i] = *(lpub_CardID + i);
    }
    // Calculate BCC - Block Check Character
    laub_BufferToSend[6] = laub_BufferToSend[2] ^ laub_BufferToSend[3] ^ laub_BufferToSend[4] ^ laub_BufferToSend[5];

    //Calculate CRC_A
    app_RC522_CalculateCRC(laub_BufferToSend, 7, &laub_BufferToSend[7]);

    lub_status = app_RC522_ToCard(PCD_Transceive, laub_BufferToSend, 9, raub_RC522_FIFOData, &ruw_RC522_FIFOReceivedLength);

    return lub_status;


} // End PICC_Select()

/**********************************************************
 * Name: app_RC522_Authenticate
 * Description: Authenticate card to start an encrypted communication
 **********************************************************/
static T_UBYTE app_RC522_Authenticate(T_UBYTE lub_Command, T_UBYTE lub_BlockAddress, const T_UBYTE *lpub_Key, T_UBYTE *lpub_CardID) {
    T_UBYTE lub_SendData[12];

    /* Prepare data to send */
    lub_SendData[0] = lub_Command;
    lub_SendData[1] = lub_BlockAddress;
    /* Store Key */
    for (T_UBYTE i = 0; i < APP_RC522_KEY_SIZE; i++) {
        lub_SendData[2 + i] = *(lpub_Key + i);
    }
    /* Store ID */
    for (T_UBYTE i = 0; i < 4; i++) {
        lub_SendData[8 + i] = *(lpub_CardID + i);
    }

    return app_RC522_ToCard(PCD_MFAuthent, lub_SendData, sizeof (lub_SendData), raub_RC522_FIFOData, &ruw_RC522_FIFOReceivedLength);
}

/**********************************************************
 * Name: app_RC522_WriteBlock
 * Description: TBD
 **********************************************************/
static T_UBYTE app_RC522_WriteBlock(T_UBYTE lub_BlockAddr, T_UBYTE *lpub_recvData, T_UWORD *luw_unLen) {
    T_UBYTE lub_status;

    lpub_recvData[0] = PICC_CMD_MF_WRITE;
    lpub_recvData[1] = lub_BlockAddr;
    app_RC522_CalculateCRC(lpub_recvData, 2, &lpub_recvData[2]);
    lub_status = app_RC522_ToCard(PCD_Transceive, lpub_recvData, 4, lpub_recvData, luw_unLen);

    if (lub_status == STATUS_OK) {
        /* Prepare data to store into CARD */
        for (T_UBYTE i = 0; i < 16U; i++) {
            if (rub_EraseRequestFlag == TRUE) {
                lpub_recvData[i] = 0;
            } else {
                lpub_recvData[i] = caub_RC522_TagData[i];
            }
        }
        app_RC522_CalculateCRC(lpub_recvData, 16, &lpub_recvData[16]);
        lub_status = app_RC522_ToCard(PCD_Transceive, lpub_recvData, 18, lpub_recvData, luw_unLen);
    } else {
        /* Do nothing */
    }

    return lub_status;
}

/**********************************************************
 * Name: app_RC522_TimeoutTask
 * Description: TBD
 **********************************************************/
void app_RC522_TimeoutTask(void) {
    if (APP_RC522_TIMER_IS_STOPPED(rub_RC522WatchDog) == TRUE) {
        /* Do Nothing */
    } else {
        rub_RC522WatchDog--;
        if ((APP_RC522_TIMER_IS_STOPPED(rub_RC522WatchDog) == TRUE)) {
            rub_RC522_TimeoutFlag = TRUE;
        } else {
            /* Do Nothing */
        }
    }
}

/**********************************************************
 * Name: app_RC522_ModuleIsFail
 * Description: TBD
 **********************************************************/
T_UBYTE app_RC522_ModuleIsFail(void) {

    if (APP_RC522_MODULE_IS_FAIL() == TRUE) {
        if (rub_RC522DematureCounter > 0U) {
            rub_RC522DematureCounter--;
        } else {
            rub_FailureFlag = FALSE;
            re_RC522State = RC522_STATE_INIT;
            re_RC522_NextState = RC522_STATE_CARD_SEARCH;
            rub_ErrorMatureCounter = 0U;
            LED_RED_OFF();
        }
    } else {
        /* Do Nothing */
    }
    return APP_RC522_MODULE_IS_FAIL();
}

/**********************************************************
 * Name: app_RC522_FailCondition
 * Description: TBD
 **********************************************************/
static void app_RC522_FailCondition(void) {
    if (APP_RC522_MODULE_IS_FAIL() == FALSE) {
        /* Increase  Error Mature Counter */
        rub_ErrorMatureCounter++;
        if (APP_RC522_MODULE_IS_FAIL() == TRUE) {
            rub_FailureFlag = TRUE;
        }
    } else {
        rub_RC522DematureCounter = 10U;
    }
}

/**********************************************************
 * Name: app_RC522_CompareData
 * Description: TBD
 **********************************************************/
static T_UBYTE app_RC522_CompareData(T_UBYTE *lpub_A, T_UBYTE *lpub_B, T_UBYTE lub_Length) {
    T_UBYTE lub_Result;

    /* Initialize the return value */
    lub_Result = TRUE;

    /* Compare each byte */
    for (T_UBYTE lub_i = 0; lub_i < lub_Length; lub_i++) {
        if (*(lpub_A + lub_i) != *(lpub_B + lub_i)) {
            /* Difference */
            lub_Result = FALSE;
        }
    }

    return lub_Result;
}

/**********************************************************
 * Name: app_RC522_WriteRequest
 * Description: TBD
 **********************************************************/
void app_RC522_WriteRequest(void) {
    rub_WriteRequestFlag = TRUE;
    rub_EraseRequestFlag = FALSE;
}

/**********************************************************
 * Name: app_RC522_EraseRequest
 * Description: TBD
 **********************************************************/
void app_RC522_EraseRequest(void) {
    rub_WriteRequestFlag = TRUE;
    rub_EraseRequestFlag = TRUE;
}

/**********************************************************
 * Name: app_RC522_SearchOnGoing
 * Description: TBD
 **********************************************************/
T_UBYTE app_RC522_SearchOnGoing(void) {
    return rub_SearchOnGoingFlag;
}