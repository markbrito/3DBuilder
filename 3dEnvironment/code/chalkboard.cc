#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "chalkboard.h"
#include "AscFile.h"
#include "list.h"

#define MAX_SPEED 10
#define PI 3.14

int objFan;
int objHooverCraft;
int objScene;
float speed=0;
float directionangle=0;
float fanangle=0;
float xpos=0;
float ypos=0;


static GLuint texName;
static GLuint texMetal;
static GLuint texTire;

List *ChalkBoard::objects;
ChalkBoard *ChalkBoard::instance;
///CollisionDetector *ChalkBoard::colldetector;

ChalkBoard::ChalkBoard(int argc, char **argv,int windowx,int windowy,
		       char *title,int x,int y,int width,int height,int z,int zdepth,List *objs)
{
  WINDOW_TITLE=strdup(title);
  X_START=x;
  Y_START=y;
  ChalkBoard::objects=objs;
    //  ChalkBoard::colldetector=new CollisionDetector(ChalkBoard::objects);

  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowSize(windowx,windowy);
  //  glutInitWindowPosition(100,100);
  glutCreateWindow(this->WINDOW_TITLE);
  glClearColor(0.0,0.0,0.0,0.0);
  glShadeModel(GL_SMOOTH);

   GLfloat light0Pos[4] = { 10.70F, 10.70F, 11.25F, 1.0F };
   GLfloat matAmb[4] = { 0.5F, 0.5F, 0.5F, .50F };
   GLfloat matDiff[4] = { 0.5F, 0.5F, 0.50F, .5F };
   GLfloat matSpec[4] = { 0.50F, 0.50F, 0.50F, .50F };
   GLfloat matShine = 20.00F;
   glEnable(GL_NORMALIZE);
 
   glEnable( GL_LIGHTING );
   glEnable( GL_LIGHT0 );
   glEnable(GL_COLOR_MATERIAL);
	 
   glEnable(GL_DEPTH_TEST);
		
   glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
   glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);

   glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialf(GL_FRONT, GL_SHININESS, matShine);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_2D);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
GLuint *textures = new GLuint[3];
  glGenTextures( 3, textures);
   //
   //   glCullFace(GL_BACK);
   //
   //   glEnable( GL_CULL_FACE );

  glViewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // glOrtho(-1.0,1.0,-1.0,1.0,-1.0,1.0);
  // glOrtho((float)x,(float)x+width,(float)y,(float)y+height,z,zdepth);
  //glFrustum((float)x,(float)x+width,(float)y,(float)y+height,zdepth,zdepth);

  gluPerspective(60.0,(float)width/(float)height,1,1300);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(-23,110,15,-23,0,15,0,0,1);
    //gluLookAt(-8,10,1,-8,0,1,0,0,1);
    //glLoadIdentity();
    //  glTranslatef(0,50,0);
    //  glRotatef(180.0,0.0,.0,1.0);
    //  glScalef(.3,.3,.3);


  objFan=glGenLists(1);
  objHooverCraft=glGenLists(1);
  objScene=glGenLists(1);

  glNewList(objHooverCraft,GL_COMPILE);
	glColor3f(1.0,0.0,0.0);
  int i;
  for(i=0;i<objects->get_count();i++)
    {
      char *name=((A3dObject *)objects->get_item(i))->get_name();
      if(strcmp(name,"Body")==0)
	{
	  int texMetalWidth=276;
	  int texMetalHeight=183;
	  texMetal = LoadTextureRAW( "metal.rgb",
				     texMetalWidth,
				     texMetalHeight);
	  if(!texMetal) 
	    {
	      printf("\nError reading texture\n");
	      return;
	    }
	  glBindTexture(GL_TEXTURE_2D, textures[0]);
	  ((A3dObject *)objects->get_item(i))->draw_object();

	}
      else if(strcmp(name,"Tube")==0)
	{
	  int texTireWidth=225;
	  int texTireHeight=225;
	  texTire = LoadTextureRAW( "tire.rgb",
				     texTireWidth,
				    texTireHeight);
	  if(!texTire) 
	    {
	      printf("\nError reading texture\n");
	      return;
	    }
	  glBindTexture(GL_TEXTURE_2D, textures[1]);
	  ((A3dObject *)objects->get_item(i))->draw_object();
	}
    }
  glEndList();

  glNewList(objScene,GL_COMPILE);
  glColor3f(0.0,1.0,0.0);
  for(i=0;i<objects->get_count();i++)
    {
      char *name=((A3dObject *)objects->get_item(i))->get_name();
      if(strcmp(name,"Floor")==0)
	{
	  int texImageWidth=2000;
	  int texImageHeight=1500;
	  texName = LoadTextureRAW( "wood.rgb",
				     texImageWidth,
				    texImageHeight);
	  if(!texName) 
	    {
	      printf("\nError reading texture\n");
	      return;
	    }
	  glBindTexture(GL_TEXTURE_2D, textures[2]);
	  ((A3dObject *)objects->get_item(i))->draw_object();

	}
      else if(strcmp(name,"Track")==0 ||strcmp(name,"MarkB")==0  
	      ||strcmp(name,"Ramp01")==0  || strcmp(name,"Ramp02")==0)
	{
	  //      	((A3dObject *)objects->get_item(i))->draw_object();
	}
	else
	  {
            if(strcmp(name,"Tube")==0 || 
	       ( strncmp(((A3dObject *)objects->get_item(i))->get_name(),"propeller",9)==0) || strcmp(name,"Body")==0)
	      {
		;
	      }
	    else 
	      {
		((A3dObject *)objects->get_item(i))->draw_object();
	      }
	  }
    }
  glEndList();

  glNewList(objFan,GL_COMPILE);
	glColor3f(0.0,0.0,1.0);
  for(i=0;i<objects->get_count();i++)
    if(strncmp(((A3dObject *)objects->get_item(i))->get_name(),"propeller",9)==0)
      ((A3dObject *)objects->get_item(i))->draw_object();
  glEndList();


 
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutDisplayFunc(display);
  glutIdleFunc(animate);
}

