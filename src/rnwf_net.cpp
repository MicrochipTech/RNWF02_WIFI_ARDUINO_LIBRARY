/**
 * \file rnwf_net.cpp
 * \brief This source file contains APIs for configuring the NET and Socket Parameters.
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

#include "rnwf_net.h"

bool NET::RNWF_WifiOpenSocket(const char *cmd) {
  RNWF_SendCmd(cmd);
  return true; 
}

bool NET::RNWF_WifiSocketBindLocal(const char *cmd) {
  RNWF_SendCmd(cmd);
  return true; 
}

bool NET::RNWF_WifiSocketBindRemote(const char *cmd) {
  RNWF_SendCmd(cmd);
  return true; 
}

bool NET::RNWF_WifiSocketWriteTcp(const char *cmd,const char *msg) {
  RNWF_SendCommand_ReceiveResponse(cmd, "#");
  RNWF_SendCmd(msg);
  return true; 
}

bool NET::RNWF_WifiSocketReadTcp(const char *cmd) {
  RNWF_SendCmd(cmd);
  return true; 
}