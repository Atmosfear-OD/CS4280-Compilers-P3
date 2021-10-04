/*
 * staticSem.h by Pascal Odijk 4/18/2021
 * P3 CS4280 Professor Hauschild
 *
 * Descr...
 */

#ifndef STATICSEM_H
#define STATICSEM_H

#include "token.h"
#include "node.h"

void push(token stackTk);
void pop(int startScope);
void initStack(void);

int find(string str);
int varExists(string str);

void semanticCheck(node *node, int count);

#endif
