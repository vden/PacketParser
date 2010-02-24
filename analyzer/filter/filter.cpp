/*
 * filter.cpp
 *
 *  Created on: 13.02.2010
 *      Author: vden
 */



#include <global.h>
#include "filter.h"

Filter::Filter(string expr) {
	filterExpr = expr;
	parse();
}

void Filter::parse() {
	string lexem = "";
	//bool isLexem = true;

	string::iterator it;
	for (it=filterExpr.begin(); it!=filterExpr.end(); it++) {
		switch (*it) {
		case '=':
			expr.push_back(lexem);
			expr.push_back("=");
			lexem = "";
			break;
		case '<':
			expr.push_back(lexem);
			expr.push_back("<>");
			lexem = "";
			it++;
			break;
		case '\n':
		case ' ':
			continue;
		default:
			lexem.push_back(*it);
		}
	}
	expr.push_back(lexem);
}
