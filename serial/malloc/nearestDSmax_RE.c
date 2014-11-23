#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "utils.c"

#define EPS 2.2204e-16
/*
function F = nearestDSmax_RE (Y, maxRowSum, maxColSum, totalSum, precision, maxLoops)

Find the nearest matrix with non-negative entries and row / column sum at
most rowSum / colSum, to A in relative entropy.

Algorithm due to R. Zass and A. Shashua.,
'Probabilistic Graph and Hypergraph Matching.',
Computer Vision and Pattern Recognition (CVPR) Anchorage, Alaska, June 2008.

Y - an m by n non-negative matrix.
maxRowSum - a non-negative column vector of length m with the maximal allowed rows sum.
maxColSum - a non-negative row vector of length n with the maximal allowed columns sum.
totalSum - a non-negative scalar, the desired sum of all elements.
precision [Optional, default=0.01] - break when the each row/column sum is at most precision away from the desired sum.
maxLoops [Optional, defalut=1000] - Max number of iterations to perform.

F [Output] - The nearest generalized doubly stochastic F to Ker in relative entropy.

Author: Ron Zass, zass@cs.huji.ac.il, www.cs.huji.ac.il/~zass 
*/


//function x = exactTotalSum (y, h, totalSum, precision)
void exactTotalSum(int length, float* y, float* h, float totalSum, float precision, float* X) {

/*	printf("X inside exactTotalSum\n");
	printMatrix(length, 1, X);
	printf("y inside exactTotalSum\n");
	printMatrix(length, 1, y);
	printf("h inside exactTotalSum\n");
	printMatrix(length, 1, h);
*/
	// y and h are vectors, totalSum and precision are scalars
	//X is the return vector and length is the length of y, h, and X
	float totalSumMinus = totalSum - precision;
	float curAlpha;

	//get the minimum of vector h
	float min = *(h);
	for (int i=1; i < length; i++)
		min = (min < *(h + i))? min : *(h + i);

 	curAlpha = -min + EPS;

	//stepAlpha = max(10, abs(curAlpha/10));
	float stepAlpha, newAlpha, newSum;
	if(10 > fabs(curAlpha/10))
		stepAlpha = 10;
	else
		stepAlpha = fabs(curAlpha/10);

	for(int j=0; j < 50; j++) {
		newAlpha = curAlpha + stepAlpha;
		newSum = 0;

		//x = y ./ (h + newAlpha);
		for(int k=0; k < length; k++) {
			*(X + k) = *(y + k) / (*(h + k) + newAlpha);
			//newSum = sum(x);
			newSum += *(X + k);
		}

		if(newSum > totalSum) {
			curAlpha = newAlpha;
		} else {
			if (newSum < totalSumMinus)
				stepAlpha = stepAlpha / 2;
			else return;
		}
	
	} //end of for j

} //end of function

void unconstrainedP(int size1, int size2, float* Y, float* H, float* X){

/*
	printf("Y is\n");
	printMatrix(size1, size2, Y);
	printf("H is\n");
	printMatrix(size1, size2, H);
*/
	//function X = unconstrainedP (Y, H)
	for(int i=0; i<size1; i++) {
		for(int j=0; j<size2; j++) {
			//X = Y ./ H;
			*(X + i*size2 + j) = *(Y + i*size2 + j) / *(H + i*size2 + j);

			//X(find(X < eps)) = eps;
			if(fabs(*(X + i*size2 + j)) < EPS) {
				*(X + i*size2 + j) = EPS;
			}

		} // end of for j
	} // end of for i
/*
	printf("Y is now\n");
	printMatrix(size1, size2, Y);
	printf("H is now\n");
	printMatrix(size1, size2, H);
*/

} // end of function



