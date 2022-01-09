#include<stdio.h>
#include<math.h>
#include<omp.h>
#include <stdlib.h>
#include<time.h>

float float_rand( float min, float max )
{
    float scale = rand() / (float) RAND_MAX; 
    return min + scale * ( max - min );     
}


float *coefficient,*powers;
int terms;
float f(float x)
{
	float y=0;
	int i;
	for(i=0 ; i<terms; i++)
	{
	
		y += (coefficient[i]*pow(x,powers[i]));
	}
	
	return y;
}


void Serial(float a,float b,float maxima)
{
	double i,p = 0,n = 100000000;// p (under the fucntion or in the area) n->(total points generated)
	double Cord_X,Cord_Y;

	
	for(i = 0 ; i<n ; i++)
	{
		Cord_X = float_rand(a,b);
		Cord_Y = float_rand(0,maxima);
		
		float y = f(Cord_X);
		
		if(Cord_Y < y)
		{
			p++;
		}
	}
	
	double areaOfTheBox = (b-a)*maxima;
	
	double  area = (p/n) * areaOfTheBox;
	
	printf("Area serial %f\n",area);
	
}
void parallel(float a,float b,float maxima)
{
	long i;
	double p = 0;
	long n = 100000000;// p (under the fucntion or in the area) n->(total points generated)
	double Cord_X,Cord_Y;
	double Y;

	double sum = 0;
	#pragma omp num_threads(10) private(Y,Cord_X,Cord_Y) shared(p)
	{
	
	
		#pragma parallel for 
		for(i = 0 ; i<n ; i++)
		{
			Cord_X = float_rand(a,b);
			Cord_Y = float_rand(0,maxima);
			
			Y = f(Cord_X);
		
				

			if(Cord_Y < Y)
			{
				p++;
			}
			
		}
		
		sum += p;
		#pragma omp barrier
	}
	
	double areaOfTheBox = (b-a)*maxima;
	
	double  area = (p/(double)n) * areaOfTheBox;
	
	printf("Area parallel %f\n",area);
	
}
void MCIntegration()
{
	srand(time(0));//providing seed for the random numbers
	int i;
	
	printf("Enter the number of terms of the polynomial:");
	scanf("%d",&terms);
	coefficient = (float*)malloc(terms * sizeof(coefficient));
	powers = (float*)malloc(terms * sizeof(*powers));

	
	printf("%d",terms);
	for(i= 0 ;i<terms ; i++)
	{
		printf("Enter the %d term cofficent and power of x:",i+1);
		scanf("%f",&coefficient[i]);
		scanf("%f",&powers[i]);
	}
	
	printf("The equation : ");
	for(i = 0 ; i<terms ;i++)
		printf("%gx^(%g) ",coefficient[i],powers[i]);
	
	float a,b;
	float maxima;

	
	

	
	printf("\nEnter the  integral  lower (a)  and upper (b) lime:");
	scanf("%f",&a);
	scanf("%f",&b);
	
	printf("Enter the maximum of the fuction within the given range(a,b)");
	scanf("%f",&maxima);
	
	
	clock_t begin = clock();
	{
		parallel(a,b,maxima);
		
	}
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	
	printf("Parallel execution time:%f \n",time_spent);
	
	begin = clock();
	{
		Serial(a,b,maxima);
		
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	printf("Serial execution time:%f \n",time_spent);

}

int main() 
{ 

	MCIntegration();
	

	return 0;
}
