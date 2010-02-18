/*
 * ethadapter.cpp
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */

#include <global.h>

#include "packetstore.h"
#include "ethadapter.h"

EthAdapter::EthAdapter() : PacketStore() {
}

void EthAdapter::feedPacket(PacketInfo* pi) {
	dataReady = false;

	current = *pi;
	cout << "Eth feed packet" << endl;

	dataReady = true;
}
