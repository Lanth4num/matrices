#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

#include "headers/matrix.h"

struct MatrixSize{
	size_t rows;
	size_t cols;
};

bool is_matrix_size(Matrix* matrix, size_t rows, size_t cols){
	if (matrix->size->rows == rows &&
			matrix->size->cols == cols)
	{
		return true;
	}
	return false;
}

MatrixSize* matrix_size(size_t rows, size_t cols){
	MatrixSize* size = malloc(sizeof(MatrixSize));
	size->rows = rows;
	size->cols = cols;
	return size;
} 

/* Returns a copy of the given matrix */
static Matrix* matrix_cpy(Matrix* src){
	
	Matrix* new_mat = matrix_new(matrix_size(src->size->rows, src->size->cols));
	/* Loop through rows */
	for (size_t i=0; i<(src->size->rows); i++) {
		/* Loop through cols */
		for (size_t j=0; j<(src->size->cols); j++) {
			mpq_set((new_mat->matrix)[i][j], (src->matrix)[i][j]);	
		}
	}

	return new_mat;
}


static void matrix_mul_cst(Matrix* matrix, mpq_t operand){
	for (size_t row=0; row<matrix->size->rows; row++) {
		for (size_t col=0; col<matrix->size->cols; col++) {
			mpq_mul((matrix->matrix)[row][col], matrix->matrix[row][col], operand);
		}
	}
}


static Matrix* matrix_transpose(Matrix* matrix){
	/* Create new matrix with inverse size*/
	Matrix* new_mat = matrix_new(matrix_size(matrix->size->cols, matrix->size->rows));

	/* Copy the matrix, inversing the cols and rows */
	for (size_t row=0; row<matrix->size->rows; row++) {
		for (size_t col=0; col<matrix->size->cols; col++) {
			mpq_set((new_mat->matrix)[col][row], (matrix->matrix)[row][col]);
		}
	}

	return new_mat;
}


static mpq_t* matrix_det_2(mpq_t** matrix){
	
	mpq_t* det = malloc(sizeof(mpq_t));
	mpq_init(*det);

	mpq_mul(*det, matrix[0][0], matrix[1][1]);

	mpq_t temp;
	mpq_init(temp);
	mpq_mul(temp, matrix[0][1], matrix[1][0]);

	mpq_sub(*det, *det, temp);

	mpq_clear(temp);
	return det;
}


static mpq_t* matrix_det_3(mpq_t** matrix){

	mpq_t* det = malloc(sizeof(mpq_t));
	mpq_init(*det);

	for (size_t i=0; i<3; i++){
		Matrix* temp = matrix_new(matrix_size(2, 2));

		/* creating the sub 2-2 matrix */
		mpq_t** mat = temp->matrix;
		size_t j = 0;

		if(j==i) j+=1;
		mpq_set(mat[0][0], matrix[1][j]);
		mpq_set(mat[1][0], matrix[2][j]);

		j+=1;
		if(j==i) j+=1;
		mpq_set(mat[0][1], matrix[1][j]);
		mpq_set(mat[1][1], matrix[2][j]);

		/* Get the det of it */
		mpq_t* temp_det = matrix_det_2(mat);

		/* If i odd then negate it */
		if (i%2 != 0) mpq_neg(*temp_det, *temp_det);

		/* Adding it to the 3-3 det (with multiplication)*/
		mpq_mul(*temp_det, *temp_det, matrix[0][i]);
		mpq_add(*det, *det, *temp_det);

		/* Free the stuff */
		matrix_free(temp);
		mpq_clear(*temp_det);
		free(temp_det);
	}

	return det;
}


