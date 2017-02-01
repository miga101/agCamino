	/********************************************************************
	* Nombre     : PARSER02.HPP 										*
	* Funci¢n    : Rutinas que permiten evaluar expresiones matematicas.*
	* Par metros : -- 													*
	* Compilador : Turbo C++ v1.                                  	  	*
	* Hecho por  : Jos‚ Rafael Sosa Brice¤o.                         	*
	* Fecha      : Febrero de 1995. 									*
	* Notas      : Implementa unas rutinas que permiten manipular  		*
	*			   expresiones matematicas en dadas en string, de esta	*
	*			   manera permite usar expresiones matematicas introdu-	*
	*			   cidas por el usuario en programas de calculo. Usa	*
	*			   como variables las letras de la 'X' a la 'Z' pero	*
	*			   solo dos por expresion								*
	********************************************************************/

#ifndef __PARSER02_HPP
#define __PARSER02_HPP 1


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <math.h>

#define Zero	0.0000001
#define NSTFUNC 10
#define NCONST	2
#define NERRP	4



//	Tipos y Prototipos:



#define EsNumero(Ch) ((Ch>='0')&&(Ch<='9'))
#define EsVariable(Ch) ((Ch>='X')&&(Ch<='Z'))

typedef enum { TipoUnario,
			   TipoBinario,
			   TipoNumero } TipoNodo;
typedef struct { TipoNodo Tipo;
				 char BinOperador;
				 void *IzqOperando,
					  *DerOperando; } NodoBinario;
typedef struct { TipoNodo Tipo;
				 char UnOperador;
				 void *Operando; } NodoUnario;
typedef struct { TipoNodo Tipo;
				 float Operando; } NodoNumero;
typedef union { NodoBinario NodoB;
				NodoUnario NodoU;
				NodoNumero NodoN; } NodoExpr;
typedef NodoExpr *NodoExprPtr;
typedef enum { Abs,
			   Sqrt,
			   Sqr,
			   Sin,
			   Cos,
			   Arctan,
			   Ln,
			   Log,
			   Exp,
			   Fact } TipoFuncion;
char NombreFuncion[NSTFUNC][7] = { "ABS",
								   "SQRT",
								   "SQR",
								   "SIN",
								   "COS",
								   "ARCTAN",
								   "LN",
								   "LOG",
								   "EXP",
								   "FACT" };
char NombreConst[NCONST][7] = { "PI",
								"E" };
char MgErrorParser[NERRP][50] = { "No hay error",
								 "Error en formato numerico",
								 "Falta parentesis",
								 "Palabra no reconocida" };


NodoExprPtr NuevoNodoBinario( char, NodoExprPtr, NodoExprPtr );
NodoExprPtr NuevoNodoUnario( char, NodoExprPtr);
NodoExprPtr NuevoNodoNumero( float );
char CharProximo( char*, int& );
char CharAnterior( char*, int& );
NodoExprPtr Factor( char*, int&, char&, int& );
NodoExprPtr Termino( char*, int&, char&, int& );
NodoExprPtr Expresion( char*, int&, char&, int& );
NodoExprPtr ParserExpr( char*, int& );
float fact( float );
float pot( float, float );
float pot( float, int );
void EliminarExpr( NodoExprPtr );
float EvalExpr( NodoExprPtr, float, float, float );



//	Implementacion:



NodoExprPtr NuevoNodoBinario( char opr, NodoExprPtr lopr, NodoExprPtr ropr )
{
	NodoExprPtr n;
	if( (lopr==NULL) || (ropr==NULL) )
	   return NULL;
	else{
		 n = (NodoExprPtr)malloc( (size_t)sizeof(NodoExpr) );
		 n->NodoB.Tipo = TipoBinario;
		 n->NodoB.BinOperador = opr;
		 n->NodoB.IzqOperando = lopr;
		 n->NodoB.DerOperando = ropr;
		 return n;
	}
}

NodoExprPtr NuevoNodoUnario( char operad, NodoExprPtr operand )
{
	NodoExprPtr n;
	n = (NodoExprPtr)malloc( (size_t)sizeof(NodoExpr) );
	n->NodoU.Tipo = TipoUnario;
	n->NodoU.UnOperador = operad;
	n->NodoU.Operando = operand;
	return n;
}

NodoExprPtr NuevoNodoNumero( float operand )
{
	NodoExprPtr n;
	n = (NodoExprPtr)malloc( (size_t)sizeof(NodoExpr) );
	n->NodoN.Tipo = TipoNumero;
	n->NodoN.Operando = operand;
	return n;
}

