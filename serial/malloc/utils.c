/* File: utils.c
 * 
 * Utility functions for serial hypergraph matching
 *
 * Reid Delaney & Kevin Liu
 */
#pragma once

#include <stdio.h>
#include <math.h>


/* transpose a size1*size2 matrix
 * returns a pointer to the new matrix
 */
void transpose(int size1, int size2, float* in_matrix, float* out_matrix) {
	for(int i=0; i<size1; i++){
		for(int j=0; j<size2; j++){
			*(out_matrix + j*size1 + i) = *(in_matrix + i*size2 + j);
			//out_matrix[j][i] = in_matrix[i][j];
		}
	}
}

/* make Y an all zero matrix
 * size1 and size2 are the size of Y
 */
void zeros(int size1, int size2, float *Y){
	for(int i=0; i<size1; i++){
		for(int j=0; j<size2; j++){
			*(Y + (i*size2 + j) ) = 0;
			//Y[i][j] = 0;
		}
	}
}


/* make Y an all one matrix
 * size1 and size2 are the size of Y
 */
void ones(int size1, int size2, float* Y){
	for(int i=0; i<size1; i++){
		for(int j=0; j<size2; j++){
			*(Y + (i*size2 + j) ) = 1;
			//Y[i][j] = 1;
		}
	}
}

/* check if a square matrix is symmetric
 * size: the size of the matrix
 * returns 1 if true, 0 if false
 */
int isSymmetric(int size, float* in_matrix){
	double diff;
	for(int i=0; i < size; i++){
		for(int j=0; j < size; j++){
			diff = *(in_matrix + i*size + j) - *(in_matrix + j*size + i);
			//diff = in_matrix[i][j] - in_matrix[j][i];
			if(diff != 0){
				return 0;
			}
		}
	}
	return 1;
}

/* check if a square matrix is symmetric with an epsilon eps
 * size: the size of the square matrix
 * eps: the epsilon value
 * returns 1 if true 0 if false
 */
int isSymmetric_eps(int size, double eps, float* in_matrix) {
	double diff;
	for(int i=0; i < size; i++){
		for(int j=0; j < size; j++){
			diff = *(in_matrix + i*size + j) - *(in_matrix + j*size + i);
			//diff = in_matrix[i][j] - in_matrix[j][i];
			if(diff > eps || diff < -eps){
				return 0;
			}
		}
	}
	return 1;
}

/* creates a m*n tiling of a matrix */
void repmat(int size1, int size2, float* in_matrix, int m, int n, float* out_matrix) {
	for(int i=0; i<size1*m; i++) {
		for(int j=0; j<size2*n; j++) {
			*(out_matrix + i*size2*n + j) = *(in_matrix + (i%size1)*size2 + (j%size2));
			//out_matrix[i][j] = in_matrix[i%size1][j%size2];
		}
	}
}

/* Perform matrix subtraction and return the difference between two matrices 
 * in_matrix1 in_matrix2: input matrices
 * diff: output matrix
 * dim1 dim2: the dimension of the matrices 
 */
void matSub(int dim1, int dim2, float* in_matrix1, float* in_matrix2, float* diff) {
	for(int i=0; i < dim1; i++){
		for(int j=0; j < dim2; j++){
			*(diff + i * dim2 + j) = *(in_matrix1 + i * dim2 + j) - *(in_matrix2 + i * dim2 + j);
			//diff[i][j] = in_matrix1[i][j] - in_matrix2[i][j];
		}
	}
}

/* Perform matrix addtion and return the sum of two matrices 
 * in_matrix1 in_matrix2: input matrices
 * sum: output matrix
 * dim1 dim2: the dimension of the matrices 
 */
void matAdd(int dim1, int dim2, float* in_matrix1, float* in_matrix2, float* sum) {
	for(int i=0; i < dim1; i++){
		for(int j=0; j < dim2; j++){
			*(sum + i * dim2 + j) = *(in_matrix1 + i * dim2 + j) + *(in_matrix2 + i * dim2 + j);
			//sum[i][j] = in_matrix1[i][j] + in_matrix2[i][j];
		}
	}
}

/* Perform matrix and scaler addtion and return the sum 
 * in_matrix1: input matrix
 * scaler: input scaler
 * product: output matrix
 * dim1 dim2: the dimension of the matrix 
 */
void matTimesScaler(int dim1, int dim2, float* in_matrix, float scaler, float* product) {
	for(int i=0; i < dim1; i++){
		for(int j=0; j < dim2; j++){
			*(product + i * dim2 + j) = *(in_matrix + i * dim2 + j) * scaler;
			//product[i][j] = in_matrix[i][j] * scaler;
		}
	}
}

