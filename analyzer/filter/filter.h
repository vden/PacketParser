/*
 * filter.h
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */

#ifndef FILTER_H_
#define FILTER_H_

class Filter {
protected:
	string filterExpr;
public:
	Filter(string);
	bool virtual applyFilter(PacketInfo*) = 0;
};

#endif /* FILTER_H_ */
