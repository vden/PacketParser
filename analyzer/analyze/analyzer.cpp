
#include "analyzer.h"

Analyzer::Analyzer(string path) {
	parser = new PacketParser(path);
}

bool Analyzer::nextPacket(char *buf, int len) {
	PacketInfo *p = new PacketInfo;
	string proto = parser->findProto(buf, len);
	string errorField = proto + ".error";

	cout << "Analyzer:: nextPacker" << endl;

	if (!parser->parse(buf, len, p, proto)) {
		(*p)[errorField] = "Protocol not found";
		cout << errorField << " " << (*p)[errorField] << endl;
	}

	vector<Filter*>::iterator it;
	for (it=captureFilters.begin(); it!=captureFilters.end(); ++it) {
		if (! (*it)->applyFilter(p)) return false;
	}

	vector<PacketStore*>::iterator it_store;
	// iterate by stores

	StoreThread* s;
	for (it_store=store.begin(); it_store!=store.end(); ++it_store) {
		s = new StoreThread(*it_store, p);
		s->Execute();
	}

	int sem = (int)syncStore.size();
	for (it_store=syncStore.begin(); it_store!=syncStore.end(); ++it_store) {
		//cout << "semaphore before: " << sem << endl;
		if ((*it_store)->isReady()) sem--;

		//cout << "semaphore after: " << sem << endl;
		if (sem == 0) break;
		if (it_store==syncStore.end()) it_store=syncStore.begin();
	}

	return true;
}
