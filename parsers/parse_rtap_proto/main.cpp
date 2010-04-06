/*
 * main.cpp
 *
 *  Created on: 16.02.2010
 *      Author: vden
 */

#include <global.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

#include "main.h"

const string PROTO = "radiotap";

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

bool checkPresence(short i, PacketInfo* pi) {
	u_int64_t f = (u_int64_t)TAG("presence");
	f >>= i;
	return (f & 0x1);
}

PacketInfo* parse(char* buf, int len) {
	cout << "parselib " << PROTO << endl;

	PacketInfo *pi = new PacketInfo;

	//ieee80211_radiotap_header *rtap_header = new ieee80211_radiotap_header;
	INSERT_TAG("version", 1, 0);
	INSERT_TAG("hdr_len", 2, 2);
	INSERT_TAG("presence", 4, 4);

	uint data_offset = (uint)TAG("hdr_len");
	if (checkPresence(0, pi)) {
		// TSFT
		INSERT_TAG("tsft", 8, data_offset);
		data_offset += 8;
	}
	if (checkPresence(1, pi)) {
		// flags
		INSERT_TAG("flags", 1, data_offset);
		data_offset++;
	}
	if (checkPresence(2, pi)) {
		// Rate
		INSERT_TAG("rate", 1, data_offset);
		data_offset++;
	}
	if (checkPresence(3, pi)) {
		// Channel
		INSERT_TAG("channel.freq", 2, data_offset);
		INSERT_TAG("channel.flags", 2, data_offset+2);
		data_offset+=4;
	}
	if (checkPresence(4, pi)) {
		// FHSS
		INSERT_TAG("fhss.set", 1, data_offset);
		INSERT_TAG("fhss.pattern", 1, data_offset+1);
		data_offset+=2;
	}
	if (checkPresence(5, pi)) {
		// Antenna signal
		INSERT_TAG("ant_signal", 1, data_offset);
		data_offset++;
	}
	if (checkPresence(6, pi)) {
		// Antenna noise
		INSERT_TAG("ant_noise", 1, data_offset);
		data_offset++;
	}
	if (checkPresence(7, pi)) {
		// Lock quality
		INSERT_TAG("lock_quality", 2, data_offset);
		data_offset += 2;
	}
	if (checkPresence(8, pi)) {
		// TX attenuation
		INSERT_TAG("tx_att", 2, data_offset);
		data_offset+=2;
	}
	if (checkPresence(9, pi)) {
		// db TX attenuation
		INSERT_TAG("tx_att_db", 2, data_offset);
		data_offset+=2;
	}
	if (checkPresence(10, pi)) {
		// dBm TX power
		INSERT_TAG("tx_power", 1, data_offset);
		data_offset++;
	}
	if (checkPresence(11, pi)) {
		// Antenna
		INSERT_TAG("antenna", 1, data_offset);
		data_offset++;
	}
	if (checkPresence(12, pi)) {
		// dB antenna signal
		INSERT_TAG("ant_signal_db", 1, data_offset);
		data_offset++;
	}
	if (checkPresence(13, pi)) {
		// dB antenna noise
		INSERT_TAG("ant_noise_db", 1, data_offset);
		data_offset++;
	}
	if (checkPresence(14, pi)) {
		// RX flags
		INSERT_TAG("rx_flags", 2, data_offset);
		data_offset +=2;
	}

	INSERT_TAG("data", len - data_offset, data_offset);
	return pi;
};

_TAGS tags() {
	string ds[] = { "type", "data" };
	_TAGS s ( ds, ds + 2 );

	return s;
};

bool findProto(char* buf, int len) {
	return true;
}
