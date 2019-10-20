#include "reader.h"

int reader(char* filename, Parameters* param) {
	FILE* file = NULL;
	file = fopen(filename, "r");
	if (file == NULL) {
		perror("fopen()");
		return -2;
	}
	for (int i = 0; i < 9; i += 1) {
		for (int j = 0; j < 9; j += 1) {
			int number = fgetc(file) - '0';
			if (number < 1 || number > 9) {
				param->line = i;
				param->row = j;
				if (number == '$' || number == '%' || number == '!') {
					fclose(file);
					return -4;
				}
				fclose(file);
				return -1;
			}
			sudoku[i][j] = number;
			number = fgetc(file);
			if (j != 8 && (number == '\n' || number == '\r' || number == EOF) || j == 8 && (number != '\n' && number != '\r' && number != EOF)) { // Some systems use '\r' as line separator
				fclose(file);
				return -3;
			}
		}
	}
	fclose(file);
	return 0;
}
