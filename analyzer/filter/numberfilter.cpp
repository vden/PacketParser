/*
 * NumberFilter.cpp
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */


#include <global.h>
#include "filter.h"
#include "numberfilter.h"


NumberFilter::NumberFilter(string expr): Filter(expr) {
}

bool NumberFilter::applyFilter(PacketInfo* pi) {
	return true;
}
