#ifndef ANALYZER_H_
#define ANALYZER_H_

#include <global.h>
#include "../filter/filter.h"
#include "../adapter/packetstore.h"
#include "../threads.h"

#include "packetparser.h"

class Analyzer {
private:
	PacketParser *parser;
	vector<Filter*> captureFilters;
	vector<PacketStore*> store;
	vector<PacketStore*> syncStore;

	const string findProto(char*,int);

public:
	Analyzer(string path="./");

	bool nextPacket(char*,int);
	int addFilter(Filter* f) { captureFilters.push_back(f); return (int)captureFilters.size(); };
	void removeFilter(int n) { captureFilters.erase(captureFilters.begin() + n - 1); };

	int addStore(PacketStore* s) { store.push_back(s); return (int)store.size(); };
	void removeStore(int n) { store.erase(store.begin() + n - 1); };

	int addSyncStore(PacketStore* s) { syncStore.push_back(s); return (int)syncStore.size(); };
	void removeSyncStore(int n) { syncStore.erase(syncStore.begin() + n - 1); };
};

class StoreThread : public Thread {
public:
	StoreThread(PacketStore* s, PacketInfo* pi): Thread() { pstore = s; pinfo = pi; };
	virtual void Execute() {
		if (!pstore) return;

		pstore->feedPacket(pinfo);
		cout << "Start store thread" << endl;
	}
private:
	PacketStore* pstore;
	PacketInfo* pinfo;
};

#endif
