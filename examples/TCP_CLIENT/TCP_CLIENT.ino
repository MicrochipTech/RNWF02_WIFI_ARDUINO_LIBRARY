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

/*
 * This application has been tested with Arduino Zero and Adafruit Grand Central boards.
 * It requires both Serial and Serial1 for UART communication, and uses pin A3 for reset control.
 */ 

#include <Arduino.h>
#include "rnwf_wifi.h"
#include "rnwf_interface.h"
#include "rnwf_net.h"

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define RNWFserial Serial1
#define USBserial Serial
#define SERIAL_BAUDRATE 230400
#define RNWF_BAUDRATE 230400
#define RESET_PIN A3

#define SYS_RNWF_WIFI_STA_SSID "DEMO_AP_RNWF"
#define SYS_RNWF_WIFI_STA_PWD "password"
#define SYS_RNWF_COUNTRYCODE "GEN"

bool Err;
bool clientStatus = false;
uint16_t receivedDataSize = 0;
uint32_t socketId = 0;
String clientId = "";
String wifiIPAddress = "";
String TEST_MESSAGE = "HELLO WORLD\r\n";

typedef enum {
  /* TODO: Define states used by the application state machine. */
  RNWF_ECHO_OFF,
  RNWF_MAN_ID,
  RNWF_REV_ID,
  RNWF_WIFI_INFO,
  RNWF_WIFI_SET_DOMAIN,
  RNWF_WIFI_GET_SOFT_AP_STATE,
  RNWF_WIFI_SOFT_AP_DISABLE,
  RNWF_WIFI_GET_STA_STATE,
  RNWF_WIFI_DISCONNECT,
  RNWF_WIFI_SET_STA_SSID,
  RNWF_WIFI_SET_STA_PWD,
  RNWF_WIFI_SET_STA_SEC,
  RNWF_WIFI_CONNECT,
  RNWF_WIFI_TCP_SOCKET_OPEN,
  RNWF_WIFI_SOCK_BIND_REMOTE,
  RNWF_WIFI_TCP_SOCKET_EST,
  RNWF_SERIAL_TRANSFER,
} STATES;

typedef struct
{
  /* The application's current state */
  STATES state;

} RNWF_STATE;

RNWF_STATE rnwf_state;
WIFI wifi;
NET net;

void setup() {

  // initialize both serial ports:
  USBserial.begin(SERIAL_BAUDRATE);
  RNWFserial.begin(RNWF_BAUDRATE);
  RNWF_SerialInit(&RNWFserial);
  pinMode(RESET_PIN, OUTPUT);
  RNWF_ResetTargetHW();
  USBserial.println("RNWF Resetting...");

  // Wait for reset to complete
  while (true) {
    if (RNWFserial.available()) {
      String response = RNWFserial.readStringUntil('\n');
      response.trim();
      if (response.startsWith("+BOOT:")) {
        USBserial.println("!!! RNWF Initialized !!!");
        break;
      }
    }
  }
}

void loop() {
  RNWF_TCP_SERVER();
}

