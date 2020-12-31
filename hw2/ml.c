/* 
 * 
 * This code calculates the house price of a house by learing from
 * training data. It uses pseudo inverse of a given matrix to find the 
 * weight of different features.
 * 
 * Predicted Price : Y = W0 + W1*x1 + W2*X2 + W3*X3 + W4*X4
 * Weight Matrix : W = pseudoInv(X)*Y
 * pseudoInv(X) = inverse(transpose(X)*X) * transpose(X)  
 * 
 * weight(w) = pseudoInv(X) * Y
 * 			where	X = Input data matrix
 * 					Y = Target vector
 * 
 */
 
#include<stdio.h>
#include<stdlib.h>

// all methods declarations
double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2);
double** transposeMatrix(double** mat, int row, int col);
double** inverseMatrix(double **matA, int dimension);

// main method starts here
int main(int argc, char** argv){

   	FILE *fp=fopen(argv[1],"r");
   	int row;
   	int col;
   	fscanf(fp, "%d\n", &col);
   	col+=1;
   	fscanf(fp, "%d\n", &row);
	double **X=malloc(sizeof(double*)*row);
	for(int i=0; i < row; i++)
	{
		X[i]=malloc(sizeof(double)*col);
	}
	double **Y=malloc(sizeof(double*)*row);
	for(int j=0; j<row;j++)
	{
		Y[j]=malloc(sizeof(double));
	}
	for(int ones=0;ones<row;ones++)
	{
		X[ones][0]=1;
	}
	double price;
	
	for(int x = 0 ; x < row ; x++) 
	{
       		 for(int y = 1 ; y < col+1; y++)
       		 {	
       		 	if(y==col)
       		 	{
       		 		if((fscanf(fp,"%lf%*c*", &price)!=EOF))
           			{
           				Y[x][0]=price;
           			} 
       		 	}
       		 	else if((fscanf(fp,"%lf%*c*", &price)!=EOF))
       		 	{
       		 		X[x][y]=price;
       		 	}		
           	 }
        }
	fclose(fp);
	
	FILE *pf=fopen(argv[2],"r");
	int input;
	fscanf(pf,"%d\n", &input);
	double **X2=malloc(sizeof(double*)*input);
	for(int u=0; u<input;u++)
	{
		X2[u]=malloc(sizeof(double)*col);
	}
	for(int p=0;p<input;p++)
	{
		X2[p][0]=1;
	}
	for(int x = 0 ; x < input; x++) 
	{
       		 for(int y = 1 ; y < col; y++)
       		 {
       		 	if((fscanf(pf,"%lf%*c*", &price)!=EOF))
       		 	{
       		 		X2[x][y]=price;
       		 	}		
           	 }
        }
        
        
	fclose(pf);
	double **tranMatrix;
	tranMatrix=transposeMatrix(X,row,col);
	double **productMat;
	productMat=multiplyMatrix(tranMatrix,X,col, row, row, col);
	double **invMatrix;
	invMatrix=inverseMatrix(productMat,col);
	double **psuedoInv;
	psuedoInv=multiplyMatrix(invMatrix,tranMatrix, col, col, col, row);
	double **weight;
	weight=multiplyMatrix(psuedoInv,Y, col, row, row, 1);
	double **final;
	final=multiplyMatrix(X2,weight, input, col, col, 1);
	
	for(int i=0; i<input;i++)
	{
		printf("%0.0lf\n",final[i][0]);
	}
	 
	for(int i=0; i<col; i++)
	{
		free(tranMatrix[i]);
		free(productMat[i]);
		free(invMatrix[i]);
		free(psuedoInv[i]);
		free(weight[i]);
	}
	for(int i=0; i<row; i++)
	{
		free(X[i]);
		free(Y[i]);
	}
	for(int i=0; i<input; i++)
	{
		free(X2[i]);
		free(final[i]);
	}
	
	free(tranMatrix);
	free(X);
	free(Y);
	free(productMat);
	free(invMatrix);
	free(psuedoInv);
	free(weight);
	free(X2);
	free(final);
	return 0;
	
}

double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2)
{
    double** result=malloc(r1*sizeof(double*));
    double total=0;
    for(int i=0; i < r1; i++)
    {
  	  result[i]=malloc(sizeof(double)*c2);
    }
	
    for(int a=0;a<r1;a++)
    {
    	for(int b=0; b<c2; b++)
    	{
    		total=0;
    		for(int c=0; c<c1;c++)
    		{
    			total=total + matA[a][c]*matB[c][b];
    		}
    		result[a][b]=total;
    	}
    }
    
    return result;
}


double** transposeMatrix(double** mat, int row, int col)
{
  
	double** matTran=malloc(col*sizeof(double*)); 
	for(int i=0; i < col; i++)
	{
		matTran[i]=malloc(sizeof(double)*row);
	}
	for(int j=0; j<row;j++)
	{
		for(int k=0; k<col; k++)
		{
			matTran[k][j]=mat[j][k];
		}
	}
    return matTran;        
}


double** inverseMatrix(double **matA, int dimension)
{

    double** matI=malloc(dimension*sizeof(double*)); 
    for(int i=0; i<dimension; i++)
    {
    	matI[i]=malloc(sizeof(double*)*dimension);
    }
   double f=0;
   for(int x = 0 ; x < dimension ; x++) 
   {
   	for(int y = 0 ; y < dimension; y++)
   	{
   		if(x==y)
   		{
   			matI[x][y]=1;
   		}
   		else
   		{
   			matI[x][y]=0;
   		}
   	}
   }
   for(int p=0;p<dimension;p++)
   {
   	f=matA[p][p];
   	for(int row=0;row<dimension;row++)
   	{
   		matA[p][row]=matA[p][row]/f;
   		matI[p][row]=matI[p][row]/f;
   	}
   	for(int i=p+1; i<dimension;i++)
   	{
   		f=matA[i][p];
   		for(int j=0; j<dimension; j++)
   		{
   			matA[i][j]=matA[i][j] - matA[p][j]*f;
   			matI[i][j]=matI[i][j] - matI[p][j]*f;
   		}
   	}
   }
   for(int p=dimension-1; p>=0; p--)
   {
   	for(int i=p-1; i>=0;i--)
   	{
   		f=matA[i][p];
   		for(int o=0; o<dimension; o++)
   		{
   			matA[i][o]=matA[i][o] - matA[p][o]*f;
   			matI[i][o]=matI[i][o] - matI[p][o]*f;
   		}
   	}
   }
    
	return matI;
}


