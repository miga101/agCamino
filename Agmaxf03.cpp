	/********************************************************************
	* Nombre     : AGMAXF03.CPP 										*
	* Funci¢n    : Hallar el maximo de una funcion. 					*
	* Par metros : -- 													*
	* Compilador : Turbo C++ v1.                                  	  	*
	* Hecho por  : Jos‚ Rafael Sosa Brice¤o.                         	*
	* Fecha      : Junio de 1994. 										*
	* Notas      : Aproxima el punto maximo usando algoritmos geneticos.*
	*			   La funcioN puede ser especificada por el ususario y  *
	*			   la salida puede ser grafica.							*
	********************************************************************/

#include <GL/glut.h>
#include "genet05.hpp"
#include "arcdat01.hpp"
#include "grfdat01.hpp"
#include "parser02.hpp"

float max_err,
	  gx1,gx2,
	  gy1,gy2,			/* Rangos de graficacion */
	  parmin,parmax,	/* Rango de la solucion */
	  (*func)(float); 	/* Funcion a graficar */
VectorF fadap,fadapp;  	/* Registros de la eficiencia del algoritmo */
NodoExprPtr cabfunc;	/* Nodo cabeza del evaluador sintactico */
int tipsal,
	numsim;
char nomfunc[60],		/* Expresion de la funcion a graficar */
	 nomtipsal[2][8] = {"Texto", "Grafica"};

float funcion_adaptacion( VectorU& );
void salida( int );
void salida_graf( int );
void salida_reg( int );

float val_gene( unsigned i )
{
	return (float)((parmax-parmin)/(2*RAND_MAX-1)*i + parmin);
}

unsigned gene_val( float f )
{
   return (unsigned)((2*RAND_MAX-1)/(parmax-parmin)*(f-parmin));
}

float funcion_adaptacion( VectorU& crom )
{
	float x,y,z;
	x = val_gene(crom[0]);
	y = val_gene(crom[1]);
	z = val_gene(crom[2]);
	return EvalExpr( cabfunc, x, y, z );
}

float funcion( float x )
{
	return EvalExpr( cabfunc, x );
}

void init(void)
{
   //glEnable (GL_BLEND);
   //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_FLAT);
   glClearColor (0.0, 0.0, 0.0, 0.0);
}

