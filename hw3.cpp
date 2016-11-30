#include <string>
#include <iostream>
#include <math.h>
#include "GLee.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
#include "glinit.h"
#include "utils.h"
#include <vector>
#include <pthread.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_precision.hpp>
void junk() {
  int i;
  i=pthread_getconcurrency();
};

using namespace std;
using namespace glm ;


GLuint gProgramShader;              // program handler for our shader
GLuint gTextureMap;				 	// texture handler for the texture map.

GLuint gVertices, gTexcoords;		// Vertex Buffer Objects for the terrain vertices and texture coordinates.

GLfloat gHeightFactor = 20.0;		// displacement level for the height map. Can be passed to the shader source

int width,height;    // texture width and height
int numVertices; // total number of Vertices to represent the heightMap
int flag = 0 ;

// camera parameters
// such as camera direction, pitch, yaw, camera position, camera's current speed
// should be declared as global variables here
// ....
vec3 cameraPos , cameraGaze , cameraDirection ,up ,cameraRight , cameraUp ,cameraLook, myXCoordinate;
float *textures ,*vertices ;
float speed = 0;

void reshape(int w, int h)
{
    w = w < 1 ? 1 : w;
    h = h < 1 ? 1 : h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	   gluPerspective(45, double(w)/h, 0.1f, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
  mat4 matrix1 ;
  vec3 end ;
  mat4 empty;

    switch(key)
    {
        case 27:	// Escape
            exit(0);
            break;
		case 'o':	// increase height factor of the height map
			gHeightFactor += 1.0f;
			break;
		case 'l':	// decrease height factor of the height map
			gHeightFactor -= 1.0f;
			break;
	// write code below to make
        // adjustments to camera parameters based on user input

        case 'w':	// increase pitch
            matrix1 = rotate( empty , 0.01f, myXCoordinate  );
            end = vec3(matrix1 * vec4(cameraGaze, 1.0f));
            cameraGaze = end;
            break;
        case 's':	// decrease pitch
            matrix1 = rotate( empty , -0.01f, myXCoordinate  );
            end = vec3(matrix1 * vec4(cameraGaze, 1.0f));
            cameraGaze = end;
            break;
        case 'a':
            matrix1 = rotate( empty , 0.01f, vec3(0.0f, 1.0f, 0.0f)  );
            end = vec3(matrix1 * vec4(cameraGaze, 1.0f));
            myXCoordinate = vec3(matrix1 * vec4(myXCoordinate, 1.0f));
            cameraGaze = end;
            break;
        case 'd':	// turn right
            matrix1 = rotate( empty, -0.01f, vec3(0.0f, 1.0f, 0.0f));
            end = vec3(matrix1 * vec4(cameraGaze, 1.0f));
            myXCoordinate = vec3(matrix1 * vec4(myXCoordinate, 1.0f));
            cameraGaze = end;
            break;
        case 'u':	// increase speed
        if(speed < 5)
            speed += 0.1f ;
            break;
        case 'j':	// decrease speed
            if(speed > -5)
              speed -= 0.1f ;
            break;
		case 'x' : // speed equal zero
			speed = 0.0f ;
			break ;
        case 'e':
          flag=0;
          break;
        case 'q':
          flag = 1 ;
          break;
        case 't':
          glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
          break;
       case 'g':
		  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		  break ;
        default:
            break;
    }
}


void init(int *argc, char** argv)
{

    // uncomment the following line if you want to see wireframe terrain
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if (*argc < 2) {
		cout << "Missing argument(s)." << endl
			 << "Usage: ./hw3 <texture file name>"  << endl;

		exit(0);
	}

    initTexture(argv[1],&width,&height);
    initShaders();

    // write code below
    // to initial camera parameters
    // camera is positioned at (x,y,z) = (width/2,width/10,width/4)
    // camera speed is 0
    // camera gaze direction is (0,0,-1)
    // ....
    // ....
    cameraPos = vec3(width/2,width/10,width/4) ;
    cameraGaze = vec3(0.0f,0.0f,-1.0f) ;
    cameraLook = cameraPos+cameraGaze  ;

    myXCoordinate = vec3(1.0f, 0.0f, 0.0f);
    up = vec3(0.0f,0.1f,0.0f) ;
    cameraRight = normalize(cross(up,cameraDirection)) ;
    //cameraUp = cross(cameraDirection,cameraRight) ;
    speed = 0.0f ;
    // initialize the terrain
    glGenBuffers(1, &gVertices);
    glGenBuffers(1, &gTexcoords);

    int numPixels = width*height ,numTextures =0  ; // total number of pixels
    numVertices = 0 ;
    vertices = (float *) malloc((numPixels*18)*(sizeof(float))) ;
    textures = (float *) malloc((numPixels*12)*(sizeof(float))) ;
    // write code below to correctly specify the vertex data for a flat height map and texture coordinate data and send to to GPU by using the vertex buffer object mechanism
    for(int i=0;i<width;i++) {
      for(int j=0;j<height;j++) {
         // taking the texture color from file and put them into textures vector
         // calculating the every vertex and saving them into vertices vector
        vec3 v1 = vec3(i, 0.0, -j);
        vec2 tex1 = vec2((float(i/(float)width)),(float(-j/(float)height))) ;

        vec3 v2 = vec3(i+1,0.0, -j);
        vec2 tex2 = vec2((float((i+1)/(float)width)),(float(-j/(float)height))) ;

        vec3 v3 = vec3(i+1,0.0, -j-1);
        vec2 tex3 = vec2((float((i+1)/(float)width)),(float((-j-1)/(float)height))) ;

        vertices[numVertices++] = v1.x;
        vertices[numVertices++] = v1.y;
        vertices[numVertices++] = v1.z;

        vertices[numVertices++] = v2.x;
        vertices[numVertices++] = v2.y;
        vertices[numVertices++] = v2.z;

        vertices[numVertices++] = v3.x;
        vertices[numVertices++] = v3.y;
        vertices[numVertices++] = v3.z;

        textures[numTextures++] = tex1.x;
        textures[numTextures++] = tex1.y;

        textures[numTextures++] = tex2.x;
        textures[numTextures++] = tex2.y;

        textures[numTextures++] = tex3.x;
        textures[numTextures++] = tex3.y;
        //glColor3f(1,0,0) ;
        vec2 tex4 = vec2((float((i+1)/(float)width)),(float((-j-1)/(float)height))) ;
        vec3 v4 = vec3(i+1, 0.0, -j-1);
        vec3 v5 = vec3(i,0.0,-j-1);
        vec2 tex5 = vec2((float((i)/(float)width)),(float((-j-1)/(float)height))) ;
        vec3 v6 = vec3(i,0.0,-j);
        vec2 tex6 = vec2((float((i)/(float)width)),(float((-j)/(float)height))) ;
        vertices[numVertices++] = v4.x;
        vertices[numVertices++] = v4.y;
        vertices[numVertices++] = v4.z;

        vertices[numVertices++] = v5.x;
        vertices[numVertices++] = v5.y;
        vertices[numVertices++] = v5.z;

        vertices[numVertices++] = v6.x;
        vertices[numVertices++] = v6.y;
        vertices[numVertices++] = v6.z;

        textures[numTextures++] = tex4.x;
        textures[numTextures++] = tex4.y;

        textures[numTextures++] = tex5.x;
        textures[numTextures++] = tex5.y;

        textures[numTextures++] = tex6.x;
        textures[numTextures++] = tex6.y;
      }
    }
    glBindBuffer(GL_ARRAY_BUFFER, gVertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices, vertices, GL_STATIC_DRAW);
    //numVertices /= 3 ;
    glBindBuffer(GL_ARRAY_BUFFER, gTexcoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) *numTextures, textures, GL_STATIC_DRAW);
}

