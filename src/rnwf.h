/*******************************************************************************
  RNWF Host Assisted System Service Implementation

  File Name:
    rnwf.h

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

#ifndef RNWF_H
#define RNWF_H

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include "rnwf_interface.h"

/* RNWF Basic Commands */
#define RNWF_SET_ECHO_OFF       "ATE0\r\n"

#define RNWF_RESET_TARGET       "AT+RST\r\n"
#define RNWF_GET_MAN_ID         "AT+GMI\r\n"
#define RNWF_GET_MOD_ID         "AT+GMM\r\n"
#define RNWF_GET_REV_ID         "AT+GMR\r\n"
#define RNWF_GET_DEV_INFO       "AT+DI\r\n"

/* RNWF SNTP Commands */
#define RNWF_SET_SNTP_ENBL      "AT+SNTPC=1,1\r\n"
#define RNWF_SET_SNTP_STATIC    "AT+SNTPC=2,1\r\n"
#define RNWF_SET_SNTP_DHCP      "AT+SNTPC=2,1\r\n"
#define RNWF_SET_SNTP_URL       "AT+SNTPC=3,\"%s\"\r\n"

/* RNWF TIME Commands */
#define RNWF_GET_SYS_TIME       "AT+TIME,3\r\n"

#define RNWF_SET_SYS_TIME_UNIX  "AT+TIME=1,%lu\r\n"
#define RNWF_SET_SYS_TIME_NTP   "AT+TIME=2,%lu\r\n"

/* Date/time in format YYYY-MM-DDTHH:MM:SS.00Z */ 
#define RNWF_SET_SYS_TIME_STRING    "AT+TIME=3,\"%s\"\r\n"

/* RNWF Certificate Commands */
#define RNWF_GET_CERT_LIST      "AT+FS=2,1\r\n"
#define RNWF_GET_KEY_LIST       "AT+FS=2,2\r\n"


#define RNWF_GET_WIFI_INFO     "AT+NETIFC=0\r\n"

#define RNWF_GET_MQTT_INFO     "AT+MQTTC\r\n"

#define RNWF_DEFAULT_RESPONSE  "OK\r\n"


/*RNWF Wi-Fi Commands*/
#define SYS_RNWF_WIFI_GET_STA_STATE     "AT+WSTA\r\n"
#define SYS_RNWF_WIFI_CONNECT           "AT+WSTA=1\r\n"
#define SYS_RNWF_WIFI_DISCONNECT        "AT+WSTA=0\r\n"
#define SYS_RNWF_WIFI_SET_STA_SSID      "AT+WSTAC=1,\"%s\"\r\n"
#define SYS_RNWF_WIFI_SET_STA_SEC       "AT+WSTAC=2,%d\r\n"
#define SYS_RNWF_WIFI_SET_STA_PWD       "AT+WSTAC=3,\"%s\"\r\n"

#define SYS_RNWF_WIFI_SET_STA_CHANNEL   "AT+WSTAC=4,%d\r\n"
#define SYS_RNWF_WIFI_SET_STA_BSSID     "AT+WSTAC=5,%s\r\n"
#define SYS_RNWF_WIFI_SET_STA_TIMEOUT   "AT+WSTAC=7,%d\r\n"

#define SYS_RNWF_WIFI_GET_AP_STATE   "AT+WAP\r\n"
#define SYS_RNWF_WIFI_SOFTAP_ENABLE  "AT+WAP=1\r\n"
#define SYS_RNWF_WIFI_SOFTAP_DISABLE "AT+WAP=0\r\n"
#define SYS_RNWF_WIFI_SET_AP_SSID    "AT+WAPC=1,\"%s\"\r\n"
#define SYS_RNWF_WIFI_SET_AP_SEC     "AT+WAPC=2,%d\r\n"
#define SYS_RNWF_WIFI_SET_AP_PWD     "AT+WAPC=3,\"%s\"\r\n"
#define SYS_RNWF_WIFI_SET_AP_CHANNEL "AT+WAPC=4,%d\r\n"
#define SYS_RNWF_WIFI_SET_AP_HIDDEN  "AT+WAPC=5,%d\r\n"

#define SYS_RNWF_WIFI_POW_SAVE_MODE "AT+WIFIC=20,1\r\n"
#define SYS_RNWF_WIFI_SET_REG_DOMAIN    "AT+WIFIC=10,\"%s\"\r\n"
#define SYS_RNWF_WIFI_PSV_SCAN      "AT+WSCN=0\r\n"
#define SYS_RNWF_WIFI_ACT_SCAN      "AT+WSCN=1\r\n"
#define SYS_RNWF_WIFI_DNS_CMD       "AT+DNSRESOLV=1,\"%s\"\r\n"
#define SYS_RNWF_WIFI_PING_CMD      "AT+PING=\"%s\"\r\n"
#define SYS_RNWF_WIFI_CONF_INFO     "AT+WIFIC\r\n"
#define SYS_RNWF_SSID_LEN_MAX       33
#define SYS_RNWF_BSSID_LEN_MAX      32
#define SYS_RNWF_PWD_LEN_MAX        128


/*RNWF Wi-Fi BT Configuration commands*/
#define SYS_RNWF_WIFI_BT_COEXIST_ENABLE "AT+WIFIC=30,%d\r\n"
#define SYS_RNWF_WIFI_BT_INF_TYPE       "AT+WIFIC=31,%d\r\n"
#define SYS_RNWF_WIFI_BT_RX_PRIO        "AT+WIFIC=32,%d\r\n"
#define SYS_RNWF_WIFI_BT_TX_PRIO        "AT+WIFIC=33,%d\r\n"
#define SYS_RNWF_WIFI_BT_ANTENNA_MODE   "AT+WIFIC=34,%d\r\n"

/*Wi-Fi max Callback service*/
#define SYS_RNWF_WIFI_SERVICE_CB_MAX     2

/*Wi-Fi STA Security Type*/
#define SYS_RNWF_WIFI_RSVD                 0                  
#define SYS_RNWF_WIFI_SECURITY_WPA2_MIXED  1             
#define SYS_RNWF_WIFI_SECURITY_WPA2        2                  
#define SYS_RNWF_WIFI_SECURITY_WPA3_TRANS  3            
#define SYS_RNWF_WIFI_SECURITY_WPA3        4

/**
  * \ingroup RNWF
  * \brief Resets the RNWF.
  * 
  * This API Resets the RNBD.
  * 
  * \return Data Mode Status
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_ResetTarget(void);

bool RNWF_SetEchoOff(void);

bool RNWF_GetManId(void);

bool RNWF_GetRevId(void);

bool RNWF_WifiInfo(void);

bool RNWF_WifiSetRegDomain(const char *cmd);

bool RNWF_WifiGetSoftApState(void);

bool RNWF_WifiSoftApDisable(void);

bool RNWF_WifiGetStaState(void);

bool RNWF_WifiDisconnect(void);

bool RNWF_WifiSetStaSsid(const char *cmd);

bool RNWF_WifiSetStaPwd(const char *cmd);

bool RNWF_WifiSetStaSec(const char *cmd);

bool RNWF_WifiConnect(void);

bool RNWF_WifiOpenSocket(const char *cmd);

bool RNWF_WifiSocketBindLocal(const char *cmd);

bool RNWF_WifiSocketBindRemote(const char *cmd);

bool RNWF_WifiSocketWriteTcp(const char *cmd,const char *msg);

bool RNWF_WifiSocketReadTcp(const char *cmd);

#endif /* RNWF_H */