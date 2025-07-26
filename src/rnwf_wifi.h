/**
 * \file rnwf_wifi.h
 * \brief This header file contains APIs for configuring the Wi-Fi Parameters.
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
#define RNWF_SET_SNTP_DHCP      "AT+SNTPC=2,0\r\n"
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

class WIFI {
  private:

  public:

/**
  * \ingroup WIFI
  * \brief Resets the RNWF.
  * 
  * This API Resets the RNWF.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_ResetTarget(void);

/**
  * \ingroup WIFI
  * \brief Disable ECHO.
  * 
  * This API Disable the ECHO.
  * 
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_SetEchoOff(void);

/**
  * \ingroup WIFI
  * \brief Gets Manufacturer Info.
  * 
  * This API Gets Manufacturer Info.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_GetManId(void);

/**
  * \ingroup WIFI
  * \brief Gets Revision Info.
  * 
  * This API Gets Revision Info.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_GetRevId(void);

/**
  * \ingroup WIFI
  * \brief Gets Wi-Fi Info.
  * 
  * This API Gets Wi-Fi Info.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiInfo(void);

/**
  * \ingroup WIFI
  * \brief Sets Regional Domain.
  * 
  * This API Sets Regional Domain.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiSetRegDomain(const char *cmd);

/**
  * \ingroup WIFI
  * \brief Gets SoftAP Status.
  * 
  * This API Gets SoftAP Status.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiGetSoftApState(void);

/**
  * \ingroup WIFI
  * \brief Disable SoftAP.
  * 
  * This API Disable SoftAP.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiSoftApDisable(void);

/**
  * \ingroup WIFI
  * \brief Gets STA Status.
  * 
  * This API Gets STA Status.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiGetStaState(void);

/**
  * \ingroup WIFI
  * \brief Disconnects from AP.
  * 
  * This API Disconnects from AP.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiDisconnect(void);

/**
  * \ingroup WIFI
  * \brief Sets SSID.
  * 
  * This API Sets SSID.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiSetStaSsid(const char *cmd);

/**
  * \ingroup WIFI
  * \brief Sets PASSWORD.
  * 
  * This API Sets PASSWORD.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiSetStaPwd(const char *cmd);

/**
  * \ingroup WIFI
  * \brief Sets STA Security type.
  * 
  * This API Sets STA Security type.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiSetStaSec(const char *cmd);

/**
  * \ingroup WIFI
  * \brief Connects to AP.
  * 
  * This API Connects to AP.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiConnect(void);

/**
  * \ingroup WIFI
  * \brief Sets SNTP Static IP.
  * 
  * This API Sets SNTP Static IP.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiSetSntpStatic(void);

/**
  * \ingroup WIFI
  * \brief Sets SNTP URL.
  * 
  * This API Sets SNTP URL.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiSetSntpUrl(const char *cmd);

/**
  * \ingroup WIFI
  * \brief Enables SNTP.
  * 
  * This API Enables SNTP.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiSntpEnable(void);

};

#endif /* RNWF_H */