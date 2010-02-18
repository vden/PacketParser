/*
 * packetstore.h
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */

#ifndef PACKETSTORE_H_
#define PACKETSTORE_H_

class PacketStore {
protected:
	bool dataReady;
	PacketInfo current;

public:
	PacketStore();
	virtual ~PacketStore();
	bool isReady();
	virtual void feedPacket(PacketInfo*) = 0;
};

#endif /* PACKETSTORE_H_ */
