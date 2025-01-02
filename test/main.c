#include <assert.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <stdlib.h>
#include "../src/headers/matrix.h"

int main(int argc, char *argv[]){

	/* 3 3 matrix */
	Matrix* mat = matrix_new(matrix_size(3, 3));
	mpq_set_ui((mat->matrix)[0][0], 0, 1);
	mpq_set_ui((mat->matrix)[0][1], 1, 1);
	mpq_set_ui((mat->matrix)[0][2], 2, 1);
	mpq_set_ui((mat->matrix)[1][0], 1, 1);
	mpq_set_ui((mat->matrix)[1][1], 2, 1);
	mpq_set_ui((mat->matrix)[1][2], 3, 1);
	mpq_set_ui((mat->matrix)[2][0], 3, 1);
	mpq_set_ui((mat->matrix)[2][1], 1, 1);
	mpq_set_ui((mat->matrix)[2][2], 1, 1);

	matrix_print(mat);

	mpq_t* det = matrix_det(mat);
	gmp_printf("\nDeterminant : %Qd\n", *det);

	if (det != 0) {
		Matrix* inverse = matrix_invert(mat);
		printf("Matrice inverse :\n");
		matrix_print(inverse);
		matrix_free(inverse);
	}

	mpq_clear(*det);
	free(det);
	matrix_free(mat);
	
	return 0;
}
