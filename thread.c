#include "thread.h"

void *threadProcessRow(void *arg) {
	int row = ((Parameters*) arg)->row;
	int result[9] = {0};	// Tableau dont chaque cellule indique le nombre d'apparition de son indice dans la colonne de sudoku.
	for (int i = 0; i < 9; i += 1)
		result[sudoku[i][row]-1] += 1;
	for (int i = 0; i < 9; i += 1) {
		if (result[i] != 1)
			pthread_exit(NULL);
	}
	validationRow[row] = 1;	// S'il y a exactement une fois les chiffres de 1 à 9 dans la colonne, on met à jour le tableau validant la colonne.
	pthread_exit(NULL);
}

void *threadProcessLine(void *arg) {
	int line = ((Parameters*) arg)->line;
	int result[9] = {0};	// Tableau dont chaque cellule indique le nombre d'apparition de son indice dans la ligne de sudoku.
	for (int i = 0; i < 9; i += 1)
		result[sudoku[line][i]-1] += 1;
	for (int i = 0; i < 9; i += 1) {
		if (result[i] != 1)
			pthread_exit(NULL);
	}
	validationLine[line] = 1;	// S'il y a exactement une fois les chiffres de 1 à 9 dans la ligne, on met à jour le tableau validant la ligne.
	pthread_exit(NULL);
}

void *threadProcessBloc(void *arg) {
	Parameters *data = (Parameters*) arg;
	int result[9] = {0};	// tableau dont chaque cellule indique le nombre d'apparition de son indice dans la sous-grille de sudoku.
	for (int i = 0; i < 9/3; i += 1) {
		for (int j = 0; j < 9/3; j += 1) {
			result[sudoku[data->line+i][data->row+j]-1] += 1;
		}
	}
	for (int i = 0; i < 9; i += 1) {
		if (result[i] != 1)
			pthread_exit(NULL);
	}
	validationBlock[data->line + data->row/3] = 1;	// S'il y a exactement une fois les chiffres de 1 à 9 dans la sous-grille, on met à jour le tableau validant la sous-grille.
	pthread_exit(NULL);
}
