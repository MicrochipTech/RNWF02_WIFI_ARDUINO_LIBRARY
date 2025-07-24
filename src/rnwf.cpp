/*******************************************************************************
  RNWF Host Assisted System Service Implementation

  File Name:
    rnwf.cpp

  Summary:
    Source code for the RNWF Host Assisted System Service implementation.

  Description:
    This source file provides a simple APIs to enable communication with 
    RNWF device 
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
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "rnwf.h"
#include "rnwf_interface.h"

bool RNWF_ResetTarget(void) {
  return RNWF_SendCommand_ReceiveResponse(RNWF_RESET_TARGET, RNWF_DEFAULT_RESPONSE);
}

bool RNWF_SetEchoOff(void) {
  return RNWF_SendCommand_ReceiveResponse(RNWF_SET_ECHO_OFF, RNWF_DEFAULT_RESPONSE);
}

bool RNWF_GetManId(void) {
  return RNWF_SendCommand_ReceiveResponse(RNWF_GET_MAN_ID, RNWF_DEFAULT_RESPONSE);
}

bool RNWF_GetRevId(void) {
  return RNWF_SendCommand_ReceiveResponse(RNWF_GET_REV_ID, RNWF_DEFAULT_RESPONSE);
}

bool RNWF_WifiInfo(void) {
  return RNWF_SendCommand_ReceiveResponse(RNWF_GET_WIFI_INFO, RNWF_DEFAULT_RESPONSE);
}

bool RNWF_WifiSetRegDomain(const char *cmd) {
  return RNWF_SendCommand_ReceiveResponse(cmd, RNWF_DEFAULT_RESPONSE);
}

bool RNWF_WifiGetSoftApState(void) {
  RNBD_SendCmd(SYS_RNWF_WIFI_GET_AP_STATE);
  return true; 
}

bool RNWF_WifiSoftApDisable(void) {
  RNBD_SendCmd(SYS_RNWF_WIFI_SOFTAP_DISABLE);
  return true; 
}

bool RNWF_WifiGetStaState(void) {
  RNBD_SendCmd(SYS_RNWF_WIFI_GET_STA_STATE);
  return true; 
}

bool RNWF_WifiDisconnect(void) {
  RNBD_SendCmd(SYS_RNWF_WIFI_DISCONNECT);
  return true; 
}

bool RNWF_WifiSetStaSsid(const char *cmd) {
  return RNWF_SendCommand_ReceiveResponse(cmd, RNWF_DEFAULT_RESPONSE);
}

bool RNWF_WifiSetStaPwd(const char *cmd) {
  return RNWF_SendCommand_ReceiveResponse(cmd, RNWF_DEFAULT_RESPONSE);
}

bool RNWF_WifiSetStaSec(const char *cmd) {
  return RNWF_SendCommand_ReceiveResponse(cmd, RNWF_DEFAULT_RESPONSE);
}

bool RNWF_WifiConnect(void) {
  RNBD_SendCmd(SYS_RNWF_WIFI_CONNECT);
  return true; 
}

bool RNWF_WifiOpenSocket(const char *cmd) {
  RNBD_SendCmd(cmd);
  return true; 
}

bool RNWF_WifiSocketBindLocal(const char *cmd) {
  RNBD_SendCmd(cmd);
  return true; 
}

bool RNWF_WifiSocketBindRemote(const char *cmd) {
  RNBD_SendCmd(cmd);
  return true; 
}

bool RNWF_WifiSocketWriteTcp(const char *cmd,const char *msg) {
  RNWF_SendCommand_ReceiveResponse(cmd, "#");
  RNBD_SendCmd(msg);
  return true; 
}

bool RNWF_WifiSocketReadTcp(const char *cmd) {
  RNBD_SendCmd(cmd);
  return true; 
}

