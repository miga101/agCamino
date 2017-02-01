	/********************************************************************
	* Nombre     : GENET05.HPP 											*
	* Funci¢n    : Clase Algoritmo genetico general. 							*
	* Par metros : -- 													*
	* Compilador : Boland C++ v3.1.                                  	*
	* Hecho por  : Jos‚ Rafael Sosa Brice¤o.                         	*
	* Fecha      : mayo de 1994. 										*
	* Notas      : Libreria de operaciones basicas de algoritmos genet. *
	* Utiliza una codificacion genetica binaria, de forma que cada 		*
	* cromosoma se representa con un vector de enteros sin signo.		*
	* Se declara una instancia del algortmo genetico llamada AG, que es	*
	* el que debe ser usado en las aplicaciones. Los Operadores genet.	*
	* usados son la Mutacion, Cruce en uno y dos puntos y el Cruce unif.*						*
	********************************************************************/

#include <stdio.h>
#include <conio.h>
#include "aleat01.h"
#include "vector02.hpp"


class AGBinario {
	public:
		Vector<VectorU> Pob,Prole;// poblacion y prole
		VectorU Ord;					// ordenamiento indexado de la ploblacion
		VectorF Adapt, AdaptNorm;		// calidad real y normalizada de los individuos
		unsigned NumPob;				// tamano de la poblacion
		unsigned NumProle; 				// proporcion de poblacion a sustituir
		unsigned NumgCrom;				// numero de genes por cromosoma
		float (*FuncAdapt)(VectorU&); 	// funcion de adaptacion
		int (*CondSalida)(void);		// condicion de salida
		void (*FuncMostrar)(int);		// mostrar el estado del AG en cada iter
		long MaxIter;				// numero maximo de iteraciones
		long PasoMost;				// salto entre cada ejecucion de FuncMostrar
		int Repetidos;				// indica si se desean individuos repetidos o no
		int NumOper;				// numero de operadores geneticos
		VectorF ProbOper;  			// probabilidades de los operadores geneticos
		float Pm,Pcu;               // probabilidad de mutacion y cruce de un gen
		int NBITS;					// numero de bits por entero

		AGBinario();
		void asignar_NumPob( unsigned );
		void asignar_NumProle( unsigned );
		void asignar_NumgCrom( unsigned );
		void asignar_FuncAdapt( float (*)(VectorU&) );
		void asignar_CondSalida( int (*)(void) );
		void asignar_FuncMostrar( void (*)(int) );
		void asignar_MaxIter( long );
		void asignar_PasoMost( long );
		void asignar_Repetidos( int );
		void asignar_ProbOper( int, float );
		void asignar_Pm( float );
		void asignar_Pcu( float );

		VectorU& Poblacion( int );
		float& Adaptacion( int );

		void select_indiv( int, unsigned&, unsigned& );
		int select_oper();
		void mutar( VectorU&, VectorU& );
		void cruce_unif( VectorU&, VectorU&, VectorU&, VectorU& );
		int pos_corte( int );
		void cruce_un_punto( VectorU&, VectorU&, VectorU&, VectorU& );
		void cruce_dos_punto( VectorU&, VectorU&, VectorU&, VectorU& );
		void init_poblacion();
		void eval_poblacion( int );
		void renormalizar_poblacion();
		friend int rel_ord( const void *elem1, const void *elem2 );
		void ordenar_poblacion();
		void ejecutar();
};

AGBinario AG;

AGBinario::AGBinario()
{
	int i;
	NumPob = 40;
	NumProle = 10;
	NumgCrom = 75;//(L(mec)**2)*3 tripletas de sentido,= 5*5*3
	Pob.dimencionar(0,NumPob-1);
	Ord.dimencionar(0,NumPob-1);
	Adapt.dimencionar(0,NumPob-1);
	AdaptNorm.dimencionar(0,NumPob-1);
	Prole.dimencionar(0,NumProle);
	for( i=0; i<NumPob; i++ ){
		Ord[i] = i;
		Pob[i].dimencionar(0,NumgCrom-1);
	}
	for( i=0; i<=NumProle; i++ ){
		Prole[i].dimencionar(0,NumgCrom-1);
	}
	FuncAdapt = NULL;
	CondSalida = NULL;
	FuncMostrar = NULL;
	MaxIter = 50;
	PasoMost = 1;
	Repetidos = 1;
	NumOper = 4;
	ProbOper.dimencionar(0,NumOper-1);
	ProbOper[0]=0.05;
	ProbOper[2]=0.2;
	ProbOper[3]=0.2;
	ProbOper[1] = 1 - ProbOper[0] - ProbOper[2] - ProbOper[3];
	Pm=0.05;
	Pcu=0.5;
	NBITS = sizeof(unsigned char);
}

