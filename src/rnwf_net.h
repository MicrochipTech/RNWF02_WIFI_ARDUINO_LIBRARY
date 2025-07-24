/*******************************************************************************
  RNWF Host Assisted System Service Implementation

  File Name:
    rnwf_net.h

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

#ifndef RNWF_NET_H
#define RNWF_NET_H

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Number of clients connecting to the server socket, with the maximum value being 5.*/
#define SYS_RNWF_NET_NO_OF_CLIENT_SOCKETS       4

/*RNWF Network Socket max callback service */
#define SYS_RNWF_NET_SOCK_SERVICE_CB_MAX    2
#define SYS_RNWF_NET_SOCK_PORT0					    80 
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

#endif /* RNWF_NET_H */