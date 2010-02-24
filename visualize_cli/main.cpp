/*
 * main.cpp
 *
 *  Created on: 12.02.2010
 *      Author: vden
 */

#include <iostream>
#include <global.h>
#include <analyzer/analyze/analyzer.h>
#include <analyzer/filter/numberfilter.h>
#include <analyzer/filter/stringfilter.h>
#include <analyzer/adapter/wlanadapter.h>
#include <analyzer/adapter/hddstore.h>

using namespace std;

int main(int argc, char **argv)
{
	Analyzer* a = new Analyzer();
	StringFilter *f;
	PacketStore* ps;

	f = new StringFilter("wlan.mgmt.ssid = 123321");
	a->addFilter(dynamic_cast<Filter*>(f));

	f = new StringFilter("wlan.mode = b");
//	a->addFilter(dynamic_cast<Filter*>(f));

	ps = new WLANAdapter();
	a->addStore(dynamic_cast<PacketStore*>(ps));

	a->addSyncStore(dynamic_cast<PacketStore*>(ps));

	PacketInfo pi;
	pi["test1"] = "8123";
	pi["test2"] = "5";

	HDDStore *hs = new HDDStore("/tmp/hstore", 1);
	a->addStore(dynamic_cast<PacketStore*>(hs));

	char* buf = 0;
	fstream fs(argc>1?argv[1]:"test.cap");
	int len = 0;
	u_quad_t offset = sizeof(CAPHeader);

	while ( (len = readNextCAPBlockHeader(fs, offset)) > 0) {
		cout << "------- frame --------------" << endl;

		buf = new char[len];
		fs.read(buf, len);

		cout << "READ FROM cap: " << len << endl;

		offset += len + sizeof(CAPBlockHeader);
		a->nextPacket(buf, len);

		cout << "----------------------------" << endl;
		delete buf;
	}

	PacketInfo *pi2;
	pi2 = hs->packetByNumber(0);
	//cout << "Test HDDStore " << (*pi2)["test1"] << endl;

	delete hs;
	delete a;
}
