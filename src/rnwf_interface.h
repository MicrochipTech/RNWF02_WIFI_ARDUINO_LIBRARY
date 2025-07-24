/*******************************************************************************
  RNWF Host Assisted System Service Implementation

  File Name:
    rnwf_interface.h

  Summary:
    Source code for the RNWF Host Assisted System Service implementation.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.
 *******************************************************************************/

/*******************************************************************************
Copyright (C) 2025 released Microchip Technology Inc.  All rights reserved.

 
Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#ifndef RNWF_INTERFACE_H
#define RNWF_INTERFACE_H

#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#define RNWFserial Serial1
#define RESPONSE_TIMEOUT 65U



String UART_RNWF_Read(void);
void UART_RNWF_write(uint8_t buffer);
bool UART_RNWF_ReceiveReady(void);
bool UART_RNWF_TransmitReady(void);
static inline void RNWF_Delay(uint32_t delayCount);
static char resp[100];

/**
  * \ingroup RNBD
  * \brief Puts the RNBD in user defined mode (Command Mode/Data Mode/ Set commands etc..).
  * 
  * This API puts the RNBD in user defined mode (Command Mode/Data Mode/ Set commands etc..). 
  * 
  * \return User Defined Mode Status
  * \retval true - Success
  * \retval false - Failure

  * Paramater 1 - Command to be sent
  * Parameter 2 - Length of Command to be sent
  * Parameter 3 - Expected Response message 
  * Parameter 4 - Expected Response length 
  */
bool RNWF_SendCommand_ReceiveResponse(const char *cmdMsg, const char *responsemsg);

void RNBD_SendCmd(const char *cmd);

#endif /* RNWF_INTERFACE_H */