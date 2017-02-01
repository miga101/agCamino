	/********************************************************************
	* Nombre     : ARCDAT01.HPP 										*
	* Funci¢n    : Operaciones de lectura y escritura de tablas de datos*
	* Par metros : -- 												   	*
	* Compilador : Turbo C++ v1.                                  	  	*
	* Hecho por  : Jos‚ Rafael Sosa Brice¤o.                         	*
	* Fecha      : Noviembre de	1994 									*
	* Notas      : Permite leer y escribir en archivos tablas de long	*
	*			   variable de datos de dimension variable usando la	*
	*			   clase Vector<Vector<float>>							*
	********************************************************************/

#ifndef __ARCDAT01_HPP
#define __ARCDAT01_HPP 1

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "vector02.hpp"

int LeerSerie( char*, VectorF& );
int EscribirSerie( char*, VectorF& );
int LeerData( char*, Vector<VectorF>& );
int EscribirData( char*, Vector<VectorF>& );
int LeerDataXYZ( char*, float**, float**, float**, int* );
int EscribirDataXYZ( char*, float*, float*, float*, int );
int LeerDataXY( char*, float**, float**, int* );
int EscribirDataXY( char*, float*, float*, int );
int LeerDataX( char*, float**, int* );
int EscribirDataX( char*, float*, int );


int LeerSerie( char *nombre, VectorF &serie )
{
	int i,ndat,nvar;
	float f;
	FILE *fp;
	fp = fopen(nombre,"r");

	if( fp!=NULL ){
		fscanf(fp," %d %d ",&ndat,&nvar);
		serie.dimencionar(0,ndat*nvar-1);
		for( i=0; i<ndat*nvar; i++ ){
			fscanf(fp," %f ",&f);
			serie(i) = f;
		}
		return 0;
	}
	else{
		ErrorMg(2,"\nArchivo no encontrado...");
		return 1;
	}
}

int EscribirSerie( char *nombre, VectorF &serie )
{
	int i,ndat;
	FILE *fp;
	fp = fopen(nombre,"w");
	if( fp!=NULL ){
		ndat = serie.Dim();
		fprintf(fp," %d 1 \n",ndat);
		for( i=serie.Ini(); i<=serie.Fin(); i++ ){
			fprintf(fp," %f \n",serie(i));
		}
		return 0;
	}
	else{
		ErrorMg(2,"\nArchivo no pudo ser abierto...");
		return 1;
	}
}

int LeerData( char *nombre, Vector<VectorF> &tabla )
{
	int i,j,ndat,nvar;
	float f;
	FILE *fp;
	fp = fopen(nombre,"r");

	if( fp!=NULL ){
		fscanf(fp," %d %d ",&ndat,&nvar);
		tabla.dimencionar(0,ndat-1);
		for( i=0; i<ndat; i++ ){
			tabla(i).dimencionar(0,nvar-1);
			for( j=0; j<nvar; j++ ){
				fscanf(fp," %f ",&f);
				tabla(i)(j) = f;
			}
		}
		return 0;
	}
	else{
		ErrorMg(2,"\nArchivo no encontrado...");
		return 1;
	}
}

int EscribirData( char *nombre, Vector<VectorF> &tabla )
{
	int i,j,ndat,nvar;
	FILE *fp;
	fp = fopen(nombre,"w");
	if( fp!=NULL ){
		ndat = tabla.Dim();
		nvar = tabla(tabla.Ini()).Dim();
		fprintf(fp," %d %d \n",ndat,nvar);
		for( i=tabla.Ini(); i<=tabla.Fin(); i++ ){
			for( j=tabla(tabla.Ini()).Ini(); j<=tabla(tabla.Ini()).Fin(); j++)
				fprintf(fp," %f ",tabla(i)(j));
			fprintf(fp,"\n");
		}
		return 0;
	}
	else{
		ErrorMg(2,"\nArchivo no pudo ser abierto...");
		return 1;
	}
}

