#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "structs.h"


//Representar um ponto aleatorio no espaço
Point::Point(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}



Translacao::Translacao(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
void Translacao::apply() {
    glTranslatef(x, y, z);
}



RotacaoAngulo::RotacaoAngulo(float angle, float x, float y, float z) {
    this->angle = angle;
    this->x = x;
    this->y = y;
    this->z = z;
}
void RotacaoAngulo::apply() {
    glRotatef(angle, x, y, z);
}



RotacaoTempo::RotacaoTempo(float time, float x, float y, float z) {
    this->time = time * 1000;
    this->x = x;
    this->y = y;
    this->z = z;
}
void RotacaoTempo::apply() {
    float angle = glutGet(GLUT_ELAPSED_TIME) * 360 / time;
    glRotatef(angle, x, y, z);
}



Scale::Scale(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
void Scale::apply() {
    glScalef(x, y, z);
}


Curve::Curve(std::vector<Point> pontos, bool align, float time) {
    this->t = 0;
    this->control_points = pontos;
    this->time = time * 1000;
    this->current_time = 0;
    this->align = align;
    this->prev_y[0] = 0;
    this->prev_y[1] = 1;
    this->prev_y[2] = 0;
}
void Curve::apply() {
    float pos[3], deriv[3];
    renderCatmullRomCurve(this->control_points);
    getGlobalCatmullRomPoint(t, pos, deriv, control_points);

    glTranslatef(pos[0], pos[1], pos[2]);

    if (align) {
        float x[3] = { deriv[0],deriv[1],deriv[2] };
        float y[3];
        float z[3];
        float m[16];

        normalize(x);
        cross(x, prev_y, z);
        normalize(z);
        cross(z, x, y);

        normalize(y);
        memcpy(prev_y, y, 3 * sizeof(float));

        buildRotMatrix(x, y, z, m);
        glMultMatrixf(m);
    }

    float newTime = glutGet(GLUT_ELAPSED_TIME);
    float diferencaTempo = newTime - current_time;

    t += diferencaTempo / time;
    current_time = newTime;

}



Model::Model(std::string model, std::vector<Transformacao*> trans) {
    this->model = model;
    //this->pontos = pts;
    this->transformacoes1 = trans;
}
void Model::draw() {
    glPushMatrix();
    for (Transformacao* t : this->transformacoes1) {
        t->apply();
    }
    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, this->verticeCount);
    glPopMatrix();
}



void buildRotMatrix(float* x, float* y, float* z, float* m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float* a, float* b, float* res) {

    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(float* a) {

    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0] / l;
    a[1] = a[1] / l;
    a[2] = a[2] / l;
}


//converter um Ponto(x,y,z) num array [x,y,z]
void pointToArray(Point p, float* point) {
    point[0] = p.x;
    point[1] = p.y;
    point[2] = p.z;
}

void multMatrixVector(float* m, float* v, float* res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }

}

void getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv) {
    //catmull-rom matrix
    float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
                         { 1.0f, -2.5f,  2.0f, -0.5f},
                         {-0.5f,  0.0f,  0.5f,  0.0f},
                         { 0.0f,  1.0f,  0.0f,  0.0f} };

    for (int i = 0; i < 3; i++) {
        float A[4];
        float temp[] = { p0[i],p1[i],p2[i],p3[i] };
        // Compute A = M * P
        multMatrixVector((float*)m, temp, A);

        // Compute pos = T * A
        pos[i] = pow(t, 3) * A[0] + pow(t, 2) * A[1] + t * A[2] + A[3];

        // compute deriv = T' * A
        deriv[i] = 3 * pow(t, 2) * A[0] + 2 * t * A[1] + A[2];

    }

}

void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv, std::vector<Point> p) {
    int POINT_COUNT = p.size();
    float t = gt * POINT_COUNT; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
    indices[1] = (indices[0] + 1) % POINT_COUNT;
    indices[2] = (indices[1] + 1) % POINT_COUNT;
    indices[3] = (indices[2] + 1) % POINT_COUNT;

    //converter de Ponto(x,y,z) para um array [x,y,z];
    float p0[3], p1[3], p2[3], p3[3];
    pointToArray(p[indices[0]], p0);
    pointToArray(p[indices[1]], p1);
    pointToArray(p[indices[2]], p2);
    pointToArray(p[indices[3]], p3);

    getCatmullRomPoint(t, p0, p1, p2, p3, pos, deriv);

}

void renderCatmullRomCurve(std::vector<Point> control_points) {
    float pos[3], deriv[3], segmentos = 100;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segmentos; ++i) {
        getGlobalCatmullRomPoint(1 / segmentos * i, pos, deriv, control_points);

        glVertex3f(pos[0], pos[1], pos[2]);
    }
    glEnd();

}