void AGBinario::asignar_NumPob( unsigned n )
{
	int i;
	NumPob = n;
	Pob.dimencionar(0,NumPob-1);
	Ord.dimencionar(0,NumPob-1);
	Adapt.dimencionar(0,NumPob-1);
	AdaptNorm.dimencionar(0,NumPob-1);
	for( i=0; i<NumPob; i++ ){
		Ord[i] = i;
		Pob[i].dimencionar(0,NumgCrom-1);
	}
}

void AGBinario::asignar_NumProle( unsigned n )
{
	int i;
	NumProle = n;
	Prole.dimencionar(0,NumProle);
	for( i=0; i<=NumProle; i++ ){
		Prole[i].dimencionar(0,NumgCrom-1);
	}
}

void AGBinario::asignar_NumgCrom( unsigned n )
{
	int i;
	NumgCrom = n;
	for( i=0; i<NumPob; i++ ){
		Pob[i].dimencionar(0,NumgCrom-1);
	}
	for( i=0; i<=NumProle; i++ ){
		Prole[i].dimencionar(0,NumgCrom-1);
	}
}

void AGBinario::asignar_FuncAdapt( float (*Func)(VectorU&) )
{
	FuncAdapt = Func;
}

void AGBinario::asignar_CondSalida( int (*Func)(void) )
{
	CondSalida = Func;
}

void AGBinario::asignar_FuncMostrar( void (*Func)(int) )
{
	FuncMostrar = Func;
}

void AGBinario::asignar_MaxIter( long n )
{
	MaxIter = n;
}

void AGBinario::asignar_PasoMost( long n )
{
	PasoMost = n;
}

void AGBinario::asignar_Repetidos( int r )
{
	Repetidos = r?1:0;
}

void AGBinario::asignar_ProbOper( int i, float p )
{
	if( (p>=0) && (p<1) ){
		switch( i ){
			case 0: ProbOper[0] = p;
					ProbOper[1] = 1 - ProbOper[0] - ProbOper[2] - ProbOper[3];
					break;
			case 1: ProbOper[1] = p;
					ProbOper[2] = 1 - ProbOper[0] - ProbOper[1] - ProbOper[3];
					break;
			case 2: ProbOper[2] = p;
					ProbOper[1] = 1 - ProbOper[0] - ProbOper[2] - ProbOper[3];
					break;
			case 3: ProbOper[3] = p;
					ProbOper[1] = 1 - ProbOper[0] - ProbOper[2] - ProbOper[3];
					break;
		}
	}
}

void AGBinario::asignar_Pm( float p )
{
	if( (p>=0) && (p<1) )
		Pm = p;
}

void AGBinario::asignar_Pcu( float p )
{
	if( (p>=0) && (p<1) )
		Pcu = p;
}

VectorU& AGBinario::Poblacion( int i )
{
	return Pob[Ord[i]];
}

float& AGBinario::Adaptacion( int i )
{
	return Adapt[Ord[i]];
}

void AGBinario::select_indiv( int n_ind, unsigned &crom1, unsigned &crom2 )
{
	float fad_total=0.0,sum,x;
	int i,j,n[2];
	for( i=0; i<NumPob; i++ )
		fad_total += AdaptNorm[i];
	i = 0;
	while( i<n_ind ) {
		sum = 0;
		x = uniformef(0.0,fad_total);
		for( j=0; j<=NumPob; j++ ){
			sum += AdaptNorm[Ord[j]];
			if(x<=sum){
				n[i] = j;
				break;
			}
		}
		if( i==1 )
			if( n[0] == n[1] )
				i--;
		i++;
	}
	crom1 = n[0];
	crom2 = n[1];
}

