#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "thread.h"
#include "reader.h"
#include "parameter.h"
#include "error.h"

int sudoku[9][9];

int validationRow[9] = {0};
int validationLine[9] = {0};
int validationBlock[9] = {0};


int main(int argc, char *argv[]) {

	if (argc != 2) {
		fprintf(stderr, "Usage: %s, <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	Parameters paramTest;
	int test = reader(argv[1], &paramTest);
	if (test == -2)
		exit(EXIT_FAILURE);
	if (test == -3) {
		fprintf(stderr, "reader(): Incorrect grid size. Must be 9*9.\n");
		exit(EXIT_FAILURE);
	}
	if (test == -4) {
		fprintf(stderr, "reader(): Unrecognized character at position [%d, %d].\n", paramTest.line, paramTest.row);
		exit(EXIT_FAILURE);
	}
	if (test == -1) {
		fprintf(stderr, "reader(): Incorrect character at position [%d, %d]. Must be between 1 and 9.\n", paramTest.line, paramTest.row);
		exit(EXIT_FAILURE);
	}
	
	int count = 0;
	pthread_t tid[9*3];
	pthread_attr_t attr;
	if (pthread_attr_init(&attr)) {
		perror("pthread_attr_init()");
		exit(EXIT_FAILURE);
	}
	Parameters *data[9*3];

	for (int i = 0; i < 9; i += 1) {
		data[count] = (Parameters*) malloc(sizeof(Parameters));
		data[count]->line = 0;
		data[count]->row = i;
		if (pthread_create(&tid[count], &attr, threadProcessRow, data[count])) {
			perror("pthread_create()");
			exit(EXIT_FAILURE);
		}
		count += 1;
	}

	for (int i = 0; i < 9; i += 1) {
		data[count] = (Parameters*) malloc(sizeof(Parameters));
		data[count]->line = i;
		data[count]->row = 0;
		if (pthread_create(&tid[count], &attr, threadProcessLine, data[count])) {
			perror("pthread_create()");
			exit(EXIT_FAILURE);
		}
		count += 1;
	}

	for (int i = 0; i < 9; i += 3) {
		for (int j = 0; j < 9; j += 3) {
			data[count] = (Parameters*) malloc(sizeof(Parameters));
			data[count]->line = i;
			data[count]->row = j;
			if (pthread_create(&tid[count], &attr, threadProcessBloc, data[count])) {
				perror("pthread_create()");
				exit(EXIT_FAILURE);
			}
			count += 1;
		}
	}

	void *errors[9*3];
	for (int i = 0; i < 9*3; i += 1) {
		if (pthread_join(tid[i], &(errors[i]))) {
			perror("pthread_join()");
			exit(EXIT_FAILURE);
		}
	}

	if (pthread_attr_destroy(&attr)) {
		perror("pthread_attr_destroy()");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 9*3; i +=1)
		free(data[i]);

	int errorCount = 0;
	for (int i = 0; i < 9; i += 1) {
		if (!validationRow[i]) {
			if (errors[i] != NULL)
				printf("Row number %d does not contain '%d'\n", ((Error**) errors)[i]->position, ((Error**) errors)[i]->number);
			errorCount += 1;
		}
		free(errors[i]);
		if (!validationLine[i]) {
			if (errors[i+9] != NULL)
				printf("Ligne number %d does not contain '%d'\n", ((Error**) errors)[i+9]->position, ((Error**) errors)[i+9]->number);
			errorCount += 1;
		}
		free(errors[i+9]);
		if (!validationBlock[i]) {
			if (errors[i+9*2] != NULL)
				printf("Block number %d does not contain '%d'\n", ((Error**) errors)[i+9*2]->position, ((Error**) errors)[i+9*2]->number);
			errorCount += 1;
		}
		free(errors[i+9*2]);
	}
	if (errorCount) {
		printf("FALSE\n");
		exit(EXIT_SUCCESS);
	}

	printf("TRUE\n");
	exit(EXIT_SUCCESS);
}
