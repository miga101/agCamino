	/********************************************************************
	* Nombre     : VECTOR02.HPP 										*
	* Funci¢n    : Define e implementa la clase Vector . 				*
	* Par metros : -- 													*
	* Compilador : Turbo C++ v1.                                  	  	*
	* Hecho por  : Jos‚ Rafael Sosa Brice¤o.                         	*
	* Fecha      : Abril. de 1994. 										*
	* Notas      : Define la estructura y propiedades basicas de los   	*
	*			   vectores de tipo generico para operaciones matematic	*
	*              el tamano del vector es pseudo-constante.			*
	*			   Permite el manejo separado de los apuntadores y la 	*
	*			   indexacion de ini a fin para compatibilidad con NR.	*
	********************************************************************/

#ifndef __VECTOR02_HPP
#define __VECTOR02_HPP 2

#include <stdarg.h>
#include <malloc.h>
#include <math.h>
#include "error01.hpp"
#define Mayor(a,b) (a>=b?a:b)
#define Menor(a,b) (a<=b?a:b)
#define sqr(a) (a*a)

template <class T>
class Vector {
	protected:
		long ini,fin;
		T* vec;
		T cero;
		char circular,ext;
	public:
		// Constructoras:

		Vector( void );                 // Crea un vector vacio
		Vector( long, long );			// Crea un vector de n elementos
		Vector( long, long, T* );		// Crea un vector sobre una memoria
		Vector( long, long, T, ... );	// Crea y llena un vector
		Vector( Vector& );				// Crea una copia dado un vector
		~Vector();						// Destruye el vector
		void acceso_circular();
		void dimencionar( long, long );
		void dimencionar( long, long, T* );

		// Consultoras:

		T* DirMem();                    // Direccion de memoria del arreglo
		long Dim();						// Dimension del vector
		long Ini();						// Dimension del vector
		long Fin();						// Dimension del vector
		T& operator ()( long );			// Op de acceso indexado
		T& operator []( long );			// Op de acceso indexado

		// Operaciones:

		friend double Mod( Vector& );         // norma del vector
		friend double Ang( Vector&, Vector& );// angulo entre vectores
		friend Vector Unit( Vector& );        // vector unitario

		void operator =( Vector& );
		void operator =( double );
		void operator +=( Vector& );
		void operator -=( Vector& );
		void operator *=( double );
		void operator /=( double );

		Vector& operator ++();
		Vector& operator --();

		Vector operator +();
		Vector operator -();

		friend int operator ==( Vector&, Vector& );
		friend int operator !=( Vector&, Vector& );

		friend double Esc( Vector&, Vector& );
		friend Vector Vec( Vector&, Vector& );

		friend Vector operator +( Vector&, Vector& );// suma de vectores
		friend Vector operator -( Vector&, Vector& );// resta de vectores
		friend Vector operator %( Vector&, Vector& );// producto vectorial
		friend double operator *( Vector&, Vector& ); 	 // producto escalar
		friend Vector operator *( double, Vector& );      // prod escalar x vector
		friend Vector operator *( Vector&, double );
		friend Vector operator /( Vector&, double );
};


// 	Implementacion de las operaciones de Vector:


template <class T>
Vector<T>::Vector( void )
{
	vec = NULL;
	ini = 0;
	fin = -1;
	circular = 0;
	ext = 0;
}


template <class T>
Vector<T>::Vector( long i, long f )
{
	vec = new T[f-i+1];
	circular = 0;
	ext = 0;
	if ( vec != NULL ){
		vec = vec-i;
		ini = i;
		fin = f;
	}
	else{
		ini = 0;
		fin = -1;
		ErrorMg(1,"ERROR: Memoria insuficiente para creacion de vector");
	}
}

template <class T>
Vector<T>::Vector( long i, long f, T* dirmem )
{
	vec = dirmem;  // se recive el vector y corrido a i
	circular = 0;
	ext = 1;
	if ( vec != NULL ){
		ini = i;
		fin = f;
	}
	else{
		ini = 0;
		fin = -1;
		ErrorMg(0,"ERROR: Vector nulo asignado");
	}
}


template <class T>
Vector<T>::Vector( long i, long f, T arg1,... )
{
	long k;
	va_list listarg;
	vec = new T[f-i+1];
	circular = 0;
	ext = 0;
	va_start(listarg,arg1);
	if ( vec != NULL ) {
		vec = vec-i;
		ini = i;
		fin = f;
		vec[ini] = arg1;
		for( k=ini; (k<=fin); k++ )
			vec[k] = (T)va_arg(listarg,T);
	}
	else{
		ini = 0;
		fin = -1;
		ErrorMg(1,"ERROR: Memoria insuficiente para creacion de vector");
	}
	va_end(listarg);
}


template <class T>
Vector<T>::Vector( Vector<T>& v )
{
	long i;
	ini = v.ini;
	fin = v.fin;
	circular = 0;
	ext = 0;
	vec = new T[fin-ini+1];
	if ( vec != NULL ) {
		vec = vec-ini;
		for( i=ini; i<=fin; i++ )
			vec[i] = v.vec[i];
	}
	else{
		ini = 0;
		fin = -1;
		ErrorMg(1,"ERROR: Memoria insuficiente para creacion de vector");
	}
}


