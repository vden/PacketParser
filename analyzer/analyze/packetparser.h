#ifndef PACKETPARSER_H_
#define PACKETPARSER_H_


typedef PacketInfo*(*parse_fn)(char*,int);
typedef string(*proto_fn)(void);
typedef _TAGS(*tags_fn)(void);
typedef bool(*find_fn)(char*,int);

class PacketParser {
private:
    map<string,parse_fn> parse_fns;
    map<string,find_fn> find_fns;
    vector<void*> handles;

public:
    PacketParser(string);
    ~PacketParser();
    
    string findProto(char*, int);
    bool parse(char*, int, PacketInfo*, string);
};

#endif
