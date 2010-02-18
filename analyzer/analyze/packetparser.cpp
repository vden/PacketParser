#include <dlfcn.h>
#include <algorithm>

#include <global.h>
#include "packetparser.h"

PacketInfo* test_parse_fn(char* buf, int len) {
	PacketInfo* pi = new PacketInfo;

	cout << "parse packets with test fn" << endl;
	return pi;
}

PacketParser::PacketParser(string path) {

	string libs[] = {"./libparse_eth_proto.so", "./libparse_wlan_proto.so"};
	int libs_len = 2;

	void* h;
	for (int i=0; i<libs_len; i++) {
		h = dlopen(libs[i].c_str(), RTLD_LAZY);
		if (!h) {
			cout << "LOAD ERR: " << dlerror() << endl;
		}

		parse_fn fn;
		fn = (parse_fn)dlsym(h, "parse");
		proto_fn proto;
		proto = (proto_fn)dlsym(h, "proto");
		tags_fn tags;
		tags = (tags_fn)dlsym(h, "tags");

		cout << "Proto loaded: " << proto() << endl;

		vector<string> ds = tags();
		vector<string>::iterator it;

		for (it = ds.begin(); it != ds.end(); it++) {
			Tags::instance().add_tag(proto() + "." + *it);
		};

		parse_fns[proto()] = fn;
		handles.push_back(h);
	}
	parse_fns["test"] = test_parse_fn;
}

PacketParser::~PacketParser() {
	for_each(handles.begin(), handles.end(), dlclose);
//	dlclose(handle);
}

bool PacketParser::parse(char* buf, int len, PacketInfo* pi, string proto) {
	parse_fn p;
	p = parse_fns.find(proto)->second;

	if (!p) return false;

	*pi = *p(buf, len);
	return true;
}
