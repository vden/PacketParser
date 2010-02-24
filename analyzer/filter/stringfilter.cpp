/*
 * stringfilter.cpp
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */
#include <global.h>
#include <cstring>
#include "filter.h"

#include "stringfilter.h"

StringFilter::StringFilter(string expr): Filter(expr) {
}

bool StringFilter::applyFilter(PacketInfo* pi) {
	cout << "i'm string filter " << filterExpr << endl;

	vector<string>::iterator it3;
	for (it3=expr.begin();it3!=expr.end();it3++) {
		cout << "L: " << *it3 << "; ";
	}

	vector<string> localExpr(expr);
	vector<string>::iterator it;
	for (it=localExpr.begin();it!=localExpr.end();it++) {
		if ( pi->find(*it) != pi->end())
			*it = (*pi)[*it];
		else
			*it = "";
	}


	vector<string>::iterator it1;
	for (it1=localExpr.begin();it1!=localExpr.end();it1++) {
		cout << "L2: " << *it1 << "; ";
	}


	return true;
}
