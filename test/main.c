#include <assert.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <stdlib.h>
#include "../src/headers/matrix.h"

int main(int argc, char *argv[]){

	/* 3 3 matrix */
	Matrix* mat = matrix_rand(matrix_size(3, 3), -5, 5);
	assert(mat != NULL);
	matrix_print(mat);

	mpq_t* det = matrix_det(mat);
	gmp_printf("\ndeterminant : %Qd\n", *det);

	mpq_clear(*det);
	free(det);
	matrix_free(mat);
	
	return 0;
}
