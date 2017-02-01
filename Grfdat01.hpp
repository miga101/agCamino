#ifndef __GRFDAT01_HPP
#define __GRFDAT01_HPP 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include "vector02.hpp"

#define pi 3.1416

float 	PropXY,         		// Proporcion de entre X y Y, Y=PropXY*X
		EscX, EscY,     		// Escala de pixel por unidad
		MaxX,MaxY,MinX,MinY;	// Rango y dominio de F
int 	x1,x2,yy1,y2,			// Coordenadas del grafico en la pantalla
		CentX,CentY,     		// Coordenadas en la pantalla del origen
		gd,gm,graferror;		// variables del modo grafico


int inicio_grafico();
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



int inicio_grafico()
{
	int xasp, yasp;
//	gd = DETECT;
//	initgraph(&gd,&gm,"");
//	graferror = graphresult();
	if( graferror != 0 )//grOk )
	{
//	   printf("Error Grafico: %s\n", grapherrormsg(graferror));
	   printf("Presione cualquier tecla para terminar...");
	   getch();
	   exit(1);
	}
//	getaspectratio( &xasp, &yasp );
	PropXY = (float)yasp/xasp;
	  glClearStencil(0);
	return graferror;
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


void dibujar_pto( float x, float y, int color )
{
//	putpixel( pto_x(x), pto_y(y), color );
}

void dibujar_pto( VectorF &p, int color )
{
//	putpixel( pto_x(p(p.Ini())), pto_y(p(p.Ini()+1)), color );
}


void dibujar_rotulo_grafico( char *titulo, char *nombx, char *nomby, int nmarcx, int nmarcy)
{
	int i;
	float aux;
	char numtext[20];
//	rectangle(x1-5,yy1-5,x2+5,y2+5);
//	settextstyle( DEFAULT_FONT, HORIZ_DIR, 0 );
//	settextjustify( CENTER_TEXT, TOP_TEXT );
	for( i=0; i<=nmarcx; i++ ){
		aux = MinX + i*(MaxX - MinX)/nmarcx;
//		line( pto_x(aux), y2+3, pto_x(aux), y2+7 );
		gcvt( aux, 6, numtext );
//		outtextxy( pto_x(aux), y2+7, numtext );
	}
//	outtextxy( getmaxx()/2, 0, titulo );
//	outtextxy( getmaxx()/2, getmaxy()-10, nombx );
//	settextjustify( RIGHT_TEXT, TOP_TEXT );
	for( i=0; i<=nmarcy; i++ ){
		aux = MinY + i*(MaxY - MinY)/nmarcy;
//		line( x1-7, pto_y(aux), x1-3, pto_y(aux) );
		gcvt( aux, 6, numtext );
//		outtextxy( x1-7, pto_y(aux), numtext );
	}
//	settextstyle( DEFAULT_FONT, VERT_DIR, 0 );
//	settextjustify( LEFT_TEXT, CENTER_TEXT );
//	outtextxy( 10, getmaxy()/2, nomby );
//	settextstyle( DEFAULT_FONT, HORIZ_DIR, 0 );
}


void graficar( float X[], float Y[], int ini, int fin, int color=15, int linea=0 )
{
	int i, col;
	if( linea ){
//		col = getcolor();
//		setcolor(color);
//		moveto( pto_x(X[ini]), pto_y(Y[ini]) );
//		for( i=ini; i<=fin; i++ )
//			lineto( pto_x(X[i]), pto_y(Y[i]) );
//		setcolor(col);
	}
	else{
		for( i=ini; i<=fin; i++ )
			dibujar_pto( X[i], Y[i], color );
	}
}

void graficar( Vector<VectorF> &tabla, int color=15, int linea=0, int ny=1 )
{
	int i, ini, fin, col;
	if( linea ){
//		col = getcolor();
//		setcolor(color);
		ini = tabla.Ini();
		fin = tabla.Fin();
//		moveto( pto_x( tabla(ini)(tabla(ini).Ini()) ), pto_y( tabla(ini)(tabla(ini).Ini()+ny) ) );
//		for( i=ini; i<=fin; i++ )
//			lineto( pto_x( tabla(i)(tabla(i).Ini()) ), pto_y( tabla(i)(tabla(i).Ini()+ny) ) );
//		setcolor(col);
	}
	else{
//		for( i=tabla.Ini(); i<=tabla.Fin(); i++ )
//			putpixel( pto_x( tabla(i)(tabla(i).Ini()) ), pto_y( tabla(i)(tabla(i).Ini()+ny) ), color );
	}
}

void graficar( VectorF &serie, int color=15, int linea=0 )
{
	int i, ini, fin, col;
	if( linea ){
//		col = getcolor();
//		setcolor(color);
		ini = serie.Ini();
		fin = serie.Fin();
//		moveto( pto_x( ini ), pto_y( serie(ini) ) );
//		for( i=ini; i<=fin; i++ )
//			lineto( pto_x( i ), pto_y( serie(i) ) );
//		setcolor(col);
	}
	else{
//		for( i=serie.Ini(); i<=serie.Fin(); i++ )
//			putpixel( pto_x( i ), pto_y( serie(i) ), color );
	}
}

void graficar( float (*func)(float), float lim1, float lim2, float paso, int color=15, int linea=0 )
{
	float x;
	int col, i;
	if( linea ){
//		col = getcolor();
//		setcolor(color);
//		moveto( pto_x(lim1), pto_y(func(lim1)) );
//		for( x=lim1; (x<=lim2)&&(!kbhit()); x+=paso )
//			lineto( pto_x(x), pto_y(func(x)) );
//		setcolor(col);
	glPushMatrix();  
	glColor3ub(0, 255, 0);
  //glTranslatef(400.0, 400.0, 0.0);
	glBegin(GL_LINE_STRIP);
	glVertex2i(pto_x(lim1), pto_y(func(lim1)));
	for( x=lim1; (x<=lim2)&&(!kbhit()); x+=paso )
		glVertex2i(pto_x(x), pto_y(func(x)));
	
	glEnd();

    //glutWireSphere(100.0, 20, 20);

	glPopMatrix();

	}
	else{
		//for( x=lim1; (x<=lim2)&&(!kbhit()); x+=paso )
		//	dibujar_pto( x, func(x), color );
	glPushMatrix();  
	glColor3ub(0, 255, 0);
	glBegin(GL_POINTS);
	glVertex2i(pto_x(lim1), pto_y(func(lim1)));
	for( x=lim1; (x<=lim2)&&(!kbhit()); x+=paso )
		glVertex2i(x, func(x));
	
	glEnd();
	glPopMatrix();

	}
}

#endif