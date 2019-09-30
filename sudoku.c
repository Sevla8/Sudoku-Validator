#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int sudoku[9][9] = {{6, 2, 4, 5, 3, 9, 1, 8, 7}, 
					{5, 1, 9, 7, 2, 8, 6, 3, 4},
					{8, 3, 7, 6, 1, 4, 2, 9, 5},
					{1, 4, 3, 8, 6, 5, 7, 2, 9},
					{9, 5, 8, 2, 4, 7, 3, 6, 1},
					{7, 6, 2, 3, 9, 1, 4, 5, 8},
					{3, 7, 1, 9, 5, 6, 8, 4, 2},
					{4, 9, 6, 1, 8, 2, 5, 7, 3},
					{2, 8, 5, 4, 7, 3, 9, 1, 6}};

typedef struct {
	int line;
	int row;
} Parameters;

int validationRow[9] = {0};
int validationLine[9] = {0};
int validationBlock[9] = {0};

void *threadProcessRow(void *arg) {
	int row = ((Parameters*) arg)->row;
	int result[9] = {0};
	for (int i = 0; i < 9; i += 1) {
		printf("%d\t", sudoku[i][row]);
		result[sudoku[i][row]-1] += 1;
	}
	printf("\n");

	for (int i = 0; i < 9; i += 1)
		printf("%d\t", result[i]);
	printf("\n");
	for (int i = 0; i < 9; i += 1) {
		if (result[i] != 1)
			pthread_exit(EXIT_SUCCESS);
	}
	validationRow[row] = 1;
	pthread_exit(EXIT_SUCCESS);
}

void *threadProcessLine(void *arg) {
	int line = ((Parameters*) arg)->line;
	int result[9] = {0};
	for (int i = 0; i < 9; i += 1)
		result[sudoku[line][i]-1] += 1;
	for (int i = 0; i < 9; i += 1) {
		if (result[i] != 1)
			pthread_exit(EXIT_SUCCESS);
	}
	validationLine[line] = 1;
	pthread_exit(EXIT_SUCCESS);
}

void *threadProcessBloc(void *arg) {
	Parameters *data = (Parameters*) arg;
	int result[9] = {0};
	for (int i = 0; i < 3; i += 1) {
		for (int j = 0; j < 3; j += 1) {
			result[sudoku[data->line+i][data->row+j]-1] += 1;
		}
	}
	for (int i = 0; i < 9; i += 1) {
		if (result[i] != 1)
			pthread_exit(EXIT_SUCCESS);
	}
	validationBlock[data->line + data->row/3] = 1;
	pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[]) {

	int count = 0;
	pthread_t tid[27];
	pthread_attr_t attr[27];
	for (int i = 0; i < 27; i += 1)
		pthread_attr_init(&attr[i]);

	Parameters *data = (Parameters*) malloc(sizeof(Parameters));

	for (int i = 0; i < 9; i += 1) {
		data->line = i;
		data->row = 0;
		pthread_create(&tid[count], &attr[count], threadProcessLine, data);
		count += 1;
	}

	for (int i = 0; i < 9; i += 1) {
		data->line = 0;
		data->row = i;
		pthread_create(&tid[count], &attr[count], threadProcessRow, data);
		count += 1;
	}

	for (int i = 0; i < 9; i += 3) {
		for (int j = 0; j < 9; j += 3) {
			data->line = i;
			data->row = j;
			pthread_create(&tid[count], &attr[count], threadProcessBloc, data);
			count += 1;
		}
	}

	for (int i = 0; i < 27; i += 1)
		pthread_join(tid[i], NULL);

	free(data);

	for (int i = 0; i < 9; i += 1) {
		if (!validationRow[i]) {
			printf("FALSE %d\n", i);
			exit(EXIT_SUCCESS);
		}
	}
	
	printf("TRUE\n");
	exit(EXIT_SUCCESS);
}