//function X = maxColSumP (Y, H, maxColSum, precision)
void maxColSumP(int dim1, int dim2, float* Y, float* H, float* maxColSum, float precision, float* X) {
	//X = unconstrainedP (Y, H);

//	printf("X before unconstrained is\n");
//	printMatrix(dim1, dim2, X);

	unconstrainedP(dim1, dim2, Y, H, X);

//	printf("X after unconstrained is\n");
//	printMatrix(dim1, dim2, X);

	//Xsum = sum(X);
	float* Xsum = (float *) malloc(dim2 * sizeof(float));
	for(int i=0; i < dim2; i++){
		for(int j=0; j < dim1; j++){
			*(Xsum + i) += *(X + j*dim2 + i);
		} 
	}
	
	float* yCol = (float *)malloc(dim1 * sizeof(float));
	float* hCol = (float *)malloc(dim1 * sizeof(float));
	float* Xcol = (float *)malloc(dim1 * sizeof(float));
	//float dim = dim1*dim2;	

//	printf("X before exactTotalSum is\n");
//	printMatrix(dim1, dim2, X);

	//for i = find(Xsum > maxColSum)
	for(int i=0; i < dim2; i++) {
		if(*(Xsum + i) > *(maxColSum + i)) {

			//X(:,i) = exactTotalSum (Y(:,i), H(:,i), maxColSum(i), precision);
			getCol(dim1, dim2, Y, yCol, i);
			getCol(dim1, dim2, H, hCol, i);

			exactTotalSum(dim1, yCol, hCol, *(maxColSum + i), precision, Xcol);
//			printf("Xcol is\n");
//			printMatrix(dim1, 1, Xcol);
//			printf("X after exactTotalSum in maxColSumP\n");
//			printMatrix(dim1, dim2, X);
			
			for(int j=0; j < dim1; j++){
				*(X + j*dim1 + i) = *(Xcol + j);
			}

		}
	}

//	printf("X after exactTotalSum is\n");
//	printMatrix(dim1, dim2, X);

	free(yCol);
	free(hCol);
	free(Xcol);
	free(Xsum);

} // end of function


