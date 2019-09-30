#include "reader.h"

int reader() {	// Fonction permettant de lire l'entrée du programme.
	for (int i = 0; i < 9; i += 1) {
		for (int j = 0; j < 9; j += 1) {
			char c1;
			scanf("%c ", &c1);	// L'espace pour enlever du buffer les caractere appartenant à IFS.
			int number = c1 - '0';	// Conversion de (char 'chiffre') en (int chiffre).
			if (number < 0 || number > 9)	// Si l'entrée comporte autre chose qu'un chiffre.
				return -1;
			sudoku[i][j] = number;
		}
	}
	return 0;
}