int LeerDataXYZ( char *nombre, float **X, float **Y, float **Z, int *ndat )
{
	int i, nvar;
	FILE *fp;
	fp = fopen(nombre,"r");

	if( fp!=NULL ){
		fscanf(fp," %d %d ",ndat,&nvar);
		*X = (float*)malloc((size_t)(*ndat)*sizeof(float));
		*Y = (float*)malloc((size_t)(*ndat)*sizeof(float));
		*Z = (float*)malloc((size_t)(*ndat)*sizeof(float));
		if( (*X!=NULL) && (*Y!=NULL) && (*Z!=NULL) ){
			for( i=0; i<*ndat; i++ )
				fscanf(fp," %f %f %f ",(*X+i),(*Y+i),(*Z+i));
			return 0;
		}
		else{
			ErrorMg(1,"ERROR: Memoria insuficiente para creacion de arreglo");
			return 1;
		}
	}
	else{
		ErrorMg(2,"\nArchivo no encontrado...");
		return 1;
	}
}

int EscribirDataXYZ( char *nombre, float X[], float Y[], float Z[], int ndat )
{
	int i;
	FILE *fp;
	fp = fopen(nombre,"w");
	if( fp!=NULL ){
		fprintf(fp," %d 3 \n",ndat);
		for( i=0; i<ndat; i++ )
			fprintf(fp," %f %f %f \n",X[i],Y[i],Z[i]);
		return 0;
	}
	else{
		Error(2,"\nArchivo no pudo ser abierto...");
		return 1;
	}
}

int LeerDataXY( char *nombre, float **X, float **Y, int *ndat )
{
	int i,nvar;
	FILE *fp;
	fp = fopen(nombre,"r");

	if( fp!=NULL ){
		fscanf(fp," %d %d ",ndat,&nvar);
		*X = (float*)malloc((size_t)(*ndat)*sizeof(float));
		*Y = (float*)malloc((size_t)(*ndat)*sizeof(float));
		if( (*X!=NULL) && (*Y!=NULL) ){
			for( i=0; i<*ndat; i++ )
				fscanf(fp," %f %f ",(*X+i),(*Y+i));
			return 0;
		}
		else{
			ErrorMg(1,"ERROR: Memoria insuficiente para creacion de arreglo");
            return 1;
		}
	}
	else{
		ErrorMg(2,"\nArchivo no encontrado...");
		return 1;
	}
}

int EscribirDataXY( char *nombre, float X[], float Y[], int ndat )
{
	int i;
	FILE *fp;
	fp = fopen(nombre,"w");
	if( fp!=NULL ){
		fprintf(fp," %d 2 \n",ndat);
		for( i=0; i<ndat; i++ )
			fprintf(fp," %f %f \n",X[i],Y[i]);
		return 0;
	}
	else{
		ErrorMg(2,"\nArchivo no pudo ser abierto...");
		return 1;
	}
}

int LeerDataX( char *nombre, float **X, int *ndat )
{
	int i,nvar;
	FILE *fp;
	fp = fopen(nombre,"r");
	if( fp!=NULL ){
		fscanf(fp," %d %d ",ndat,&nvar);
		*X = (float*)malloc((size_t)(*ndat)*sizeof(float));
		if( *X != NULL ){
			for( i=0; i<*ndat; i++ )
				fscanf(fp," %f ",(*X+i));
			return 0;
		}
		else{
			ErrorMg(1,"ERROR: Memoria insuficiente para creacion de arreglo");
			return 1;
		}
	}
	else{
		ErrorMg(2,"\nArchivo no encontrado...");
		return 1;
	}
}

int EscribirDataX( char *nombre, float X[], int ndat )
{
	int i;
	FILE *fp;
	fp = fopen(nombre,"w");
	if( fp!=NULL ){
		fprintf(fp," %d 1 \n",ndat);
		for( i=0; i<ndat; i++ )
			fprintf(fp," %f \n",X[i]);
		return 0;
	}
	else{
		ErrorMg(2,"\nArchivo no pudo ser abierto...");
		return 1;
	}
}

#endif