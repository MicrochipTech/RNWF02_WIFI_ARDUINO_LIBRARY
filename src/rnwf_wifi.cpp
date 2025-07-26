/**
 * \file rnwf_wifi.cpp
 * \brief This source file contains APIs for configuring the Wi-Fi Parameters.
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

#include "rnwf_wifi.h"

bool WIFI::RNWF_ResetTarget(void) {
  return RNWF_SendCommand_ReceiveResponse(RNWF_RESET_TARGET, RNWF_DEFAULT_RESPONSE);
}

bool WIFI::RNWF_SetEchoOff(void) {
  return RNWF_SendCommand_ReceiveResponse(RNWF_SET_ECHO_OFF, RNWF_DEFAULT_RESPONSE);
}

bool WIFI::RNWF_GetManId(void) {
  return RNWF_SendCommand_ReceiveResponse(RNWF_GET_MAN_ID, RNWF_DEFAULT_RESPONSE);
}

bool WIFI::RNWF_GetRevId(void) {
  return RNWF_SendCommand_ReceiveResponse(RNWF_GET_REV_ID, RNWF_DEFAULT_RESPONSE);
}

bool WIFI::RNWF_WifiInfo(void) {
  return RNWF_SendCommand_ReceiveResponse(RNWF_GET_WIFI_INFO, RNWF_DEFAULT_RESPONSE);
}

bool WIFI::RNWF_WifiSetRegDomain(const char *cmd) {
  return RNWF_SendCommand_ReceiveResponse(cmd, RNWF_DEFAULT_RESPONSE);
}

bool WIFI::RNWF_WifiGetSoftApState(void) {
  RNWF_SendCmd(SYS_RNWF_WIFI_GET_AP_STATE);
  return true; 
}

bool WIFI::RNWF_WifiSoftApDisable(void) {
  RNWF_SendCmd(SYS_RNWF_WIFI_SOFTAP_DISABLE);
  return true; 
}

bool WIFI::RNWF_WifiGetStaState(void) {
  RNWF_SendCmd(SYS_RNWF_WIFI_GET_STA_STATE);
  return true; 
}

bool WIFI::RNWF_WifiDisconnect(void) {
  RNWF_SendCmd(SYS_RNWF_WIFI_DISCONNECT);
  return true; 
}

bool WIFI::RNWF_WifiSetStaSsid(const char *cmd) {
  return RNWF_SendCommand_ReceiveResponse(cmd, RNWF_DEFAULT_RESPONSE);
}

bool WIFI::RNWF_WifiSetStaPwd(const char *cmd) {
  return RNWF_SendCommand_ReceiveResponse(cmd, RNWF_DEFAULT_RESPONSE);
}

bool WIFI::RNWF_WifiSetStaSec(const char *cmd) {
  return RNWF_SendCommand_ReceiveResponse(cmd, RNWF_DEFAULT_RESPONSE);
}

bool WIFI::RNWF_WifiConnect(void) {
  RNWF_SendCmd(SYS_RNWF_WIFI_CONNECT);
  return true; 
}

bool WIFI::RNWF_WifiSetSntpStatic(void) {
  return RNWF_SendCommand_ReceiveResponse(RNWF_SET_SNTP_STATIC, RNWF_DEFAULT_RESPONSE);
}

bool WIFI::RNWF_WifiSetSntpUrl(const char *cmd) {
  return RNWF_SendCommand_ReceiveResponse(cmd, RNWF_DEFAULT_RESPONSE);
}

bool WIFI::RNWF_WifiSntpEnable(void) {
  return RNWF_SendCommand_ReceiveResponse(RNWF_SET_SNTP_ENBL, RNWF_DEFAULT_RESPONSE);
}