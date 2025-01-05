#include <assert.h>
#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "../src/headers/matrix.h"

int main(int argc, char *argv[]){
	srand(time(NULL));

	/* Initialization */
	Matrix* mat1 = matrix_rand(matrix_size(2, 3), -3, 3);
	assert(mat1 != NULL);
	Matrix* mat2 = matrix_rand(matrix_size(3, 1), -3, 3);
	assert(mat2 != NULL);
	Matrix* mul = matrix_mul(mat1, mat2);
	assert(mul != NULL);

	/* Printing */
	matrix_print(mat1);
	printf("x\n");
	matrix_print(mat2);
	printf("=\n");
	matrix_print(mul);

	matrix_free(mat1);
	matrix_free(mat2);
	matrix_free(mul);

	return 0;
}
