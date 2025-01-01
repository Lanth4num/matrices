#include <assert.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <stdlib.h>
#include "../src/headers/matrix.h"

int main(int argc, char *argv[]){

	/* 2 2 matrix */
	// Matrix* mat = matrix_rand(matrix_size(2, 2), -6, 6);
	// assert(mat != NULL);
	//
	// mpq_t* det = matrix_det(mat);
	//
	// Matrix* inv = matrix_invert(mat);
	// assert(inv != NULL);
	//
	// char* str = matrix_to_latex(mat);
	//
	// mpq_clear(*det);
	// free(det);
	// free(str);
	// matrix_free(mat);
	// matrix_free(inv);
	
	Matrix* matrix = matrix_rand(matrix_size(2, 2), -6, 6);
	char* matrix_str = matrix_to_latex(matrix);
	Matrix* inverse = matrix_invert(matrix);
	char* inverse_str = NULL;

	matrix_print(matrix);

	if (inverse != NULL){ 
		matrix_print(inverse);
		inverse_str = matrix_to_latex(inverse);
	} else {
		char dummy[] = "\\text{Impossible}";
		inverse_str = calloc(sizeof(dummy) + 1, sizeof(char)); 
		strcpy(inverse_str, dummy);
	}

	matrix_free(matrix);
	free(matrix_str);
	if (inverse != NULL) matrix_free(inverse);
	if (inverse_str != NULL) free(inverse_str);


	/* 3 3 matrix */
	// Matrix* mat2 = matrix_rand(3, -5, 5);
	// assert(mat2 != NULL);
	// matrix_print(mat2);
	
	return 0;
}
