/*
 * bufferedpacketstore.cpp
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */


#include <global.h>

#include "packetstore.h"
#include "bufferedpacketstore.h"

BufferedPacketStore::BufferedPacketStore(unsigned int size) : PacketStore() {
	//localBuffer = new PacketInfo[size];
	this->m_size = size;
}

BufferedPacketStore::~BufferedPacketStore() {
	localBuffer.clear();
}
