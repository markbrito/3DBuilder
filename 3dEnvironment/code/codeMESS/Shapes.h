#ifndef SHAPES_H
#define SHAPES_H

#include <stdio.h>
#include <string.h>
#include "list.h"
extern "C" 
{
//#include <gl/glaux.h>
	#include <gl/glu.h>
	#include <gl/glu.h>
}

/*
  This file contains data structures that represent geometry
 */
class Point: public Object
{
public:
	float x,y,z;
	Point(){x=y=0;}
	Point(float x,float y,float z=0):x(x),y(y),z(z) {}	
};

class Line: public Object
{
protected:
	Point *A;
	Point *B;
public:
	Line(Point a,Point b){A=new Point(a.x,a.y);B=new Point(b.x,b.y);}
	Line(int x1,int y1,int x2,int y2){A=new Point(x1,y1); B=new Point(x2,y2);}
	~Line(){delete A;delete B;}
};


class APolygon: public Object
{
protected:
	List points;
	Point *normal;
	int counter;
public:
	APolygon(){counter=0;}
	int get_point_count(){return counter;}
	void add_point(Point *p){points.add_item(p,counter++);}
	void set_normal(Point *n){normal=n;}
	Point *get_point(int index){return (Point *)points.get_item(index);}
	void display_points(){
		for(int x=0;x<counter;x++){
			Point *p=(Point *)points.get_item(x);
			printf(" %f %f 0.0,",p->x,p->y);
		}
	}
	void draw_polygon(){
		  
	//	glBegin(GL_LINE_LOOP);			
		glBegin(GL_TRIANGLES);
		  
		Point *p[3];
		int x;
		for(x=0;x<counter;x++)			
			p[x]=(Point *)points.get_item(x);

		glNormal3f( normal->x,normal->y,normal->z);
		for(x=0;x<counter;x++){
			glVertex3f((float)p[x]->x, (float)p[x]->y, (float)p[x]->z);
		}
		glEnd();
		glFlush();
	}
};

class A3dObject: public Object
{
protected:
  List polygons;
  int polycount;
  char *name;
public:
  A3dObject(char *name){polycount=0;this->name=strdup(name);}
  int get_poly_count(){return polycount;}
  char *get_name(){return name;}
  void add_polygon(APolygon *p){polygons.add_item(p,polycount++);}
  APolygon *get_polygon(int index){return (APolygon *)polygons.get_item(index);}
  void draw_object(){
    APolygon *p;
    for(int i=0;i<polycount;i++)
      {
		p=(APolygon *)polygons.get_item(i);
		p->draw_polygon();
      }   
  }
};
#endif