void nearestDSmax_RE(int m, int n, float* Y, float* maxRowSum, float* maxColSum, float totalSum, float precision, float maxLoops, float* F){
//m and n are the dimensions of Y

/*
 *   lambda1 = zeros(size(Y));
 *   lambda2 = lambda1;
 *   lambda3 = lambda1;
 */

	zeros(m, n, F);

	float* lambda1 = (float *) malloc(m*n*sizeof(float));
	float* lambda2 = (float *) malloc(m*n*sizeof(float));
	float* lambda3 = (float *) malloc(m*n*sizeof(float));
	zeros(m, n, lambda1);
	zeros(m, n, lambda2);
	zeros(m, n, lambda3);
		
/*	printf("lambda1\n");
	printMatrix(m, n, lambda1);
	printf("lambda2\n");
	printMatrix(m, n, lambda2);
	printf("lambda3\n");
	printMatrix(m, n, lambda3);
*/		

/*    
 *	F1 = totalSum * (Y ./ sum(Y(:)));
 *  F2 = F1;
 *  F3 = F1;
 */
	float* F1 = (float *) malloc(m*n*sizeof(float));
	float* F2 = (float *) malloc(m*n*sizeof(float));
	float* F3 = (float *) malloc(m*n*sizeof(float));
	
	//sum(Y(:))
	float Ysum = 0;
	for(int i=0; i < m; i++){
		for(int j=0; j < n; j++){

			Ysum += *(Y + i*n + j);

		}
	}

	for(int i=0; i < m; i++){
		for(int j=0; j < n; j++){

			*(F1 + i*n + j) = totalSum * (*(Y + i*n + j) / Ysum);
			*(F2 + i*n + j) = *(F3 + i*n + j) = *(F1 + i*n + j);

		}
	}
/*	printf("F1\n");
	printMatrix(m, n, F1);
	printf("F2\n");
	printMatrix(m, n, F2);
	printf("F3\n");
	printMatrix(m, n, F3);
*/
	float* H1 = (float *) malloc(m*n*sizeof(float));
	float* H2 = (float *) malloc(m*n*sizeof(float));
	float* H3 = (float *) malloc(m*n*sizeof(float));

//allocate all the memory needed for the for loop
	float* F3eps = (float *) malloc(m*n*sizeof(float));
	float* YdivF3eps = (float *) malloc(m*n*sizeof(float));
	float* H1t = (float *) malloc(m*n*sizeof(float));
	float* negH1t = (float *) malloc(m*n*sizeof(float));
	float* Yt = (float *) malloc(m*n*sizeof(float));
	float* maxRowSumT = (float *) malloc(m*sizeof(float));
	float* F1t = (float *) malloc(m*n*sizeof(float));
	float* F1eps = (float *) malloc(m*n*sizeof(float));
	float* YdivF1eps = (float *) malloc(m*n*sizeof(float));
	float* negH2 = (float *) malloc(m*n*sizeof(float));
	float* F2eps = (float *) malloc(m*n*sizeof(float));
	float* YdivF2eps = (float *) malloc(m*n*sizeof(float));
	float* X = (float *) malloc(m*n*sizeof(float));
	float* negH3 = (float *) malloc(m*n*sizeof(float));
	float* Yv = (float *) malloc(m*n*sizeof(float));
	float* Xp = (float *) malloc(m*n*sizeof(float));
	float* Fdiff1 = (float *) malloc(m*n*sizeof(float));
	float* Fdiff2 = (float *) malloc(m*n*sizeof(float));

//for t = 1 : maxLoops
	for(int t=0; t < 10; t++) {

/*		if(t < 3){
			printf("t is now %d\n", t);
			printf("F1\n");
			printMatrix(m, n, F1);
			printf("F2\n");
			printMatrix(m, n, F2);
			printf("F3\n");
			printMatrix(m, n, F3);
		} 
*/
// Max row sum 
		// H1 = lambda1 - (Y ./ (F3+eps));
		
		matPlusScaler(m, n, F3, EPS, F3eps);
		matDiv(m, n, Y, F3eps, YdivF3eps);
		matSub(m, n, lambda1, YdivF3eps, H1);

/*		if(t < 3){
			printf("H1 is\n");
			printMatrix(m, n, H1);
		}

		if(t < 3){
			printf("F1 before maxColSumP is now\n");
			printMatrix(m, n, F1);
		}
*/		
		// F1 = maxColSumP (Y', -H1', maxRowSum', precision)';
		//-H1'
		transpose(m, n, H1, H1t);
		matTimesScaler(n, m, H1t, -1, negH1t);
		//Y'
		transpose(m, n, Y, Yt);
		//maxRowSum'	
		transpose(m, 1, maxRowSum, maxRowSumT);
		//maxColSumP (Y', -H1', maxRowSum', precision)'
		maxColSumP(n, m, Yt, negH1t, maxRowSumT, EPS, F1t);
		//F1
		transpose(n,m,F1t,F1);

/*		if(t < 3){
			printf("Yt\n");
			printMatrix(m, n, Yt);
			printf("negH1t\n");
			printMatrix(n, m, negH1t);
			printf("maxRowSumT\n");
			printMatrix(m, 1, maxRowSumT);
			printf("F1t is now \n");
			printMatrix(n, m, F1t);
			printf("F1 after maxColSumP is now\n");
			printMatrix(m, n, F1);
		}
*/
		// lambda1 = lambda1 - (Y ./ (F3+eps)) + (Y ./ (F1+eps));
		matPlusScaler(m, n, F1, EPS, F1eps);
		matDiv(m, n, Y, F1eps, YdivF1eps);
		matSub(m, n, lambda1, YdivF3eps, lambda1);
		matAdd(m, n, lambda1, YdivF1eps, lambda1);
/*		if(t < 3){
			printf("F1\n");
			printMatrix(m, n, F1);
			printf("YdivF3eps\n");
			printMatrix(m, n, YdivF3eps);
			printf("YdivF1eps\n");
			printMatrix(m, n, YdivF1eps);
			printf("lambda1\n");
			printMatrix(m, n, lambda1);
		}
*/
// Max col sum 
		// H2 = lambda2 - (Y ./ (F1+eps));
		matPlusScaler( m, n, F1, EPS, F1eps);
//		matDiv(m, n, Y, F1eps, YdivF1eps);
		matSub(m, n, lambda2, YdivF1eps, H2);

		// F2 = maxColSumP (Y, -H2, maxColSum, precision);	
		matTimesScaler( m, n, H2, -1, negH2);
		maxColSumP(n, m, Y, negH2, maxColSum, precision, F2);
	
		// lambda2 = lambda2 - (Y ./ (F1+eps)) + (Y ./ (F2+eps));		
		matPlusScaler( m, n, F2, EPS, F2eps);		 
		matDiv(m, n, Y, F2eps, YdivF2eps);
		matSub(m, n, lambda2, YdivF1eps, lambda2);
		matAdd(m, n, lambda2, YdivF2eps, lambda2);

// Total sum
		// H3 = lambda3 - (Y ./ (F2 + eps));
		matSub(m, n, lambda3, YdivF2eps, H3);
		
		// F3 = reshape( exactTotalSum (Y(:), -H3(:), totalSum, precision), size(Y));

		for(int i=0; i<n; i++){ 
			for(int j=0; j < m; j++){
				*(Yv + i*m + j) = *(Y + j*n + i);
				*(negH3 + m*i + j) = -(*(H3 + j*n + i));
			}
		}

		exactTotalSum(m*n, Yv, negH3, totalSum, precision, X);
		
		for(int i=0; i<n; i++){
			for(int j=0; j < m; j++){
				*(Xp + n*i + j) = *(X + n*i + j);
			}
		}

		reshape(m*n, 1, Xp, m, n, F3);
		
		//lambda3 = lambda3 - (Y ./ (F2+eps)) + (Y ./ (F3+eps));
		matPlusScaler(m, n, F3, EPS, F3eps);
		matDiv(m, n, Y, F3eps, YdivF3eps);
		matSub(m, n, lambda3, YdivF2eps, lambda3);
		matAdd(m, n, lambda3, YdivF3eps, lambda3);	

/*		if(t < 3){
			printf("lambda1\n");
			printMatrix(m, n, lambda1);
			printf("lambda2\n");
			printMatrix(m, n, lambda2);
			printf("lambda3\n");
			printMatrix(m, n, lambda3);
		}
*/	
		//if (max(abs(F1(:) - F2(:))) < precision && max(abs(F1(:) - F3(:))) < precision)

		for(int i=0; i < m; i++){
			for(int j=0; j < n; j++){
				*(Fdiff1 + i*n + j) = fabs(*(F1 + i*n + j) - *(F2 + i*n +j));
				*(Fdiff2 + i*n + j) = fabs(*(F1 + i*n + j) - *(F3 + i*n + j));
			}
		}
		
		float fdMax1 = *(Fdiff1);
		float fdMax2 = *(Fdiff2);
		for(int k=0; k < m; k++){
			for(int l=0; l < n; l++){
				fdMax1 = (*(Fdiff1 + k*n + l) > fdMax1) ? *(Fdiff1 + k*n + l) : fdMax1;
				fdMax2 = (*(Fdiff2 + k*n + l) > fdMax2) ? *(Fdiff2 + k*n + l) : fdMax2;
			}
		}

		if(fdMax1 < precision && fdMax2 < precision){
			break;
		}

	} // end of t for loop

	// F = (F1 + F2 + F3) / 3;
//	printf("F1 F2 and F3\n");
//	printMatrix(m, n, F1);
//	printMatrix(m, n, F2);
//	printMatrix(m, n, F3);
	matAdd(m, n, F1, F2, F);
	matAdd(m, n, F, F3, F);
	float* Fdiv = (float *) malloc(m*n*sizeof(float));
	ones(m, n, Fdiv);
	matTimesScaler(m, n, Fdiv, 3, Fdiv);
	matDiv(m, n, F, Fdiv, F);
//	printf("F matrix\n");
//	printMatrix(m, n, F);

	free(lambda1);
	free(lambda2);
	free(lambda3);
	free(F1);
	free(F2);
	free(F3);
	free(H1);
	free(H2);
	free(H3);
	free(F3eps);
	free(YdivF3eps);
	free(H1t);
	free(negH1t);
	free(Yt);
	free(maxRowSumT);
	free(F1t);
	free(F1eps);
	free(YdivF1eps);
	free(negH2);
	free(F2eps);
	free(YdivF2eps);
	free(X);
	free(negH3);
	free(Yv);
	free(Xp);
	free(Fdiff1);
	free(Fdiff2);
	free(Fdiv);
	
} //end of function