// OpenGL display function called continuously in an infinite loop
void display()
{
    long startTime = getCurrentTime();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glUseProgram(gProgramShader);
    // compute new camera parameters at the begining of the frame and set camera location and gaze
    // .......
    // gluLookAt(.....);
    // Display the Terrain using the written shaders
    cameraLook = cameraPos+cameraGaze;

    gluLookAt(cameraPos.x,cameraPos.y,cameraPos.z,
      cameraLook.x,cameraLook.y,cameraLook.z,
      0,1,0);


    cameraPos = cameraPos+(cameraGaze*speed);


    // the light position is set to the following coordinates
    glUniform4f(glGetUniformLocation(gProgramShader, "lightPosition")   , width/2.0,width+height,height/2.0,1);

    // texture related function calls
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,gTextureMap);
    glEnable(GL_TEXTURE_2D);
    glUniform1i(glGetUniformLocation(gProgramShader, "texture"), 0);
    glUniform1i(glGetUniformLocation(gProgramShader, "width"), width);
    glUniform1i(glGetUniformLocation(gProgramShader, "height"), height);
    glUniform1f(glGetUniformLocation(gProgramShader,"heightFactor"), gHeightFactor);
    glUniform1i(glGetUniformLocation(gProgramShader,"flag"), flag);


    glBindBuffer(GL_ARRAY_BUFFER, gVertices);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gTexcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    // Draw the terrain as triangles
    glDrawArrays(GL_TRIANGLES, 0, numVertices);

    glutSwapBuffers();

    // Set FPS to 100
    long diff = getCurrentTime() - startTime;
    if (diff < 10000)
    usleep(10000 - diff);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("20151 - CEng477 - HW3");

    init(&argc, argv);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(glutPostRedisplay);

    glutMainLoop();

    return 0;
}
