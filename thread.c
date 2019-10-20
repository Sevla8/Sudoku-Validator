#include "thread.h"

void *threadProcessRow(void *arg) {
	int row = ((Parameters*) arg)->row;
	int result[9] = {0};
	for (int i = 0; i < 9; i += 1)
		result[sudoku[i][row]-1] += 1;
	for (int i = 0; i < 9; i += 1) {
		if (result[i] == 0) {
			Error *error = (Error*) malloc(sizeof(Error));
			error->position = row;
			error->number = i+1;
			pthread_exit(error);
		}
	}
	validationRow[row] = 1;
	pthread_exit(NULL);
}

void *threadProcessLine(void *arg) {
	int line = ((Parameters*) arg)->line;
	int result[9] = {0};
	for (int i = 0; i < 9; i += 1)
		result[sudoku[line][i]-1] += 1;
	for (int i = 0; i < 9; i += 1) {
		if (result[i] == 0) {
			Error *error = (Error*) malloc(sizeof(Error));
			error->position = line;
			error->number = i+1;
			pthread_exit(error);
		}
	}
	validationLine[line] = 1;
	pthread_exit(NULL);
}

void *threadProcessBloc(void *arg) {
	Parameters *data = (Parameters*) arg;
	int result[9] = {0};
	for (int i = 0; i < 9/3; i += 1) {
		for (int j = 0; j < 9/3; j += 1) {
			result[sudoku[data->line+i][data->row+j]-1] += 1;
		}
	}
	for (int i = 0; i < 9; i += 1) {
		if (result[i] == 0) {
			Error *error = (Error*) malloc(sizeof(Error));
			error->position = data->line + data->row/3;
			error->number = i+1;
			pthread_exit(error);
		}
	}
	validationBlock[data->line + data->row/3] = 1;
	pthread_exit(NULL);
}
