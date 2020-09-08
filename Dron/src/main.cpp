#include <iostream>
#include <ogl/glew.h>
#include <ogl/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shaders.h"
#include "Model.h"

#include <sstream>
std::string toString(const int &i) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

#define I glm::mat4(1.0)

void funInit();
void funReshape(int w, int h);
void funDisplay();
void funRotar(int key, int x, int y);
void keyFunc(unsigned char key, int x, int y);
void funTimer(int value);
void funZoom(int key,int status, int x, int y);
void SpecialKeyHandler(int key, int x, int y);
void funMotion(int x, int y);

void setLights(glm::mat4 P, glm::mat4 V);
void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);

void drawSuelo (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSuelo2(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAspa  (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawArticulacion(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSoporte(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBrazo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCuerpo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawDron(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCupula(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawPared(glm::mat4 P, glm::mat4 V, glm::mat4 M);

// Shaders
   Shaders shaders;
   
// Modelos
   Model modelPlane;
   Model modelCone;
   Model modelCylinder;
   Model modelSphere;
   
// Texturas
   Texture textureEmissive;
   Texture textureNoEmissive;
   Texture textureAzulejoDiffuse;
   Texture textureAzulejoSpecular;
   Texture textureAzulejoEmisive;
   Texture textureSuelolDiffuse;
   Texture textureSueloSpecular;
   Texture textureSueloNormal;
   Texture textureSueloEmissive;
   
// Viewport
   int w = 600;
   int h = 600;

   float rotZ = 0.0;
   GLint speed = 20;
   float rotHelice = 0.0;
   bool rotar = true; 
   float rotX = 0.0;
   float desY = 0.2;
   float fov = 30.0;
   float desZ = 0.0;
   float desX = 0.0;
   bool helices = true;
   
   float luzDambient = 0.1;
   float luzDdifusa = 0.7;
   float luzDesp = 0.7;
   
   bool luzF = true;
   float luzF1 = 0.2;
   float luzF2 = 0.9;
   
// Camara
   float alphaX =  2.409;
   float alphaY =  0.261;

// Luces y materiales
   #define   NLD 1
   #define   NLP 1
   #define   NLF 1
   Light     lightG;
   Light     lightD[NLD];
   Light     lightP[NLP];
   Light     lightF[NLF];
   Material  matRuby;
   Material  matLuces;
   Material  matGold;
   Material  matBronze;
   Material  matPolishedBronze;
   
   Textures  texLuces;
   Textures  texGold;
   Textures  texAzulejo;
   Textures  texSuelo;
   Textures  texSuelo2;
   
   
int main(int argc, char** argv) {

 // Inicializamos GLUT
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);   
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE); 
    glutInitContextProfile(GLUT_CORE_PROFILE); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Practica3");
    
 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;
    
 // Inicializaciones específicas
    funInit();
    
 // Configuración CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
    glutTimerFunc(speed,funTimer,0);
    glutKeyboardFunc(keyFunc);
    glutMouseFunc(funZoom);
    glutSpecialFunc(SpecialKeyHandler);
    glutMotionFunc(funMotion);
              
 // Bucle principal
    glutMainLoop();
    
    return 0;
}

void funInit() {
      
 // Test de profundidad
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
 // Transparencias
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    
   

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");
    
 // Modelos
    modelPlane.initModel("resources/models/plane.obj");
    modelCone.initModel("resources/models/cone.obj");
    modelCylinder.initModel("resources/models/cylinder.obj");
    modelSphere.initModel("resources/models/sphere.obj");
    
    // Texturas
    textureEmissive.initTexture("resources/textures/img4.png");
    textureNoEmissive.initTexture("resources/textures/img1.png");
    
    textureAzulejoDiffuse.initTexture("resources/textures/img6.png");
    textureAzulejoEmisive.initTexture("resources/textures/img7.png");
    textureAzulejoSpecular.initTexture("resources/textures/img7.png");
    
    textureSueloEmissive.initTexture("resources/textures/img2.png");
    textureSuelolDiffuse.initTexture("resources/textures/img5.png");
    textureSueloSpecular.initTexture("resources/textures/img3.png");
    
    textureSueloNormal.initTexture("resources/textures/img1.png");
    
    
 // Luz ambiental global
    lightG.ambient        = glm::vec3(0.5, 0.5, 0.5);
     
 // Luces direccionales
    lightD[0].direction   = glm::vec3( 0.0,-1.0, 0.0);
    lightD[0].ambient     = glm::vec3( luzDambient, luzDambient, luzDambient);
    lightD[0].diffuse     = glm::vec3( luzDdifusa, luzDdifusa, luzDdifusa);
    lightD[0].specular    = glm::vec3( luzDesp, luzDesp, luzDesp);
    
 // Luces posicionales
    lightP[0].position    = glm::vec3(0.0, 0.2, -1.5);
    lightP[0].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightP[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].specular    = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;    
    
 // Luces focales
    lightF[0].position    = glm::vec3( 3.0,  3.0, -3.0);
    lightF[0].direction   = glm::vec3( 0.0,  0.0,  0.0);
    lightF[0].ambient     = glm::vec3( luzF1,  luzF1,  luzF1);
    lightF[0].diffuse     = glm::vec3( luzF2,  luzF2,  luzF2);
    lightF[0].specular    = glm::vec3( luzF2,  luzF2,  luzF2);
    lightF[0].innerCutOff = 15.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 5.0;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;
	
    
 // Materiales
    matLuces.ambient      = glm::vec4(0.0, 0.0, 0.0, 1.0);
    matLuces.diffuse      = glm::vec4(0.0, 0.0, 0.0, 1.0);
    matLuces.specular     = glm::vec4(0.0, 0.0, 0.0, 1.0);
    matLuces.emissive     = glm::vec4(1.0, 1.0, 1.0, 1.0);
    matLuces.shininess    = 1.0;
	
    matRuby.ambient       = glm::vec4(0.174500, 0.011750, 0.011750, 1.00);
    matRuby.diffuse       = glm::vec4(0.614240, 0.041360, 0.041360, 1.00);
    matRuby.specular      = glm::vec4(0.727811, 0.626959, 0.626959, 1.00);
    matRuby.emissive      = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    matRuby.shininess     = 76.8;  
    
    matGold.ambient       = glm::vec4(0.247250, 0.199500, 0.074500, 1.00);
    matGold.diffuse       = glm::vec4(0.751640, 0.606480, 0.226480, 1.00);
    matGold.specular      = glm::vec4(0.628281, 0.555802, 0.366065, 1.00);
    matGold.emissive      = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    matGold.shininess     = 51.2;
    
    matBronze.ambient       = glm::vec4(0.2125f, 0.1275f, 0.054f, 1.0f );
    matBronze.diffuse       = glm::vec4(0.714f, 0.4284f, 0.18144f, 1.0f);
    matBronze.specular      = glm::vec4(0.393548f, 0.271906f, 0.166721f, 1.0f);
    matBronze.emissive      = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    matBronze.shininess     = 25.6f;;
    
    matPolishedBronze.ambient       = glm::vec4(0.25f,0.148f , 0.06475f , 0.75f);
    matPolishedBronze.diffuse       = glm::vec4(0.4f, 0.2368f, 0.1036f, 0.75f);
    matPolishedBronze.specular      = glm::vec4(0.774597f, 0.458561f, 0.200621f, 0.75f);
    matPolishedBronze.emissive      = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    matPolishedBronze.shininess     = 76.8f;
    
    //Texturas
    
    texAzulejo.diffuse       = textureAzulejoDiffuse.getTexture();
    texAzulejo.specular      = textureAzulejoSpecular.getTexture();
    texAzulejo.emissive      = textureNoEmissive.getTexture();
    texAzulejo.normal        = textureAzulejoEmisive.getTexture();  
    texAzulejo.shininess     = 10.0;
    
    texSuelo.diffuse      = textureSuelolDiffuse.getTexture();  
    texSuelo.specular     = textureSueloSpecular.getTexture();  
    texSuelo.emissive     = textureSueloEmissive.getTexture();
    texSuelo.normal       = textureSueloNormal.getTexture();
    texSuelo.shininess    = 10.0;
    

    texLuces.diffuse       = textureEmissive.getTexture();
    texLuces.specular     = textureEmissive.getTexture(); 
    texLuces.emissive      = textureEmissive.getTexture();
    texLuces.normal        = textureNoEmissive.getTexture();
    texLuces.shininess     = 10.0; 
}


void funReshape(int wnew, int hnew) {
    
 // Configuración del Viewport
    glViewport(0, 0, wnew, hnew);

 // Captura de w y h
    w = wnew;
    h = hnew;
    
}

void funDisplay() {
    
 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
 // Indicamos los shaders a utilizar
    shaders.useShaders();
    
 // Matriz de Proyección P (Perspectiva)
    float fovy   = fov;
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P  = glm::perspective(glm::radians(fov), aspect, nplane, fplane); 

 // Matriz de Vista V (Cámara)
    float x = 10.0*glm::cos(alphaY)*glm::sin(alphaX);
    float y = 10.0*glm::sin(alphaY);
    float z = 10.0*glm::cos(alphaY)*glm::cos(alphaX);
    glm::vec3 pos   (x  , y  , z  );
    glm::vec3 lookat(0.0, 0.0,  0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(pos, lookat, up);  
    shaders.setVec3 ("ucpos",pos);

// Fijamos las luces
    setLights(P,V);
    
 // Dibujamos la escena
    
    drawSuelo(P,V,I);
    drawSuelo2(P,V,I);
    //drawBrazo(P,V,I);
    //drawCuerpo(P,V,I);
    //drawCupula(P,V,T1);
    glm::mat4 T = glm::translate(I, glm::vec3(desX, desY, desZ));
    glm::mat4 R = glm::rotate(I, glm::radians(rotX), glm::vec3(0.0, 1.0, 0.0));
    drawDron(P,V,T*R);
    
    drawPared(P,V,I);
    
 // Intercambiamos los buffers
    glutSwapBuffers();
}
void setLights(glm::mat4 P, glm::mat4 V) {
    
    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);
    
    for(int i=0; i<NLP; i++) {
        glm::mat4 M = glm::scale(glm::translate(I,lightP[i].position),glm::vec3(0.01));
        drawObjectTex(modelSphere,texLuces,P,V,M);
    }

    for(int i=0; i<NLF; i++) {
        glm::mat4 M = glm::scale(glm::translate(I,lightF[i].position),glm::vec3(0.01));
        drawObjectTex(modelSphere,texLuces,P,V,M);
    }    
}

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);
}

void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",false);
    if(textures.normal!=0) shaders.setBool("uNormalMap",true);
    else                   shaders.setBool("uNormalMap",false);
    shaders.setTextures("utextures",textures);
    model.renderModel(GL_FILL);   
}

void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S = glm::scale(I, glm::vec3(2.0, 1.0, 2.0));
    drawObjectTex(modelPlane,texSuelo,P,V,M*S);
}
void drawSuelo2(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S = glm::scale(I, glm::vec3(2.0, 1.0, 2.0));
    glm::mat4 R180 = glm::rotate(I, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
    drawObjectTex(modelPlane,texSuelo,P,V,M*R180*S);
}

void drawAspa(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    
    glm::mat4 S = glm::scale(I, glm::vec3(0.01116484395, 0.06179796046 ,0.03721614651 ));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -2.75455951691, 0.0));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    //2.68700575829 * 2 = 5.37401151658 diametro 0.06/5.37401151658 =0.01116484395 0.2/5.37401151658= 0.03721614651
    //0.3/(2.75455951691+2.09996962547)= 0.06179796046 Altura
    drawObjectMat(modelCone,matRuby,P,V,M*R90*S*T);
}

