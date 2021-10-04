/*
 * staticSem.cpp by Pascal odijk 4/18/2021
 * P3 CS4280 Professor Hauschild
 *
 * Des...
 */

#include <stdio.h>
#include <stdlib.h>
#include "staticSem.h"

// Stack to hold tokens
token tkStack[100];

// Number of variables
static int numVars = 0;
// Start point of scope
static int startScope = 0;

// Function to initialize all stack locations with an empty string
void initStack(void) {
	for(int i = 0 ; i < 101 ; i++) { tkStack[i].tkName = ""; }

	return;
}

// Function to push tokens onto stack
void push(token stackTk) {
	// Catch error if stack is full or if variable has already been declared in same scope
	if(numVars == 100) {
		cout << "ERROR: Stack is full\n";
		exit(1);
	}
	
	for(int i = startScope ; i < numVars ; i++) {
		if(tkStack[i].tkName == stackTk.tkName) {
			cout << "ERROR: " << stackTk.tkName << " has already been declared in this scope at line "<< tkStack[i].lineNum  << "\n";
			exit(1);
		}
	}

	// If not error, push token onto stack and increment variable count
	tkStack[numVars] = stackTk;
	tkStack[numVars].scope = startScope;
	numVars++;
	//cout << "DEBUG: " << stackTk.tkName << "\n";
        //cout << "Scope: " << startScope << "\n";

}

// Function to pop tokens off stack
void pop(int startScope) {
	for(int i = 0 ; i < numVars ; i++) {
		//cout << "tkStack[" << i << "].tkName and scope = " << tkStack[i].tkName << " " << tkStack[i].scope << " compared to startScope of " << startScope << "\n";
		if(tkStack[i].scope == startScope) {
			numVars--;
			tkStack[i].tkName = "";
		}
	}
}

// Function to search if variable already exists in current local scope -- if so, return position
int find(string str) {
	for(int i = 0 ; i < 100 ; i++) {
		if(tkStack[i].tkName == str && tkStack[i].scope == startScope) { return (numVars - 1 - i); } 
	}

	return -1;
}

// Function to search if variable already exists in current or parent's scope -- if so, return position 
int varExists(string str) {
	for(int i = 0 ; i < numVars ; i++) {
		//cout << "DEBUG: " << tkStack[i].tkName << " compared to " << str << "\n";
		//cout << "Scope: " << startScope << "\n";
		if(tkStack[i].tkName == str && tkStack[i].scope <= startScope) { 
			return (numVars - 1 - i); 
		}
	}

	return -1;
}

