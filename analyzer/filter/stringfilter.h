/*
 * stringfilter.h
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */

#ifndef STRINGFILTER_H_
#define STRINGFILTER_H_


class StringFilter: public Filter {
public:
	StringFilter(string);
	bool applyFilter(PacketInfo*);
};

#endif /* STRINGFILTER_H_ */
