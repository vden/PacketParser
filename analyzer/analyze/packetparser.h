#ifndef PACKETPARSER_H_
#define PACKETPARSER_H_


typedef PacketInfo*(*parse_fn)(char*,int);
typedef string(*proto_fn)(void);
typedef _TAGS(*tags_fn)(void);

class PacketParser {
private:
    map<string,parse_fn> parse_fns;
    vector<void*> handles;

public:
    PacketParser(string);
    ~PacketParser();
    
    bool parse(char*, int, PacketInfo*, string);
};

#endif
