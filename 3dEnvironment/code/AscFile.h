#ifndef ASCFILE_H
#define ASCFILE_H
#include <stdio.h>
#include "list.h"
#include "Shapes.h"

class AscFile
{

protected:
  List *points;
  List *objects;
public:
  AscFile(char *file);
  ~AscFile();
  List *getPoints(){return points;}
  List *getObjects(){return objects;}
  void draw_objects();
};
#endif
