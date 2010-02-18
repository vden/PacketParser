#include "../global.h"

void Tags::add_tag(string tag) {
	tags.insert(tag);
}

bool Tags::has_tag(string tag) {
	return true;
}

void Tags::show_tags() {
	set<string>::iterator it;
	for (it=tags.begin();it!=tags.end();++it) {
		cout << *it << endl;
	}
}

int readNextCAPBlockHeader(fstream& fs, u_quad_t offset) {
	//TODO: disable test condition
	if (offset > 400) return -1;

	fs.seekg(offset, ios::beg);
	CAPBlockHeader *cap = new CAPBlockHeader;

	fs.read((char*)cap, sizeof(CAPBlockHeader));
	fs.seekg(offset+sizeof(CAPBlockHeader), ios::beg);

	uint len = cap->length;

	delete cap;
	return len;
}
