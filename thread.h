#ifndef __THREAD_H__
#define __THREAD_H__

extern int sudoku[9][9];

void* threadProcessRow(void*);
void* threadProcessLine(void*);
void* threadProcessBloc(void*);

#endif
