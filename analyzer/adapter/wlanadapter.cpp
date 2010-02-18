/*
 * wlanadapter.cpp
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */

#include <cstdlib>
#include <cstring>
#include <global.h>

#include "packetstore.h"
#include "wlanadapter.h"

WLANAdapter::WLANAdapter() : PacketStore() {
	types[0x00] = "Management";
	types[0x01] = "Authorization";
	types[0x02] = "Data";

	subtypes[0x00] = "Association request";
	subtypes[0x01] = "Association response";
	subtypes[0x02] = "Reassociation request";
	subtypes[0x03] = "Reassociation response";
	subtypes[0x04] = "Probe request";
	subtypes[0x05] = "Probe response";
	subtypes[0x08] = "Beacon";
	subtypes[0x09] = "ATIM";
	subtypes[0x0A] = "Dissociation";
	subtypes[0x0B] = "Authentication";
	subtypes[0x0C] = "Deauthentication";

	subtypes[0x1A] = "Power Save Poll";
	subtypes[0x1B] = "RTS";
	subtypes[0x1C] = "CTS";
	subtypes[0x1D] = "ACK";
	subtypes[0x1E] = "Contention-Free End";
	subtypes[0x1F] = "CF-End + CF-Ack";

	subtypes[0x20] = "Data";
	subtypes[0x21] = "Data + CF-Ack";
	subtypes[0x22] = "Data + CF-Poll";
	subtypes[0x23] = "Data + CF-Ack + CF-Poll";
	subtypes[0x24] = "Null function";
	subtypes[0x25] = "CF-Ack (no data)";
	subtypes[0x26] = "CF-Poll (no data)";
	subtypes[0x27] = "CF-Ack + CF-Poll (no data)";
}

string WLANAdapter::getType(int t) {
	return types[t];
}

string WLANAdapter::getSubType(int ft) {
	return subtypes[ft];
}

void WLANAdapter::feedPacket(PacketInfo* pi) {
	dataReady = false;

	current = *pi;
	cout << "WLAN adapter feeding packet" << endl;

	(*pi)["wlan.type_str"] = new char[24];
	strcpy((*pi)["wlan.type_str"],  getType((int)(*pi)["wlan.fc.type"]).c_str());
	(*pi)["wlan.subtype_str"] = new char[24];
	strcpy((*pi)["wlan.subtype_str"], getSubType((int)(*pi)["wlan.fulltype"]).c_str());

	cout << "Type: " <<(*pi)["wlan.type_str"] << "; subtype: " <<(*pi)["wlan.subtype_str"] << endl;
	if (pi->find("wlan.mgmt.ssid") != pi->end()) {
		cout << "SSID: " << (*pi)["wlan.mgmt.ssid"] << endl;
	}
	dataReady = true;
}

