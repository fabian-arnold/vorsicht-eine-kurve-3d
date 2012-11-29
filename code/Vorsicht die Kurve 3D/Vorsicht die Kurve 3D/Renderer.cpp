#include "Input.h"
#include "Renderer.h"
#include <iostream>
#include <Windows.h>
#include <vfw.h>
#include <sstream>
#include <fstream>
#include <gl/glew.h>
#include <gl/glu.h>
#include <vector>
#include "vsShaderLib.h";
using std::vector;

#define WINWIDTH 1000
#define WINHEIGHT 750
GLuint theMotionTex;
using namespace std;
void initMotionBlur()
{
    //Textur in Bildschirmgrösse
    int texSize = WINWIDTH * WINHEIGHT * 3;
    float* motionTex = new float[texSize];
 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    glGenTextures( 1, &theMotionTex );
    glBindTexture( GL_TEXTURE_2D, theMotionTex );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINWIDTH, WINHEIGHT, 0, GL_RGB, GL_FLOAT, motionTex);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );   
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );  
 
    //Modulate-Modus, um Textur mit glColor zu mischen
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
 
 	
    delete [] motionTex;
}



void beginInfoScreen( int winwidth, int winheight )
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WINWIDTH, 0, WINHEIGHT);
 
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}
 
void endInfoScreen()
{
    glPopMatrix();
 
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
 
    glMatrixMode(GL_MODELVIEW); 
}
 
void screenQuad()
{
    beginInfoScreen( WINWIDTH, WINHEIGHT );
        glBegin(GL_QUADS);
            glTexCoord2i( 0, 0 );
            glVertex2i( 0, 0 );
            glTexCoord2i( 1, 0 );
            glVertex2i( WINWIDTH, 0 );
            glTexCoord2i( 1, 1 );
            glVertex2i( WINWIDTH, WINHEIGHT );
            glTexCoord2i( 0, 1 );
            glVertex2i( 0, WINHEIGHT );
        glEnd();
    endInfoScreen();
}

 

 
void beginMotionBlur()
{
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
 
    glDepthMask( false );
 
    //Motion-Faktor als Alpha-Faktor übergeben, Wert abhängig von Framerate ( z.B. 0.98 )
    glColor4d( 1.0f, 1.0f, 1.0f, 0.0f );
 
    //Motion-Textur über Szene blenden
    glBindTexture( GL_TEXTURE_2D, theMotionTex );
    screenQuad();	
 
    glDepthMask( true );
 
    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_2D );
}
 
void endMotionBlur()
{
    //Bildschirm zurück in Textur kopieren
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, theMotionTex );
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, WINWIDTH, WINHEIGHT);
    glDisable( GL_TEXTURE_2D );
}
 
 

/*

Die welt :
	1000 * 1000 * 1000
http://wiki.delphigl.com/index.php/Motion-Blur

*/

VSShaderLib shader;
void SetUpLightning(){
	// Somewhere in the initialization part of your programm
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	// Create light components
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	

	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	

}
void Renderer::Init(){
	glCullFace(GL_BACK);
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "simple.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "simple.frag");
	shader.prepareProgram();
	cout << shader.getAllInfoLogs() << "\n";
	//SetUpLightning();
}



void calc(){
	
}

void drawBox() { 
		// this func just draws a perfectly normal box with some texture coordinates
		glBegin(GL_QUADS);
			// Front Face
			glVertex3f(-1.0f, -1.0f,  1.0f);	
			glVertex3f( 1.0f, -1.0f,  1.0f);
			glVertex3f( 1.0f,  1.0f,  1.0f);	
			glVertex3f(-1.0f,  1.0f,  1.0f);
			// Back Face
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f,  1.0f, -1.0f);
			glVertex3f( 1.0f,  1.0f, -1.0f);	
			glVertex3f( 1.0f, -1.0f, -1.0f);	
			// Top Face
			glVertex3f(-1.0f,  1.0f, -1.0f);
			glVertex3f(-1.0f,  1.0f,  1.0f);	
			glVertex3f( 1.0f,  1.0f,  1.0f);	
			glVertex3f( 1.0f,  1.0f, -1.0f);	
			// Bottom Face
			glVertex3f(-1.0f, -1.0f, -1.0f);	
			glVertex3f( 1.0f, -1.0f, -1.0f);	
			glVertex3f( 1.0f, -1.0f,  1.0f);	
			glVertex3f(-1.0f, -1.0f,  1.0f);	
			// Right face
			glVertex3f( 1.0f, -1.0f, -1.0f);	
			glVertex3f( 1.0f,  1.0f, -1.0f);
			glVertex3f( 1.0f,  1.0f,  1.0f);
			glVertex3f( 1.0f, -1.0f,  1.0f);	
			// Left Face
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f,  1.0f);
			glVertex3f(-1.0f,  1.0f,  1.0f);
			glVertex3f(-1.0f,  1.0f, -1.0f);
		glEnd();
	}

float angle;

struct Vector3
{
	double x;
	double y;
	double z;
};
struct Vector5
{
	double x;
	double y;
	double z;
	double w;
	double v;
};

vector<Vector5> path;

Vector3 position;
Vector3 direction;
double speed= 0.001;

void SetUpCamera(){
	glLoadIdentity();
	gluLookAt(
		position.x, position.y-1, position.z-1, //Position
		position.x+0.5, position.y, position.z+10, //Target
		0, 1, 0);
}


void Renderer::doRender(){
	SetUpCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat qaLightPosition[]	= {.5, 1, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

	// Set material properties
	GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat qaGreen[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaGreen);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);

	// Draw square with many little squares
	glPushMatrix();
	glTranslatef(0, 0, 5);
	//glRotated(angle, 0, 1,0);
	//angle += 0.01;

	path.push_back(position);


	if(GetKeyState('A')& 0x80){
		direction.x += 0.01;
	}
	 if(GetKeyState('D')& 0x80){
		direction.x -= 0.01;
	}

	position.x += cos(direction.x)*speed;
	position.y += sin(direction.x)*speed;

	glUseProgram(shader.getProgramIndex());
	//drawBox();
	//GLUquadric *obj = gluNewQuadric();
	glLineWidth(10.0);
	glBegin(GL_LINE_STRIP);{
	
		for(vector<Vector5>::iterator item =  path.begin(); item !=  path.end(); item++){
			//glVertex3d(item->x, item->y, item->z);
		}

	}glEnd();

	//gluSphere(obj, 0.5, 100,100);
	
	glPopMatrix();
	
}
	