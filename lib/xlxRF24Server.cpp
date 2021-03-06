/**
 * xlxRF24Server.cpp - Xlight RF2.4 server library based on via RF2.4 module
 * for communication with instances of xlxRF24Client
 *
 * Created by Baoshi Sun <bs.sun@datatellit.com>
 * Copyright (C) 2015-2016 DTIT
 * Full contributor list:
 *
 * Documentation:
 * Support Forum:
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Created by Baoshi Sun <bs.sun@datatellit.com>
 *
 * Dependancy
 * 1. Particle RF24 library, refer to
 *    https://github.com/stewarthou/Particle-RF24
 *    http://tmrh20.github.io/RF24/annotated.html
 *
 * DESCRIPTION
 * 1. Mysensors protocol compatible and extended
 * 2. Address algorithm
 * 3. PipePool (0 AdminPipe, Read & Write; 1-5 pipe pool, Read)
 * 4. Session manager, optional address shifting
 * 5.
 *
 * ToDo:
 * 1. Two pipes collaboration for security: divide a message into two parts
 * 2. Apply security layer, e.g. AES, encryption, etc.
 *
**/

#include "xlxRF24Server.h"
#include "xliPinMap.h"
#include "xlSmartController.h"
#include "xlxLogger.h"

#include "MyParserSerial.h"

//------------------------------------------------------------------
// the one and only instance of RF24ServerClass
RF24ServerClass theRadio(PIN_RF24_CE, PIN_RF24_CS);
MyMessage msg;
MyParserSerial msgParser;
UC *msgData = (UC *)&(msg.msg);

RF24ServerClass::RF24ServerClass(uint8_t ce, uint8_t cs, uint8_t paLevel)
	:	MyTransportNRF24(ce, cs, paLevel)
{
	_times = 0;
	_succ = 0;
	_received = 0;
}

bool RF24ServerClass::ServerBegin()
{
  // Initialize RF module
	if( !init() ) {
    LOGC(LOGTAG_MSG, F("RF24 module is not valid!"));
		return false;
	}

  // Set role to Controller or Gateway
	SetRole_Gateway();
  return true;
}

// Make NetworkID with the right 4 bytes of device MAC address
uint64_t RF24ServerClass::GetNetworkID()
{
  uint64_t netID = 0;

  byte mac[6];
  WiFi.macAddress(mac);
  for (int i=2; i<6; i++) {
    netID += mac[i];
    netID <<= 8;
  }

  return netID;
}

bool RF24ServerClass::ChangeNodeID(const uint8_t bNodeID)
{
	if( bNodeID == getAddress() ) {
			return true;
	}

	if( bNodeID == 0 ) {
		SetRole_Gateway();
	} else {
		setAddress(bNodeID, RF24_BASE_RADIO_ID);
	}

	return true;
}

void RF24ServerClass::SetRole_Gateway()
{
	uint64_t lv_networkID = GetNetworkID();
	setAddress(GATEWAY_ADDRESS, lv_networkID);
}

bool RF24ServerClass::ProcessSend(String &strMsg, MyMessage &my_msg)
{
	bool sentOK = false;
	bool bMsgReady = false;
	int iValue;
	float fValue;
	char strBuffer[64];

	int nPos = strMsg.indexOf(':');
	uint8_t lv_nNodeID;
	uint8_t lv_nMsgID;
	if (nPos > 0) {
		// Extract NodeID & MessageID
		lv_nNodeID = (uint8_t)(strMsg.substring(0, nPos).toInt());
		lv_nMsgID = (uint8_t)(strMsg.substring(nPos + 1).toInt());
	}
	else {
		// Parse serial message
		lv_nMsgID = 0;
	}

	switch (lv_nMsgID)
	{
	case 0: // Free style
		iValue = min(strMsg.length(), 63);
		strncpy(strBuffer, strMsg.c_str(), iValue);
		strBuffer[iValue] = 0;
		// Serail format to MySensors message structure
		bMsgReady = msgParser.parse(msg, strBuffer);
		if (bMsgReady) {
			SERIAL("Now sending message...");
		}
		break;

	case 1:   // Request new node ID
		if (getAddress() == GATEWAY_ADDRESS) {
			SERIAL_LN("Controller can not request node ID\n\r");
		}
		else {
			ChangeNodeID(AUTO);
			msg.build(AUTO, BASESERVICE_ADDRESS, NODE_SENSOR_ID, C_INTERNAL, I_ID_REQUEST, false);
			msg.set("DTIT-is-great");     // Optional Key
			bMsgReady = true;
			SERIAL("Now sending request node id message...");
		}
		break;

	case 2:   // Lamp present, req ack
		msg.build(getAddress(), lv_nNodeID, NODE_SENSOR_ID, C_PRESENTATION, S_LIGHT, true);
		msg.set("Found Sunny");
		bMsgReady = true;
		SERIAL("Now sending lamp present message...");
		break;

	case 3:   // Temperature sensor present with sensor id 1, req no ack
		msg.build(getAddress(), lv_nNodeID, 1, C_PRESENTATION, S_TEMP, false);
		msg.set("");
		bMsgReady = true;
		SERIAL("Now sending DHT11 present message...");
		break;

	case 4:   // Temperature set to 23.5, req no ack
		msg.build(getAddress(), lv_nNodeID, 1, C_SET, V_TEMP, false);
		fValue = 23.5;
		msg.set(fValue, 2);
		bMsgReady = true;
		SERIAL("Now sending set temperature message...");
		break;

	case 5:   // Humidity set to 45, req no ack
		msg.build(getAddress(), lv_nNodeID, 1, C_SET, V_HUM, false);
		iValue = 45;
		msg.set(iValue);
		bMsgReady = true;
		SERIAL("Now sending set humidity message...");
		break;
	}

	if (bMsgReady) {
		SERIAL("to %d...", msg.getDestination());
		sentOK = ProcessSend();
		my_msg = msg;
		SERIAL_LN(sentOK ? "OK" : "failed");
	}

	return sentOK;
}