void RNWF_TCP_SERVER() {
  switch (rnwf_state.state) {
    case RNWF_ECHO_OFF:
      {
        Err = wifi.RNWF_SetEchoOff();
        if (Err) {
          Err = false;
          USBserial.println("RNWF Echo Off");
          rnwf_state.state = RNWF_MAN_ID;
        }
      }
      break;
    case RNWF_MAN_ID:
      {
        Err = wifi.RNWF_GetManId();
        if (Err) {
          Err = false;
          USBserial.println("RNWF MAN ID RECEIVED");
          rnwf_state.state = RNWF_REV_ID;
        }
      }
      break;
    case RNWF_REV_ID:
      {
        Err = wifi.RNWF_GetRevId();
        if (Err) {
          Err = false;
          USBserial.println("RNWF REV ID RECEIVED");
          rnwf_state.state = RNWF_WIFI_INFO;
        }
      }
      break;
    case RNWF_WIFI_INFO:
      {
        Err = wifi.RNWF_WifiInfo();
        if (Err) {
          Err = false;
          USBserial.println("RNWF WIFI INFO RECEIVED");
          rnwf_state.state = RNWF_WIFI_SET_DOMAIN;
        }
      }
      break;
    case RNWF_WIFI_SET_DOMAIN:
      {
        char command[30];
        sprintf(command, SYS_RNWF_WIFI_SET_REG_DOMAIN, SYS_RNWF_COUNTRYCODE);
        Err = wifi.RNWF_WifiSetRegDomain(command);
        if (Err) {
          Err = false;
          USBserial.println("RNWF WIFI DOMAIN SET");
          rnwf_state.state = RNWF_WIFI_GET_SOFT_AP_STATE;
        }
      }
      break;
    case RNWF_WIFI_GET_SOFT_AP_STATE:
      {
        Err = wifi.RNWF_WifiGetSoftApState();
        if (Err) {
          Err = false;
          String response = "";
          response = RNWFserial.readString();

          USBserial.println("`````````````````SOFT AP STATE`````````````````");
          USBserial.println(response);
          USBserial.println("```````````````````````````````````````````````");

          // Check if response starts with "+WAP:"
          if (response.startsWith("+WAP:")) {
            USBserial.println("RNWF WIFI SOFT AP STATE RECEIVED");
            int separatorIndex = response.indexOf(':');
            if (separatorIndex != -1 && separatorIndex < response.length() - 1) {
              char state = response.charAt(separatorIndex + 1);
              if (state == '0') {
                USBserial.println("Soft AP is DISABLED");
                rnwf_state.state = RNWF_WIFI_GET_STA_STATE;
              } else if (state == '1') {
                USBserial.println("Soft AP is ENABLED");
                rnwf_state.state = RNWF_WIFI_SOFT_AP_DISABLE;
              }
            }
          }
        }
      }
      break;
    case RNWF_WIFI_SOFT_AP_DISABLE:
      {
        Err = wifi.RNWF_WifiSoftApDisable();
        if (Err) {
          Err = false;
          USBserial.println("RNWF WIFI SOFT AP DISABLED");
          rnwf_state.state = RNWF_WIFI_DISCONNECT;
        }
      }
      break;
    case RNWF_WIFI_GET_STA_STATE:
      {
        Err = wifi.RNWF_WifiGetStaState();
        if (Err) {
          Err = false;
          String response = "";
          response = RNWFserial.readString();
          USBserial.println("```````````````````STA STATE```````````````````");
          USBserial.println(response);
          USBserial.println("```````````````````````````````````````````````");

          // Check if response starts with "+WSTA:"
          if (response.startsWith("+WSTA:")) {
            USBserial.println("RNWF WIFI STA STATE RECEIVED");
            int separatorIndex = response.indexOf(':');
            if (separatorIndex != -1 && separatorIndex < response.length() - 1) {
              char state = response.charAt(separatorIndex + 1);
              if (state == '0') {
                USBserial.println("STA is DISABLED");
                rnwf_state.state = RNWF_WIFI_SET_STA_SSID;
              } else if (state == '1') {
                USBserial.println("STA is ENABLED");
                rnwf_state.state = RNWF_WIFI_DISCONNECT;
              }
            }
          }
        }
      }
      break;
    case RNWF_WIFI_DISCONNECT:
      {
        Err = wifi.RNWF_WifiDisconnect();
        if (Err) {
          Err = false;
          USBserial.println("RNWF WIFI DISCONNECTED");
          rnwf_state.state = RNWF_WIFI_SET_STA_SSID;
        }
      }
      break;
    case RNWF_WIFI_SET_STA_SSID:
      {
        char command[30];
        sprintf(command, SYS_RNWF_WIFI_SET_STA_SSID, SYS_RNWF_WIFI_STA_SSID);
        Err = wifi.RNWF_WifiSetStaSsid(command);
        if (Err) {
          Err = false;
          USBserial.println("RNWF WIFI STA SSID SET");
          rnwf_state.state = RNWF_WIFI_SET_STA_PWD;
        }
      }
      break;
    case RNWF_WIFI_SET_STA_PWD:
      {
        char command[30];
        sprintf(command, SYS_RNWF_WIFI_SET_STA_PWD, SYS_RNWF_WIFI_STA_PWD);
        Err = wifi.RNWF_WifiSetStaPwd(command);
        if (Err) {
          Err = false;
          USBserial.println("RNWF WIFI STA PASSWORD SET");
          rnwf_state.state = RNWF_WIFI_SET_STA_SEC;
        }
      }
      break;
    case RNWF_WIFI_SET_STA_SEC:
      {
        char command[30];
        sprintf(command, SYS_RNWF_WIFI_SET_STA_SEC, SYS_RNWF_WIFI_SECURITY_WPA3_TRANS);
        Err = wifi.RNWF_WifiSetStaSec(command);
        if (Err) {
          Err = false;
          USBserial.println("RNWF WIFI STA SECURITY SET");
          rnwf_state.state = RNWF_WIFI_CONNECT;
        }
      }
      break;
    case RNWF_WIFI_CONNECT:
      {
        Err = wifi.RNWF_WifiConnect();
        unsigned long startTime = millis();
        String receivedString;
        if (Err) {
          Err = false;

          // Read data for 15 second
          while (millis() - startTime < 15000) {
            receivedString += UART_RNWF_Read();
          }

          USBserial.println("````````````WIFI CONNECTION RESPONSE```````````");
          USBserial.print(receivedString);
          USBserial.println("```````````````````````````````````````````````");

          if (receivedString.indexOf("+WSTALU:") == -1) {
            USBserial.println("!!! STA NOT FOUND !!!");
            rnwf_state.state = RNWF_WIFI_CONNECT;
          } else {
            USBserial.println("RNWF WIFI CONNECTED");
            rnwf_state.state = RNWF_WIFI_TCP_SOCKET_OPEN;

            int ipIndex = receivedString.indexOf("+WSTAAIP:");
            if (ipIndex != -1) {
              int start = ipIndex + 12;
              int end = receivedString.indexOf("\"", start);
              if (end != -1) {
                wifiIPAddress = receivedString.substring(start, end);
                USBserial.print("Extracted IP Address: ");
                USBserial.println(wifiIPAddress);
              }
            }
          }
        }
      }
      break;
    case RNWF_WIFI_TCP_SOCKET_OPEN:
      {
        char command[30];
        sprintf(command, SYS_RNWF_SOCK_OPEN_TCP, SYS_RNWF_NET_IPV4);
        Err = net.RNWF_WifiOpenSocket(command);
        if (Err) {
          Err = false;
          String response = RNWFserial.readString();

          USBserial.println("``````````````TCP SOCKET RESPONSE``````````````");
          USBserial.println(response);
          USBserial.println("```````````````````````````````````````````````");

          if (response.indexOf("+WSTALD:1") != -1) {
            USBserial.println("!!! WIFI DISCONNECTED - RECONNECTING !!!");
            rnwf_state.state = RNWF_WIFI_CONNECT;
          } else {
            USBserial.println("RNWF WIFI OPENED TCP SOCKET");

            int index = response.indexOf("+SOCKO:");
            if (index != -1) {
              int startIdx = index + 7;
              int endIdx = response.indexOf("\r\n", startIdx);

              if (endIdx != -1) {
                String socketStr = response.substring(startIdx, endIdx);
                socketId = socketStr.toInt();

                uint8_t socket_id[32] = { 0 };
                snprintf((char *)socket_id, sizeof(socket_id), "%lu", socketId);

                USBserial.print("Extracted Socket ID: ");
                USBserial.println((char *)socket_id);
              }
            }
            rnwf_state.state = RNWF_WIFI_SOCK_BIND_REMOTE;
          }
        }
      }
      break;

    case RNWF_WIFI_SOCK_BIND_REMOTE:
      {
        char ipAddress[16];  // Buffer to store user input IP address
        bool validIP = false;

        USBserial.println("Please enter the IP address:");

        // Wait for a valid IP address from the user
        while (!validIP) {
          while (USBserial.available() == 0)
            ;  // Wait until data is available

          String input = USBserial.readStringUntil('\n');  // Read user input
          input.trim();                                 // Remove any leading/trailing spaces

          // Validate IP address format
          int segments = 0;
          int lastPos = -1;
          validIP = true;

          for (int i = 0; i < input.length(); i++) {
            if (input[i] == '.') {
              if (i == 0 || i == input.length() - 1 || i - lastPos == 1) {
                validIP = false;  // No empty segments, leading, or trailing dots
                break;
              }
              segments++;
              lastPos = i;
            } else if (!isdigit(input[i])) {
              validIP = false;  // Only digits and dots are allowed
              break;
            }
          }

          if (segments != 3) validIP = false;  // An IP address should have exactly 3 dots

          if (validIP) {
            strncpy(ipAddress, input.c_str(), sizeof(ipAddress));
            ipAddress[sizeof(ipAddress) - 1] = '\0';  // Ensure null termination
          } else {
            USBserial.println("Invalid IP address. Please enter a valid IP address:");
          }
        }

        char command[50];
        sprintf(command, SYS_RNWF_SOCK_BIND_REMOTE, socketId, ipAddress, SYS_RNWF_NET_SOCK_PORT0);
        Err = net.RNWF_WifiSocketBindRemote(command);

        if (Err) {
          Err = false;
          USBserial.println("RNWF WIFI SOCKET BIND REMOTE SUCCESSFUL");
          USBserial.print("IP Address: ");
          USBserial.println(ipAddress);
          USBserial.print("Port: ");
          USBserial.println(SYS_RNWF_NET_SOCK_PORT0);
          USBserial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
          rnwf_state.state = RNWF_WIFI_TCP_SOCKET_EST;
        }
      }
      break;
    case RNWF_WIFI_TCP_SOCKET_EST:
      {
        String response = RNWFserial.readString();
        String serialInput = USBserial.readString();

        if (response.indexOf("+WSTALD:") != -1) {
          USBserial.println("!!!!!! WIFI DISCONNECTED - RECONNECTING !!!!!!!");
          rnwf_state.state = RNWF_WIFI_CONNECT;
        } else if (response.indexOf("+SOCKIND:") != -1) {
          int index = response.indexOf("+SOCKIND:");
          int startIdx = index + 9;
          int endIdx = response.indexOf(",", startIdx);
          if (endIdx != -1) {
            clientId = response.substring(startIdx, endIdx);
            USBserial.print("\nClient ID Joined: ");
            USBserial.println(clientId);
          }
          uint32_t client = clientId.toInt();
          char command[50];
          int dataLength = strlen(TEST_MESSAGE.c_str());
          sprintf(command, SYS_RNWF_SOCK_BINARY_WRITE_TCP, client, dataLength);
          Err = net.RNWF_WifiSocketWriteTcp(command, TEST_MESSAGE.c_str());
          if (Err) {
            Err = false;
            clientStatus = true;
          }
        } else if (response.indexOf("+SOCKCL:") != -1) {
          USBserial.print("\nClient ID Disconnected: ");
          USBserial.println(clientId);
          clientId = "";
          clientStatus = false;
          rnwf_state.state = RNWF_WIFI_TCP_SOCKET_OPEN;
        } else if (response.indexOf("+SOCKRXT:") != -1) {
          USBserial.print("\nData Response: ");
          USBserial.println(response);
          int commaIndex = response.indexOf(",");
          if (commaIndex != -1) {
            String dataSizeStr = response.substring(commaIndex + 1);
            receivedDataSize = dataSizeStr.toInt();
          }
          // USBserial.print("Received Data Size: ");
          // USBserial.println(receivedDataSize);
          char command[50];
          uint32_t client = clientId.toInt();
          sprintf(command, SYS_RNWF_SOCK_READ, client, SYS_RNWF_BINARY_MODE, receivedDataSize);
          net.RNWF_WifiSocketReadTcp(command);
          String receivedData = RNWFserial.readString();
          int hashIndex = receivedData.indexOf('#');
          int okIndex = receivedData.indexOf("OK");
          if (hashIndex != -1 && okIndex != -1 && hashIndex < okIndex) {
            String extractedData = receivedData.substring(hashIndex + 1, okIndex);
            USBserial.print("\nData Received: ");
            USBserial.println(extractedData);
          } else {
            USBserial.println("Invalid data format!");
          }
        } else if (serialInput.length() > 0 && clientStatus){

          uint32_t client = clientId.toInt();
          char command[50];
          int dataLength = strlen(serialInput.c_str());  // Get the length of the string
          sprintf(command, SYS_RNWF_SOCK_BINARY_WRITE_TCP, client, dataLength);
          USBserial.print("\n");
          Err = net.RNWF_WifiSocketWriteTcp(command, serialInput.c_str());  // Convert String to C-string
          if (Err) {
            Err = false;
            USBserial.println("Data Sent: " + serialInput);
          }
        } else {
          USBserial.print(".");
        }
      }
      break;
    case RNWF_SERIAL_TRANSFER:
      {
        serial_transfer();
      }
      break;
  }
}

void serial_transfer() {

  // read from RNWF and Print on Arduino Zero
  if (RNWFserial.available()) {
    String BU_data = RNWFserial.readString();
    USBserial.println(BU_data);
  }

  // read from Arduino Zero and Print on RNWF
  if (USBserial.available()) {
    String AR_data = USBserial.readString();
    RNWFserial.print(AR_data);
  }
}

void RNWF_ResetTargetHW(void) {
  digitalWrite(RESET_PIN, LOW);
  delay(50);
  digitalWrite(RESET_PIN, HIGH);
}