void drawSoporte(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(0.025, 0.1, 0.025));
    glm::mat4 Ty = glm::translate(I, glm::vec3(0.0, 1.0, 0.0));
    drawObjectMat(modelCylinder,matBronze,P,V,M*S*Ty);  
}

void drawCupula(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S = glm::scale(I, glm::vec3(0.15, 0.15, 0.15));
    glDepthMask(GL_FALSE);
        drawObjectMat(modelSphere,matPolishedBronze,P,V,M*S);
    glDepthMask(GL_TRUE);
}

void drawPared(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S = glm::scale(I, glm::vec3(2.0, 1.0, 2.0));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 R180 = glm::rotate(I, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 Ty = glm::translate(I, glm::vec3(-2.0, 0.0, 0.0));
    
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
       drawObjectMat(modelPlane,matPolishedBronze,P,V,M*Ty*R90*R180*S);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
}

void drawArticulacion(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(0.0375, 0.0375, 0.0375));
    drawObjectTex(modelSphere,texAzulejo,P,V,M*S);   
}

void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    
    glm::mat4 Rz = glm::rotate(I, glm::radians(rotZ), glm::vec3(0.0, 0.0, -1.0));
    M = M * Rz;
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f),glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 R45 = glm::rotate(I, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 Ty = glm::translate(I, glm::vec3(0.0, 0.2, 0.0));
    glm::mat4 R = glm::rotate(I, glm::radians(rotHelice), glm::vec3(0.0, 1.0, 0.0));
    drawArticulacion(P,V,M);
    drawSoporte(P,V,M*R);
    drawAspa(P,V,M*Ty*R45*R);
    drawAspa(P,V,M*Ty*R90*R45*R);
    drawAspa(P,V,M*Ty*R90*R90*R45*R);
    drawAspa(P,V,M*Ty*R90*R90*R90*R45*R);
}

