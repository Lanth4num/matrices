#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "headers/matrix.h"

int main(int argc, char *argv[]){


	Matrix* matrix = matrix_rand(matrix_size(2, 2), -6, 6);
	Matrix* inverse = matrix_invert(matrix);

	char* matrix_str = matrix_to_latex(matrix);
	char* inverse_str = NULL;

	if (inverse != NULL){ 
		inverse_str = matrix_to_latex(inverse);
	} else {
		/* Set the inverse_str to "Impossible" if not able to inverse it */
		char dummy[] = "\\text{Impossible}";
		inverse_str = calloc(sizeof(dummy) + 1, sizeof(char)); 
		strcpy(inverse_str, dummy);
	}

	/* Free */

	matrix_free(matrix);
	if (inverse != NULL) matrix_free(inverse);

	free(matrix_str);
	if (inverse_str != NULL) free(inverse_str);


	return 0;
}
