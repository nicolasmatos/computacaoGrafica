#include <GL/glut.h>
#include "Matrices.h"
#include "Vectors.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <string>
using namespace std;

GLfloat orientacaoEmGraus = 0;
GLfloat velocidadeAngular = 0.05;
GLfloat x = 0, y = 0;
const GLfloat velocidadeTangencial = 0.5;

#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)

Vector3 translatePoint(Vector3 v, float dx, float dy, float dz)
{
  std::cout << "3x1 ponto original:\n" << v << std::endl;

 /*constroe e inicializa a matriz*/
  Matrix4 m;
  m.identity();
  std::cout << "4x4 matrix identidade:\n" << m << std::endl;


    /*multiplica a matriz identidade pelo vetor de translação*/
    m.translate(dx, dy, dz);
    std::cout << "4x4 matrix de translação:\n" << m << std::endl;

    /*Vetor resultande da multiplicação*/
    Vector4 vt(v.x, v.y, v.z, 1);
    vt = m*vt;
    std::cout << "4x1 ponto transladado:\n" << vt << std::endl;

    v.set(vt.x, vt.y, vt.z);

    return v;
}

// Inicia algumas variáveis de estado
void inicializa(void)
{
    // cor para limpar a tela
    glClearColor(0, 0, 0, 0);      // preto

    // imprime instruções
    printf("Instrucoes:\n");
    printf("\t+: gira no sentido horario\n");
    printf("\t-: gira no sentido anti-horario\n\n");
}

// Callback de redimensionamento
void redimensiona(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //glOrtho(-100, 100, -100, 100, -1, 1);
   gluPerspective(45, w/h,0.1,5000);
   //gluLookAt(100,20,60, 0,0,0, 0,1,0);
   gluLookAt(100,50,10, 0,0,0, 0,1,0);
   glutPostRedisplay();
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void atualiza(int idx) {
    // O ângulo esperado pelas funções "cos" e "sin" da math.h devem
    // estar em radianos
    GLfloat orientacaoEmRadianos = grausParaRadianos(orientacaoEmGraus);
    x += cos(orientacaoEmRadianos) * velocidadeTangencial;
    y += sin(orientacaoEmRadianos) * velocidadeTangencial;

    orientacaoEmGraus += velocidadeAngular;

    glutPostRedisplay();
    glutTimerFunc(17, atualiza, 0);
}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y)
{
    switch(key)
    {
        // Tecla ESC
        case 27:
            exit(0);
            break;
        case '+':
        case '=':
            velocidadeAngular += 0.5;
            break;
        case '-':
        case '_':
            velocidadeAngular -= 0.5;
            break;
        default:
            break;
    }
}

typedef struct vertice Vertice;
typedef struct face Face;
typedef struct listaV ListaV;
typedef struct listaF ListaF;

struct vertice {
	double pt1;
	double pt2;
	double pt3;

	Vertice * prox;
};

struct face {
    Vertice * v1, * v2, * v3, * v4;
    int vt1, vt2, vt3, vt4;
    int vn1, vn2, vn3, vn4;

    Face * prox;
};

struct listaV {
	int tam;
	Vertice * ini, * fim;
};

struct listaF {
	int tam;
	Face * ini, * fim;
};

ListaV * criarV() {
	ListaV * lv = (ListaV *)malloc(sizeof(ListaV));
	lv->ini = NULL;
	lv->fim = NULL;
	lv->tam = 0;
	return lv;
}

ListaF * criarF() {
	ListaF * lf = (ListaF *)malloc(sizeof(ListaF));
	lf->ini = NULL;
	lf->fim = NULL;
	lf->tam = 0;
	return lf;
}

void inserir_ultimoV(ListaV * lv, Vertice * v) {
	Vertice * aux = (Vertice *)malloc(sizeof(Vertice));

	aux->pt1 = v->pt1;
	aux->pt2 = v->pt2;
	aux->pt3 = v->pt3;
	aux->prox = NULL;
	if (lv->fim == NULL) {
        lv->ini = aux;
        lv->ini->prox = lv->fim;
        lv->fim = aux;
	}
	else {
        lv->fim->prox = aux;
        lv->fim = aux;
	}
	lv->tam++;
}

void inserir_ultimoF(ListaF * lf, Face * f) {
	Face * aux = (Face *)malloc(sizeof(Face));

	aux->v1 = f->v1;
    aux->vt1 = f->vt1;
    aux->vn1 = f->vn1;

    aux->v2 = f->v2;
    aux->vt2 = f->vt2;
    aux->vn2 = f->vn2;

    aux->v3 = f->v3;
    aux->vt3 = f->vt3;
    aux->vn3 = f->vn3;

    aux->v4 = f->v4;
    aux->vt4 = f->vt4;
    aux->vn4 = f->vn4;
	aux->prox = NULL;
	if (lf->fim == NULL) {
        lf->ini = aux;
        lf->ini->prox = lf->fim;
        lf->fim = aux;
	}
	else {
        lf->fim->prox = aux;
        lf->fim = aux;
	}
	lf->tam++;
}

void imprimirListaV(ListaV * lv) {
	Vertice * aux = lv->ini;
	printf("[ ");
	while (aux != NULL) {
		printf("(");
		printf("%lf ", aux->pt1);
		printf("%lf ", aux->pt2);
		printf("%lf ", aux->pt3);
		printf(")");
		aux = aux->prox;
	}
	printf("]\n");
}