int AGBinario::select_oper()
{
	float sum=0.0, x;
	int i, encontro=0, oper;
	x = unif01();
	for( i=0; (i<NumOper)&&(!encontro); i++ ){
		sum+= ProbOper[i];
		if(x<=sum){
			oper = i;
			encontro = 1;
		}
	}
	return oper;
}

void AGBinario::mutar( VectorU& ini, VectorU& fin )
{
	int i,j,masc,x;
	for(j=0; j<=NumgCrom; j++ ){
		masc = 0;
		for( i=0; i<= NBITS; i++ )
			if( bernoulli(Pm) ){
				x =  1;
				masc = masc ^ (x<<i);
			}
		fin[j] = ini[j] ^ masc;
	}
}

void AGBinario::cruce_unif( VectorU& ini1, VectorU& ini2, VectorU& fin1, VectorU& fin2 )
{
	int masc, x, i, j;
	for( j=0; j<NumgCrom; j++ ) {
		for( i=0; i<=NBITS; i++ )
			if( bernoulli(Pcu) ){
				x = 1;
				masc = masc ^ (x<<i);
			}
		x = (ini1[j] ^ (ini2[j])) & masc;
		fin1[j] = ini1[j] ^ x;
		fin2[j] = ini2[j] ^ x;
	}
}

int AGBinario::pos_corte( int x )
{
	int i, encontro=0, pos;
	for( i=1; (i<=NumgCrom)&&(!encontro); i++ )
		if( i*NBITS > x ){
			pos = i;
			encontro = 1;
		}
	return pos-1;
}

void AGBinario::cruce_un_punto( VectorU& ini1, VectorU& ini2, VectorU& fin1, VectorU& fin2 )
{
	int pto, b_corte, masc, cab1, cab2, col1, col2, i;
	pto = uniformei(0,NumgCrom*NBITS-1);
	b_corte = pos_corte(pto);
	masc = (1<<((b_corte+1)*NBITS-pto))-1;
	cab1 = ini1[b_corte]&~masc;
	cab2 = ini2[b_corte]&~masc;
	col1 = ini1[b_corte]&masc;
	col2 = ini2[b_corte]&masc;
	fin1[b_corte] = cab1|col2;
	fin2[b_corte] = cab2|col1;
	for( i=0; i<NumgCrom; i++ ){
		if(i<b_corte){
			fin1[i] = ini1[i];
			fin2[i] = ini2[i];
		}
		if(i>b_corte){
			fin1[i] = ini2[i];
			fin2[i] = ini1[i];
		}
	}
}

void AGBinario::cruce_dos_punto( VectorU& ini1, VectorU& ini2, VectorU& fin1, VectorU& fin2 )
{
	int pto1, pto2, b_corte1, b_corte2, masc, cab1, cab2, col1, col2, i;
	pto1 = uniformei(0,NumgCrom*NBITS-1);
	do{
		pto2 = uniformei(0,NumgCrom*NBITS-1);
	}while( pto1==pto2 );
	if( pto1>pto2 ){
		i = pto1;
		pto1 = pto2;
		pto2 = i;
	}
	b_corte1 = pos_corte(pto1);
	masc = (1<<((b_corte1+1)*NBITS-pto1))-1;
	cab1 = ini1[b_corte1]&~masc;
	cab2 = ini2[b_corte1]&~masc;
	col1 = ini1[b_corte1]&masc;
	col2 = ini2[b_corte1]&masc;
	fin1[b_corte1] = cab1|col2;
	fin2[b_corte1] = cab2|col1;
	b_corte2 = pos_corte(pto2);
	masc = (1<<((b_corte2+1)*NBITS-pto2))-1;
	cab1 = fin1[b_corte2]&~masc;
	cab2 = fin2[b_corte2]&~masc;
	col1 = fin1[b_corte2]&masc;
	col2 = fin2[b_corte2]&masc;
	fin1[b_corte2] = cab1|col2;
	fin2[b_corte2] = cab2|col1;
	for( i=0; i<NumgCrom; i++ ){
		if( (i<b_corte1) || (i>b_corte2) ){
			fin1[i] = ini1[i];
			fin2[i] = ini2[i];
		}
		if( (i>b_corte1) && (i<b_corte2) ){
			fin1[i] = ini2[i];
			fin2[i] = ini1[i];
		}
	}
}