void menu(int opcion)
{
	char dato[20];
	int  err, auxi;
	float auxf;
//	clrscr();
	/*printf("	M E N U:\n");
	printf("\n1.- 	Funcion a graficar: f=%s", nomfunc);
	printf("\n2.-	Tamano de la poblacion: %d",AG.NumPob);
	printf("\n3.-	Tamano de cada cromosoma: %d",AG.NumCrom);
	printf("\n4.-	Rango de las soluciones: %8.4f, %8.4f",parmin,parmax);
	printf("\n5.-	Permitir individuos repetidos: %d",AG.Repetidos);
	printf("\n6.-	Tamano de la prole: %d",AG.NumProle);
	printf("\n7.-	Probabilidad de los operadores geneticos");
	printf("\n9.-	Probabilidad de mutacion de un gen (Pm): %8.5f",AG.Pm);
	printf("\n10.-	Probabilidad de cruce uniforme de un gen (Pcu): %8.5f",AG.Pcu);
	printf("\n11.-	Numero maximo de iteraciones: %ld",AG.MaxIter);
	printf("\n12.-	Numero de iteraciones entre cada muestra de soluciones: %ld",AG.PasoMost);
	printf("\n13.-	Error maximo permitido: %10.6f",max_err);
	printf("\n14.- 	Rango de graficacion en X: x1=%f, x2=%f",gx1,gx2);
	printf("\n15.- 	Rango de graficacion en Y: y1=%f, y2=%f",gy1,gy2);
	printf("\n16.- 	Tipo de salida: %s",nomtipsal[tipsal]);
	printf("\n17.-	Ejecutar el algoritmo genetico.");
	printf("\n18.-	Estadisticas de la eficiencia del A.G.");
	printf("\n19.-	Salir.");*/
//	printf("\n\nOpcion: ");
//	gets(dato);
//	opcion = atoi(dato);
//	clrscr();
	switch( opcion ){
		case 1: printf("Funcion a graficar: ");
					gets( dato );
					strcpy( nomfunc, " " );
					strcat( nomfunc, dato );
					if( cabfunc!=NULL )
						EliminarExpr( cabfunc );
					cabfunc = ParserExpr( nomfunc, err );
					if( err ){
						printf("\n %s",MgErrorParser[err]);
						getche();
						strcpy(nomfunc,"0");
					}
					break;
		case 2:  printf(" Tamano de la poblacion (np): ");
				 gets(dato);
				 auxi = atoi(dato);
				 AG.asignar_NumPob( auxi );
				 break;
		case 3:  printf(" Tamano de cada cromosoma (ng): ");
				 gets(dato);
				 auxi = atoi(dato);
				 AG.asignar_NumCrom( auxi );
				 break;
		case 4:  printf(" Valor minimo de las soluciones (parmin): ");
				 gets(dato);
				 parmin = atof(dato);
				 printf(" Valor maximo de las soluciones (parmax): ");
				 gets(dato);
				 parmax = atof(dato);
				 break;
		case 5:  AG.asignar_Repetidos( !AG.Repetidos );
				 break;
		case 6:  printf(" Tamano de la prole (Pr): ");
				 gets(dato);
				 auxi = atoi(dato);
				 AG.asignar_NumProle( auxi );
				 break;
		case 7:  do{
//				 clrscr();
				 printf("\n0.-	Probabilidad de mutacion: %8.5f",AG.ProbOper[0]);
				 printf("\n1.-	Probabilidad de cruce de un punto: %8.5f",AG.ProbOper[1]);
				 printf("\n2.-	Probabilidad de cruce uniforme: %8.5f",AG.ProbOper[2]);
				 printf("\n3.-	Probabilidad de cruce de dos puntos: %8.5f",AG.ProbOper[3]);
				 printf("\n4.-	Salir");
				 printf("\n\nOpcion: ");
				 gets(dato);
				 auxi = atoi(dato);
				 if( auxi!=4 ){
					printf("\nProbabilidad: ");
					gets(dato);
					auxf = atof(dato);
					AG.asignar_ProbOper(auxi,auxf);
				 }
				 }while( auxi!=4 );
				 break;
		case 9:  printf(" Probabilidad de mutacion de un gen (Pm): ");
				 gets(dato);
				 auxf = atof(dato);
				 AG.asignar_Pm( auxf );
				 break;
		case 10: printf(" Probabilidad de cruce uniforme de un gen (Pcu): ");
				 gets(dato);
				 auxf = atof(dato);
				 AG.asignar_Pcu( auxf );
				 break;
		case 11: printf(" Numero maximo de iteraciones (max_iter): ");
				 gets(dato);
				 auxi = atoi(dato);
				 AG.asignar_MaxIter( auxi );
				 break;
		case 12: printf(" Numero de iteraciones entre cada muestra de soluciones: ");
				 gets(dato);
				 auxi = atoi(dato);
				 AG.asignar_PasoMost( auxi );
				 break;
		case 13: printf(" Error maximo permitido (max_err): ");
				 gets(dato);
				 max_err = atof(dato);
				 break;
		case 14: printf("\n Limite inferior en X: %f. Nuevo valor: ",gx1);
				 gets(dato);
				 gx1 = atof(dato);
				 printf("\n Limite superior en X: %f. Nuevo valor: ",gx2);
				 gets(dato);
				 gx2 = atof(dato);
				 break;
		case 15: printf("\n Limite inferior en Y: %f. Nuevo valor: ",gy1);
				 gets(dato);
				 gy1 = atof(dato);
				 printf("\n Limite superior en Y: %f. Nuevo valor: ",gy2);
				 gets(dato);
				 gy2 = atof(dato);
				 break;
		case 16: tipsal = (tipsal<1)?tipsal+1:0;
				 break;
		case 17: int i;

				AG.init_poblacion();
			if(tipsal){
				asignar_ventana_grafico(50,20,640-20,480-40);
				asignar_dom_rang_grafico(gx1,gx2,gy1,gy2);
				ajustar_param_grafico();
				//dibujar_rotulo_grafico("ALGORITMO GENETICO","X","F(X)",10,10);
				graficar(funcion,gx1*1,gx2*1,(gx2-gx1)/400,0,1);//WHITE = 0
				AG.asignar_FuncMostrar( salida_graf );
				AG.ejecutar();
				printf("\nMejor indiv: F.adapt= %10.6f",AG.Adaptacion(0));
				printf("\nSoluciones:\n");
				for( i=0; i<AG.NumCrom; i++)
					printf("   %10.6f  ",val_gene(AG.Poblacion(0)[i]) );
				/*getche();*/
			}
			else{
				AG.asignar_FuncMostrar( salida );
				AG.ejecutar();
				printf("\nMejor indiv: F.adapt= %10.6f",AG.Adaptacion(0));
				printf("\nSoluciones:\n");
				for( i=0; i<AG.NumCrom; i++)
					printf("   %10.6f  ",val_gene(AG.Poblacion(0)[i]) );
				getche();
			}
				 break;
		case 18: {
					char nomarch[20];
					int i;
					FILE *fp;
					printf("\nNombre del archivo a crear: ");
					gets( dato );
					strcpy( nomarch, dato );
					printf("\nNumero de corridas del algoritmo genetico:");
					gets(dato);
					numsim = atoi(dato);
					fadap.dimencionar(0,AG.MaxIter-1);
					fadapp.dimencionar(0,AG.MaxIter-1);
					fadap *= 0;
					fadapp *= 0;
					AG.asignar_PasoMost( 1 );
					AG.asignar_FuncMostrar( salida_reg );
					for( i=0; i<numsim; i++ ){
						AG.init_poblacion();
						AG.ejecutar();
					}
					if( (fp=fopen(nomarch,"w")) ){
						fprintf(fp," %d 3 \n",AG.MaxIter);
						for( i=0; i<AG.MaxIter; i++ )
							fprintf(fp," %d  %f %f \n",i,fadap[i],fadapp[i]);
						fclose( fp );
					}
				 }
				 break;
			case 19:
				exit(0);
				break;
	}
}