static Matrix* matrix_invert_2(Matrix* arg_matrix){

	/* Get the determinant and check for 0 */
	mpq_t* det = matrix_det_2(arg_matrix->matrix);

	if ( mpq_sgn(*det) == 0 ){
		mpq_clear(*det);
		free(det);
		return NULL;
	}

	Matrix* res_matrix = matrix_cpy(arg_matrix);
	mpq_t** matrix = res_matrix->matrix;

	/* Swaps what needs to be swaped (\) */
	mpq_t temp;
	mpq_init(temp);
	mpq_set(temp, matrix[0][0]);
	mpq_set(matrix[0][0], matrix[1][1]);
	mpq_set(matrix[1][1], temp);
	mpq_clear(temp);

	/* And reverse the other 2 (/)*/
	mpq_neg(matrix[0][1], matrix[0][1]);
	mpq_neg(matrix[1][0], matrix[1][0]);

	/* Now multiply each by 1/determinant */
	/* Invert det */
	mpq_inv(*det, *det);

	mpq_mul(matrix[0][0], matrix[0][0], *det);
	mpq_mul(matrix[0][1], matrix[0][1], *det);
	mpq_mul(matrix[1][0], matrix[1][0], *det);
	mpq_mul(matrix[1][1], matrix[1][1], *det);

	mpq_clear(*det);
	free(det);

	return res_matrix;
}


static Matrix* matrix_invert_3(Matrix* arg_matrix){

	mpq_t** matrix = arg_matrix->matrix;
	mpq_t* det = matrix_det_3(matrix);

	/* Verify that it is inversible */
	if ( mpq_sgn(*det) == 0 ){
		mpq_clear(*det);
		free(det);
		return NULL;
	}

	Matrix* inverse_mat = matrix_new(matrix_size(3, 3));

	for ( size_t row=0; row<3; row++){
		for (size_t col=0; col<3; col++){
			Matrix* temp = matrix_new(matrix_size(2, 2));

			/* creating the sub 2-2 matrix */
			mpq_t** mat = temp->matrix;

			size_t i = 0; /* Used for row */
			size_t j = 0; /* Used for col */

			if(i==row) i+=1;
			if(j==col) j+=1;
			mpq_set(mat[0][0], matrix[i][j]);

			i+=1;
			if(i==row) i+=1;
			mpq_set(mat[1][0], matrix[i][j]);

			j+=1;
			if(j==col) j+=1;

			i=0;
			if(i==row) i+=1;
			mpq_set(mat[0][1], matrix[i][j]);

			i+=1;
			if(i==row) i+=1;
			mpq_set(mat[1][1], matrix[i][j]);

			/* Get the det of it */
			mpq_t* temp_det = matrix_det_2(mat);

			/* If i odd then negate it */
			if (row%2 == 0){
				if (col%2 != 0) mpq_neg(*temp_det, *temp_det);
			} else {
				if (col%2 == 0) mpq_neg(*temp_det, *temp_det); 
			}

			/* Put it in the inverse matrix */
			mpq_set((inverse_mat->matrix)[row][col], *temp_det);

			/* Free the stuff */
			matrix_free(temp);
			mpq_clear(*temp_det);
			free(temp_det);
		}
	}

	Matrix* transpose = matrix_transpose(inverse_mat);
	/* Invert the determinant */
	mpq_inv(*det, *det);
	/* Multiply matrix by it */
	matrix_mul_cst(transpose, *det);

	matrix_free(inverse_mat);
	mpq_clear(*det);
	free(det);
	return transpose;
}


Matrix * matrix_new(MatrixSize* size){

	Matrix* matrix = (Matrix*) malloc(sizeof(Matrix));
	if (matrix == NULL) return NULL;

	matrix->size = size;

	// allocating the rows
	matrix->matrix = (mpq_t **) malloc(sizeof(mpq_t *) * (size->rows));
	if (matrix->matrix == NULL){
		free(matrix);
		return NULL;
	}

	// allocating the columns
	for (size_t i=0; i<size->rows; i++) {
		
		(matrix->matrix)[i] = (mpq_t *) malloc(sizeof(mpq_t) * size->cols);	
		if ((matrix->matrix)[i] == NULL){
			free(matrix->matrix);
			free(matrix);
			return NULL;
		}

		// initializing the numbers
		for (size_t j=0; j<size->cols; j++) {
			mpq_init( (matrix->matrix)[i][j] );
		}
	}	

	return matrix;
}


