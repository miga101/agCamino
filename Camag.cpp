#include <GL/glut.h>
#include "Gencam.hpp"
#include "arcdat01.hpp"
#include "OpGLdat.hpp"
#include "parser02.hpp"


double cmX = 5.0;//-1
double cmY = 5.0;//0.5
double cmZ = 25.0;//1
double cmtX = 0;
double cmtY = 0.0;
double cmtZ = 0.0;



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

void clrPantalla(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void intTripBits(unsigned int numero, unsigned int *cam)
{
int i,bit,bit2,bit3,octal,masc,j;
	masc = 1;
	j=-1;
	//scanf("%d",&numero);
	for(i=0;i+3<16;i+=3){
		bit =  numero>>(15-i);
		bit =  bit & masc;
		bit2 = numero>>(14-i);
		bit2 = bit2 & masc;
		bit3 = numero>>(13-i);
		bit3 = bit3 & masc;
		octal = bit3 + 2*bit2 + bit;
		cam[j+=1] =  octal;
		//printf("%d",octal);
	}
}	// devuelve 5 octales o 5 dir porque un ENTERO tiene 16bits entre (tripletas3) = ~5

int ExistePos(int x, int y)
{
int i,ex;
	ex = -1;
	for(i=0;((i<Smec)&&(ex!=1));i++){
		if ((Mec[i].x == x) && (Mec[i].y == y))
			ex = i;
	}
	return ex;
}	

void dibujar_Linea(int x, int y, int x2, int y2)
{
int i;
	glPushMatrix();  
	glColor3ub(0, 0, 255);
	glBegin(GL_LINE_STRIP);
	 glVertex2i(x, y);
	 glVertex2i(x2, y2);
	glEnd();
	glPopMatrix(); 

  glutSwapBuffers();
  //glutPostRedisplay();

}

int pasoValido(int nodo, int sentido)
{
int v;
	v = -1;
	switch( sentido ){
		case 0: //N
			if ((v = ExistePos(Mec[nodo].x, Mec[nodo].y + Esp_pos))!= -1){
				//dibujar_Linea(Mec[nodo].x, Mec[nodo].y, Mec[nodo].x, Mec[nodo].y + Esp_pos);
				//v = nodo - Lmec;// id desplazado hacia atras Lmes veces al nodo
			}
				break;
		case 1: //NE
			if ((v = ExistePos(Mec[nodo].x + Esp_pos, Mec[nodo].y + Esp_pos))!= -1){
				//dibujar_Linea(Mec[nodo].x, Mec[nodo].y, Mec[nodo].x + Esp_pos, Mec[nodo].y + Esp_pos);
				//v = nodo - (Lmec - 1);// id desplazado hacia atras Lmes-1 veces al nodo
			}
				break;
		case 2: //E
			if ((v = ExistePos(Mec[nodo].x + Esp_pos, Mec[nodo].y))!= -1){
				//dibujar_Linea(Mec[nodo].x, Mec[nodo].y, Mec[nodo].x + Esp_pos, Mec[nodo].y);
				//v = nodo + 1;// id siguiente inmediato al nodo
			}
				break;
		case 3: //SE
			if ((v = ExistePos(Mec[nodo].x + Esp_pos, Mec[nodo].y - Esp_pos))!= -1){
				//dibujar_Linea(Mec[nodo].x, Mec[nodo].y, Mec[nodo].x + Esp_pos, Mec[nodo].y - Esp_pos);
				//v = nodo + (Lmec + 1);// id desplazado hacia adelante Lmes+1 veces al nodo
			}
				break;
		case 4: //S
			if ((v = ExistePos(Mec[nodo].x, Mec[nodo].y - Esp_pos))!= -1){
				//dibujar_Linea(Mec[nodo].x, Mec[nodo].y, Mec[nodo].x, Mec[nodo].y - Esp_pos);
				//v = nodo + Lmec;// id desplazado hacia adelante Lmes veces al nodo
			}
				break;
		case 5: //SO
			if ((v = ExistePos(Mec[nodo].x - Esp_pos , Mec[nodo].y - Esp_pos))!= -1){
				//dibujar_Linea(Mec[nodo].x, Mec[nodo].y, Mec[nodo].x - Esp_pos, Mec[nodo].y - Esp_pos);
				//v = nodo + (Lmec - 1);// id desplazado hacia adelante Lmes-1 veces al nodo
			}
				break;
		case 6: //O
			if ((v = ExistePos(Mec[nodo].x - Esp_pos, Mec[nodo].y))!= -1){
				//dibujar_Linea(Mec[nodo].x, Mec[nodo].y, Mec[nodo].x - Esp_pos, Mec[nodo].y);
				//v = nodo - 1;// id siguiente inmediato al nodo
			}
				break;
		case 7: //NO
			if ((v = ExistePos(Mec[nodo].x - Esp_pos, Mec[nodo].y + Esp_pos))!= -1){
				//dibujar_Linea(Mec[nodo].x, Mec[nodo].y, Mec[nodo].x - Esp_pos, Mec[nodo].y + Esp_pos);
				//v = nodo - (Lmec + 1);// id desplazado hacia atras Lmes-1 veces al nodo
			}
				break;
	}

	return v;
}

float funcion_adaptacion( VectorU& Cro_gen )
{
int cali, i, j, IDpaso, idNodoAct, numPasos;
unsigned int cami[5];//5 pasos juntos
	cali = 0;
	idNodoAct = 0;
	numPasos = 0;
	for (i=0;((i<AG.NumgCrom)&&(cali!=-1000)&&(cali!=1000));i++){	//5 diviciones de enteros (16bits c/u) de 
		intTripBits(Cro_gen[i], cami);								//5 tripletas de bits del cromo tamano 5x5x3 = 75 bits
		for (j=0;((j<5)&&(cali!=-1000)&&(cali!=1000));j++){		//de 5 en 5 nodos
			if ((IDpaso = pasoValido(idNodoAct, cami[j])) != -1){
				if (Mec[IDpaso].estado){
					if (strcmp("fin", Mec[IDpaso].nomb)== 0){
						cali =  1000;}//fin
					else
						cali = cali + 5;//rojo
				}
				else
					cali = -1000;//verde
				dibujar_Linea(Mec[idNodoAct].x, Mec[idNodoAct].y, Mec[IDpaso].x, Mec[IDpaso].y);
				idNodoAct = IDpaso;
			}else
				cali = -1000;//invalida
			numPasos+=j;
		}
	}
 clrPantalla();//
 Dibujar_Mec();
 glutSwapBuffers();
 return cali-numPasos*4 ;
}

void dibujar_Camino(VectorU& Cro_gen)
{
int i, j, IDpaso, idNodoAct, fin;
idNodoAct = 0;
fin = 0;
unsigned int cami[5];
	for (i=0;((i<AG.NumgCrom)&&(fin!=1));i++){
		intTripBits(Cro_gen[i], cami);
		for (j=0;((j<5)&&(fin!=1));j++){
			IDpaso = pasoValido(idNodoAct, cami[j]);
dibujar_Linea(Mec[idNodoAct].x, Mec[idNodoAct].y, Mec[IDpaso].x, Mec[IDpaso].y);
			idNodoAct = IDpaso;
			if (strcmp("fin", Mec[IDpaso].nomb)== 0) fin = 1;
		}
	}
}

void init(void)
{
  //glShadeModel (GL_FLAT);
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  inicio_grafico();
}

void menu(int opcion)
{
	char dato[20];
	int  err, auxi;
	float auxf;
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
				 AG.asignar_NumgCrom( auxi );
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
		case 17: 
				int i;
				AG.init_poblacion();
				asignar_ventana_grafico(50,20,640-20,480-40);
				asignar_dom_rang_grafico(gx1,gx2,gy1,gy2);
				ajustar_param_grafico();
				AG.asignar_FuncMostrar( salida_graf );
				AG.ejecutar();
				dibujar_Camino(AG.Pob[0]);
				printf("\nMejor indiv: F.adapt= %10.6f",AG.Adaptacion(0));
				printf("\nSoluciones:\n");
				for( i=0; i<AG.NumgCrom; i++)
					printf("   %10.6f  ",val_gene(AG.Poblacion(0)[i]) );
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
				//exit(0);
					clrPantalla();
					init();
					 Dibujar_Mec();
				break;
	}
}

