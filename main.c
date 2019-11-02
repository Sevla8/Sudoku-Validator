#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	int line;
	int row;
} Parameters;

typedef struct {
	int position;
	int number;
} Error;	// Même sturcture que Parameters mais pour une meilleur lisibilité on crée une autre structure pour les erreurs.

int sudoku[9][9];

int validationRow[9] = {0};	// Tableau indiquant si les colonnes sont valides.
int validationLine[9] = {0};	// Tableau indiquant si les lignes sont valides.
int validationBlock[9] = {0};	// Tableau indiquant si les sous-grilles sont valides.

void *threadProcessRow(void *arg) {
	int row = ((Parameters*) arg)->row;
	int result[9] = {0};	// Tableau dont chaque cellule indique le nombre d'apparition de son indice dans la colonne de sudoku.
	for (int i = 0; i < 9; i += 1)
		result[sudoku[i][row]-1] += 1;
	for (int i = 0; i < 9; i += 1) {
		if (result[i] == 0) {
			Error *error = (Error*) malloc(sizeof(Error));
			error->position = row;	// Numéro de la colonne
			error->number = i+1;	// Identification d'un chiffre manquant
			pthread_exit(error);
		}
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
		if (result[i] == 0) {
			Error *error = (Error*) malloc(sizeof(Error));
			error->position = line;	// Numéro de la ligne
			error->number = i+1;	// Identification d'un chiffre manquant
			pthread_exit(error);
		}
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
		if (result[i] == 0) {
			Error *error = (Error*) malloc(sizeof(Error));
			error->position = data->line + data->row/3;	// Numéro du bloc
			error->number = i+1;	// Identification d'un chiffre manquant
			pthread_exit(error);
		}
	}
	validationBlock[data->line + data->row/3] = 1;	// S'il y a exactement une fois les chiffres de 1 à 9 dans la sous-grille, on met à jour le tableau validant la sous-grille.
	pthread_exit(NULL);
}

int reader(char* filename, Parameters* param) {	// Fonction permettant de lire l'entrée du programme.
	FILE* file = NULL;
	file = fopen(filename, "r");
	if (file == NULL) {
		perror("fopen()");
		return -2;
	}
	for (int i = 0; i < 9; i += 1) {
		for (int j = 0; j < 9; j += 1) {
			int number = fgetc(file) - '0';	// Conversion de (char 'chiffre') en (int chiffre).
			if (number < 1 || number > 9) {	// Si l'entrée comporte autre chose qu'un chiffre.
				param->line = i;
				param->row = j;
				if (number == '$' || number == '%' || number == '!') {
					fclose(file);
					return -4;	// Special character error
				}
				fclose(file);
				return -1;	// Not a digit error
			}
			sudoku[i][j] = number;
			number = fgetc(file);
			if (j != 8 && (number == '\n' || number == '\r' || number == EOF) || j == 8 && (number != '\n' && number != '\r' && number != EOF)) { // Some systems use '\r' as line separator
				fclose(file);
				return -3;	// Grid size error
			}
		}
	}
	fclose(file);
	return 0;
}

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
	
	int count = 0;	// Compteur des threads.
	pthread_t tid[9*3];	// Tableau contenant les identifiants des différents threads.
	pthread_attr_t attr;	// On aurait simplement pu mettre NULL en second paramètre de pthread_create
	if (pthread_attr_init(&attr)) {
		perror("pthread_attr_init()");
		exit(EXIT_FAILURE);
	}
	Parameters *data[9*3];	// Tableau contenant les différents paramètres que l'on va passer en paramètres des threads.

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

	for (int i = 0; i < 9; i += 3) {	// On renseigne une sous-grille par la ligne et la colonne de sa première case.
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

	void *errors[9*3];	// Tableau d'erreurs
	for (int i = 0; i < 9*3; i += 1) {
		if (pthread_join(tid[i], &(errors[i]))) {	// On attend que touts les threads se terminent. Et on récupère les erreurs.
			perror("pthread_join()");
			exit(EXIT_FAILURE);
		}
	}

	if (pthread_attr_destroy(&attr)) {
		perror("pthread_attr_destroy()");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 9*3; i +=1)
		free(data[i]);	// Libération de la mémoire dynamique.

	int errorCount = 0;
	for (int i = 0; i < 9; i += 1) {
		if (!validationRow[i]) {	// Si une colonne n'est pas valide.
			if (errors[i] != NULL)
				printf("Row number %d does not contain '%d'\n", ((Error**) errors)[i]->position, ((Error**) errors)[i]->number);
			errorCount += 1;
		}
		free(errors[i]);
		if (!validationLine[i]) {	// Si une ligne n'est pas valide.
			if (errors[i+9] != NULL)
				printf("Ligne number %d does not contain '%d'\n", ((Error**) errors)[i+9]->position, ((Error**) errors)[i+9]->number);
			errorCount += 1;
		}
		free(errors[i+9]);
		if (!validationBlock[i]) {	// Si une sous-grille n'est pas valide.
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