void matrix_free(Matrix * matrix){

	/* Loop through rows */
	for (size_t i=0; i<(matrix->size->rows); i++) {
		/* Loop through cols */
		for (size_t j=0; j<(matrix->size->cols); j++) {
			/* Free the rational number */
			mpq_clear((matrix->matrix)[i][j]);
		}
		/* Free the cols */
		free((matrix->matrix)[i]);
	}

	/* Free the rows */
	free(matrix->matrix);
	/* Free the size */
	free(matrix->size);
	/* Free the matrix */
	free(matrix);

	return;
}


/* Creates a random matrix with number from min to max inclusive */
Matrix* matrix_rand(MatrixSize* size, int min, int max){

	/* srand should be initialized before */

	if (min > max) {printf("Expects a min < max (matrix_rand)\n"); return NULL;}

	Matrix* matrix = matrix_new(size);

	/* Iterating through rows */
	for (size_t i=0; i<size->rows; i++) {

		/* Iterating through cols */
		for (size_t j=0; j<size->cols; j++) {
			/* Generating random integer (+1 because max is inclusive)*/	
			int rand_int = (rand()%(max+1-min)) + min; 
			mpq_set_si((matrix->matrix)[i][j], rand_int, 1);
		}
	}

	return matrix;
}


/* Wrapper */
mpq_t * matrix_det(Matrix* matrix){
	if ( is_matrix_size(matrix, 2, 2) ){
		return matrix_det_2(matrix->matrix);
	} 
	else if (is_matrix_size(matrix, 3, 3)){
		return matrix_det_3(matrix->matrix);
	}	

	/* If not supported size */
	return NULL;
}


/* Wrapper */
Matrix * matrix_invert(Matrix* matrix){
	if ( is_matrix_size(matrix, 2, 2) ){
		return matrix_invert_2(matrix);
	} else if ( is_matrix_size(matrix, 3, 3) ){
		return matrix_invert_3(matrix);
	}
	return NULL;
}

void matrix_print(Matrix *matrix){

	/* For each row */
	for (size_t i=0; i<matrix->size->rows; i++) {
		printf("|");
		for (size_t j=0; j<matrix->size->cols; j++) {
			if (j!=0) printf("\t");
			gmp_printf("%Qd", (matrix->matrix)[i][j]);
		}
		printf("|\n");
	}

	return;
}


char* matrix_to_latex(Matrix* matrix){
	
	#define MAX_LEN 512
	char* str = calloc(MAX_LEN, sizeof(char));

	/* Start of the string */
	/* Get the right number of columns */
	size_t cols = matrix->size->cols;	
	char cols_str[cols+1]; 
	for (size_t i=0; i<cols; i++) {
		cols_str[i] = 'c';
	}
	cols_str[cols] = '\0';

	gmp_snprintf(str, MAX_LEN, "\\left(\\begin{array}{%s}", cols_str);

	/* Adding the elements */
	/* Rows */
	for (size_t i=0; i<(matrix->size->rows); i++) {
		/* Cols */	
		for (size_t j=0; j<(matrix->size->cols); j++) {

			/* Wether to add the & or not */
			char and = j==0 ? ' ' : '&'; 
			gmp_snprintf(str+strlen(str), MAX_LEN-strlen(str), " %c%Qd", and, (matrix->matrix)[i][j]);

		}
		gmp_snprintf(str+strlen(str), MAX_LEN-strlen(str), "\\\\"); /* Newline */
	}

	/* End of the string */
	gmp_snprintf(str+strlen(str), MAX_LEN-strlen(str), " \\end{array}\\right)");
	
	return str;
}


