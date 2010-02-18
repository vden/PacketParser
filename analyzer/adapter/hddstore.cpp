/*
 * hddstore.cpp
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */


#include <global.h>

#include "packetstore.h"
#include "hddstore.h"

HDDStore::HDDStore(string path, uint size) : BufferedPacketStore(size) {
	fs.open(path.c_str(), ios::binary | ios::in | ios::out | ios::trunc);

	if (!fs.is_open())
		cout << "File is not open!" << path.c_str() << endl;

	localOffset = 0;
}

HDDStore::~HDDStore() {
	cout << "HDDstore destructor" << endl;
	fs.flush();
	fs.close();
}

void HDDStore::feedPacket(PacketInfo* pi) {
	dataReady = false;

	unsigned int len = sizeof(*pi);
	char* buf = reinterpret_cast<char*>(pi);
	fs.write(buf, len);
	// serialize
	cout << "BUF: " << len << endl;

	fileMapItem i;
	i.length = len;
	if ((int)fileMap.size()==0)
		i.offset = 0;
	else
		i.offset = fileMap.back().length + fileMap.back().offset;

	fileMap.push_back(i);
	localBuffer.push_back(pi);
	if ((uint)localBuffer.size()>m_size) {
		localBuffer.pop_front();
		localOffset++;
		cout << "Offset now: " << localOffset << endl;
	}

	dataReady = true;
}

PacketInfo* HDDStore::packetByNumber(ulong n) {
	if (n >= (ulong)fileMap.size()) return 0;

	if ( (n >= localOffset) && (n < localOffset + m_size) ) {
		// packet from local buffer
		return localBuffer[n-localOffset];
	} else {
		// need to shift local buffer

		localOffset = n;
		for (uint i=0; i < m_size && i+n < (ulong)fileMap.size(); i++) {
			unsigned int len = fileMap[n+i].length;
			u_quad_t off = fileMap[n+i].offset;
			char *buf = new char[len];

			fs.seekg(off, ios::beg);
			fs.read(buf, len);

			localBuffer[i] = reinterpret_cast<PacketInfo*>(buf);
		//  deserialization
		}

		return localBuffer[0];
	}


}
