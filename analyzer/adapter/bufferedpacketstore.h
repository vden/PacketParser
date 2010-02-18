/*
 * bufferedpacketstore.h
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */

#ifndef BUFFEREDPACKETSTORE_H_
#define BUFFEREDPACKETSTORE_H_

class BufferedPacketStore: public PacketStore {
protected:
	unsigned int m_size;
	deque<PacketInfo*> localBuffer;

public:
	BufferedPacketStore(unsigned int);
	virtual ~BufferedPacketStore();
	virtual PacketInfo* packetByNumber(ulong) = 0;
	unsigned int size() {return m_size; };
};

#endif /* BUFFEREDPACKETSTORE_H_ */
