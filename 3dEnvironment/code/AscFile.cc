#include "AscFile.h"

// AscFile represents the 3d geometry file
AscFile::AscFile(char *file)
{
  float x,y,z;
  int ct=0,objct=0,ptoffset=0,a,b,c,ab,bc,ca,result;
  FILE *fp;
  APolygon *thispolygon;
  char u[50],v[50];
  fp=fopen(file,"rt");

  points=new List;
  objects=new List;

  result=fscanf(fp,"%s %s",u,v); // read "Object" "Name"
  
  while(!feof(fp))
    {
		if(fscanf(fp,"%f %f %f",&x,&y,&z)==3)
		{
			points->add_item(new Point(x,y,z),ct++);
		}
		else //Polygons?
		{
		  // This logic populates the points and objects Lists
		  // by parsing the file
		  A3dObject *thisObject=new A3dObject(v);
		  fscanf(fp,"%s",u); //read "Polygons"
		  while(!feof(fp))
			{
			  result=fscanf(fp,"%d %d %d %d %d %d",&a,&b,&c,&ab,&bc,&ca);
			  if(result<6)
			{
			  result=fscanf(fp,"%s %s",u,v); // read "Object" "Name"
			  ptoffset=ct;
			  break; //Object?
			}
			  
			  Point *p[3];
			  Point pts[2];
			if(ab==1)
			{
			  thispolygon=new APolygon;
			  thispolygon->add_point(p[0]=(Point *)points->get_item(ptoffset+a));
			  thispolygon->add_point(p[1]=(Point *)points->get_item(ptoffset+b));
	//added
			  thispolygon->add_point(p[2]=(Point *)points->get_item(ptoffset+c));
			  pts[0].x=p[0]->x-p[2]->x;
			  pts[0].y=p[0]->y-p[2]->y;
			  pts[0].z=p[0]->z-p[2]->z;
			  pts[1].x=p[1]->x-p[2]->x;
			  pts[1].y=p[1]->y-p[2]->y;
			  pts[1].z=p[1]->z-p[2]->z;
			  thispolygon->set_normal(new Point(
						pts[0].y*pts[1].z - pts[0].z*pts[1].y,
						pts[0].z*pts[1].x - pts[0].x*pts[1].z,
						pts[0].x*pts[1].y - pts[0].y*pts[1].x));
			
			  thisObject->add_polygon(thispolygon);
			}
			  
			if(bc==1)
			{
			  thispolygon=new APolygon;
	//added
			  thispolygon->add_point(p[2]=(Point *)points->get_item(ptoffset+a));

			  thispolygon->add_point(p[0]=(Point *)points->get_item(ptoffset+b));
			  thispolygon->add_point(p[1]=(Point *)points->get_item(ptoffset+c));
			  pts[0].x=p[0]->x-p[2]->x;
			  pts[0].y=p[0]->y-p[2]->y;
			  pts[0].z=p[0]->z-p[2]->z;
			  pts[1].x=p[1]->x-p[2]->x;
			  pts[1].y=p[1]->y-p[2]->y;
			  pts[1].z=p[1]->z-p[2]->z;
			  thispolygon->set_normal(new Point(
						pts[0].y*pts[1].z - pts[0].z*pts[1].y,
						pts[0].z*pts[1].x - pts[0].x*pts[1].z,
						pts[0].x*pts[1].y - pts[0].y*pts[1].x));


			  thisObject->add_polygon(thispolygon);
			}
			  
			if(ca==1)
			{

			  thispolygon=new APolygon;
			  thispolygon->add_point(p[0]=(Point *)points->get_item(ptoffset+c));
	//added
			  thispolygon->add_point(p[2]=(Point *)points->get_item(ptoffset+b));

			  thispolygon->add_point(p[1]=(Point *)points->get_item(ptoffset+a));
			  pts[0].x=p[0]->x-p[2]->x;
			  pts[0].y=p[0]->y-p[2]->y;
			  pts[0].z=p[0]->z-p[2]->z;
			  pts[1].x=p[1]->x-p[2]->x;
			  pts[1].y=p[1]->y-p[2]->y;
			  pts[1].z=p[1]->z-p[2]->z;
			  thispolygon->set_normal(new Point(
						pts[0].y*pts[1].z - pts[0].z*pts[1].y,
						pts[0].z*pts[1].x - pts[0].x*pts[1].z,
						pts[0].x*pts[1].y - pts[0].y*pts[1].x));

			  
			  thisObject->add_polygon(thispolygon);	  
			}
			  
			} //while
		  objects->add_item(thisObject,objct++);	
		} //else (polygons)
    } //While


  fclose(fp);
}

AscFile::~AscFile()
{
}

void AscFile::draw_objects()
{
  for(int i=0;i<objects->get_count();i++)
    ((A3dObject *)objects->get_item(i))->draw_object();
}


