CC	= gcc  
SYS_DEF	= -DOPENGL -DANSI_CPP
INCDIRS	= -I/usr/include -I/usr/local/include
LIBDIRS	= -L/lib -L/usr/local/lib -L/usr/local/lib -L/usr/X11/lib

AUX_LIB    = -lMesaaux
TK_LIB     = -lMesatk
GLUT_LIB   = -lglut
#OGL_LIB    = -lMesaGLU -lMesaGL
OGL_LIB    = -lGLU -lGL
SYS_LIBS   = -lXext -lX11 -lXmu -lm -lXi -lstdc++
#SYS_LIBS   = -lXext -lX11 -lXmu -lm -lXi -lg++

LIBS	=  $(OGL_LIB) $(SYS_LIBS) $(GLUT_LIB)

CFLAGS	= $(SYS_DEF) $(INCDIRS) 
#-g -ggdb -dH -o0

# Source files

SRCS	= chalkboard.cc	main.cc list.cc AscFile.cc

OBJS	= chalkboard.o main.o list.o AscFile.o

PROGRAM	= threeD


#----------------------------------------------------------------------
# You probably will not need to modify anything below this line.
#----------------------------------------------------------------------

# Add all the flags to CFLAGS


default:    $(PROGRAM)

program:    $(PROGRAM)

all:	    $(PROGRAM)


$(PROGRAM): $(OBJS) 
	@echo -n "Loading $(PROGRAM) ... "
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBDIRS)  $(LIBS)
	@echo "done."

clean:
	rm -f *.o $(PROGRAM)
