/*
 * filter.h
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */

#ifndef FILTER_H_
#define FILTER_H_

typedef enum {SIGN_EQUAL, SIGN_LESS, SIGN_MORE, SIGN_NEQUAL} signs;

class Filter {
protected:
	string filterExpr;
	vector<string> expr;
	void parse();
public:
	Filter(string);
	bool virtual applyFilter(PacketInfo*) = 0;
};

#endif /* FILTER_H_ */