void salida( int iter )
{
	int i;
	printf("\nIteracion = %d",iter);
	printf("\nMejor indiv: F.adapt= %10.6f",AG.Adaptacion(0));
	printf("\nSoluciones:\n");
	for( i=0; i<AG.NumCrom; i++)
		printf("   %10.6f  ",val_gene(AG.Poblacion(0)[i]) );
	printf("\n");
}

void Display(void)
{
	/*int i;
	glPushMatrix();  
	glColor3ub(0, 255, 0);
	glBegin(GL_POINTS);
	for( i=0; i<AG.NumPob; i++)
		glVertex2i(pto_x(val_gene(AG.Poblacion(i)[0])), pto_y(MinY));
	
	glEnd();
	glPopMatrix();*/
}

void salida_graf( int iter )
{
int i;
//	setcolor(BLACK);
//	line( pto_x(MinX), pto_y(MinY), pto_x(MaxX), pto_y(MinY) );
//	setcolor(WHITE);
//	for( i=0; i<AG.NumPob; i++)
//		putpixel( pto_x(val_gene(AG.Poblacion(i)[0])), pto_y(MinY), WHITE );

	/*glPushMatrix();  
	glColor3ub(255, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2i(pto_x(MinX), pto_y(MinY));
	glVertex2i(pto_x(MaxX), pto_y(MinY));
	glEnd();
	glPopMatrix();*/

	glPushMatrix();  
	glColor3ub(0, 0, 255);
	glBegin(GL_POINTS);
	for( i=0; i<AG.NumPob; i++)
		glVertex2i(pto_x(val_gene(AG.Poblacion(i)[0])), pto_y(MinY));
	
	glEnd();
	glPopMatrix();

}


void salida_reg( int iter )
{
	int i;
	float suma=0;
	fadap[iter] += AG.Adaptacion(0)/numsim;
	for( i=0; i<AG.NumPob; i++){
		suma += AG.Adaptacion(i);
	}
	fadapp[iter] += (suma/AG.NumPob)/numsim;
	printf("\nIteracion = %d",iter);
}

void clrPantalla(void)
{
	//glColorMask(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, w, h, 0.0, -1.0, 1.0);
   clrPantalla();
}

void mouse(int button, int state, int x, int y)
{
  //tbMouse(button, state, x, y);
}



void keyboard(unsigned char key, int x, int y)
{
  switch (key) {

  case 27:
    exit(0);
    break;
  }
  //glutPostRedisplay();
}

int main(int argc, char** argv)
{
	int i,err,opcion;

	max_err=0.0;
	parmin = 0;
	parmax = 30;
	tipsal = 1;
	gx1 = 0;
	gy1 = 0;
	gx2 = 30;
	gy2 = 35;
//	inirandom();


	strcpy(nomfunc," 5*abs(sin(6.28*x/5))+x");
	cabfunc = ParserExpr( nomfunc, err );
	AG.asignar_FuncAdapt( funcion_adaptacion );
	AG.init_poblacion();

  glutInit(&argc, argv);
  glutInitWindowSize(640, 480);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutCreateWindow("Prueba AG");
  glutReshapeFunc(reshape);
  glutDisplayFunc(Display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  init();


    glutCreateMenu(menu);
    glutAddMenuEntry("Numero maximo de iteraciones: %ld", 11);
    glutAddMenuEntry("Numero de iteraciones entre cada muestra de soluciones: %ld", 12);
    glutAddMenuEntry("", -1);
    glutAddMenuEntry("Ejecutar el algoritmo genetico.", 17);
    glutAddMenuEntry("", -1);
    glutAddMenuEntry("Salida", 19);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutMainLoop();

  return 0;
}