bool RF24ServerClass::ProcessSend(String &strMsg)
{
	MyMessage tempMsg;
	return ProcessSend(strMsg, tempMsg);
}

// ToDo: add message to queue instead of sending out directly
bool RF24ServerClass::ProcessSend(MyMessage *pMsg)
{
	if( !pMsg ) { pMsg = &msg; }

	// Determine the receiver addresse
	_times++;
	if( send(pMsg->getDestination(), *pMsg) )
	{
		_succ++;
		return true;
	}

	return false;
}

bool RF24ServerClass::ProcessReceive()
{
  bool sentOK = false;
  uint8_t to = 0;
  uint8_t pipe;
  if (!available(&to, &pipe))
    return false;

  uint8_t len = receive(msgData);
  if( len < HEADER_SIZE )
  {
    LOGW(LOGTAG_MSG, "got corrupt dynamic payload!");
    return false;
  } else if( len > MAX_MESSAGE_LENGTH )
  {
    LOGW(LOGTAG_MSG, "message length exceeded: %d", len);
    return false;
  }

  char strDisplay[SENSORDATA_JSON_SIZE];
  _received++;
  LOGD(LOGTAG_MSG, "Received from pipe %d msg-len=%d, from:%d to:%d dest:%d cmd:%d type:%d sensor:%d payl-len:%d",
        pipe, len, msg.getSender(), to, msg.getDestination(), msg.getCommand(),
        msg.getType(), msg.getSensor(), msg.getLength());
	/*
  memset(strDisplay, 0x00, sizeof(strDisplay));
  msg.getJsonString(strDisplay);
  SERIAL_LN("  JSON: %s, len: %d", strDisplay, strlen(strDisplay));
  memset(strDisplay, 0x00, sizeof(strDisplay));
  SERIAL_LN("  Serial: %s, len: %d", msg.getSerialString(strDisplay), strlen(strDisplay));
	*/

  switch( msg.getCommand() )
  {
    case C_INTERNAL:
      if( msg.getType() == I_ID_REQUEST && msg.getSender() == AUTO ) {
        // On ID Request message
        /// Get new ID
        UC newID = GetNextAvailableNodeId();
        UC replyTo = msg.getSender();
        /// Send response message
        msg.build(getAddress(), replyTo, newID, C_INTERNAL, I_ID_RESPONSE, false);
        msg.set(getMyNetworkID());
        SERIAL("Now sending NodeId response message to %d with new NodeID:%d, NetworkID:%s...", replyTo, newID, PrintUint64(strDisplay, getMyNetworkID()));
        _times++;
        sentOK = send(replyTo, msg, pipe);
        if( sentOK ) {
          _succ++;
          SERIAL_LN("OK");
        } else {
          SERIAL_LN("failed");
        }
      } else if( msg.getType() == I_ID_RESPONSE ) {
        if( msg.getSensor() > MAX_DEVICE_PER_CONTROLLER ) {
            SERIAL_LN("Node Table is full!");
        } else {
					uint64_t lv_networkID = msg.getUInt64();
          uint8_t lv_nodeID = msg.getSensor();
          SERIAL_LN("Get NodeId: %d, networkId: %s", lv_nodeID, PrintUint64(strDisplay, lv_networkID));
          setAddress(lv_nodeID, lv_networkID);
        }
      }
      break;

    default:
      break;
  }

  return true;
}

// Just for testing now
uint8_t RF24ServerClass::GetNextAvailableNodeId()
{
  // ToDo: maintain a ID table (e.g. DevStatusRow_t[n])
  static uint8_t nextID = 0;
  if( ++nextID > MAX_DEVICE_PER_CONTROLLER ) {
    nextID = 1;
  }

  return nextID;
}