char CharProximo( char *Expr, int &Pos )
{
	char Ch;
	do{
		Pos++;
		if( Pos<=strlen(Expr) )
			Ch = Expr[Pos];
		else
			Ch = '\0';
	}while( Ch==' ' );
	return Ch;
}

char CharAnterior( char *Expr, int &Pos )
{
	char Ch;
	do{
		if( Pos>0 ){
			Pos--;
			Ch = Expr[Pos];
		}
	}while( Ch==' ' );
	return Ch;
}

NodoExprPtr Factor( char *Expr, int &Pos, char &Ch, int &Error )
{
	int Encontro, Inicio, L, i;
	float F;
	char AuxExpr[20];
	NodoExprPtr AuxExprPtr;

	AuxExprPtr = NULL;
	Ch = CharProximo( Expr, Pos );
	if( EsNumero(Ch) ){
		Inicio = Pos;
		do{
			Ch = CharProximo( Expr, Pos );
		}while( EsNumero( Ch ) );
		if( Ch=='.' ){
			do{
				Ch = CharProximo( Expr, Pos );
			}while( EsNumero( Ch ) );
		}
		if( Ch=='E' ){
			Ch = CharProximo( Expr, Pos );
			do{
				Ch = CharProximo( Expr, Pos );
			}while( EsNumero( Ch ) );
		}
		memcpy( AuxExpr, (Expr+Inicio), Pos-Inicio );
		AuxExpr[Pos-Inicio] = '\0';
		F = atof( AuxExpr );
		Ch = CharAnterior( Expr, Pos );
//			Error = 1; 					// Error en formato numerico
		return NuevoNodoNumero( F );
	}
	else if( Ch=='-' )
		return NuevoNodoUnario( '-', Factor( Expr, Pos, Ch, Error ) );
	else if( Ch=='(' ){
		AuxExprPtr = Expresion( Expr, Pos, Ch, Error );
		Ch = CharProximo( Expr, Pos );
		if( Ch!=')' )
			Error = 2; 					// Falta parentesis
		return AuxExprPtr;
	}
	else if( EsVariable( Ch ) )
		return NuevoNodoUnario( Ch, NULL );
	else{
		 Encontro = 0;
		 for( i=0; (i<NSTFUNC)&&(!Encontro); i++ ){
			L = strlen( NombreFuncion[i] );
			memcpy( AuxExpr, (Expr+Pos), L );
			AuxExpr[L] = '\0';
			if( !strcmp( AuxExpr, NombreFuncion[i] ) ){
				Pos = Pos+L-1;
				Ch = CharProximo( Expr, Pos );
				AuxExprPtr = Expresion( Expr, Pos, Ch, Error );
				Ch = CharProximo( Expr, Pos );
				switch( i ){
					case 0: AuxExprPtr = NuevoNodoUnario( '0', AuxExprPtr );
							break;
					case 1: AuxExprPtr = NuevoNodoUnario( '1', AuxExprPtr );
							break;
					case 2: AuxExprPtr = NuevoNodoUnario( '2', AuxExprPtr );
							break;
					case 3: AuxExprPtr = NuevoNodoUnario( '3', AuxExprPtr );
							break;
					case 4: AuxExprPtr = NuevoNodoUnario( '4', AuxExprPtr );
							break;
					case 5: AuxExprPtr = NuevoNodoUnario( '5', AuxExprPtr );
							break;
					case 6: AuxExprPtr = NuevoNodoUnario( '6', AuxExprPtr );
							break;
					case 7: AuxExprPtr = NuevoNodoUnario( '7', AuxExprPtr );
							break;
					case 8: AuxExprPtr = NuevoNodoUnario( '8', AuxExprPtr );
							break;
					case 9: AuxExprPtr = NuevoNodoUnario( '9', AuxExprPtr );
							break;
				}
				Encontro = 1;
			}
		 }
		 if( !Encontro ){
			 for( i=0; (i<NCONST)&&(!Encontro); i++ ){
				L = strlen( NombreConst[i] );
				memcpy( AuxExpr, (Expr+Pos), L );
				AuxExpr[L] = '\0';
				if( !strcmp( AuxExpr, NombreConst[i] ) ){
					Pos = Pos+L-1;
					Ch = CharProximo( Expr, Pos );
					switch( i ){
						case 0 : AuxExprPtr = NuevoNodoUnario( 'p', NULL );
								 break;
						case 1 : AuxExprPtr = NuevoNodoUnario( 'e', NULL );
								 break;
					}
					Encontro = 1;
				}
			 }
		 }
		 if( !Encontro ){
			Error = 3;                  // Palabra no identificada
			AuxExprPtr = NULL;
		 }
		 return AuxExprPtr;
	}
}