template <class T>
Vector<T>::~Vector()
{
	if( (vec != NULL) && (!ext) ){
		vec = vec+ini;
		delete vec;
		vec = NULL;
	}
	ini = 0;
	fin = -1;
	circular = 0;
	ext = 0;
}


template <class T>
void Vector<T>::acceso_circular()
{
	circular = !circular;
}


template <class T>
void Vector<T>::dimencionar( long i, long f )
{
	if( (fin-ini)!=(f-i) ){
		if( (vec != NULL) && (!ext) ){
			vec = vec+ini;
			delete vec;
			vec = NULL;
		}
		ini = i;
		fin = f;
		ext = 0;
		vec = new T[f-i+1];
		if( vec==NULL ){
			ini = 0;
			fin = -1;
			ErrorMg(1,"ERROR: Memoria insuficiente para creacion de vector");
		}
		else
			vec = vec-i;
	}
	else{
		vec = vec+ini;
		ini = i;
		fin = f;
		vec = vec-ini;
	}
}


template <class T>
void Vector<T>::dimencionar( long i, long f, T *dirmem )
{
	if( (vec != NULL) && (!ext) ){
		vec = vec+ini;
		delete vec;
		vec = NULL;
	}
	vec = dirmem;
	circular = 0;
	ext = 1;
	if ( vec != NULL ){
		ini = i;
		fin = f;
	}
	else{
		ini = 0;
		fin = -1;
		ErrorMg(0,"ERROR: Vector nulo asignado");
	}
}


template <class T>
T* Vector<T>::DirMem()
{
	return vec;
}


template <class T>
long Vector<T>::Dim()
{
	return fin-ini+1;
}


template <class T>
long Vector<T>::Ini()
{
	return ini;
}


template <class T>
long Vector<T>::Fin()
{
	return fin;
}


template <class T>
T& Vector<T>::operator ()( long i )
{
	if( (i>=ini) && (i<=fin) )
		return vec[i];
	else if( (circular) && (fin-ini+1>0) ){
		long ind=i-ini, tam=fin-ini+1;
		if(ind>=tam)
			return vec[(ind%tam)+ini];
		else
			return vec[tam+(ind%tam)+ini];
	}
	else{
		cero = 0;
		return cero;
	}
}

template <class T>
T& Vector<T>::operator []( long i )
{
	if( (i>=ini) && (i<=fin) )
		return vec[i];
	else if( (circular) && (fin-ini+1>0) ){
		long ind=i-ini, tam=fin-ini+1;
		if(ind>=tam)
			return vec[(ind%tam)+ini];
		else
			return vec[tam+(ind%tam)+ini];
	}
	else{
		cero = 0;
		return cero;
	}
}


template <class T>
double Mod( Vector<T>& v )
{
	long i;
	double sum=0;
	for( i=v.ini; i<=v.fin; i++)
		sum += sqr( v.vec[i] );
	return sqrt( sum );
}


template <class T>
double Ang( Vector<T>& v1, Vector<T>& v2 )
{
	return (double)acos( ( v1 * v2 ) / ( Mod(v1) * Mod(v2) ) );
}


template <class T>
Vector<T> Unit( Vector<T>& v )
{
	return ( v / Mod(v) );
}


template <class T>
void Vector<T>::operator =( Vector<T>& v )
{
	long i;
	if( fin-ini != v.fin-v.ini ){
		if( (vec != NULL) && (!ext) ){
			vec = vec+ini;
			delete vec;
			vec = NULL;
		}
		if( v.Dim()>0 ){
			ini = v.ini;
			fin = v.fin;
			vec = new T [fin-ini+1];
			if( vec==NULL ){
				ini = 0;
				fin = -1;
				ErrorMg(1,"ERROR: Memoria insuficiente para creacion de vector");
			}
			else
				vec = vec-ini;
		}
	}
	else{
		vec = vec+ini;
		ini = v.ini;
		fin = v.fin;
		vec = vec-ini;
	}
	if ( vec != NULL ) {
		for( i=ini; i<=fin; i++ )
			vec[i] = v.vec[i];
	}
}


template <class T>
void Vector<T>::operator =( double e )
{
	long i;
	if ( vec != NULL ){
		if( (e!=0) || (ext) )
			for( i=ini; i<=fin; i++ )
				vec[i] = e;
		else{
			if( (vec!=NULL) && (!ext) ){
				vec = vec+ini;
				delete vec;
				vec = NULL;
			}
			ini = 0;
			fin = -1;
			circular = 0;
			ext = 0;
		}
	}
}


template <class T>
void Vector<T>::operator +=( Vector<T>& v )
{
	long i;
	if( (ini == v.ini) && (fin == v.fin) ){
		for ( i=ini; i<=fin; i++ )
			vec[i] += v.vec[i];
	}
	else
		*this = (*this + v);
}


template <class T>
void Vector<T>::operator -=( Vector<T>& v )
{
	long i;
	if( (ini == v.ini) && (fin == v.fin) ){
		for ( i=ini; i<=fin; i++ )
			vec[i] -= v.vec[i];
	}
	else
		*this = (*this - v);
}


