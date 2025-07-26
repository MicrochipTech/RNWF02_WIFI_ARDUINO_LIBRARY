/**
 * \file rnwf_interface.cpp
 * \brief This source file contains APIs to Write and Read the command and data via UART.
 */
/*
© 2025 Microchip Technology Inc. and its subsidiaries. All rights reserved.

Subject to your compliance with these terms, you may use this Microchip software and any derivatives 
exclusively with Microchip products. You are responsible for complying with third party license terms 
applicable to your use of third party software (including open source software) that may accompany this 
Microchip software. SOFTWARE IS “AS IS.” NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR 
STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL 
MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, 
DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER 
CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP’S TOTAL LIABILITY ON ALL 
CLAIMS RELATED TO THE SOFTWARE WILL NOT EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY 
TO MICROCHIP FOR THIS SOFTWARE
*/

#include <Arduino.h>
#include "rnwf_interface.h"
#include <string.h>
#include <stdio.h>

static size_t dummyread = 0;
static char ReadData[100];

// Static variable to hold the user's Serial pointer
static HardwareSerial* RNWFserial = nullptr;

void RNWF_SerialInit(HardwareSerial* serial) {
    RNWFserial = serial;
}

String UART_RNWF_Read(void) {
  String receivedString = RNWFserial->readString();
  return receivedString;
}

void UART_RNWF_write(const char *buffer) {
  dummyread = RNWFserial->write(buffer);
}

bool UART_RNWF_ReceiveReady(void) {
  return RNWFserial->available() != 0U ? true : false;
}

bool UART_RNWF_TransmitReady(void) {
  return RNWFserial->availableForWrite() != 0U ? true : false;
}

static inline void RNWF_Delay(uint32_t delayCount) {
  if (delayCount > 0U) {
    delay(delayCount);
  }
}

void RNWF_SendCmd(const char *cmd) {
  while (!UART_RNWF_TransmitReady()) {}
  UART_RNWF_write(cmd);
  while (!UART_RNWF_TransmitReady()) {}

  Serial.print("CMD SENT:");
  Serial.println(cmd);
}

bool RNWF_SendCommand_ReceiveResponse(const char *cmdMsg, const char *responsemsg) {
  unsigned long startTime = millis();
  String receivedString;

  //Flush out any unread data
  while (UART_RNWF_ReceiveReady()) {
    (void)UART_RNWF_Read();
  }

  //Sending Command to UART
  RNWF_SendCmd(cmdMsg);

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