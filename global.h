#ifndef GLOBAL_H_
#define GLOBAL_H_

#pragma once

#include <iostream>
#include <set>
#include <string>
#include <map>
#include <utility> // header for pair
#include <map>
#include <fstream>
#include <deque>
#include <vector>

#ifdef WIN32
#include <windows.h>
#include <windef.h>

typedef unsigned long long u_quad_t;
typedef unsigned short ushort;
typedef unsigned int uint;

//TODO: find correct replacements in windows.h
typedef unsigned char u_int8_t;
typedef unsigned int u_int16_t;
typedef unsigned long u_int32_t;
typedef unsigned long long u_int64_t;

#else

#include <sys/types.h>

#endif

using namespace std;

typedef map<string,char*> PacketInfo;

class Tags {
 private:
    set<string> tags;
    
    Tags() {};
    ~Tags() {};
    Tags(const Tags&);
    Tags& operator=(const Tags&);


 public:
    void add_tag(string);
    bool has_tag(string);
    void show_tags();

    static Tags& instance() {
    	static Tags singleton;
    	return singleton;
    }	    
};

typedef struct {
    uint magic;      /* magic number */
    ushort version_major;  /* major version number */
    ushort version_minor;  /* minor version number */
    int  thiszone;   /* GMT to local correction */
    uint sigfigs;    /* accuracy of timestamps */
    uint snaplen;    /* max length of captured packets, in octets */
    uint network;    /* data link type */
} CAPHeader;


typedef struct {
	uint ts_sec;
	uint ts_usec;
	uint pckt_cnt;
	uint length;
} CAPBlockHeader;

typedef vector<string> _TAGS;

int readNextCAPBlockHeader(fstream&,u_quad_t);

#endif