void drawBrazo(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    
    glm::mat4 T = glm::translate(I, glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.5, 0.05, 0.05));
    drawObjectMat(modelCylinder,matGold,P,V,M*S*T*R90);
    
    glm::mat4 Tx = glm::translate(I, glm::vec3(1.0, 0.0, 0.0));
    drawHelice(P,V,M*T);
}

void drawCuerpo (glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(0.25, 0.1, 0.25));
    drawObjectTex(modelSphere,texAzulejo,P,V,M*S);
}

void drawDron(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 R72 = glm::rotate(I, glm::radians(72.0f), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 R = glm::rotate(I, glm::radians(rotZ), glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 T1 = glm::translate(I, glm::vec3(0.0, 0.15, 0.0));
    drawCuerpo(P,V,M);
    drawBrazo(P,V,M);
    drawBrazo(P,V,M*R72);
    drawBrazo(P,V,M*R72*R72);
    drawBrazo(P,V,M*R72*R72*R72);
    drawBrazo(P,V,M*R72*R72*R72*R72);
    drawCupula(P,V,M*T1);
}

void funZoom(int key, int status, int x, int y) {
    switch(key) {
        case 3:  
            fov += 1.0;   
        break;
        case 4: 
            fov -= 1.0;   
        break;
    }
    if (fov >= 60){
        fov = 60;
    }
    if (fov <= 10){
        fov = 10;
    }
    glutPostRedisplay();
}

void keyFunc(unsigned char key, int x, int y){
        switch(key) {
        case 'a' :  
            rotZ += 5.0;   
        break;
        case 'A' : 
            rotZ -= 5.0;   
        break;
        case 'r' :  
            rotX += 5.0;   
        break;
        case 'R' : 
            rotX -= 5.0;   
        break;
        case 'y' :  
            desY += 0.05;   
        break;
        case 'Y' : 
            desY -= 0.05;  
        break;
        case 'd':
            luzDambient -= 0.03;
                if(luzDambient < 0) luzDambient = 0.0;
            luzDdifusa -= 0.15;
                if(luzDdifusa < 0) luzDdifusa = 0.0;
            luzDesp -= 0.15;
                if(luzDesp < 0) luzDesp = 0.0;
        break; 
        case 'D':
            luzDambient += 0.03;
                if(luzDambient > 0.7) luzDambient = 0.7;
            luzDdifusa += 0.15;
                if(luzDdifusa > 0.7) luzDdifusa = 0.7;
            luzDesp += 0.15;
                if(luzDesp > 0.7) luzDesp = 0.7;
        break;
        case 'f':
            luzF = !luzF;
            if (luzF = true){
                luzF1 == 0.2;
                luzF2 == 0.9;
            }
            if (luzF = false){
                luzF1 == 0.0;
                luzF2 == 0.0;
            }
        break;
        case 's':
            helices = !helices;
        break;  
    }
    //Actualizar las variables de luz direccional
        lightD[0].ambient     = glm::vec3( luzDambient, luzDambient, luzDambient);
        lightD[0].diffuse     = glm::vec3( luzDdifusa, luzDdifusa, luzDdifusa);
        lightD[0].specular    = glm::vec3( luzDesp, luzDesp, luzDesp);
        
    
        lightF[0].position    = glm::vec3( 3.0,  3.0, -3.0);
        lightF[0].direction   = glm::vec3( 0.0,  0.0,  0.0);
        lightF[0].ambient     = glm::vec3( luzF1,  luzF1,  luzF1);
        lightF[0].diffuse     = glm::vec3( luzF2,  luzF2,  luzF2);
        lightF[0].specular    = glm::vec3( luzF2,  luzF2,  luzF2);
        lightF[0].innerCutOff = 10.0;
        lightF[0].outerCutOff = lightF[0].innerCutOff + 5.0;
        lightF[0].c0          = 1.000;
        lightF[0].c1          = 0.090;
        lightF[0].c2          = 0.032;
   
    if (desY <= 0.2){
        desY = 0.2;
    }
    if (desY >= 2){
        desY = 2;
    }
    if (rotZ <= 0.0){
        rotZ = 0.0;
    }
    if (rotZ >= 180.0){
        rotZ = 180.0;
    }
}

void SpecialKeyHandler(int key, int x, int y)
{switch(key) {
    case GLUT_KEY_UP:
            desZ -= 0.1;   
        break;
        case GLUT_KEY_DOWN:  
            desZ += 0.1;   
        break;
        case GLUT_KEY_LEFT:  
            desX -= 0.1;   
        break;
        case GLUT_KEY_RIGHT: 
            desX += 0.1;   
        break;
}
    if (desX <= -0.8){
        desX = -0.8;
    }
    if (desX >= 1.8){
        desX = 1.8;
    }
    if (desZ <= -0.8){
        desZ = -0.8;
    }
    if (desZ >= 1.8){
        desZ = 1.8;
    }
   glutPostRedisplay();
}
 
void funTimer(int ignore) {
    if(helices == true){
        rotHelice += 5.0;
    }
    glutPostRedisplay();  
    glutTimerFunc(speed,funTimer,0);
}

void funMotion(int x, int y) {
      
    float pi   = glm::pi<float>();
    float limY = glm::radians( 89.0f);
    
    alphaX = pi*((float)x/(float)w - 0.5f);
    alphaY = pi*(0.5f - (float)y/(float)h);
    if(alphaY<-limY) alphaY = -limY;      
    if(alphaY> limY) alphaY =  limY;
    glutPostRedisplay();

}
