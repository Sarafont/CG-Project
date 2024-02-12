#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "tinyxml2.h"
#include <map>
#include "structs.h"


std::string path3d;
std::string pathXML;
std::vector<Model> modelos;
std::map<std::string, std::vector<float>> pontosModelo;
std::map<std::string, GLuint> bufferModelo;


// dados para a câmera
float alpha1 = 0.0f, beta1 = 0.0f, raio = 5.0f;
float eyeX = 5.0, eyeY = 5.0, eyeZ = 5.0, //posição da camera
centroX = 0.0, centroY = 0.0, centroZ = 0.0, // coordenadas para onde aponta
upX = 0.0, upY = 1.0, upZ = 0.0; //vetor de orientação
float fov = 45.0f, near = 1.0f, far = 1000.0f; //vetor da projeção
float dx = 0, dy = 0, dz = 0;



//le os pontos armazenados num ficheiro .3d e guarda-os num vetor modelos (em que cada modelo é representado por um vetor<Point>)
std::vector<float> getModel(std::string source) {
    using namespace std;
    float x, y, z;
    vector<float> pontos;
    ifstream file_input(source);
    while (file_input >> x >> y >> z) {
        pontos.push_back(x);
        pontos.push_back(y);
        pontos.push_back(z);
    }
    file_input.close();
    return pontos;
}


//funcao para ler grupos
void lerGrupo(tinyxml2::XMLElement* grupo, std::vector<Transformacao*>transformacoes) {
    std::vector<Transformacao*> tfs = transformacoes;

    while (grupo) {
        transformacoes = tfs;
        tinyxml2::XMLElement* transformacao = grupo->FirstChildElement("transform");
        if (transformacao) {
            for (tinyxml2::XMLElement* t = transformacao->FirstChildElement(); t; t = t->NextSiblingElement()) {
                std::string name = std::string(t->Name());
                if (name == "scale") {
                    float x, y, z;
                    x = atof(t->Attribute("x"));
                    y = atof(t->Attribute("y"));
                    z = atof(t->Attribute("z"));

                    transformacoes.push_back(new Scale(x, y, z));
                }
                else if (name == "translate") {
                    if (t->Attribute("time") == nullptr) {
                        float x, y, z;
                        x = atof(t->Attribute("x"));
                        y = atof(t->Attribute("y"));
                        z = atof(t->Attribute("z"));

                        transformacoes.push_back(new Translacao(x, y, z));

                    }
                    else {
                        float time;
                        std::string align;
                        std::vector<Point> curve;
                        time = atof(t->Attribute("time"));
                        align = t->Attribute("align");

                        for (tinyxml2::XMLElement* p = t->FirstChildElement("point"); p; p = p->NextSiblingElement("point")) {
                            float x, y, z;
                            x = atof(p->Attribute("x"));
                            y = atof(p->Attribute("y"));
                            z = atof(p->Attribute("z"));
                            curve.push_back(Point(x, y, z));
                        }

                        transformacoes.push_back(new Curve(curve, align == "true", time));
                    }



                }
                else if (name == "rotate") {
                    float x, y, z, angle, time;
                    x = atof(t->Attribute("x"));
                    y = atof(t->Attribute("y"));
                    z = atof(t->Attribute("z"));
                    if (t->Attribute("time") == nullptr) {
                        angle = atof(t->Attribute("angle"));
                        transformacoes.push_back(new RotacaoAngulo(angle, x, y, z));
                    }
                    else {
                        time = atof(t->Attribute("time"));
                        transformacoes.push_back(new RotacaoTempo(time, x, y, z));
                    }

                }
                else printf("Transformacao inexistente");
            }
        }

        tinyxml2::XMLElement* models = grupo->FirstChildElement("models");

        if (models) {
            for (tinyxml2::XMLElement* modelo = models->FirstChildElement("model"); modelo; modelo = modelo->NextSiblingElement()) {
                std::string model_path = modelo->Attribute("file");
                if (!pontosModelo.count(model_path)) {
                    std::vector<float> pontos = getModel(path3d + model_path);
                    pontosModelo[model_path] = pontos;

                }

                modelos.push_back(Model(model_path, transformacoes));
            }
        }

        lerGrupo(grupo->FirstChildElement("group"), transformacoes);
        grupo = grupo->NextSiblingElement("group");
    }

}

