/**
 * \file rnwf_interface.h
 * \brief This header file contains APIs to Write and Read the command and data via UART.
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

#ifndef RNWF_INTERFACE_H
#define RNWF_INTERFACE_H

#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#define RESPONSE_TIMEOUT 65U

void RNWF_SerialInit(HardwareSerial* serial);
String UART_RNWF_Read(void);
void UART_RNWF_write(uint8_t buffer);
bool UART_RNWF_ReceiveReady(void);
bool UART_RNWF_TransmitReady(void);
static inline void RNWF_Delay(uint32_t delayCount);
static char resp[100];

/**
  * \ingroup RNWF
  * \brief Send command and read the response.
  * 
  * This API Send command and read the response from RNWF.
  * 
  * \return User Defined Mode Status
  * \retval true - Success
  * \retval false - Failure

  * Paramater 1 - Command to be sent
  * Parameter 2 - Expected Response message
  */
bool RNWF_SendCommand_ReceiveResponse(const char *cmdMsg, const char *responsemsg);

/**
  * \ingroup RNWF
  * \brief Send command.
  * 
  * This API Send command to RNWF.
  * 
  * \return User Defined Mode Status
  * \retval true - Success
  * \retval false - Failure

  * Paramater 1 - Command to be sent
  */
void RNWF_SendCmd(const char *cmd);

#endif /* RNWF_INTERFACE_H */