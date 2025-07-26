/**
 * \file rnwf_net.h
 * \brief This header file contains APIs for configuring the NET and Socket Parameters.
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

#ifndef RNWF_NET_H
#define RNWF_NET_H

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include "rnwf_interface.h"

/* Number of clients connecting to the server socket, with the maximum value being 5.*/
#define SYS_RNWF_NET_NO_OF_CLIENT_SOCKETS       4

/*RNWF Network Socket max callback service */
#define SYS_RNWF_NET_SOCK_SERVICE_CB_MAX    2
#define SYS_RNWF_NET_SOCK_PORT0					6666
#define SYS_RNWF_SOCK_ID_LEN_MAX            8
#define SYS_RNWF_SOCK_ADDR_LEN_MAX          32
#define SYS_RNWF_SOCK_TLS_CFG_LEN_MAX       64

/*RNWF NET Service commands */
#define SYS_RNWF_DHCPS_ENABLE               "AT+DHCPSC=0,1,1\r\n"
#define SYS_RNWF_DHCPS_DISABLE              "AT+DHCPSC=0,1,0\r\n"
#define SYS_RNWF_DHCPS_POOL_START           "AT+DHCPSC=0,2,\"%s\"\r\n"
#define SYS_RNWF_DHCPS_SET_GW               "AT+DHCPSC=0,10,\"%s\"\r\n"
#define SYS_RNWF_NETIF_SET_IP               "AT+NETIFC=0,40,\"%s\"\r\n"
#define SYS_RNWF_SOCK_GET_LIST              "AT+SOCKLST\r\n"
#define SYS_RNWF_SOCK_OPEN_UDP              "AT+SOCKO=1,4\r\n"
#define SYS_RNWF_SOCK_OPEN_TCP              "AT+SOCKO=2,%d\r\n"
#define SYS_RNWF_SOCK_OPEN_RESP             "+SOCKO:"
#define SYS_RNWF_SOCK_BIND_LOCAL            "AT+SOCKBL=%lu,%d,%d\r\n"
#define SYS_RNWF_SOCK_BIND_REMOTE           "AT+SOCKBR=%lu,\"%s\",%d\r\n"
#define SYS_RNWF_SOCK_BIND_MCAST            "AT+SOCKBM=%lu,%s,%d\r\n"
#define SYS_RNWF_SOCK_BINARY_WRITE_TCP      "AT+SOCKWR=%lu,%u\r\n"
#define SYS_RNWF_SOCK_BINARY_WRITE_UDP      "AT+SOCKWRTO=%lu,%s,%d,%d\r\n"
#define SYS_RNWF_SOCK_ASCII_WRITE_TCP       "AT+SOCKWR=%d,%d,\"%.*s\"\r\n"
#define SYS_RNWF_SOCK_ASCII_WRITE_UDP       "AT+SOCKWRTO=%d,\"%s\",%d,%d,\"%s\"\r\n"
#define SYS_RNWF_SOCK_READ_BUF              "AT+SOCKRDBUF=%lu,%d,%d\r\n"
#define SYS_RNWF_SOCK_READ                  "AT+SOCKRD=%lu,%d,%d\r\n"
#define SYS_RNWF_SOCK_READ_RESP             "+SOCKRD:"
#define SYS_RNWF_SOCK_CLOSE                 "AT+SOCKCL=%lu\r\n"
#define SYS_RNWF_SOCK_CONFIG_TLS            "AT+SOCKTLS=%lu,%d\r\n"
#define SYS_RNWF_SOCK_CONFIG_KEEPALIVE      "AT+SOCKC=%lu,1,%d\r\n"
#define SYS_RNWF_SOCK_CONFIG_NODELAY        "AT+SOCKC=%lu,2,%d\r\n"
#define SYS_RNWF_SOCK_TLS_GET_CONFIG        "AT+TLSC\r\n"
#define SYS_RNWF_SOCK_TLS_SET_CA_NAME       "AT+TLSC=%d,1,\"%s\"\r\n"
#define SYS_RNWF_SOCK_TLS_SET_CERT_NAME     "AT+TLSC=%d,2,\"%s\"\r\n"
#define SYS_RNWF_SOCK_TLS_SET_KEY_NAME      "AT+TLSC=%d,3,\"%s\"\r\n"
#define SYS_RNWF_SOCK_TLS_SET_KEY_PWD       "AT+TLSC=%d,4,\"%s\"\r\n"
#define SYS_RNWF_SOCK_TLS_SERVER_NAME       "AT+TLSC=%d,5,\"%s\"\r\n"
#define SYS_RNWF_SOCK_TLS_DOMAIN_NAME       "AT+TLSC=%d,6,\"%s\"\r\n"
#define SYS_RNWF_NET_PEER_AUTHENTICATION    "AT+TLSC=%d,40,%d\r\n"
#define SYS_RNWF_SOCK_TLS_DOMAIN_NAME_VERIFY  "AT+TLSC=%d,41,%d\r\n"
#define SYS_RNWF_NET_IPV4  4
#define SYS_RNWF_NET_IPV6  6
#define SYS_RNWF_ASCII_MODE  1    
#define SYS_RNWF_BINARY_MODE 2 

class NET {
  private:

  public:

/**
  * \ingroup NET
  * \brief Open New Socket.
  * 
  * This API Opens New Socket.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiOpenSocket(const char *cmd);

/**
  * \ingroup NET
  * \brief Socket Bind Local.
  * 
  * This API Sets Socket Bind Local.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiSocketBindLocal(const char *cmd);

/**
  * \ingroup NET
  * \brief Socket Bind Remote.
  * 
  * This API Sets Socket Bind Remote.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiSocketBindRemote(const char *cmd);

/**
  * \ingroup NET
  * \brief Socket TCP Write.
  * 
  * This API writes data in TCP.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiSocketWriteTcp(const char *cmd,const char *msg);

/**
  * \ingroup NET
  * \brief Socket TCP Read.
  * 
  * This API Reads data in TCP.
  *
  * \retval true - Success
  * \retval false - Failure
  */
bool RNWF_WifiSocketReadTcp(const char *cmd);
};

#endif /* RNWF_NET_H */