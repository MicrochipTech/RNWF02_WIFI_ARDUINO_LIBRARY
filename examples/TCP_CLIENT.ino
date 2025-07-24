#include <Arduino.h>
#include "rnwf.h"
#include "rnwf_interface.h"
#include "rnwf_net.h"

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define SERIAL_BAUDRATE 230400
#define RNWF_BAUDRATE 230400
#define RESET_PIN A3

#define SYS_RNWF_WIFI_STA_SSID "MCHPWSGTest" //"DEMO_AP_RNWF"
#define SYS_RNWF_WIFI_STA_PWD "12345678"  //"password"
#define SYS_RNWF_COUNTRYCODE "GEN"

bool Err;
bool clientStatus = false;
uint16_t receivedDataSize = 0;
uint32_t socketId = 0;
String clientId = "";
String wifiIPAddress = "";
String TEST_MESSAGE = "GET / HTTP/1.1\r\nHost: 192.168.0.12\r\nConnection: open\r\n\r\n"; //"HELLO WORLD\r\n";

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

void setup() {

  // initialize both serial ports:
  Serial.begin(SERIAL_BAUDRATE);
  RNWFserial.begin(RNWF_BAUDRATE);
  pinMode(RESET_PIN, OUTPUT);
  RNWF_ResetTargetHW();
  Serial.println("RNWF Resetting...");

  // Wait for reset to complete
  while (true) {
    if (RNWFserial.available()) {
      String response = RNWFserial.readStringUntil('\n');
      response.trim();
      if (response.startsWith("+BOOT:")) {
        Serial.println("!!! RNWF Initialized !!!");
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
        Err = RNWF_SetEchoOff();
        if (Err) {
          Err = false;
          Serial.println("RNWF Echo Off");
          rnwf_state.state = RNWF_MAN_ID;
        }
      }
      break;
    case RNWF_MAN_ID:
      {
        Err = RNWF_GetManId();
        if (Err) {
          Err = false;
          Serial.println("RNWF MAN ID RECEIVED");
          rnwf_state.state = RNWF_REV_ID;
        }
      }
      break;
    case RNWF_REV_ID:
      {
        Err = RNWF_GetRevId();
        if (Err) {
          Err = false;
          Serial.println("RNWF REV ID RECEIVED");
          rnwf_state.state = RNWF_WIFI_INFO;
        }
      }
      break;
    case RNWF_WIFI_INFO:
      {
        Err = RNWF_WifiInfo();
        if (Err) {
          Err = false;
          Serial.println("RNWF WIFI INFO RECEIVED");
          rnwf_state.state = RNWF_WIFI_SET_DOMAIN;
        }
      }
      break;
    case RNWF_WIFI_SET_DOMAIN:
      {
        char command[30];
        sprintf(command, SYS_RNWF_WIFI_SET_REG_DOMAIN, SYS_RNWF_COUNTRYCODE);
        Err = RNWF_WifiSetRegDomain(command);
        if (Err) {
          Err = false;
          Serial.println("RNWF WIFI DOMAIN SET");
          rnwf_state.state = RNWF_WIFI_GET_SOFT_AP_STATE;
        }
      }
      break;
    case RNWF_WIFI_GET_SOFT_AP_STATE:
      {
        Err = RNWF_WifiGetSoftApState();
        if (Err) {
          Err = false;
          String response = "";
          response = RNWFserial.readString();

          Serial.println("`````````````````SOFT AP STATE`````````````````");
          Serial.println(response);
          Serial.println("```````````````````````````````````````````````");

          // Check if response starts with "+WAP:"
          if (response.startsWith("+WAP:")) {
            Serial.println("RNWF WIFI SOFT AP STATE RECEIVED");
            int separatorIndex = response.indexOf(':');
            if (separatorIndex != -1 && separatorIndex < response.length() - 1) {
              char state = response.charAt(separatorIndex + 1);
              if (state == '0') {
                Serial.println("Soft AP is DISABLED");
                rnwf_state.state = RNWF_WIFI_GET_STA_STATE;
              } else if (state == '1') {
                Serial.println("Soft AP is ENABLED");
                rnwf_state.state = RNWF_WIFI_SOFT_AP_DISABLE;
              }
            }
          }
        }
      }
      break;
    case RNWF_WIFI_SOFT_AP_DISABLE:
      {
        Err = RNWF_WifiSoftApDisable();
        if (Err) {
          Err = false;
          Serial.println("RNWF WIFI SOFT AP DISABLED");
          rnwf_state.state = RNWF_WIFI_DISCONNECT;
        }
      }
      break;
    case RNWF_WIFI_GET_STA_STATE:
      {
        Err = RNWF_WifiGetStaState();
        if (Err) {
          Err = false;
          String response = "";
          response = RNWFserial.readString();
          Serial.println("```````````````````STA STATE```````````````````");
          Serial.println(response);
          Serial.println("```````````````````````````````````````````````");

          // Check if response starts with "+WSTA:"
          if (response.startsWith("+WSTA:")) {
            Serial.println("RNWF WIFI STA STATE RECEIVED");
            int separatorIndex = response.indexOf(':');
            if (separatorIndex != -1 && separatorIndex < response.length() - 1) {
              char state = response.charAt(separatorIndex + 1);
              if (state == '0') {
                Serial.println("STA is DISABLED");
                rnwf_state.state = RNWF_WIFI_SET_STA_SSID;
              } else if (state == '1') {
                Serial.println("STA is ENABLED");
                rnwf_state.state = RNWF_WIFI_DISCONNECT;
              }
            }
          }
        }
      }
      break;
    case RNWF_WIFI_DISCONNECT:
      {
        Err = RNWF_WifiDisconnect();
        if (Err) {
          Err = false;
          Serial.println("RNWF WIFI DISCONNECTED");
          rnwf_state.state = RNWF_WIFI_SET_STA_SSID;
        }
      }
      break;
    case RNWF_WIFI_SET_STA_SSID:
      {
        char command[30];
        sprintf(command, SYS_RNWF_WIFI_SET_STA_SSID, SYS_RNWF_WIFI_STA_SSID);
        Err = RNWF_WifiSetStaSsid(command);
        if (Err) {
          Err = false;
          Serial.println("RNWF WIFI STA SSID SET");
          rnwf_state.state = RNWF_WIFI_SET_STA_PWD;
        }
      }
      break;
    case RNWF_WIFI_SET_STA_PWD:
      {
        char command[30];
        sprintf(command, SYS_RNWF_WIFI_SET_STA_PWD, SYS_RNWF_WIFI_STA_PWD);
        Err = RNWF_WifiSetStaPwd(command);
        if (Err) {
          Err = false;
          Serial.println("RNWF WIFI STA PASSWORD SET");
          rnwf_state.state = RNWF_WIFI_SET_STA_SEC;
        }
      }
      break;
    case RNWF_WIFI_SET_STA_SEC:
      {
        char command[30];
        sprintf(command, SYS_RNWF_WIFI_SET_STA_SEC, SYS_RNWF_WIFI_SECURITY_WPA3_TRANS);
        Err = RNWF_WifiSetStaSec(command);
        if (Err) {
          Err = false;
          Serial.println("RNWF WIFI STA SECURITY SET");
          rnwf_state.state = RNWF_WIFI_CONNECT;
        }
      }
      break;
    case RNWF_WIFI_CONNECT:
      {
        Err = RNWF_WifiConnect();
        unsigned long startTime = millis();
        String receivedString;
        if (Err) {
          Err = false;

          // Read data for 15 second
          while (millis() - startTime < 15000) {
            receivedString += UART_RNWF_Read();
          }

          Serial.println("````````````WIFI CONNECTION RESPONSE```````````");
          Serial.print(receivedString);
          Serial.println("```````````````````````````````````````````````");

          if (receivedString.indexOf("+WSTALU:") == -1) {
            Serial.println("!!! STA NOT FOUND !!!");
            rnwf_state.state = RNWF_WIFI_CONNECT;
          } else {
            Serial.println("RNWF WIFI CONNECTED");
            rnwf_state.state = RNWF_WIFI_TCP_SOCKET_OPEN;

            int ipIndex = receivedString.indexOf("+WSTAAIP:");
            if (ipIndex != -1) {
              int start = ipIndex + 12;
              int end = receivedString.indexOf("\"", start);
              if (end != -1) {
                wifiIPAddress = receivedString.substring(start, end);
                Serial.print("Extracted IP Address: ");
                Serial.println(wifiIPAddress);
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
        Err = RNWF_WifiOpenSocket(command);
        if (Err) {
          Err = false;
          String response = RNWFserial.readString();

          Serial.println("``````````````TCP SOCKET RESPONSE``````````````");
          Serial.println(response);
          Serial.println("```````````````````````````````````````````````");

          if (response.indexOf("+WSTALD:1") != -1) {
            Serial.println("!!! WIFI DISCONNECTED - RECONNECTING !!!");
            rnwf_state.state = RNWF_WIFI_CONNECT;
          } else {
            Serial.println("RNWF WIFI OPENED TCP SOCKET");

            int index = response.indexOf("+SOCKO:");
            if (index != -1) {
              int startIdx = index + 7;
              int endIdx = response.indexOf("\r\n", startIdx);

              if (endIdx != -1) {
                String socketStr = response.substring(startIdx, endIdx);
                socketId = socketStr.toInt();

                uint8_t socket_id[32] = { 0 };
                snprintf((char *)socket_id, sizeof(socket_id), "%lu", socketId);

                Serial.print("Extracted Socket ID: ");
                Serial.println((char *)socket_id);
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

        Serial.println("Please enter the IP address:");

        // Wait for a valid IP address from the user
        while (!validIP) {
          while (Serial.available() == 0)
            ;  // Wait until data is available

          String input = Serial.readStringUntil('\n');  // Read user input
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
            Serial.println("Invalid IP address. Please enter a valid IP address:");
          }
        }

        char command[50];
        sprintf(command, SYS_RNWF_SOCK_BIND_REMOTE, socketId, ipAddress, SYS_RNWF_NET_SOCK_PORT0);
        Err = RNWF_WifiSocketBindRemote(command);

        if (Err) {
          Err = false;
          Serial.println("RNWF WIFI SOCKET BIND REMOTE SUCCESSFUL");
          // Serial.println("! CONNECT TCP CLIENT TO BELOW ADDRESS & PORT !");
          Serial.print("IP Address: ");
          Serial.println(ipAddress);
          Serial.print("Port: ");
          Serial.println(SYS_RNWF_NET_SOCK_PORT0);
          Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
          rnwf_state.state = RNWF_WIFI_TCP_SOCKET_EST;
        }
      }
      break;

    // case RNWF_WIFI_SOCK_BIND_LOCAL:
    //   {
    //     char command[50];
    //     sprintf(command, SYS_RNWF_SOCK_BIND_LOCAL, socketId, SYS_RNWF_NET_SOCK_PORT0, SYS_RNWF_NET_NO_OF_CLIENT_SOCKETS);
    //     Err = RNWF_WifiSocketBindLocal(command);
    //     if (Err) {
    //       Err = false;
    //       Serial.println("RNWF WIFI SOCKET BIND LOCAL SUCCESSFUL");

    //       Serial.println("! CONNECT TCP CLIENT TO BELOW ADDERESS & PORT !");
    //       Serial.print("IP Address: ");
    //       Serial.println(wifiIPAddress);
    //       Serial.print("Port: ");
    //       Serial.println(SYS_RNWF_NET_SOCK_PORT0);
    //       Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    //       rnwf_state.state = RNWF_WIFI_TCP_SOCKET_EST;
    //     }
    //   }
    //   break;
    case RNWF_WIFI_TCP_SOCKET_EST:
      {
        String response = RNWFserial.readString();
        String serialInput = Serial.readString();
        Serial.println("---------------------------------------------------");
        Serial.println(response);
        Serial.println("---------------------------------------------------");

        if (response.indexOf("+WSTALD:") != -1) {
          Serial.println("!!!!!! WIFI DISCONNECTED - RECONNECTING !!!!!!!");
          rnwf_state.state = RNWF_WIFI_CONNECT;
        } else if (response.indexOf("+SOCKIND:") != -1) {
          int index = response.indexOf("+SOCKIND:");
          int startIdx = index + 9;
          int endIdx = response.indexOf(",", startIdx);
          if (endIdx != -1) {
            clientId = response.substring(startIdx, endIdx);
            Serial.print("\nClient ID Joined: ");
            Serial.println(clientId);
          }
          uint32_t client = clientId.toInt();
          char command[50];
          int dataLength = strlen(TEST_MESSAGE.c_str());
          sprintf(command, SYS_RNWF_SOCK_BINARY_WRITE_TCP, client, dataLength);
          Err = RNWF_WifiSocketWriteTcp(command, TEST_MESSAGE.c_str());
          if (Err) {
            Err = false;
            clientStatus = true;
          }
        }else if (response.indexOf("+SOCKRXT:") != -1) {
          Serial.print("\nData Response: ");
          Serial.println(response);
          int commaIndex = response.indexOf(",");
          if (commaIndex != -1) {
            String dataSizeStr = response.substring(commaIndex + 1);
            receivedDataSize = dataSizeStr.toInt();

          Serial.print("Received Data Size: ");
          Serial.println(receivedDataSize);

          }
          else if (response.indexOf("+SOCKCL:") != -1) {
          Serial.print("\nClient ID Disconnected: ");
          Serial.println(clientId);
          clientId = "";
          clientStatus = false;
          rnwf_state.state = RNWF_WIFI_TCP_SOCKET_OPEN;
        }
          char command[50];
          uint32_t client = clientId.toInt();
          sprintf(command, SYS_RNWF_SOCK_READ, client, SYS_RNWF_BINARY_MODE, receivedDataSize);
          RNWF_WifiSocketReadTcp(command);
          String receivedData = RNWFserial.readString();
          int hashIndex = receivedData.indexOf('#');
          int okIndex = receivedData.indexOf("OK");
          if (hashIndex != -1 && okIndex != -1 && hashIndex < okIndex) {
            String extractedData = receivedData.substring(hashIndex + 1, okIndex);
            Serial.print("\nData Received: ");
            Serial.println(extractedData);
          } else {
            Serial.println("Invalid data format!");
          }
        } else if (!serialInput.isEmpty() && clientStatus) {

          uint32_t client = clientId.toInt();
          char command[50];
          int dataLength = strlen(serialInput.c_str());  // Get the length of the string
          sprintf(command, SYS_RNWF_SOCK_BINARY_WRITE_TCP, client, dataLength);
          Serial.print("\n");
          Err = RNWF_WifiSocketWriteTcp(command, serialInput.c_str());  // Convert String to C-string
          if (Err) {
            Err = false;
            Serial.println("Data Sent: " + serialInput);
          }
        } else {
          Serial.print(".");
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
    Serial.println(BU_data);
  }

  // read from Arduino Zero and Print on RNWF
  if (Serial.available()) {
    String AR_data = Serial.readString();
    RNWFserial.print(AR_data);
  }
}

void RNWF_ResetTargetHW(void) {
  digitalWrite(RESET_PIN, LOW);
  delay(50);
  digitalWrite(RESET_PIN, HIGH);
}
