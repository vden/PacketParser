/*
 * NumberFilter.h
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */

#ifndef NUMBERFILTER_H_
#define NUMBERFILTER_H_


class NumberFilter: public Filter {
public:
	NumberFilter(string);
	bool applyFilter(PacketInfo*);
};

#endif /* NUMBERFILTER_H_ */
