#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int sudoku[9][9];
/*
{{6, 2, 4, 5, 3, 9, 1, 8, 7}, 
{5, 1, 9, 7, 2, 8, 6, 3, 4},
{8, 3, 7, 6, 1, 4, 2, 9, 5},
{1, 4, 3, 8, 6, 5, 7, 2, 9},
{9, 5, 8, 2, 4, 7, 3, 6, 1},
{7, 6, 2, 3, 9, 1, 4, 5, 8},
{3, 7, 1, 9, 5, 6, 8, 4, 2},
{4, 9, 6, 1, 8, 2, 5, 7, 3},
{2, 8, 5, 4, 7, 3, 9, 1, 6}};
*/

typedef struct {
	int line;
	int row;
} Parameters;

int validationRow[9] = {0};	// Tableau indiquant si les colonnes sont valides.
int validationLine[9] = {0};	// Tableau indiquant si les lignes sont valides.
int validationBlock[9] = {0};	// Tableau indiquant si les sous-grilles sont valides.

void *threadProcessRow(void *arg) {
	int row = ((Parameters*) arg)->row;
	int result[9] = {0};	// Tableau dont chaque cellule indique le nombre d'apparition de son indice dans la colonne de sudoku.
	for (int i = 0; i < 9; i += 1)
		result[sudoku[i][row]-1] += 1;
	for (int i = 0; i < 9; i += 1) {
		if (result[i] != 1)
			pthread_exit(EXIT_SUCCESS);
	}
	validationRow[row] = 1;	// S'il y a exactement une fois les chiffres de 1 à 9 dans la colonne, on met à jour le tableau validant la colonne.
	pthread_exit(EXIT_SUCCESS);
}

void *threadProcessLine(void *arg) {
	int line = ((Parameters*) arg)->line;
	int result[9] = {0};	// Tableau dont chaque cellule indique le nombre d'apparition de son indice dans la ligne de sudoku.
	for (int i = 0; i < 9; i += 1)
		result[sudoku[line][i]-1] += 1;
	for (int i = 0; i < 9; i += 1) {
		if (result[i] != 1)
			pthread_exit(EXIT_SUCCESS);
	}
	validationLine[line] = 1;	// S'il y a exactement une fois les chiffres de 1 à 9 dans la ligne, on met à jour le tableau validant la ligne.
	pthread_exit(EXIT_SUCCESS);
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
			pthread_exit(EXIT_SUCCESS);
	}
	validationBlock[data->line + data->row/3] = 1;	// S'il y a exactement une fois les chiffres de 1 à 9 dans la sous-grille, on met à jour le tableau validant la sous-grille.
	pthread_exit(EXIT_SUCCESS);
}

int reader() {
	for (int i = 0; i < 9; i += 1) {
		for (int j = 0; j < 9; j += 1) {
			char c1, c2;
			scanf("%c%c", &c1, &c2);
			int number = c1 - '0';
			if (number < 0 || number > 9)
				return -1;
			sudoku[i][j] = number;
		}
	}
	return 0;
}

int main(int argc, char const *argv[]) {

	if (reader() == -1) {
		printf("reader(): Wrong format\n");
		exit(EXIT_FAILURE);
	}

	int count = 0;	// Compteur des threads.
	pthread_t tid[9*3];	// Tableau contenant les identifiants des différents threads.
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	Parameters *data[9*3];	// Tableau contenant les différents paramètres que l'on va passer en paramètres des threads.

	for (int i = 0; i < 9; i += 1) {
		data[count] = (Parameters*) malloc(sizeof(Parameters));
		data[count]->line = 0;
		data[count]->row = i;
		pthread_create(&tid[count], &attr, threadProcessRow, data[count]);
		count += 1;
	}

	for (int i = 0; i < 9; i += 1) {
		data[count] = (Parameters*) malloc(sizeof(Parameters));
		data[count]->line = i;
		data[count]->row = 0;
		pthread_create(&tid[count], &attr, threadProcessLine, data[count]);
		count += 1;
	}

	for (int i = 0; i < 9; i += 3) {	// On renseigne une sous-grille par la ligne et la colonne de sa première case.
		for (int j = 0; j < 9; j += 3) {
			data[count] = (Parameters*) malloc(sizeof(Parameters));
			data[count]->line = i;
			data[count]->row = j;
			pthread_create(&tid[count], &attr, threadProcessBloc, data[count]);
			count += 1;
		}
	}

	for (int i = 0; i < 9*3; i += 1) {
		pthread_join(tid[i], NULL);	// On attend que touts les threads se terminent.
		free(data[i]);	// Libération de la mémoire dynamique.
	}

	for (int i = 0; i < 9; i += 1) {
		if (!validationRow[i] || !validationLine[i] || !validationBlock[i]) {	// Si une ligne ou une colonne ou une sous-grille n'est pas valide.
			printf("FALSE\n");
			exit(EXIT_SUCCESS);
		}
	}

	printf("TRUE\n");
	exit(EXIT_SUCCESS);
}