void semanticCheck(node *node, int count) {
	// Check if null
	if(node == NULL) { return; }
	
	// Process <program>
	if(node->name == "<program>") {
		int vars = 0;
		
		// Process children of <program>	
		if(node->child1 != NULL) { semanticCheck(node->child1, vars); }
		if(node->child2 != NULL) { semanticCheck(node->child2, vars); }
		if(node->child3 != NULL) { semanticCheck(node->child3, vars); }
                if(node->child4 != NULL) { semanticCheck(node->child4, vars); }

	} 
	// Process <vars>
	else if(node->name == "<vars>") {
		int exists;
		//startScope = numVars;
		//cout << "DEBUG: startScope <vars> : " << startScope << "\n";
		exists = find(node->nToken2.tkName);
		
		if(exists == -1 || exists > count) {
			push(node->nToken2);
			count++;
		} else if(exists < count) {
			cout << "ERROR: " << node->nToken2.tkName << " has already been declared in this scope on line " << node->nToken2.lineNum << "\n";
                        exit(1);
		}

		if(node->child1 != NULL) { semanticCheck(node->child1, count); }
		if(node->child2 != NULL) { semanticCheck(node->child2, count); }
                if(node->child3 != NULL) { semanticCheck(node->child3, count); }
		if(node->child4 != NULL) { semanticCheck(node->child4, count); }

	}
	// Process <block>
	else if(node->name == "<block>") {
		int vars = 0;
		startScope++;
		//cout << "DEBUG: startScope <block> before pop(): " << startScope << "\n";

		if(node->child1 != NULL) { semanticCheck(node->child1, vars); }
                if(node->child2 != NULL) { semanticCheck(node->child2, vars); }
		
		pop(startScope);
		startScope--;	
		//cout << "DEBUG: startScope <block> after pop(): " << startScope << "\n";
	}
	// Process <expr>
	else if(node->name == "<expr>") {
		if(node->nToken.tkIdentifier == minusTk) {
			if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                        if(node->child2 != NULL) { semanticCheck(node->child2, count); }

		} else {
			if(node->child1 != NULL) { semanticCheck(node->child1, count); }
		} 
	}
	// Process <N>
	else if(node->name == "<N>") {
		if(node->nToken.tkIdentifier == slashTk) {
			if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                        if(node->child2 != NULL) { semanticCheck(node->child2, count); }
		} else if(node->nToken.tkIdentifier == asteriskTk) {
			if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                        if(node->child2 != NULL) { semanticCheck(node->child2, count); }
		} else {
			if(node->child1 != NULL) { semanticCheck(node->child1, count); }
		}
	}
	// Process <A>
	else if(node->name == "<A>") {
		if(node->nToken.tkIdentifier == plusTk) {
			if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                        if(node->child2 != NULL) { semanticCheck(node->child2, count); }
		} else {
			if(node->child1 != NULL) { semanticCheck(node->child1, count); }
		}
	}
	// Process <M>
	else if(node->name == "<M>") {
		if(node->nToken.tkIdentifier == asteriskTk) {
			if(node->child1 != NULL) { semanticCheck(node->child1, count); }
		} else {
			if(node->child1 != NULL) { semanticCheck(node->child1, count); }
		}
	}
	// Process <R>
	else if(node->name == "<R>") {
		if(node->nToken.tkIdentifier == open_parTk) {
			if(node->child1 != NULL) { semanticCheck(node->child1, count); }
		} else if(node->nToken.tkIdentifier == idTk) {
			int exists = varExists(node->nToken.tkName);
			//cout << "In <R>: " << node->nToken.tkName << "\n";
			if(exists == -1) {
				cout << "ERROR: " << node->nToken.tkName << " at line " << node->nToken.lineNum <<" was not declared in this scope\n";
				exit(1); 
			}
		} else {
			if(node->child1 != NULL) { semanticCheck(node->child1, count); }
		}
	}
	// Process <stats>
	else if(node->name == "<stats>") {
		if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                if(node->child2 != NULL) { semanticCheck(node->child2, count); }
	}
	// Process <mStat>
	else if(node->name == "<mStat>") {
		if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                if(node->child2 != NULL) { semanticCheck(node->child2, count); }
	}
	// Process <stat>
	else if(node->name == "<stat>") {
		if(node->child1->name == "<in>") {
			if(node->child1 != NULL) { semanticCheck(node->child1, count); }
		} else if(node->child1->name == "<out>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                } else if(node->child1->name == "<block>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                } else if(node->child1->name == "<if>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                } else if(node->child1->name == "<loop>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                } else if(node->child1->name == "<assign>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                } else if(node->child1->name == "<goto>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                } else if(node->child1->name == "<label>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                }
	}
	// Process <in>
	else if(node->name == "<in>") {
		if(node->nToken.tkIdentifier == getterTk) {
			int exists = varExists(node->nToken2.tkName);
			if(exists == -1) {
				cout << "ERROR: " << node->nToken2.tkName << " at line " << node->nToken2.lineNum <<" was not declared in this scope\n";
                                exit(1);
			}
		}
	}
	// Process <out>
	else if(node->name == "<out>") {
		if(node->nToken.tkIdentifier == outterTk) {
			 if(node->child1 != NULL) { semanticCheck(node->child1, count); }
		}
	}
	// Process <if>
	else if(node->name == "<if>") {
		if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                if(node->child2 != NULL) { semanticCheck(node->child2, count); }
		if(node->child3 != NULL) { semanticCheck(node->child3, count); }
                if(node->child4 != NULL) { semanticCheck(node->child4, count); }
	}
	// Process <loop>
	else if(node->name == "<loop>") {
		if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                if(node->child2 != NULL) { semanticCheck(node->child2, count); }
                if(node->child3 != NULL) { semanticCheck(node->child3, count); }
                if(node->child4 != NULL) { semanticCheck(node->child4, count); }
	}
	// Process <assign>
	else if(node->name == "<assign>") {
		int exists = varExists(node->nToken2.tkName);
		if(exists == -1) {
                	cout << "ERROR: " << node->nToken2.tkName << " at line " << node->nToken2.lineNum << " was not declared in this scope\n";
                        exit(1);
                }
		
		if(node->child1 != NULL) { semanticCheck(node->child1, count); }
	}
	// Process <label>
	else if(node->name == "<label>") {
		int exists = find(node->nToken2.tkName);

                if(exists == -1 || exists > count) {
                        push(node->nToken2);
                        count++;
                } else if(exists < count) {
                        cout << "ERROR: " << node->nToken2.tkName << " has already been declared in this scope on line " << node->nToken2.lineNum << "\n";
                        exit(1);
                }

                if(node->child1 != NULL) { semanticCheck(node->child1, count); }
	}
	// Process <goto>
	else if(node->name == "<goto>") {
		int exists = varExists(node->nToken2.tkName);
                if(exists == -1) {
			cout << "ERROR: " << node->nToken2.tkName << " at line " << node->nToken2.lineNum << " was not declared in this scope\n";
                        exit(1);
                 }

                if(node->child1 != NULL) { semanticCheck(node->child1, count); }
	}
	else {
		if(node->child1 != NULL) { semanticCheck(node->child1, count); }
                if(node->child2 != NULL) { semanticCheck(node->child2, count); }
                if(node->child3 != NULL) { semanticCheck(node->child3, count); }
                if(node->child4 != NULL) { semanticCheck(node->child4, count); }
	}
}
