	/********************************************************************
	* Nombre     : ERROR01.HPP											*
	* Funci¢n    : Rutinas y variables sobre errores de programa. 		*
	* Par metros : -- 													*
	* Compilador : Turbo C++ v1.                                  	  	*
	* Hecho por  : Jos‚ Rafael Sosa Brice¤o.                         	*
	* Fecha      : Enero de 1995. 										*
	* Notas      : Rutinas para ser usadas en todos los programas, per-	*
	*			   mite monitorear los errores cuando se usan muchas	*
	*			   librerias. Esta libreria permite mayor facilidad en 	*
	*			   control de los errores sin la necesidad de abortar.	*
	********************************************************************/

#ifndef __ERROR01_HPP
#define __ERROR01_HPP 1

#include <stdio.h>
#include <conio.h>
#include <string.h>

typedef enum {  NoError,
				NoMemoria,
				Noarchivo
			 } tipo_error;

char MgError[80];
tipo_error TipError;

void Error( int, char* );
void ErrorMg( int, char* );

void Error( int tip, char *mg )
{
	TipError = (tipo_error)tip;
	strcpy(MgError,mg);
}

void ErrorMg( int tip, char *mg )
{
	TipError = (tipo_error)tip;
	strcpy(MgError,mg);
	printf(mg);
	getche();
}

#endif









