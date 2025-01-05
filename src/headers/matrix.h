#ifndef INCLUDE_MATRIX
#define INCLUDE_MATRIX

#include <gmp.h>

typedef struct Matrix Matrix;
typedef struct MatrixSize MatrixSize;

struct Matrix{
	MatrixSize* size;
	mpq_t ** matrix;
};

MatrixSize* matrix_size(size_t rows, size_t cols);
Matrix * matrix_new(MatrixSize* size);
void matrix_free(Matrix *);
Matrix* matrix_rand(MatrixSize* size, int min, int max);

mpq_t * matrix_det(Matrix *); 
Matrix * matrix_invert(Matrix *); 
Matrix* matrix_mul(Matrix*, Matrix*);

void matrix_print(Matrix *);
char* matrix_to_latex(Matrix *);

#endif  // INCLUDE_MATRIX
