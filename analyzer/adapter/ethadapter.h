/*
 * ethadapter.h
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */

#ifndef ETHADAPTER_H_
#define ETHADAPTER_H_


class EthAdapter: public PacketStore {
public:
	EthAdapter();
	char* getSrcMac() { return current["eth.src"]; };
	char* getDstMac() { return current["eth.dst"]; };
	char* getEtherType() { return current["eth.type"]; };
	char* getData() { return current["eth.data"]; };

	void feedPacket(PacketInfo*);
};

#endif /* ETHADAPTER_H_ */
