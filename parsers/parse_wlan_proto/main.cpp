/*
 * main.cpp
 *
 *  Created on: 16.02.2010
 *      Author: vden
 */

#include "../src/global.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

#include "main.h"

const string PROTO = "wlan";

// return tag value
#define TAG(x) (*pi)[PROTO + "." + x]

// insert tag
#define INSERT_TAG(x,size,offset) (*pi)[PROTO+"."+x] = new char[size]; \
	memcpy(&(*pi)[PROTO + "." + x], buf+offset, size);

// insert tag representing string (adds '\0' at the end)
#define INSERT_STRING_TAG(x,size,offset) TAG(x) = new char[size+1]; \
	memcpy(TAG(x), buf+offset, size); \
	TAG(x)[(int)size] = '\0';

// insert tag sized less than byte with mask and shift
#define INSERT_TAGm(x,size,mask,shift,offset) (*pi)[PROTO+"."+x] = new char[size]; \
		memcpy(&(*pi)[PROTO + "." + x], buf+offset, size); \
		(*pi)[PROTO + "." + x] = (char*)((int)(*pi)[PROTO + "." + x] & mask); \
(*pi)[PROTO + "." + x] = (char*)((int)(*pi)[PROTO + "." + x] >> shift);

string proto() {
	return PROTO;
};

uint loadNextTag(PacketInfo* pi, char* buf, uint offset) {
	unsigned char id = buf[offset];
	char len = buf[offset+1];

	offset += 2;
	switch (id) {
	case 0:
		INSERT_STRING_TAG("mgmt.ssid", len, offset); break;
	case 1:
		INSERT_TAG("mgmt.rates", len, offset); break;
	case 2:
		INSERT_TAG("mgmt.fh_param", len, offset); break;
	case 3:
		INSERT_TAG("mgmt.ds_param", len, offset); break;
	case 4:
		INSERT_TAG("mgmt.cf_param", len, offset); break;
	case 5:
		INSERT_TAG("mgmt.tim", len, offset); break;
	case 6:
		INSERT_TAG("mgmt.ibss", len, offset); break;
	case 7: // 802.11d
		INSERT_STRING_TAG("mgmt.country", len, offset); break;
	case 8: // 802.11d
		INSERT_TAG("mgmt.hopping_params", len, offset); break;
	case 9: // 802.11d
		INSERT_TAG("mgmt.hopping_table", len, offset); break;
	case 10: // 802.11d
		INSERT_TAG("mgmt.request", len, offset); break;
	case 16:
		INSERT_STRING_TAG("mgmt.challenge", len, offset); break;
	case 42: // 802.11g
		INSERT_TAG("mgmt.erp", len, offset); break;
	case 50: // 802.11g
		INSERT_TAG("mgmt.rates_extended", len, offset); break;
	default:
		// numbered vendor-specific tag
		char t[32]; sprintf(t, "%d", id);
		string s = "mgmt.tag_" + string(t);
		INSERT_TAG(s, len, offset);
		break;
	}

	return offset+len;
}

