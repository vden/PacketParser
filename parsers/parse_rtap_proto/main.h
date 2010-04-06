/*
 * main.h
 *
 *  Created on: 16.02.2010
 *      Author: vden
 */

#ifndef MAIN_H_
#define MAIN_H_


struct ieee80211_radiotap_header {
        u_int8_t        it_version;     /* set to 0 */
        u_int8_t        it_pad;
        u_int16_t       it_len;         /* entire length */
        u_int32_t       it_present;     /* fields present */
} __attribute__((__packed__));

extern "C" {
	string proto();
	_TAGS tags();
	bool findProto(char*, int);
	PacketInfo* parse(char*, int);
}

#endif /* MAIN_H_ */