void ChalkBoard::animate(void)
{
  fanangle++;
  if(fanangle>=360) fanangle=0;
  glutPostRedisplay();
}

void ChalkBoard::keyboard(unsigned char key, int x, int y)
{
  
	switch(key)
    {
    case 's':
      (ChalkBoard::instance)->WriteFile(strcat(strcat("ScreenShot",__TIME__),".ppm"));
      break;
	case '5':
      glMatrixMode(GL_MODELVIEW);
      glRotatef(-5,1.0,0.0,0.0);
      break;
    case '/':
      glMatrixMode(GL_MODELVIEW);
      glRotatef(5,1.0,0.0,0.0);
      break;
    case '7':
      glMatrixMode(GL_MODELVIEW);
      glRotatef(5,0.0,1.0,0.0);
      break;
    case '9':
      glMatrixMode(GL_MODELVIEW);
      glRotatef(-5,0.0,1.0,0.0);
      break;

    case '-':
      glMatrixMode(GL_MODELVIEW);
      glScalef(1.0/1.2,1.0/1.2,1.0/1.2);
      break;
    case '+':
      glMatrixMode(GL_MODELVIEW);
      glScalef(1.2,1.2,1.2);

      break;

    case 'q':
    case 'Q':
      exit(0);
      break;
    case '8':
      if(speed<=MAX_SPEED)
	speed++;
      break;
    case '2':
      if(speed>= -MAX_SPEED)
	speed--;
      break;
    case '4':
      directionangle+=5;
      if(directionangle>=360) directionangle=0;

      break;
    case '6':
      directionangle-=5;
      if(directionangle<=-360) directionangle=0;

      break;

    case 'a': case 'A':
      
      break;
    default:
      printf("%d",key);
      break;
    }
}

void ChalkBoard::display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0,1.0,1.0);


 // colldetector->DrawBoundingBoxes();
  
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);

  ypos-=cos((PI/180.0)*directionangle)*speed*10;
  xpos+=sin((PI/180.0)*directionangle)*speed*10;
  glTranslatef(xpos,ypos,0);
  glRotatef(directionangle,0,0,1);

  glCallList(objHooverCraft);
  glCallList(objFan);

  glPopMatrix();
  glCallList(objScene);
   
  //  glTranslatef(xpos,ypos,0);
  glLoadIdentity();


  gluLookAt(-23+xpos+2*sin((PI/180.0)*directionangle)*110,110+ypos,15,-23+xpos,ypos,15,0,0,1);


  glFlush();
  glutSwapBuffers();  
struct timespec req;
 req.tv_sec = 1/2;
 req.tv_nsec = 1000/2;
  int res = clock_nanosleep (CLOCK_REALTIME, 0, &req, NULL);

}

void ChalkBoard::reshape(int w, int h)
{

}

void ChalkBoard::run(void)
{
  glutMainLoop();

    FreeTexture( texName );
    FreeTexture( texTire );
    FreeTexture( texMetal );
}


GLuint ChalkBoard::LoadTextureRAW( const char * filename, 
				   int w, int h)
{
  GLuint texture;
  int width, height;
  FILE * file;
    file = fopen( filename, "rb" );
  if ( file == NULL ) return 0;
  width = w;
  height = h;
  unsigned char *data = (unsigned char *)malloc( width * height * 3 );
  fread( data, width * height * 3, 1, file );
  fclose( file );

  glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
 GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
 GL_LINEAR );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 
GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 
GL_REPEAT );

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    //automatic texture coordinates generation
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);


    free( data );


    return texture; 
}

void ChalkBoard::FreeTexture( GLuint texture )
{
  glDeleteTextures( 1, &texture ); 
}
void ChalkBoard::WriteFile(char *filename)
{
   FILE *f;
   GLubyte *image;
   int i;
   int gWidth=MAINWIDTH;
   int gHeight=MAINHEIGHT;

   image = (GLubyte *)malloc(gWidth * gHeight * 3 * sizeof(GLubyte));
   if (!image) {
      printf("Error: couldn't allocate image buffer\n");
      return;
   }

   glPixelStorei(GL_PACK_ALIGNMENT, 1);
   glReadPixels(0, 0, gWidth, gHeight, GL_RGB, GL_UNSIGNED_BYTE, image);

   f = fopen(filename, "w");
   if (!f) {
      printf("Couldn't open image file: %s\n", filename);
      return;
   }
   fprintf(f,"P6\n");
   fprintf(f,"# ppm-file created by %s\n", "trdemo2");
   fprintf(f,"%i %i\n", gWidth, gHeight);
   fprintf(f,"255\n");
   fclose(f);
   f = fopen(filename, "ab");  /* now append binary data */
   if (!f) {
      printf("Couldn't append to image file: %s\n", filename);
      return;
   }

   for (i=0;i<gHeight;i++) {
      GLubyte *rowPtr;
      /* Remember, OpenGL images are bottom to top.  Have to reverse. */
      rowPtr = image + (gHeight-1-i) * gWidth*3;
      fwrite(rowPtr, 1, gWidth*3, f);
   }

   fclose(f);
   free(image);

   printf("Wrote %d by %d image file: %s\n", gWidth, gHeight, filename);
}
