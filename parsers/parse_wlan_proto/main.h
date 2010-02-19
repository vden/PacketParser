/*
 * main.h
 *
 *  Created on: 16.02.2010
 *      Author: vden
 */

#ifndef MAIN_H_
#define MAIN_H_

extern "C" {
	string proto();
	_TAGS tags();
	bool findProto(char*, int);
	PacketInfo* parse(char*, int);
}

#endif /* MAIN_H_ */
