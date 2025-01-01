#include <assert.h>
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include "../src/headers/matrix.h"

int main(int argc, char *argv[]){

	/* 2 2 matrix */
	Matrix* mat = matrix_rand(matrix_size(2, 2), -6, 6);
	assert(mat != NULL);

	mpq_t* det = matrix_det(mat);

	Matrix* inv = matrix_invert(mat);
	assert(inv != NULL);

	char* str = matrix_to_latex(mat);

	mpq_clear(*det);
	free(det);
	free(str);
	matrix_free(mat);
	matrix_free(inv);

	/* 3 3 matrix */
	// Matrix* mat2 = matrix_rand(3, -5, 5);
	// assert(mat2 != NULL);
	// matrix_print(mat2);
	
	return 0;
}