void imprimirListaF(ListaF * lf) {
	Face * aux = lf->ini;
	printf("[ ");
	while (aux != NULL) {
		printf("{");
		printf("(%lf, %lf, %lf)", aux->v1->pt1, aux->v1->pt2, aux->v1->pt3);
		printf("/%d", aux->vt1);
		printf("/%d", aux->vn1);
		printf("}");

		printf(" | {");
		printf("(%lf, %lf, %lf)", aux->v2->pt1, aux->v2->pt2, aux->v2->pt3);
		printf("|%d", aux->vt2);
		printf("|%d", aux->vn2);
		printf("}");

		printf(" | {");
		printf("(%lf, %lf, %lf)", aux->v3->pt1, aux->v3->pt2, aux->v3->pt3);
		printf("|%d", aux->vt3);
		printf("|%d", aux->vn3);
		printf("}");

		printf(" | {");
		printf("(%lf, %lf, %lf)", aux->v4->pt1, aux->v4->pt2, aux->v4->pt3);
		printf("|%d", aux->vt4);
		printf("|%d", aux->vn4);
		printf("}");
		aux = aux->prox;
	}
	printf("]\n");
}

Vertice * buscarV(ListaV * lv, int v) {
	int i = 1;
	Vertice * aux = lv->ini;

	while (aux != NULL && i < v) {
		aux = aux->prox;
		i++;
	}

	return aux;
}

void ler_obj(ListaV * lv, ListaF * lf) {
    FILE * fp;
	fp = fopen("C:\\Dev\\C-C++\\computacaoGrafica\\modelo.txt", "r");

    char linha[80];
    while (fgets(linha, sizeof(linha) - 1, fp) != NULL)
    {
        /*double v1, v2, v3;
        sscanf(linha, "%*[^|]|%lf|%lf|%lf[^\0]", &v1, &v2, &v3);
        printf("%lf %lf %lf\n", v1, v2, v3);*/

        if (linha[0] == 'v' && linha[1] != 't' && linha[1] != 'n') {
            Vertice * v = (Vertice *)malloc(sizeof(Vertice));

            sscanf(linha, "%*[^|]|%lf|%lf|%lf[^\0]", &v->pt1, &v->pt2, &v->pt3);

            inserir_ultimoV(lv, v);
        }
        if (linha[0] == 'f') {
            Face * f = (Face *)malloc(sizeof(Face));

            int pt1_1, pt1_2, pt1_3, pt2_1, pt2_2, pt2_3, pt3_1, pt3_2, pt3_3, pt4_1, pt4_2, pt4_3;

            sscanf(linha, "%*[^|]|%d/%d/%d|%d/%d/%d|%d/%d/%d|%d/%d/%d[^\0]", &pt1_1, &pt1_2, &pt1_3, &pt2_1, &pt2_2, &pt2_3, &pt3_1, &pt3_2, &pt3_3, &pt4_1, &pt4_2, &pt4_3);
            //printf("%d/%d/%d|%d/%d/%d|%d/%d/%d|%d/%d/%d\n",pt1_1, pt1_2, pt1_3, pt2_1, pt2_2, pt2_3, pt3_1, pt3_2, pt3_3, pt4_1, pt4_2, pt4_3);

            Vertice * v1 = buscarV(lv, pt1_1);
            f->v1 = v1;
            f->vt1 = pt1_2;
            f->vn1 = pt1_3;

            Vertice * v2 = buscarV(lv, pt2_1);
            f->v2 = v2;
            f->vt2 = pt2_2;
            f->vn2 = pt2_3;

            Vertice * v3 = buscarV(lv, pt3_1);
            f->v3 = v3;
            f->vt3 = pt3_2;
            f->vn3 = pt3_3;

            Vertice * v4 = buscarV(lv, pt4_1);
            f->v4 = v4;
            f->vt4 = pt4_2;
            f->vn4 = pt4_3;

            inserir_ultimoF(lf, f);
        }
    }
}

ListaV * lv = criarV();
ListaF * lf = criarF();
//imprimirListaV(lv);
//imprimirListaF(lf);

void desenhaObj(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Começa a usar a cor azul
    glColor3f(.5, .5, 1);

    glPushMatrix();
        // Move o sistema de coordenadas para a posição onde deseja-se desenhar
        glTranslatef(x, y, 0);

        glBegin (GL_LINE_LOOP); // Comece a desenhar uma linha primitiva
            Face * aux = lf->ini;
            while (aux != NULL) {
                glVertex3f (aux->v1->pt1, aux->v1->pt2, aux->v1->pt3); // O canto inferior esquerdo
                glVertex3f (aux->v2->pt1, aux->v2->pt2, aux->v2->pt3); // O canto inferior esquerdo
                glVertex3f (aux->v3->pt1, aux->v3->pt2, aux->v3->pt3); // O canto inferior esquerdo
                glVertex3f (aux->v4->pt1, aux->v4->pt2, aux->v4->pt3); // O canto inferior esquerdo

                aux = aux->prox;
            }
        glEnd ();

    glPopMatrix();

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

// Rotina principal
int main(int argc, char **argv)
{
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(300, 0);

    // Abre a janela
    glutCreateWindow("Trabalho de computação gráfica");

    //Chama a função que ler o arquivo obj e aramazena as informações em memória
    ler_obj(lv, lf);

    //Desenha o obj lido na tela
    glutDisplayFunc(desenhaObj);

    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    //glutTimerFunc(0, atualiza, 0);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