PacketInfo* parse(char* buf, int len) {
	cout << "parselib " << PROTO << endl;

	PacketInfo *pi = new PacketInfo;

	// frame control sequence; 0-1 bytes
	INSERT_TAG("flags", 1, 1);
	INSERT_TAGm("fc.protoversion", 1, 0x03, 0, 0);
	INSERT_TAGm("fc.type", 1, 0x0C, 2, 0);
	INSERT_TAGm("fc.subtype", 1, 0xF0, 4, 0);
	INSERT_TAGm("fc.ds", 1, 0x03, 0, 1);
	INSERT_TAGm("fc.frag", 1, 0x04, 2, 1);
	INSERT_TAGm("fc.retry", 1, 0x08, 3, 1);
	INSERT_TAGm("fc.pwrmgmt", 1, 0x10, 4, 1);
	INSERT_TAGm("fc.moredata", 1, 0x20, 5, 1);
	INSERT_TAGm("fc.protected", 1, 0x40, 6, 1);
	INSERT_TAGm("fc.order", 1, 0x80, 7, 1);

	// type + subtype in 1 byte
	int ft = (int)TAG("fc.type") << 4 | (int)TAG("fc.subtype");
	INSERT_TAGm("fulltype", 1, 0xFC, 0, 0);
	TAG("fulltype") = (char*)ft;

	// duration; 2-3 bytes
	INSERT_TAG("duration", 2, 2);

	// ************ CONTROL FRAMES *******************
	switch ((uint)TAG("fulltype")) {
	case 0x1B:
		// RTS Control frame
		INSERT_TAG("receiver", 6, 4);
		INSERT_TAG("transmitter", 6, 10); break;
	case 0x1C:
		// CTS Control frame
	case 0x1D:
		// Ack Control frame
		INSERT_TAG("receiver", 6, 4); break;
	case 0x1A:
		// PS-Poll Control frame
		INSERT_TAG("aid", 2, 2);
		INSERT_TAG("bssid", 6, 4);
		INSERT_TAG("transmitter", 6, 10); break;
	case 0x1E:
		// CF-End Control frame
	case 0x1F:
		// CF-End + CF-Ack Control frame
		INSERT_TAG("receiver", 6, 4);
		INSERT_TAG("bssid", 6, 10); break;
	};

	if ((uint)TAG("fc.type") == 0x01) {
		// Control frames have not data payload
		INSERT_TAG("data", 1, 0);
		TAG("data") = 0;
	}

	// ********* DATA FRAMES *******************
	if ((uint)TAG("fc.type") == 0x02) {
		// Data frame, is independent of subtype

		switch ((uint)TAG("fc.ds")) {
		case 0x00:
			INSERT_TAG("dest", 6, 4);
			INSERT_TAG("src", 6, 10);
			INSERT_TAG("bssid", 6, 16); break;
		case 0x01:
			INSERT_TAG("bssid", 6, 4);
			INSERT_TAG("src", 6, 10);
			INSERT_TAG("dest", 6, 16); break;
		case 0x02:
			INSERT_TAG("dest", 6, 4);
			INSERT_TAG("bssid", 6, 10);
			INSERT_TAG("src", 6, 16); break;
		case 0x03:
			INSERT_TAG("receiver", 6, 4);
			INSERT_TAG("transmitter", 6, 10);
			INSERT_TAG("dest", 6, 16);
			INSERT_TAG("src", 6, 24); break;
		}

		// Data payload
		INSERT_TAG("data", len - 30, 30);
		INSERT_TAGm("seq", 2, 0xFFF0, 4, 22);
		INSERT_TAGm("frag", 2, 0x000F, 0, 22);
	}

	// ******** MANAGEMENT FRAMES *********************************
	if ((uint)TAG("fc.type") == 0x00) {
		// Management frame, is partially independent of subtype

		INSERT_TAG("data", len - 24, 24);
		int off;
		switch ((uint)TAG("fc.subtype")) {
		case 0x08:
			// Beacon frame
		case 0x05:
			// Probe response frame

			// Fixed width fields
			INSERT_TAG("mgmt.timestamp", 8, 24);
			INSERT_TAG("mgmt.beacon_interval", 2, 32);
			INSERT_TAG("mgmt.capability", 2, 34);

			// tagged fields
			off = 36;
			while ( off < len )
				off = loadNextTag(pi, buf, off);
			break;

		case 0x0A:
			// Dissasociation frame
			INSERT_TAG("mgmt.reason", 2, 24);
			break;

		case 0x00:
			// Association req frame
			INSERT_TAG("mgmt.capability", 2, 24);
			INSERT_TAG("mgmt.listen_interval", 2, 26);

			off = 28;
			while ( off < len )
				off = loadNextTag(pi, buf, off);
			break;

		case 0x01:
			// Association resp frame
		case 0x03:
			// Reassociation resp frame
			INSERT_TAG("mgmt.capability", 2, 24);
			INSERT_TAG("mgmt.status", 2, 26);
			INSERT_TAG("mgmt.aid", 2, 28);

			off = 30;
			while ( off < len )
				off = loadNextTag(pi, buf, off);
			break;

		case 0x02:
			// Reassociation req frame
			INSERT_TAG("mgmt.capability", 2, 24);
			INSERT_TAG("mgmt.listen_interval", 2, 26);
			INSERT_TAG("mgmt.current_ap", 6, 28);

			off = 34;
			while ( off < len )
				off = loadNextTag(pi, buf, off);
			break;

		case 0x04:
			// Probe request frame
			off = 24;
			while ( off < len )
				off = loadNextTag(pi, buf, off);
			break;

		case 0x0B:
			// authentication frame
			INSERT_TAG("mgmt.auth.algorithm", 2, 24);
			INSERT_TAG("mgmt.auth.seq", 2, 26);

			off = 28;
			while ( off < len )
				off = loadNextTag(pi, buf, off);
			break;

		case 0x0C:
			// deauthentication frame
			INSERT_TAG("mgmt.reason", 2, 24);
		}
	}

	return pi;
};

_TAGS tags() {
	string ds[] = { "type", "data" };
	_TAGS s ( ds, ds + 2 );

	return s;
};