//funçao para ler ficheiro XML
void lerXML(std::string source) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(source.data());


    tinyxml2::XMLElement* camera = doc.FirstChildElement("world")->FirstChildElement("camera");
    tinyxml2::XMLElement* position = camera->FirstChildElement("position");
    eyeX = atof(position->Attribute("x"));
    eyeY = atof(position->Attribute("y"));
    eyeZ = atof(position->Attribute("z"));

    raio = sqrt(eyeX * eyeX + eyeY * eyeY + eyeZ * eyeZ);
    beta1 = asin(eyeY / raio);
    alpha1 = asin(eyeX / (raio * cos(beta1)));

    tinyxml2::XMLElement* lookAt = camera->FirstChildElement("lookAt");
    centroX = atof(lookAt->Attribute("x"));
    centroY = atof(lookAt->Attribute("y"));
    centroZ = atof(lookAt->Attribute("z"));

    tinyxml2::XMLElement* up = camera->FirstChildElement("up");
    upX = atof(up->Attribute("x"));
    upY = atof(up->Attribute("y"));
    upZ = atof(up->Attribute("z"));

    tinyxml2::XMLElement* projection = camera->FirstChildElement("projection");
    fov = atof(projection->Attribute("fov"));
    near = atof(projection->Attribute("near"));
    far = atof(projection->Attribute("far"));

    tinyxml2::XMLElement* grupo = doc.FirstChildElement("world")->FirstChildElement("group");

    std::vector<Transformacao*> transformacoes;

    lerGrupo(grupo, transformacoes);
}

// calcular a posição da câmera
void posCamera() {
    eyeX = raio * cos(beta1) * sin(alpha1);
    eyeY = raio * sin(beta1);
    eyeZ = raio * cos(beta1) * cos(alpha1);
}


void changeSize(int w, int h) {
    if (h == 0)
        h = 1;

    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(fov, ratio, near, far);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    dx = sin(alpha1) * cos(beta1);
	dy = sin(beta1);
	dz = cos(alpha1) * cos(beta1);
    gluLookAt(eyeX, eyeY, eyeZ,
        centroX + dx, centroY + dy, centroZ + dz,
        upX, upY, upZ);

    glPolygonMode(GL_FRONT, GL_LINE);

    // desenho dos eixos - comentar para o sistema solar
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-raio, 0, 0);
    glVertex3f(raio, 0, 0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, -raio, 0);
    glVertex3f(0, raio, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, -raio);
    glVertex3f(0, 0, raio);

    glEnd();
    glColor3f(1.0, 1.0, 1.0);

    // modelos -> desenha os modelos guardados no vector modelos
    glColor3f(1.0f, 1.0f, 1.0f);   
    for (Model modelo : modelos) {
        modelo.draw();
    }

    // transformações geométricas
    glRotatef(alpha1, 0, 1, 0);
    glTranslatef(eyeX, 0, eyeZ);


    // End of frame
    glutSwapBuffers();
}

// write function to process keyboard events
void onKeyboard(unsigned char key_code, int x, int y) {
    
    if (key_code == 'e' || key_code == 'E') {
        alpha1 += 0.05;
    } else if (key_code == 'q' || key_code == 'Q') {
        alpha1 -= 0.05;
    }
    if (key_code == 'w' || key_code == 'W') {
        raio -= 2;
    }
    else if (key_code == 's' || key_code == 'S') {
        raio += 2;
    }
    else if (key_code == 'a' || key_code == 'A') {
        beta1 -= 0.05;
    }
    else if (key_code == 'd' || key_code == 'D') {
        beta1 += 0.05;
    }
    posCamera();
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    int i = 0;
    //caminhos para os ficheiros 3d e xml
    path3d = "../3d/";
    pathXML = "../xml/";

    posCamera();

    if (argc == 2)
        lerXML(pathXML + argv[1]);
    else
        lerXML(pathXML + "test_1_2.xml");


    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG@DI-UM");

    // Required callback registry 
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    glewInit();

    glEnableClientState(GL_VERTEX_ARRAY);
    GLuint buffers[pontosModelo.size()];
    glGenBuffers(pontosModelo.size(), buffers);

    for (std::pair<std::string, std::vector<float>> pM : pontosModelo) {
        std::string modelo = pM.first;
        std::vector<float> pontos = pM.second;
        bufferModelo[modelo] = buffers[i];
        glBindBuffer(GL_ARRAY_BUFFER,bufferModelo[modelo]);
        glBufferData(GL_ARRAY_BUFFER,pontos.size() * sizeof(float), pontos.data(), GL_STATIC_DRAW);
        i++;
    }

    for(Model & group:modelos){
        group.vertices = bufferModelo[group.model];
        group.verticeCount = pontosModelo[group.model].size();
    }

    // put here the registration of the keyboard callbacks
    glutKeyboardFunc(onKeyboard);

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}

