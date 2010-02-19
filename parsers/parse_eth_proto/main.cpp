/*
 * main.cpp
 *
 *  Created on: 16.02.2010
 *      Author: vden
 */

#include <global.h>

#include <cstring>
#include "main.h"

const string PROTO = "eth";

#define INSERT_TAG(x,size,offset) (*pi)[PROTO+"."+x] = new char[size]; \
	memcpy(&(*pi)[PROTO + "." + x], buf+offset, size);

string proto() {
	return PROTO;
};

PacketInfo* parse(char* buf, int len) {
	cout << "parselib eth" << endl;

	PacketInfo* pi = new PacketInfo;
	INSERT_TAG("data", len-18, 14);
	INSERT_TAG("dest", 6, 0);
	INSERT_TAG("src", 6, 6);
	INSERT_TAG("type", 2, 12);

	return pi;
};

_TAGS tags() {
	string ds[] = {"data", "dest", "src", "type"};
	_TAGS s( ds, ds + 4);

	return s;
};


bool findProto(char* buf, int len) {
	return false;
}
