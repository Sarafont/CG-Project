#ifndef ENGINE_STRUCTS_H
#define ENGINE_STRUCTS_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <vector>
#include <math.h>
#include "structs.h"
#include "string.h"
#include <string>


struct Point {
    float x, y, z;
    Point(float x, float y, float z);
};

//Representar um ponto aleatório no espaço


// Transformações
// Translação -> Rotação -> Escala
class Transformacao {
    //aplica a transformação geométrica correspondente 
    // no sistema de coordenadas
public:
    void virtual apply() = 0;
};

class Translacao : public Transformacao {
    float x, y, z;
public:
    Translacao(float x, float y, float z);

    //aplicamos o translate do OpenGL
    void apply();
};


class RotacaoAngulo : public Transformacao {
    float x, y, z, angle;
public:
    RotacaoAngulo(float angle, float x, float y, float z);

    void apply();
};

class RotacaoTempo : public Transformacao {
    float x, y, z, time;
public:
    RotacaoTempo(float time, float x, float y, float z);

    void apply();
};

class Scale : public Transformacao {
    float x, y, z;
public:
    Scale(float x, float y, float z);

    void apply();
};

struct Model {
    std::string model;
    std::vector<Transformacao*> transformacoes1;
    GLuint vertices, verticeCount;
    Model(std::string model, std::vector<Transformacao*> trans);
    void draw();
};

class Curve : public Transformacao {
    float t, time, current_time;
    std::vector<Point> control_points;
    bool align;
    float prev_y[3];
public:
    Curve(std::vector<Point> pontos, bool align, float time);
    void apply();
};

void buildRotMatrix(float* x, float* y, float* z, float* m);


void cross(float* a, float* b, float* res);


void normalize(float* a);


//converter um Ponto(x,y,z) num array [x,y,z]
void pointToArray(Point p, float* point);

void multMatrixVector(float* m, float* v, float* res);

void getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv);

void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv, std::vector<Point> p);

void renderCatmullRomCurve(std::vector<Point> control_points);

#endif