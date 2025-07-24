/*******************************************************************************
     RNWF Host Assisted System Service Implementation

     File Name:
       rnwf_interface.cpp

     Summary:
       Source code for the RNWF Host Assisted System Service implementation.

     Description:
       This file contains the source code for the RNWF Interface with arduino 
       implementation.
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

#include <Arduino.h>
#include "rnwf_interface.h"
#include <string.h>
#include <stdio.h>

static size_t dummyread = 0;
static char ReadData[100];


String UART_RNWF_Read(void) {
  String receivedString = RNWFserial.readString();
  return receivedString;
}

void UART_RNWF_write(const char *buffer) {
  dummyread = RNWFserial.write(buffer);
}

bool UART_RNWF_ReceiveReady(void) {
  return RNWFserial.available() != 0U ? true : false;
}

bool UART_RNWF_TransmitReady(void) {
  return RNWFserial.availableForWrite() != 0U ? true : false;
}

static inline void RNWF_Delay(uint32_t delayCount) {
  if (delayCount > 0U) {
    delay(delayCount);
  }
}

void RNBD_SendCmd(const char *cmd) {
  while (!UART_RNWF_TransmitReady()) {}
  UART_RNWF_write(cmd);
  while (!UART_RNWF_TransmitReady()) {}

  Serial.print("CMD SENT:");
  Serial.println(cmd);
}

bool RNWF_SendCommand_ReceiveResponse(const char *cmdMsg, const char *responsemsg) {  //(const char *cmdMsg, uint8_t cmdLen, const char *responsemsg, uint8_t responseLen)
  unsigned long startTime = millis();
  String receivedString;

  //Flush out any unread data
  while (UART_RNWF_ReceiveReady()) {
    (void)UART_RNWF_Read();
  }

  //Sending Command to UART
  RNBD_SendCmd(cmdMsg);

  // Read data for 2 second
  while (millis() - startTime < 2000) {
    if (UART_RNWF_ReceiveReady()) {
      receivedString += UART_RNWF_Read();
    }
  }

  Serial.println("```````````````RESPONSE RECEIVED```````````````");
  Serial.println(receivedString);
  Serial.println("```````````````````````````````````````````````");

  // Check if the response message is present in the received string
  if (receivedString.indexOf(responsemsg) == -1) {
    Serial.println("!!!---RESPONSE NOT MATCHED---!!!");
    return false;
  }
  return true;
}