#ifndef __GRFDAT01_HPP
#define __GRFDAT01_HPP 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include "vector02.hpp"



struct Posc {
	int ID;
	int estado;
	char nomb[10];
	int x;
	int y;
	int z;
} Mec[25];//Matriz Espacial Cuadrada 25 = Smec

struct {
	int IDpos;
} Vexcam[25];//camino en Mec 25 = Smec


int Lmec = 5;
int Smec = Lmec*Lmec;
int Esp_pos = 2;
int PsPntllx = 0;//300
int PsPntlly = 0;//100
int Sz_posc = 1;


#define pi 3.1416

float 	PropXY,         		// Proporcion de entre X y Y, Y=PropXY*X
		EscX, EscY,     		// Escala de pixel por unidad
		MaxX,MaxY,MinX,MinY;	// Rango y dominio de F
int 	x1,x2,yy1,y2,			// Coordenadas del grafico en la pantalla
		CentX,CentY,     		// Coordenadas en la pantalla del origen
		gd,gm,graferror;		// variables del modo grafico


void inicio_grafico();
void asignar_ventana_grafico( int vx1, int vy1, int vx2, int vy2 );
void asignar_dom_rang_grafico( float d1, float d2, float r1, float r2 );
void ajustar_param_grafico();
void ajustar_param_grafico( float X[], float Y[], int ndat );
void ajustar_param_grafico( Vector<VectorF>& );
void ajustar_param_grafico( VectorF& );
int pto_x( float x);
int pto_y( float y);
void dibujar_pto( float x, float y, int color );
void dibujar_pto( VectorF&, int color );
void dibujar_rotulo_grafico( char *titulo, char *nombx, char *nomby, int nmarcx, int nmarcy);
void graficar( float X[], float Y[], int ini, int fin, int color, int linea );
void graficar( Vector<VectorF>&, int color, int linea );
void graficar( VectorF&, int color, int linea, int ny );
void graficar( float (*func)(float), float lim1, float lim2, float paso, int color, int linea );
void Dibujar_Mec(void);


void Dibujar_Mec(void)
{
int i;
	for( i=0; i<Smec; i++ ){
	  if (Mec[i].estado==0) 
		  glColor3ub(0, 255, 0);
	  else 
		  if ((strcmp(Mec[i].nomb, "start")==0) | (strcmp(Mec[i].nomb, "fin")==0))
	  		  glColor3ub(255, 255, 0);
		  else
			  glColor3ub(255, 0, 0);

	  glPushMatrix();
	    glTranslatef(Mec[i].x, Mec[i].y, 0);
	    glutWireCube(Sz_posc);
	  glPopMatrix();
	}
  //glutPostRedisplay();

}

void inicio_grafico()
{
int i, fin;
	srand( (unsigned)time( NULL )/2 );
	Mec[0].ID = 0;
	Mec[0].estado = 1;// start
	strcpy(Mec[0].nomb, "start");// start
	Mec[0].x = PsPntllx;
	Mec[0].y = PsPntlly;
	for( i=1; i<Smec; i++ ){
		//Mec[i].estado = unif01() < 0.5 ? 0 : 1;
		Mec[i].estado = 1;
		Mec[i].ID = i;
		Mec[i].x = Mec[i-1].x + Esp_pos;
		if (i%Lmec==0){
			Mec[i].y = Mec[i-1].y + Esp_pos;
			Mec[i].x = PsPntllx;
		}
		else
			Mec[i].y = Mec[i-1].y;
		if (Mec[i].estado) fin = i;// fin
	}
	strcpy(Mec[fin].nomb, "fin");// start	
}


void asignar_ventana_grafico( int vx1, int vy1, int vx2, int vy2 )
{
	x1 = vx1;
	yy1 = vy1;
	x2 = vx2;
	y2 = vy2;
}


