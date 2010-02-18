/*
 * wlanadapter.h
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */

#ifndef WLANADAPTER_H_
#define WLANADAPTER_H_


class WLANAdapter: public PacketStore {
private:
	string getType(int);
	string getSubType(int);
	map<int, string> subtypes;
	map<int, string> types;

public:
	WLANAdapter();

	char* getSrcMac() { return current["wlan.src"]; };
	char* getDstMac() { return current["wlan.dst"]; };
	char* getApMac() { return current["wlan.ap"]; };
	char* getPacketType() { return current["wlan.type"]; };
	char* getSSID() { return current["wlan.ssid"]; };

	void feedPacket(PacketInfo*);

};

#endif /* WLANADAPTER_H_ */
