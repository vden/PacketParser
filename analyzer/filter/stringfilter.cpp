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

	vector<string>::iterator it;
	for (it=expr.begin();it!=expr.end();it++) {
		cout << "L: " << *it << "; " << endl;
	}

	vector<string> localExpr(expr);
	int i;
	for (it=localExpr.begin(), i=0; it!=localExpr.end();it++, i++) {
		if ( pi->find(*it) != pi->end())
			*it = (*pi)[*it];
	/*	else
		{
			if ( (expr[i] != "=") && (expr[i] != "<>") ) {
				if ((i < (int) expr.size() - 1)) {
					cout << i << " " << expr[i] << endl;
					if ((expr[i + 1] != "=") && (expr[i + 1] != "<>")) {
						// next lexem is not sign
						*it = "";
					}
				} else
					// last lexem
					*it = "";
			}
		}*/
	}


	for (it=localExpr.begin();it!=localExpr.end();it++) {
		cout << "L2: " << *it << "; " << endl;

	}


	return true;
}
