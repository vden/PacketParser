/*
 * packetstore.cpp
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */

#include <global.h>

#include "packetstore.h"

PacketStore::PacketStore() {
}

PacketStore::~PacketStore() {
}

bool PacketStore::isReady() {
	return dataReady;
}
