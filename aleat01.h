    /********************************************************************
    * Nombre     : ALEAT01.H 											*
    * Funci¢n    : Variables aleatorias. 								*
    * Par metros : -- 													*
    * Compilador : Turbo C++ v1.                                  	  	*
    * Hecho por  : Jos‚ Rafael Sosa Brice¤o.                         	*
    * Fecha      : Junio de 1994. 										*
    * Notas      : Implementa varias generadoras de numeros aleatorios 	*
    ********************************************************************/


#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)

static long idum3;

int bernoulli( float );
int uniformei( int, int );
float uniformef( float, float );
long int exponencial( float );
int triangular( int, int, int );
float unif01();


int bernoulli( float p )
{
	if( unif01() < p )
    	return 1;
    else
    	return 0;
}


int uniformei( int a, int b )
{
   int x = (int)( floor(unif01()*(b-a+1) + a) );
   if( x > b )
   		return b;
   else
   		return x;
}


float uniformef( float a, float b )
{
   return  ( unif01()*(b-a) + a );
}


long int exponencial(float k)
{
	float x;
    x = uniformef(0.0,10000.0);
    x = x * 0.0001;
    if (x < 0.9999)
    	return (long)(-(log(1-x)/k));
    else
    	return 10000;
};


int triangular(int min, int mod, int max)
{
 	float x,a,b,c;
    a = (float)min;
    b = (float)max;
    c = (float)mod;
    x = uniformef(0.0,10000.0);
    x = x * 0.0001;
    if (x <= (c-a)/(b-a))
    	return (int)(a+sqrt(x*(b-a)*(c-a)));
    else
        return (int)(b-sqrt(pow(c-b,2)+(c-b)*(x*(b-a)-(c-a))));
};


void inirandom(long cemilla)
{
	cemilla=1234;
	idum3 = cemilla;
    //randomize();
}


float unif01()
{
	

	return ((float)rand()/RAND_MAX);
}


float rand3()
{
	static int inext,inextp;
	static long ma[56];
	static int iff=0;
	long mj,mk;
	int i,ii,k;

	if (idum3 < 0 || iff == 0) {
		iff=1;
		mj=MSEED-(idum3 < 0 ? -idum3 : idum3);
		mj %= MBIG;
		ma[55]=mj;
		mk=1;
		for (i=1;i<=54;i++) {
			ii=(21*i) % 55;
			ma[ii]=mk;
			mk=mj-mk;
			if (mk < MZ) mk += MBIG;
			mj=ma[ii];
		}
		for (k=1;k<=4;k++)
			for (i=1;i<=55;i++) {
				ma[i] -= ma[1+(i+30) % 55];
				if (ma[i] < MZ) ma[i] += MBIG;
			}
		inext=0;
		inextp=31;
		idum3=1;
	}
	if (++inext == 56) inext=1;
	if (++inextp == 56) inextp=1;
	mj=ma[inext]-ma[inextp];
	if (mj < MZ) mj += MBIG;
	ma[inext]=mj;
	return mj*FAC;
}
#undef MBIG
#undef MSEED
#undef MZ
#undef FAC


