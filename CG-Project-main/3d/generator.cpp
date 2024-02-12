#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <math.h>
#include <vector>
#include <cstdio>
#include <cstdlib>


struct Point{
    float x,y,z;
    Point() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    Point(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

void createPlane(float length, float divisions, char* filename) {

    float unidade = length / divisions;
    float x1, x2, z1, z2;

    FILE* fd;
    fd = fopen(filename, "w");

    if (fd != NULL) {
        for (int i = 0; i < divisions; i++) {
            for (int j = 0; j < divisions; j++) {
                x1 = i * unidade - length / 2;
                z1 = j * unidade - length / 2;
                x2 = (i + 1) * unidade - length / 2;
                z2 = (j + 1) * unidade - length / 2;
                
                fprintf(fd, "%f %d %f \n", x1, 0, z1);
                fprintf(fd, "%f %d %f \n", x2, 0, z2);
                fprintf(fd, "%f %d %f \n", x2, 0, z1);

                fprintf(fd, "%f %d %f \n", x1, 0, z1);
                fprintf(fd, "%f %d %f \n", x1, 0, z2);
                fprintf(fd, "%f %d %f \n", x2, 0, z2);
            }
        }
    }
    else {
        printf("Erro a criar ficheiro 3D...\n");
    }

    fclose(fd);
}

void createBox(float length, float divisions, char* filename) {
    float unidade = length / divisions;
    float x1, x2, y1, y2, z1, z2;

    FILE* fd;
    fd = fopen(filename, "w");

    if (fd != NULL) {
        for (int i = 0; i < divisions; i++) {
            for (int j = 0; j < divisions; j++) {
                x1 = i * unidade - length / 2;
                z1 = j * unidade - length / 2;
                x2 = (i + 1) * unidade - length / 2;
                z2 = (j + 1) * unidade - length / 2;

                fprintf(fd, "%f %f %f \n", x1, length / 2, z1);
                fprintf(fd, "%f %f %f \n", x2, length / 2, z2);
                fprintf(fd, "%f %f %f \n", x2, length / 2, z1);

                fprintf(fd, "%f %f %f \n", x1, length / 2, z1);
                fprintf(fd, "%f %f %f \n", x1, length / 2, z2);
                fprintf(fd, "%f %f %f \n", x2, length / 2, z2);

                fprintf(fd, "%f %f %f \n", x2, -(length / 2), z2);
                fprintf(fd, "%f %f %f \n", x1, -(length / 2), z1);
                fprintf(fd, "%f %f %f \n", x2, -(length / 2), z1);

                fprintf(fd, "%f %f %f \n", x1, -(length / 2), z2);
                fprintf(fd, "%f %f %f \n", x1, -(length / 2), z1);
                fprintf(fd, "%f %f %f \n", x2, -(length / 2), z2);
                
            }
        }

        for (int i = 0; i < divisions; i++) {
            for (int j = 0; j < divisions; j++) {
                x1 = i * unidade - length / 2;
                y1 = j * unidade - length / 2;
                x2 = (i + 1) * unidade - length / 2;
                y2 = (j + 1) * unidade - length / 2;

                fprintf(fd, "%f %f %f \n", x2, y2, length / 2);
                fprintf(fd, "%f %f %f \n", x1, y1, length / 2);
                fprintf(fd, "%f %f %f \n", x2, y1, length / 2);

                fprintf(fd, "%f %f %f \n", x1, y2, length / 2);
                fprintf(fd, "%f %f %f \n", x1, y1, length / 2);
                fprintf(fd, "%f %f %f \n", x2, y2, length / 2);

                fprintf(fd, "%f %f %f \n", x1, y1, -(length / 2));
                fprintf(fd, "%f %f %f \n", x2, y2, -(length / 2));
                fprintf(fd, "%f %f %f \n", x2, y1, -(length / 2));

                fprintf(fd, "%f %f %f \n", x1, y1, -(length / 2));
                fprintf(fd, "%f %f %f \n", x1, y2, -(length / 2));
                fprintf(fd, "%f %f %f \n", x2, y2, -(length / 2));
            }
        }

        for (int i = 0; i < divisions; i++) {
            for (int j = 0; j < divisions; j++) {
                z1 = i * unidade - length / 2;
                y1 = j * unidade - length / 2;
                z2 = (i + 1) * unidade - length / 2;
                y2 = (j + 1) * unidade - length / 2;

                fprintf(fd, "%f %f %f \n", length / 2, y1, z1);
                fprintf(fd, "%f %f %f \n", length / 2, y2, z2);
                fprintf(fd, "%f %f %f \n", length / 2, y1, z2);

                fprintf(fd, "%f %f %f \n", length / 2, y1, z1);
                fprintf(fd, "%f %f %f \n", length / 2, y2, z1);
                fprintf(fd, "%f %f %f \n", length / 2, y2, z2);

                fprintf(fd, "%f %f %f \n", -(length / 2), y2, z2);
                fprintf(fd, "%f %f %f \n", -(length / 2), y1, z1);
                fprintf(fd, "%f %f %f \n", -(length / 2), y1, z2);

                fprintf(fd, "%f %f %f \n", -(length / 2), y2, z1);
                fprintf(fd, "%f %f %f \n", -(length / 2), y1, z1);
                fprintf(fd, "%f %f %f \n", -(length / 2), y2, z2);
            }
        }
    }
    else {
        printf("Erro a criar ficheiro 3D...\n");
    }

    fclose(fd);
}

void createSphere(float radius, int slices, int stacks, char* filename) {
    float x1, x2, x3, x4, y1, y2, z1, z2, z3, z4;
    float alpha = (2 * M_PI) / slices;
    float beta = M_PI / stacks;

    FILE* fd;
    fd = fopen(filename, "w");

    if (fd != NULL) {
        for (int i = 0; i < slices; i++) {
            for (int j = 0; j < stacks; j++) {
                x1 = radius * cos(M_PI_2 - beta * j) * sin(alpha * i);
                x2 = radius * cos(M_PI_2 - beta * (j + 1)) * sin(alpha * i);
                x3 = radius * cos(M_PI_2 - beta * (j + 1)) * sin(alpha * (i + 1));
                x4 = radius * cos(M_PI_2 - beta * j) * sin(alpha * (i + 1));

                y1 = radius * sin(M_PI_2 - beta * j);
                y2 = radius * sin(M_PI_2 - beta * (j + 1));

                z1 = radius * cos(M_PI_2 - beta * j) * cos(alpha * i);
                z2 = radius * cos(M_PI_2 - beta * (j + 1)) * cos(alpha * i);
                z3 = radius * cos(M_PI_2 - beta * (j + 1)) * cos(alpha * (i + 1));
                z4 = radius * cos(M_PI_2 - beta * j) * cos(alpha * (i + 1));

                if (j != stacks - 1) {
                    fprintf(fd, "%f %f %f \n", x1, y1, z1);
                    fprintf(fd, "%f %f %f \n", x2, y2, z2);
                    fprintf(fd, "%f %f %f \n", x3, y2, z3);
                }
                if (j != 0) {
                    fprintf(fd, "%f %f %f \n", x1, y1, z1);
                    fprintf(fd, "%f %f %f \n", x3, y2, z3);
                    fprintf(fd, "%f %f %f \n", x4, y1, z4);
                }
            }
        }
    }
    else {
        printf("Erro a criar ficheiro 3D...\n");
    }

    fclose(fd);
}

void createCone(float radius, float height, int slices, int stacks, char * filename){
    float alfa = 2*M_PI / slices;
    float ratio = height/radius;
    float stack_size = height/stacks;
    float x1, x2, x3, x4, y1, y2, z1, z2, z3, z4, h1, h2;

    FILE* fd;
    fd = fopen(filename, "w");

    if (fd != NULL){
        for (int i = 0; i < slices; i++) {
            x1 = radius * sin(alfa * i);
            x2 = radius * sin(alfa * (i+1));
            z1 = radius * cos(alfa * i);
            z2 = radius * cos(alfa * (i+1));

            fprintf(fd, "%f %f %f \n", x1, 0.0, z1);
            fprintf(fd, "%f %f %f \n", 0.0, 0.0, 0.0);
            fprintf(fd, "%f %f %f \n", x2, 0.0 ,z2);
        }

        for (int i = 0; i < stacks; i++) {
            for (int j = 0; j < slices; j++) {
                h1 = height - (i * stack_size);
                h2 = height - ((i+1) * stack_size);

                x1 = (h1 / ratio) * sin(alfa * j);
                x2 = (h1 / ratio) * sin(alfa * (j+1));
                x3 = (h2 / ratio) * sin(alfa * (j+1));
                x4 = (h2 / ratio) * sin(alfa * j);

                y1 = (i * stack_size);
                y2 = (i+1) * stack_size;

                z1 = (h1 / ratio) * cos(alfa * j);
                z2 = (h1 / ratio) * cos(alfa * (j+1));
                z3 = (h2 / ratio) * cos(alfa * (j+1));
                z4 = (h2 / ratio) * cos(alfa * j);

                fprintf(fd, "%f %f %f\n", x1, y1, z1);
                fprintf(fd, "%f %f %f\n", x2, y1, z2);
                fprintf(fd, "%f %f %f\n", x4, y2, z4);

                if (i != stacks - 1) {
                    fprintf(fd, "%f %f %f \n", x4, y2, z4);
                    fprintf(fd, "%f %f %f\n", x2, y1, z2);
                    fprintf(fd, "%f %f %f\n", x3, y2, z3);
                }   
            }         
        } 
        
    } else {
        printf("Erro a criar ficheiro 3D...\n");
    }

    fclose(fd);
}

void createRing(float smallRadius, float bigRadius, int slices, int stacks, char * filename){
    float alfa = (2*M_PI)/stacks;
    float beta = (2*M_PI)/slices;
    float x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;
    bigRadius = (bigRadius + smallRadius)/2;
    smallRadius = bigRadius - smallRadius;

    FILE* fd;
    fd = fopen(filename, "w");
    if(fd != NULL){
        for(int i=0; i<stacks; i++){
            for(int j=0; j<slices; j++){
                x1 = (bigRadius + smallRadius * cos(alfa * i)) * cos(beta * j);
                x2 = (bigRadius + smallRadius * cos(alfa * (i+1))) * cos(beta * j);
                x3 = (bigRadius + smallRadius * cos(alfa * (i+1))) * cos(beta * (j+1));
                x4 = (bigRadius + smallRadius * cos(alfa * i)) * cos(beta * (j+1));

                y1 = smallRadius * sin(alfa * i);
                y2 = smallRadius * sin(alfa * (i+1));

                z1 = (bigRadius + smallRadius * cos(alfa * i)) * sin(beta * j);
                z2 = (bigRadius + smallRadius * cos(alfa * (i+1))) * sin(beta * j);
                z3 = (bigRadius + smallRadius * cos(alfa * (i+1))) * sin(beta * (j+1));
                z4 = (bigRadius + smallRadius * cos(alfa * i)) * sin(beta * (j+1));

                fprintf(fd, "%f %f %f\n", x1, y1, z1);
                fprintf(fd, "%f %f %f\n", x2, y2, z2);
                fprintf(fd, "%f %f %f\n", x4, y1, z4);

                fprintf(fd, "%f %f %f\n", x2, y2, z2);
                fprintf(fd, "%f %f %f\n", x3, y2, z3);
                fprintf(fd, "%f %f %f\n", x4, y1, z4);
            }
        }
    }else{
        printf("Erro a criar ficheiro 3D...\n");
    }
    fclose(fd);
}

void multMatrixVector(float m[4][4], float *v, float *r){
    for(int i=0; i<4; ++i){
        r[i] = 0;
        for(int j=0; j<4; ++j){
            r[i] += v[j] * m[i][j];
        }
    }
}

void multMatrixMatrix(float m1[4][4], float m2[4][4], float r[4][4]){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            r[i][j] = 0;
            for(int k=0; k<4; k++){
                r[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

float B(float u, float v, float m[4][4]){
    float v1[4];
    float v2[4];
    float r;

    v1[0] = powf(v, 3);
    v1[1] = powf(v, 2);
    v1[2] = v;
    v1[3] = 1;

    multMatrixVector(m, v1, v2);
    r = powf(u,3)*v2[0] + powf(u,2)*v2[1] + u*v2[2] + v2[3];

    return r;
}

std::string createSurface(float mx[4][4], float my[4][4], float mz[4][4], int tesselation){
    std::stringstream buffer;
    float x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;
    float levelOfTesselation = 1.0/tesselation;

    
    for(float i=0; i<1; i+=levelOfTesselation){
        for(float j=0; j<1; j+=levelOfTesselation){
            x1 = B(i,j,mx);
            x2 = B(i+levelOfTesselation,j,mx);
            x3 = B(i+levelOfTesselation,j+levelOfTesselation,mx);
            x4 = B(i,j+levelOfTesselation,mx);

            y1 = B(i,j,my);
            y2 = B(i+levelOfTesselation,j,my);
            y3 = B(i+levelOfTesselation,j+levelOfTesselation,my);
            y4 = B(i,j+levelOfTesselation,my);

            z1 = B(i,j,mz);
            z2 = B(i+levelOfTesselation,j,mz);
            z3 = B(i+levelOfTesselation,j+levelOfTesselation,mz);
            z4 = B(i,j+levelOfTesselation,mz);


            buffer << x1 << ' ' << y1 << ' ' << z1 << '\n';
            buffer << x2 << ' ' << y2 << ' ' << z2 << '\n';
            buffer << x4 << ' ' << y4 << ' ' << z4 << '\n';
            
            buffer << x2 << ' ' << y2 << ' ' << z2 << '\n';
            buffer << x3 << ' ' << y3 << ' ' << z3 << '\n';
            buffer << x4 << ' ' << y4 << ' ' << z4 << '\n';          
        }
    }
    return buffer.str();  
}

std::string createBezier(const char input[], float tesselation){
    std::stringstream buffer;
    std::ifstream fp(std::string("../patch/") + input);
    std::vector<std::vector<int>> indices;
    std::vector<Point> points;
    float mx[4][4];
    float my[4][4];
    float mz[4][4];
    int numPatches;
    int numPoints;
    float bezierMat[4][4] = {{-1.0f,  3.0f, -3.0f, 1.0f},
                             { 3.0f, -6.0f,  3.0f, 0.0f},
                             {-3.0f,  3.0f,  0.0f, 0.0f},
                             { 1.0f,  0.0f,  0.0f, 0.0f}};

    fp >> numPatches;

    for(int i=0; i<numPatches; i++){
        std::vector<int> v;
        int p;
        std::string comma;
        for (int j=0; j<16; j++){
            fp >> p;
            if(j!=15)
                fp>>comma;
            v.push_back(p);
        }
        indices.push_back(v);
    }

    fp >> numPoints;

    for(int i=0; i<numPoints; i++){
        float x,y,z;
        std::string comma;
        fp >> x; 
        fp >> comma; 
        fp >> y;
        fp >> comma; 
        fp >> z;
        points.push_back(Point(x,y,z));
    }

    float aux[4][4];

    for(std::vector<int> indice: indices){
        for(int i=0; i<4; i++){
            for(int j = 0; j<4; j++){
                mx[j][i] = points[indice[i*4+j]].x;
                my[j][i] = points[indice[i*4+j]].y;
                mz[j][i] = points[indice[i*4+j]].z;
            }
        }
        
        multMatrixMatrix(bezierMat,mx,aux);
        multMatrixMatrix(aux,bezierMat,mx);

        multMatrixMatrix(bezierMat,my,aux);
        multMatrixMatrix(aux,bezierMat,my);

        multMatrixMatrix(bezierMat,mz,aux);
        multMatrixMatrix(aux,bezierMat,mz);
        
        buffer << createSurface(mx, my, mz, tesselation);
    }

    return buffer.str();
}

void bufferToFile(const char* buffer, const char* filename) {
    FILE* file = std::fopen(filename, "w");
    if (file == nullptr) {
        std::perror("Error opening file");
        std::exit(EXIT_FAILURE);
    }

    std::size_t bytesWritten = std::fwrite(buffer, 1, strlen(buffer), file);
    if (bytesWritten != strlen(buffer)) {
        std::perror("Error writing file");
        std::exit(EXIT_FAILURE);
    }

    std::fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("Falta Parametros\n");
    }
    else {
        if (!strcmp("plane", argv[1])) {
            if (argc != 5) {
                printf("Falta Parametros\n");
            }
            else {
                float length = atof(argv[2]);
                float width = atof(argv[3]);

                char* filename = (char*)malloc(sizeof(char) * 64);
                filename = strdup(argv[4]);
                
                printf("%s \n",argv[4]);
                createPlane(length, width, filename);
            }
        }
        else if (!strcmp("box", argv[1])) {
            if (argc != 5) {
                printf("Falta Parametros\n");
            }
            else {
                float length = atof(argv[2]);
                float divisions = atof(argv[3]);
                

                char* filename = (char*)malloc(sizeof(char) * 64);
                filename = strdup(argv[4]);
                

                
                createBox(length, divisions, filename);
            }
        }
        else if (!strcmp("sphere", argv[1])) {
            if (argc != 6) {
                printf("Falta Parametros\n");
            }
            else {
                float radius = atof(argv[2]);
                float slices = atof(argv[3]);
                float stacks = atof(argv[4]);
                
                char* filename = (char*)malloc(sizeof(char) * 64);
                filename = strdup(argv[5]);



                createSphere(radius, slices, stacks, filename);
            }
        }
        else if (!strcmp("cone", argv[1])) {
            if (argc != 7) {
                printf("Falta Parametros\n");
            }
            else {
                float radius = atof(argv[2]);
                float height = atof(argv[3]);
                float slices = atof(argv[4]);
                float stacks = atof(argv[5]);
                
                char* filename = (char*)malloc(sizeof(char) * 64);
                filename = strdup(argv[6]);


                createCone(radius, height, slices, stacks, filename);
            }
        }
        else if (!strcmp("ring", argv[1])){
            if (argc != 7){
                printf("Falta Parametros\n");
            }
            else{
                float smallRadius = atof(argv[2]);
                float bigRadius = atof(argv[3]);
                float slices = atof(argv[4]);
                float stacks = atof(argv[5]);
                
                char* filename = (char*)malloc(sizeof(char) * 64);
                filename = strdup(argv[6]);


                createRing(smallRadius ,bigRadius ,slices, stacks, filename);
            }
        }
        else if(!strcmp("patch", argv[1])){
            if(argc != 5){
                printf("Falta Parametros\n");
            }
            else{
                char* input = (char*)malloc(sizeof(char) * 64);
                input = strdup(argv[2]);
                float tesselation = atof(argv[3]); 

                char* filename = (char*)malloc(sizeof(char) * 64);
                filename = strdup(argv[4]);

                std::string str = createBezier(input, tesselation);
                const char* buffer = str.c_str();
                bufferToFile(buffer, filename);
            }
        }
    }
    return 0;
}