void asignar_dom_rang_grafico( float d1, float d2, float r1, float r2 )
{
	MinX = d1;
	MaxX = d2;
	MinY = r1;
	MaxY = r2;
}


void ajustar_param_grafico()
{
	EscX = (x2 - x1) / (MaxX - MinX);
	EscY = (y2 - yy1) / (MaxY - MinY);
	CentX =  x1 - (int)floor(MinX * EscX);
	CentY =  y2 + (int)floor(MinY * EscY);
}

void ajustar_param_grafico( float X[], float Y[], int ndat )
{
	int i;
	MaxX = MinX = X[0];
	MaxY = MinY = Y[0];
	for( i=0; i<ndat; i++ ){
		if(MinX > X[i])
			MinX = X[i];
		if(MaxX < X[i])
			MaxX = X[i];
		if(MinY > Y[i])
			MinY = Y[i];
		if(MaxY < Y[i])
			MaxY = Y[i];
	}
	EscX = (x2 - x1) / (MaxX - MinX);
	EscY = (y2 - yy1) / (MaxY - MinY);
	CentX =  x1 - (int)floor(MinX * EscX);
	CentY =  y2 + (int)floor(MinY * EscY);
}

void ajustar_param_grafico( Vector<VectorF> &tabla )
{
	int i;
	long tabini,tabfin,ptoini;
	tabini = tabla.Ini();
	tabfin = tabla.Fin();
	ptoini = tabla(tabini).Ini();
	MaxX = MinX = tabla(tabini)(ptoini);
	MaxY = MinY = tabla(tabini)(ptoini+1);
	for( i=tabini; i<=tabfin; i++ ){
		if(MinX > tabla(i)(ptoini))
			MinX = tabla(i)(ptoini);
		if(MaxX < tabla(i)(ptoini))
			MaxX = tabla(i)(ptoini);
		if(MinY > tabla(i)(ptoini+1))
			MinY = tabla(i)(ptoini+1);
		if(MaxY < tabla(i)(ptoini+1))
			MaxY = tabla(i)(ptoini+1);
	}
	EscX = (x2 - x1) / (MaxX - MinX);
	EscY = (y2 - yy1) / (MaxY - MinY);
	CentX =  x1 - (int)floor(MinX * EscX);
	CentY =  y2 + (int)floor(MinY * EscY);
}

void ajustar_param_grafico( VectorF &serie )
{
	int i;
	long tabini,tabfin;
	tabini = serie.Ini();
	tabfin = serie.Fin();
	MaxX = MinX = tabini;
	MaxY = MinY = serie(tabini);
	for( i=tabini; i<=tabfin; i++ ){
		if(MinX > i)
			MinX = i;
		if(MaxX < i)
			MaxX = i;
		if(MinY > serie(i))
			MinY = serie(i);
		if(MaxY < serie(i))
			MaxY = serie(i);
	}
	EscX = (x2 - x1) / (MaxX - MinX);
	EscY = (y2 - yy1) / (MaxY - MinY);
	CentX =  x1 - (int)floor(MinX * EscX);
	CentY =  y2 + (int)floor(MinY * EscY);
}


int pto_x( float x)
{
	return (int)floor( CentX + x*EscX );
}


int pto_y( float y)
{
	return (int)floor( CentY - y*EscY );
}



void graficar( float (*func)(float), float lim1, float lim2, float paso, int color=15, int linea=0 )
{
	float x;
	int col, i;
	if( linea ){
	glPushMatrix();  
	glColor3ub(0, 255, 0);
  //glTranslatef(400.0, 400.0, 0.0);
	glBegin(GL_LINE_STRIP);
	glVertex2i(pto_x(lim1), pto_y(func(lim1)));
	for( x=lim1; (x<=lim2)&&(!kbhit()); x+=paso )
		glVertex2i(pto_x(x), pto_y(func(x)));
	
	glEnd();

    glutWireSphere(100.0, 20, 20);

	glPopMatrix();

	}
}

#endif