template <class T>
void Vector<T>::operator *=( double e )
{
	long i;
	for ( i=ini; i<=fin; i++ )
		vec[i] *= e;
}


template <class T>
void Vector<T>::operator /=( double e )
{
	long i;
	for ( i=ini; i<=fin; i++ )
		vec[i] /= e;
}


template <class T>
Vector<T>& Vector<T>::operator ++()
{
	long i;
	for ( i=ini; i<=fin; i++)
		++vec[i];
	return *this;
}


template <class T>
Vector<T>& Vector<T>::operator --()
{
	long i;
	for ( i=ini; i<=fin; i++)
		--vec[i];
	return *this;
}


template <class T>
Vector<T> Vector<T>::operator +()
{
	return *this;
}


template <class T>
Vector<T> Vector<T>::operator -()
{
	long i;
	Vector<T>  v(*this);
	for ( i=v.ini; i<=v.fin; i++ )
		v.vec[i] *= -1;
	return v;
}


template <class T>
int operator ==( Vector<T>& v1, Vector<T>& v2 )
{
	int igual,fin,ini;
	long i;
	if( (v1.ini == v2.ini) && (v1.fin == v2.fin) ){
		for ( i=v1.ini, igual=1; (i<=v1.fin)&&(igual); i++ )
			if ( v1.vec[i] != v2.vec[i] )
				igual = 0;
	}
	else{
		fin = Mayor(v1.fin, v2.fin);
		ini = Menor(v1.ini, v2.ini);
		for ( i=ini, igual=1; (i<=fin)&&(igual); i++ )
			if ( v1(i) != v2(i) )
				igual = 0;
	}
	return igual;
}


template <class T>
int operator !=( Vector<T>& v1, Vector<T>& v2 )
{
	return !( v1 == v2 );
}


template <class T>
Vector<T> operator +( Vector<T>& v1, Vector<T>& v2 )
{
	long i,sini,sfin;
	sini = Menor(v1.ini, v2.ini);
	sfin = Mayor(v1.fin, v2.fin);
	Vector<T> s(sini,sfin);
	if( (v1.ini == v2.ini) && (v1.fin == v2.fin) ){
		for ( i=sini; i<=sfin; i++ )
			s.vec[i] = v1.vec[i] + v2.vec[i];
	}
	else{
		for ( i=sini; i<=sfin; i++ )
			s.vec[i] = v1(i) + v2(i);
	}
	return s;
}


template <class T>
Vector<T> operator -( Vector<T>& v1, Vector<T>& v2 )
{
	long i,rini,rfin;
	rini = Menor(v1.ini, v2.ini);
	rfin = Mayor(v1.fin, v2.fin);
	Vector<T> r(rini,rfin);
	if( (v1.ini == v2.ini) && (v1.fin == v2.fin) ){
		for ( i=rini; i<=rfin; i++ )
			r.vec[i] = v1.vec[i] - v2.vec[i];
	}
	else{
		for ( i=rini; i<=rfin; i++ )
			r.vec[i] = v1(i) + v2(i);
	}
	return r;
}


template <class T>
Vector<T> operator %( Vector<T>& v1, Vector<T>& v2 )
{
	return (v1+v2);    // implementar luego...
}


template <class T>
Vector<T> operator /( Vector<T>& v, double e )
{
	long i;
	Vector<T>  d(v);
	for ( i=d.ini; i<=d.fin; i++ )
		d.vec[i] /= e;
	return d;
}


template <class T>
double operator *( Vector<T>& v1, Vector<T>& v2 )
{
	long i,pini,pfin;
	double result=0;
	pini = Mayor(v1.ini, v2.ini);
	pfin = Menor(v1.fin, v2.fin);
	for ( i=pini; i<=pfin; i++ )
		result += v1.vec[i] * v2.vec[i];
	return result;
}


template <class T>
Vector<T> operator *( Vector<T>& v, double e )
{
	long i;
	Vector<T> p(v);
	for ( i=p.ini; i<=p.fin; i++ )
		p.vec[i] *= e;
	return p;
}


template <class T>
Vector<T> operator *( double e, Vector<T>& v )
{
	long i;
	Vector<T> p(v);
	for ( i=p.ini; i<=p.fin; i++ )
		p.vec[i] *= e;
	return p;
}


template <class T>
double Esc( Vector<T>& v1, Vector<T>& v2 )
{
	long i,pini,pfin;
	double result=0;
	pini = Mayor(v1.ini, v2.ini);
	pfin = Menor(v1.fin, v2.fin);
	for ( i=pini; i<=pfin; i++ )
		result += v1.vec[i] * v2.vec[i];
	return result;
}


template <class T>
Vector<T> Vec( Vector<T>& v1, Vector<T>& v2 )
{
	return (v1+v2);  // implementar luego...
}

#define VectorF Vector<float>
#define VectorD Vector<double>
#define VectorI Vector<int>
#define VectorU Vector<unsigned>
#define VectorL Vector<long>
#define VectorC Vector<char>


#endif