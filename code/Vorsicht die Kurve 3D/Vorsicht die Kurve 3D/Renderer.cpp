#include "Renderer.h"
#include <iostream>
#include <Windows.h>
#include <vfw.h>
#include <sstream>
#include <fstream>
#include <gl/glew.h>
#include <gl/glu.h>
#include <vector>
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
int nr = 0;
void RenderToFile(){
	GLint m_viewport[4];
	glGetIntegerv( GL_VIEWPORT, m_viewport );
	int width  = m_viewport[2];
	int height =  m_viewport[3];
	 
	GLuint texture;
	glGenTextures( 1, &texture );
	glBindTexture(GL_TEXTURE_2D, texture);
	 
	// rgb image
	glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_viewport[0],
	        m_viewport[1], m_viewport[2], m_viewport[3],0);
	 
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	BYTE *raw_img = (BYTE*) malloc(sizeof(BYTE) * width * height * 3);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_img);
	//ofstream myfile;
  	//myfile.open ("example.txt");
	//  myfile.
	stringstream ss;
	ss << "image"<< nr<<".crw";
	 FILE * pFile;
    
	 pFile = fopen ( ss.str().c_str() , "wb" );
	 fwrite(raw_img , 1 , sizeof(BYTE) * width * height * 3 , pFile );
	 fclose (pFile);
	 nr++;
	
/*	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			char r,g,b;
			r = raw_img[(x*height*3)+(y*3)+0];
			g = raw_img[(x*height*3)+(y*3)+1];
			b = raw_img[(x*height*3)+(y*3)+2];
			
			myfile << "R: "<< r <<"\n";
			myfile << "G: "<< g <<"\n";
			myfile << "B: "<< b <<"\n";
		}
	}+/
	myfile.close();*/
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
    
    RenderToFile();
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
float scale = 0.0f;
#define SIZE 100
vector<vector<vector<char> > > world;
double angle;
void Renderer::Init(){
	initMotionBlur();
	world.resize(SIZE);
	for(int x = 0; x < SIZE; x++){
		world[x].resize(SIZE);
	    for(int y = 0; y < SIZE; y++){
	    	world[x][y].resize(SIZE);
	    	for(int z = 0; z < SIZE; z++){
	    		if(x%10==0)
	    		world[x][y][z]=1;
	    		else
	    		world[x][y][z]=0;
	    	}
	    	
	    }	
    }
    glEnable( GL_DEPTH_TEST );
    scale = 1.0 /SIZE;
}



void calc(){
	for(int x = 0; x < SIZE; x++){
	    for(int y = 0; y < SIZE; y++){
	    	for(int z = 0; z < SIZE; z++){
	    		//x plus modula operator weil modula sonst nicht richtig berechnet wird
	    		if(world[((x+SIZE)+1)%SIZE][y][z]==1){
	    			world[x][y][z] = 1;
	    		}else{
	    			world[x][y][z] = 0;
				}
	    		
	    		
	    		//world[x][y][z] = 12;
	   		}
	    }	
    }
}


void Renderer::doRender(){
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable(GL_DEPTH);
	glEnable(GL_COLOR);
	glEnable(GL_POINT_SMOOTH);
	
	glPushMatrix ();
	glTranslatef(-scale * 0.5 * SIZE,-scale * 0.5 * SIZE,-2);//in die mitte schieben
	
    //glRotatef (angle, 0.0f, 1.0f, 1.0f);
    angle += 1;
    glPointSize(1.0f);
    glBegin (GL_POINTS);
    
    for(int x = 0; x < SIZE; x++){
	    for(int y = 0; y < SIZE; y++){
	    	for(int z = 0; z < SIZE; z++){
	    		//glColor3f(x/(double)SIZE, z/(double)SIZE, y/(double)SIZE);
	    		if(world[x][y][z]==1){
	    			glVertex3f(x*scale,y*scale,z*scale);
	    		}
	    		
	    		
	    		//world[x][y][z] = 12;
	   		}
	    }	
    }
       
    
    glEnd ();
    glPopMatrix();
    
 	beginMotionBlur();
	endMotionBlur();
  	calc();
 
    
    
	
}