void salida( int iter )
{
	int i;
	printf("\nIteracion = %d",iter);
	printf("\nMejor indiv: F.adapt= %10.6f",AG.Adaptacion(0));
	printf("\nSoluciones:\n");
	for( i=0; i<AG.NumgCrom; i++)
		printf("   %10.6f  ",val_gene(AG.Poblacion(0)[i]) );
	printf("\n");
}

void Display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glLoadIdentity ();            
  gluLookAt (cmX, cmY, cmZ, cmtX, cmtY, cmtZ, 0.0, 1.0, 0.0);

  Dibujar_Mec();
  //glutSolidCube(2);

  glutSwapBuffers();
  //glutPostRedisplay();

}

void salida_graf( int iter )
{
int i;
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

/*
void init(void)
{
  tbInit(GLUT_LEFT_BUTTON);
  models();
  lists();
  glClearColor(0.30, 0.28, 0.30, 0.0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_diffuseAmb);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
}*/

void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, 1.0, 1, 80.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
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
	AG.asignar_FuncAdapt( funcion_adaptacion );
	AG.asignar_MaxIter(100);
	AG.asignar_NumPob(40);
	AG.asignar_NumProle(10);//hijos
	AG.asignar_NumgCrom(Lmec);//tamano del cromo o individuo en enteros	
	AG.init_poblacion();
	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE );
	glutCreateWindow("Prueba AG");
	glutReshapeFunc(reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	init();
    glutCreateMenu(menu);
    glutAddMenuEntry("Rando Matriz Espacial", 19);
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
