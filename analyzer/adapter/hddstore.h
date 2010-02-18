/*
 * hddstore.h
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */

#ifndef HDDSTORE_H_
#define HDDSTORE_H_

#include "bufferedpacketstore.h"

typedef struct {
	u_quad_t offset;
	unsigned int length;
} fileMapItem;

class HDDStore: public BufferedPacketStore {
private:
	std::fstream fs;
	// vector of packet lengths
	std::vector<fileMapItem> fileMap;
	// offset for local buffer
	u_quad_t localOffset;
public:
	HDDStore(string path, uint size);
	~HDDStore();
	void feedPacket(PacketInfo*);
	PacketInfo* packetByNumber(ulong);
};

#endif /* HDDSTORE_H_ */