void AGBinario::init_poblacion()
{
	int i,j;
	for( i=0; i<NumPob; i++ ){
		Ord[i] = i;
		for( j=0; j<NumgCrom; j++)
			Pob[i][j] = bernoulli(0.5)?rand():-rand();

	}
}

void AGBinario::eval_poblacion( int pos )
{
	int i;
	for( i=pos; i<NumPob; i++ )
		Adapt[Ord[i]] = FuncAdapt( Pob[Ord[i]] );
}

void AGBinario::renormalizar_poblacion()
{
	int i;
//	float xmax,xmin,delta;
//	xmax = Adapt[Ord[0]];
//	xmin = Adapt[Ord[NumPob-1]];
//	delta = (xmax-xmin)/NumPob;
	for( i=0; i<NumPob; i++ )
		AdaptNorm[Ord[i]] = NumPob-i;
}

int rel_ord( const void *elem1, const void *elem2 )
{
	if( AG.Adapt[*((int*)elem1)] == AG.Adapt[*((int*)elem2)] )
		return 0;
	else if( AG.Adapt[*((int*)elem1)] > AG.Adapt[*((int*)elem2)] )
		return -1;
	else
		return 1;
}

void AGBinario::ordenar_poblacion()
{
	qsort((void*)Ord.DirMem(), NumPob, sizeof(unsigned), rel_ord );
}

void AGBinario::ejecutar()
{
	int op,npro,nh,ngen,k,j,igual,salir=0;
	long iter;
	unsigned pad1, pad2;

	eval_poblacion( 0 );

	for( iter=0,salir=kbhit(); (iter<MaxIter)&&(!salir); iter++ ){

		ordenar_poblacion();
		renormalizar_poblacion();

		if( CondSalida!=NULL )
			salir = CondSalida();

		if( !salir ) {
			for( npro=0; npro<NumProle; npro+=nh ){
				op = select_oper();
				switch (op) {
					case 0: nh = 1;
							select_indiv(nh,pad1,pad2);
							mutar(Pob[Ord[pad1]],Prole[npro]);
							break;
					case 1: nh = 2;
							select_indiv(nh,pad1,pad2);
							cruce_un_punto(Pob[Ord[pad1]],Pob[Ord[pad2]],Prole[npro],Prole[npro+1]);
							break;
					case 2: nh = 2;
							select_indiv(nh,pad1,pad2);
							cruce_unif(Pob[Ord[pad1]],Pob[Ord[pad2]],Prole[npro],Prole[npro+1]);
							break;
					case 3: nh = 2;
							select_indiv(nh,pad1,pad2);
							cruce_dos_punto(Pob[Ord[pad1]],Pob[Ord[pad2]],Prole[npro],Prole[npro+1]);
							break;
				}
				if( !Repetidos )
				for( k=npro+nh-1; k>=npro; k-- ){
					igual = 0;
					for( j=0; (j<NumProle)&&(!igual); j++ )
						igual = ( Pob[Ord[j]]==Prole[k] );
					for( j=0; (j<k)&&(!igual); j++ )
						igual = ( Prole[j]==Prole[k] );
					if( igual )
						nh--;
					if( !salir ) salir = kbhit();
				}
			}

			for( npro=0; npro<NumProle; npro++ )
				for( ngen=0; ngen<NumgCrom; ngen++)
					Pob[Ord[NumPob-npro-1]][ngen] = Prole[npro][ngen];

			eval_poblacion(NumPob-NumProle);
		}

		if( ((iter%PasoMost)==0) && (FuncMostrar!=NULL) )
			FuncMostrar(iter);
		if( !salir ) salir = kbhit();
	}
}
