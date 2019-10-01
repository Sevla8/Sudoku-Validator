#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "thread.h"
#include "reader.h"
#include "parameter.h"

int sudoku[9][9];

int validationRow[9] = {0};	// Tableau indiquant si les colonnes sont valides.
int validationLine[9] = {0};	// Tableau indiquant si les lignes sont valides.
int validationBlock[9] = {0};	// Tableau indiquant si les sous-grilles sont valides.


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
