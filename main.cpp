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

Vector3
translatePoint(Vector3 v, float dx, float dy, float dz)
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

void desenhaCena(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Começa a usar a cor azul
    glColor3f(.5, .5, 1);

    glPushMatrix();
        // Move o sistema de coordenadas para a posição onde deseja-se desenhar
        glTranslatef(x, y, 0);
        // Rotaciona o sistema de coordenadas para o ângulo de orientação,
        // no eixo Z (como estamos em 2D, só faz sentido rotacionar em 'z')
        // O ângulo esperado pela glRotate deve estar em graus
        // Os argumentos "0, 0, 1" indicam que a rotação é no eixo Z
        glRotatef(orientacaoEmGraus, 0, 0, 1);

        // Desenha um triângulo na origem
        glBegin(GL_TRIANGLES);
            glVertex2f(-6, -3);
            glVertex2f( 6,  0);
            glVertex2f(-6,  3);
        glEnd();
    glPopMatrix();

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

void desenhaCena1(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Começa a usar a cor azul
    glColor3f(.5, .5, 1);

    glPushMatrix();
        // Move o sistema de coordenadas para a posição onde deseja-se desenhar
        glTranslatef(x, y, 0);
        // Rotaciona o sistema de coordenadas para o ângulo de orientação,
        // no eixo Z (como estamos em 2D, só faz sentido rotacionar em 'z')
        // O ângulo esperado pela glRotate deve estar em graus
        // Os argumentos "0, 0, 1" indicam que a rotação é no eixo Z
        glRotatef(orientacaoEmGraus, 0, 0, 1);

        Vector3 p(-12, -6, 0);

        p = translatePoint(p, 20, 50, 1);

        // Desenha um triângulo na origem
        glBegin(GL_TRIANGLES);
            glVertex3f(p.x, p.y, p.z);
            glVertex3f( 12,  0, 0);
            glVertex3f(-12,  6, 0);
        glEnd();
    glPopMatrix();

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
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
   glOrtho(-100, 100, -100, 100, -1, 1);
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
typedef struct vertice3d Vertice3d;
typedef struct face Face;
typedef struct listaV ListaV;
typedef struct listaF ListaF;

struct vertice {
	double pt1;
	double pt2;
	double pt3;

	Vertice * prox;
};

struct vertice3d {
    Vertice * v1;
    Vertice * v2;
    Vertice * v3;
};

struct face {
    Vertice3d fv1;
    Vertice3d fv2;
    Vertice3d fv3;

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

	aux->fv1.v1 = f->fv1.v1;
	aux->fv2.v2 = f->fv1.v2;
	aux->fv3.v3 = f->fv1.v3;
	aux->prox = NULL;
	lf->fim->prox = aux;
	lf->fim = aux;
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

void ler_obj() {
    ifstream myfile ("C:\\Users\\Nicollas Mattos\\Dropbox\\Faculdade\\6º Semestre (2017.2)\\Computação gráfica - Tiago Sombra\\Trabalho\\trabalho\\modelo.obj", ios::in);
    string line;
    if(myfile.is_open()){
        while(getline(myfile, line)){
            line = line;
            ifstream testfile (line.c_str(), ifstream::in);
            if(testfile.is_open()){
                cout << "O arquivo '"<< line <<"' existe\n";
            }else{
                cout << "O arquivo '" << line << "' nao existe\n";
            }
        }
    }
    /*
    string line;
    ifstream myfile ("example.txt"); // ifstream = padrão ios:in
    if (myfile.is_open())
    {
    while (! myfile.eof() ) //enquanto end of file for false continua
    {
      getline (myfile,line); // como foi aberto em modo texto(padrão)
                             //e não binário(ios::bin) pega cada linha
      cout << line << endl;
    }
    myfile.close();
    }*/

    else cout << "Erro na abertura do arquivo \n";

    myfile.close();

}

void ler_obj_c(ListaV * lv, ListaF * lf) {
    FILE * fp;
	fp = fopen("C:\\Users\\Nicollas Mattos\\Dropbox\\Faculdade\\6º Semestre (2017.2)\\Computação gráfica - Tiago Sombra\\Trabalho\\trabalho\\teste.txt", "r");

    char linha[80];
    while (fgets(linha, sizeof(linha) - 1, fp) != NULL)
    {
        /*double v1, v2, v3;
        sscanf(linha, "%*[^|]|%lf|%lf|%lf[^\0]", &v1, &v2, &v3);
        printf("%lf %lf %lf\n", v1, v2, v3);*/

        Vertice * v = (Vertice *)malloc(sizeof(Vertice));

        sscanf(linha, "%*[^|]|%lf|%lf|%lf[^\0]", &v->pt1, &v->pt2, &v->pt3);

        inserir_ultimoV(lv, v);
        imprimirListaV(lv);
    }
}

// Rotina principal
int main(int argc, char **argv)
{
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    // Abre a janela
    glutCreateWindow("aula prática");

    // Registra callbacks para alguns eventos
    //glutDisplayFunc(desenhaCena);
    //glutDisplayFunc(desenhaCena1);
    //ler_obj();

	ListaV * lv = criarV();
	ListaF * lf = criarF();
    ler_obj_c(lv, lf);

    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    //glutTimerFunc(0, atualiza, 0);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
