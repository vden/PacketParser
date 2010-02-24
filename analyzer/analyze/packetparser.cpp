#include <dlfcn.h>
#include <algorithm>
#include <dirent.h>
#include <global.h>
#include <cstring>
#include "packetparser.h"

PacketInfo* test_parse_fn(char* buf, int len) {
	PacketInfo* pi = new PacketInfo;

	cout << "parse packets with test fn" << endl;
	return pi;
}

PacketParser::PacketParser(string path) {
	void* h;

#ifdef WIN32
	flist list = {0, 0, NULL};
	HANDLE h;
	WIN32_FIND_DATA info;
	int i;

	SetCurrentDirectory(path.c_str());
	// build a list of files
	h = FindFirstFile("libparse*.*", &info);
	if (h != INVALID_HANDLE_VALUE) {
		do {

#else // WIN32
	DIR *dp;
	struct dirent *dir;
	if((dp = opendir(path.c_str())) == NULL) {
		cerr << "Error opening " << path << endl;
	}

	while ((dir = readdir(dp)) != NULL) {
		if (strncmp(dir->d_name, "libparse", 8))
			continue;

		h = dlopen(dir->d_name, RTLD_LAZY);
		if (!h) {
			cerr << "LOAD ERR: " << dlerror() << endl;
		}

		parse_fn fn;
		fn = (parse_fn)dlsym(h, "parse");
		proto_fn proto;
		proto = (proto_fn)dlsym(h, "proto");
		tags_fn tags;
		tags = (tags_fn)dlsym(h, "tags");
		find_fn find;
		find = (find_fn)dlsym(h, "findProto");

#endif //WIN32

		cout << "Proto loaded: " << proto() << endl;

		vector<string> ds = tags();
		vector<string>::iterator it;

		for (it = ds.begin(); it != ds.end(); it++) {
			Tags::instance().add_tag(proto() + "." + *it);
		};

		parse_fns[proto()] = fn;
		find_fns[proto()] = find;

		handles.push_back(h);

#ifdef WIN32
	} while (FindNextFile(h, &info));
	FindClose(h);
#else
	}
	closedir(dp);
#endif
	parse_fns["test"] = test_parse_fn;
}

PacketParser::~PacketParser() {
#ifdef WIN32

#else
	for_each(handles.begin(), handles.end(), dlclose);
	//	dlclose(handle);
#endif
}

string PacketParser::findProto(char* buf, int len) {
	map<string,find_fn>::iterator it;

	for (it=find_fns.begin();it!=find_fns.end();it++) {
		if (it->second(buf,len))
			return it->first;
	}
	// return "null" meta-protocol, if no one can parse buf
	return "null";
};

bool PacketParser::parse(char* buf, int len, PacketInfo* pi, string proto) {
	if (proto == "null") return false;

	parse_fn p;
	p = parse_fns.find(proto)->second;

	if (!p) return false;

	*pi = *p(buf, len);
	return true;
}
