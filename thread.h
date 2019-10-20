#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include "parameter.h"
#include "error.h"

extern int sudoku[9][9];
extern int validationRow[9];
extern int validationLine[9];
extern int validationBlock[9];

void* threadProcessRow(void*);
void* threadProcessLine(void*);
void* threadProcessBloc(void*);

#endif
