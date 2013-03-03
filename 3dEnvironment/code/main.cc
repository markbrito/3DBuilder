#include "AscFile.h"
#include "chalkboard.h"

/*
  This is where the hovercraft game starts.  
 */
int main(int argc,char **argv)
{
  // load the geometry file
  AscFile a("shapes.shp");

  // chalk board is the window where the graphics are animated
  ChalkBoard *board;
  board=new ChalkBoard(argc,argv,MAINWIDTH,MAINHEIGHT,
		       "3D Builder",-300,-300,600,600,
		       -300,600,a.getObjects());
  ChalkBoard::instance=board;
//  board=new ChalkBoard(argc,argv,600,600,"HooverCraft",-3,-3,6,6,
		    //   -3,6,a.getObjects());

  // start the game
  board->run();

  return 0;
}
