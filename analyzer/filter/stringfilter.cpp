/*
 * stringfilter.cpp
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */
#include <global.h>
#include "filter.h"

#include "stringfilter.h"

StringFilter::StringFilter(string expr): Filter(expr) {
}

bool StringFilter::applyFilter(PacketInfo* pi) {
	cout << "i'm string filter " << filterExpr << endl;
	return true;
}