NodoExprPtr Termino( char *Expr, int &Pos, char &Ch, int &Error )
{
	NodoExprPtr N;
	char operador;
	if( !Error ){
		N = Factor( Expr, Pos, Ch, Error );
		if( N!=NULL ){
			Ch = CharProximo( Expr, Pos );
			if( (Ch=='^') || (Ch=='*') || (Ch=='/') ){
				operador = Ch;
				N = NuevoNodoBinario( operador, N, Termino(Expr,Pos,Ch,Error) );
			}
			else
				Ch = CharAnterior( Expr, Pos );
		}
		return N;
	}
	else
		return NULL;
}

NodoExprPtr Expresion( char *Expr, int &Pos, char &Ch, int &Error )
{
	NodoExprPtr	N;
	char operador;
	if( !Error ){
		N = Termino( Expr, Pos, Ch, Error );
		if( N!=NULL ){
			Ch = CharProximo( Expr, Pos );
			if( (Ch=='+') || (Ch=='-') ){
				operador = Ch;
				N = NuevoNodoBinario( operador, N, Expresion(Expr,Pos,Ch,Error) ) ;
			}
			else if( Ch!='\0' )
				Ch = CharAnterior( Expr, Pos );
		}
		return N;
	}
	else
		return NULL;
}

NodoExprPtr ParserExpr( char *Expr, int &Error )
{
	int Pos;
	char Ch;
	NodoExprPtr N;

	Error = 0;
	Pos = -1;
	strupr( Expr );
	N = Expresion( Expr, Pos, Ch, Error );
	if( Ch=='\0' )
		Error = 0;
	return N;
}

float fact( float x )
{
	if( x>0 )
		return (x*fact(x-1));
	else
		return 1;
}

float pot( float x, float y )
{
	if( x>0 )
		return exp(y*log(x));
	else if( x==0 )
		return 1;
	else
		return 0;
}

float pot( float x, int y )
{
	float r;
	int i;
	for( r=x, i=2; i<=y; i++)
		r *= r;
	return r;

}

void EliminarExpr( NodoExprPtr N )
{
	switch( N->NodoU.Tipo ){
		case TipoBinario: 	EliminarExpr( (NodoExprPtr)(N->NodoB.IzqOperando) );
							EliminarExpr( (NodoExprPtr)(N->NodoB.DerOperando) );
							free( (char*)N );
							break;
		case TipoUnario:	if( N->NodoU.Operando!=NULL )
							EliminarExpr( (NodoExprPtr)(N->NodoU.Operando) );
							free( (char*)N );
							break;
		case TipoNumero:	free( (char*)N );
							break;
   }
}

float EvalExpr( NodoExprPtr N, float X=0.0, float Y=0.0, float Z=0.0 )
{
	float Op1, Op2, F=0;
	switch( N->NodoU.Tipo ){
		case TipoBinario: Op1 = EvalExpr( (NodoExprPtr)(N->NodoB.IzqOperando), X, Y, Z );
						  Op2 = EvalExpr( (NodoExprPtr)(N->NodoB.DerOperando), X, Y, Z );
						  switch( N->NodoB.BinOperador ){
							case '+': F = (Op1 + Op2);
									  break;
							case '-': F =  (Op1 - Op2);
									  break;
							case '*': F = (Op1 * Op2);
									  break;
							case '/': if( Op2==0 )
										Op2 = Zero;
									  F = (Op1 / Op2);
									  break;
							case '^': F = pot( Op1, (int)Op2 );
									  break;
						  }
						  break;
		case TipoUnario : if( N->NodoU.Operando!=NULL )
							Op1 = EvalExpr( (NodoExprPtr)(N->NodoU.Operando), X, Y, Z );
						  switch( N->NodoU.UnOperador ){
							case '-': F = -Op1;
									  break;
							case 'X': F = X;
									  break;
							case 'Y': F = Y;
									  break;
							case 'Z': F = Z;
									  break;
//							case 'p': F = M_PI;
//									  break;
//							case 'e': //F = M_E;
//									  break;
							case '0': F = fabs(Op1);
									  break;
							case '1': F = sqrt(Op1);
									  break;
							case '2': F = Op1*Op1;
									  break;
							case '3': F = sin(Op1);
									  break;
							case '4': F = cos(Op1);
									  break;
							case '5': F = atan(Op1);
									  break;
							case '6': F = log(Op1);
									  break;
							case '7': F = log10(Op1);
									  break;
							case '8': F = exp(Op1);
									  break;
							case '9': F = fact( (int)Op1 );
									  break;
						  }
						  break;
		case TipoNumero : F = N->NodoN.Operando;
						  break;
   }
   return F;
}

#endif