/* Perform matrix and scaler multiplication and return the product 
 * in_matrix1: input matrix
 * scaler: input scaler
 * sum: output matrix
 * dim1 dim2: the dimension of the matrix 
 */
void matPlusScaler(int dim1, int dim2, float* in_matrix, float scaler, float* sum) {
	for(int i=0; i < dim1; i++){
		for(int j=0; j < dim2; j++){
			*(sum + i * dim2 + j) = *(in_matrix + i * dim2 + j) + scaler;
			//sum[i][j] = in_matrix[i][j] + scaler;
		}
	}
}

/* Perform element wise matrix division
 * in_matrix1 in_matrix2: input matrices
 * out: output matrix
 * dim1 dim2: the dimension of the matrices 
 */
void matDiv(int dim1, int dim2, float* in_matrix1, float* in_matrix2, float* out){
	for(int i=0; i < dim1; i++){
		for(int j=0; j < dim2; j++){
			*(out + i * dim2 + j) = *(in_matrix1 + i * dim2 + j) / *(in_matrix2 + i * dim2 + j);
			//out[i][j] = in_matrix1[i][j] / in_matrix2[i][j];
		}
	}
}
/* find the maximum element in an array */
float maxOfArray(int size,float* array) {
	float m = *(array);
	for (int i=0; i<size; i++) {
		m = (m > *(array+i))? m : *(array+i);
	}
	return m;
}

/* find the maximum elements in a matrix
 * dim: the dimension wrt which you want to find the maximums
 * 
 * Example:
 * Consider the following matrix A:
 * 	1 2 3
 * 	4 5 6
 * 	7 8 9
 * maxOfMatrix(A,1) will return [7 8 9]
 * maxOfMatrix(A,2) will return [3 6 9]                            
 */
void maxOfMatrix(int size1, int size2, float* matrix, float *out, int dim) {
	if (dim == 1) {
		for(int i=0; i<size2; i++) {
			float m = *(matrix + i);
			//float m = matrix[0][i];
			for (int j=0; j<size1; j++) {
				m = (m > *(matrix+j*size2+i))? m : *(matrix+j*size2+i);
				//m = (m > matrix[j][i]) ? m : matrix[j][i];
			}
		*(out + i) = m;
		}
	} else if (dim = 2) {
		for(int i=0; i<size1; i++) {
			float m = *(matrix + i*size2);
			//float m = matrix[i][0];
			for (int j=0; j<size2; j++) {
				m = (m > *(matrix+i*size2+j))? m : *(matrix+i*size2+j);
				//m = (m > matrix[i][j]) ? m : matrix[i][j];
			}
		*(out + i) = m;
		}
	}
}

/* prints a matrix 
 * matrix: pointer to the matrix to be printed
 * size1 size2: size of matrix
 */
void printMatrix( int size1, int size2, float* matrix) {
	for (int i=0; i<size1; i++){
		for(int j=0; j<size2; j++) {
			printf("%.2f ", *(matrix + i*size2 + j));
			//printf("%.4f ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

/* gets one row from a matrix 
 * in_matrix: input matrix
 * size: the length of one row
 * output: an array containing the row
 * rownum: row number
 */
void getRow(int size1, int size2, float* in_matrix, float* output, int rownum) {
	for(int i=0; i<size2; i++) {
		*(output+i) = *(in_matrix + (rownum)*size2 + i);
		//output[0][i] = in_matrix[rownum][i];
	}
}

/* gets one column from a matrix 
 * in_matrix: input matrix
 * size: the length of one column
 * output: an array containing the column
 * colnum: column number
 */
void getCol(int size1, int size2, float* in_matrix, float* output, int colnum) {
	for(int i=0; i<size1; i++) {
		*(output+i) = *(in_matrix + i*size2 + (colnum));
		//output[i][0] = in_matrix[i][colnum];
	}
}

/* find an element in an array and return its index 
 * note: this function only returns the index of the 
 * first match element
 */
int indexOfElement(int size, float* in, float element) {
	for (int i=0; i<size; i++) {
		if (*(in + i) == element)
		//if (in[i][0] == element)
			return i;
	}
	return -1;
}

/* reshapes a matrix into another dimension
 * the number of elements must remain the same
 */
void reshape(int insize1, int insize2, float* in, int outsize1, int outsize2, float* out) {
	if (insize1*insize2 == outsize1*outsize2) {
		int c = 0;
		for (int i=0; i<insize2; i++) {
			for (int j=0; j<insize1; j++) {
				*(out+(c%outsize1)*outsize2+(c/outsize1)) = *(in+j*insize2+i);
				//out[c%outsize2][c/outsize1] = in[j][i];
				c++;
			}
		}
	} else	
		printf("Error: cannot reshape. the number of elements must agree");
}
