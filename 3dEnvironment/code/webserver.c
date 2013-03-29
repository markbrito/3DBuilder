#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fnmatch.h>
#include <libgen.h>
#include "chalkboard.h"
#define DEBUG
#define PORT 8484
#define DOCUMENTROOT "c:\\Users\\Mark\\Desktop\\3dEnvironment\\code"

void LogError( const char *LogError ){
  syslog( 0, LogError );
  printf(LogError);
}/*LogError*/

int loadfile(char *request_file,void **memory_buffer,size_t *buffer_size ){
struct stat	size;
int					infile;

	if( stat( request_file, &size ) != 0 )
		LogError( "File not found.\n" );

	*buffer_size = size.st_size;

	void *memory_buffer_malloc=(void *)malloc( *buffer_size );
	*memory_buffer=memory_buffer_malloc;
	if( memory_buffer_malloc==NULL )
		LogError( "Out of memory.\n" );
	printf(request_file);
	infile=open( request_file, O_RDONLY );
	if( read( infile, *memory_buffer, *buffer_size ) != *buffer_size )
		LogError( "Unable to read file." );
	LogError( "Successful Read file." );

	close( infile );
}/*sendfile*/

int socketInIDconnect( void ){
int                 socketInID;
struct sockaddr_in  socketAddrIn;

	socketInID = socket (PF_INET, SOCK_STREAM, 0);
	if(socketInID < 0)
		LogError( "Error open socket.\n" );

	socketAddrIn.sin_family=AF_INET;
	socketAddrIn.sin_port=htons(PORT);
	socketAddrIn.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(socketInID, (struct sockaddr *) &socketAddrIn, sizeof(socketAddrIn)) < 0)
		LogError( "Error binding socket.\n" );
	
  return socketInID;
}/*socketInIDconnect*/


int readSocketInput( int socketInID, char *request_file,char *uri ){
char    memoria[1024];
int     n;
	sprintf( request_file, DOCUMENTROOT );

	n=read(socketInID, memoria, 1024);
	if( n < 0 )
		LogError( "Error reading socket.\n" );
	sscanf(memoria, "GET %s", uri);
	sscanf(memoria, "GET %s", 
			&request_file[strlen(DOCUMENTROOT)] );
	

	if( request_file[strlen(request_file)-1]=='/' )
			sprintf(&request_file[strlen(request_file)], "index.html" );
	
	#ifdef DEBUG
	printf("File request: %s\n", request_file );
	#endif

	return( n );
}/*readSocketInput*/
	

int sendSocketOutput( int socketInID, char *request_file,
		      void *memory_buffer,size_t *buffer_size ){
char head_template[]="HTTP/1.0 200 OK\r\n\
Server: miniserver/0.1\r\n\
Connection: close\r\n\
Content-Type: %s\r\n\r\n";
char	head[512];
char  *type;
int		irt;

	type="text/html";
	if( fnmatch("*.gif", basename(request_file), 0) == 0 )
		type="image/gif";
	if( fnmatch("*.jpg", basename(request_file), 0) == 0 )
		type="image/jpeg";
	if( fnmatch("*.png", basename(request_file), 0) == 0 )
		type="image/png";
	
	sprintf( head, head_template, type ); 

	irt=write( socketInID, head, strlen(head));
	
	if( memory_buffer != NULL ){
	  irt=write( socketInID, memory_buffer, *buffer_size );
	  printf("Successful Write to File");
	}
	else
	  printf( "Error: no data to send.\n" );
 
	return(1);
}/*sendSocketOutput*/

void* startConnection(void *arg)
{
  void *memory_buffer;
  char request_file[1024];
  char uri[1024];
  int socketID=*((int *)arg);
  size_t buffer_size;

  int f=0;

#ifdef DEBUG
  LogError("StartConnectionThread");
#endif

  readSocketInput( socketID,request_file,uri );

  for(f=0;f<1024;++f){
    if(request_file[f]=='/')
      request_file[f]='\\';
  }

#ifdef DEBUG
  printf("Read uri %s",uri);
  printf("Read file %s",request_file);
#endif

char ok[3]="OK";
  char dt[1024];
strcat(dt,__DATE__);  
strcat(dt,__TIME__);
  int d=0;
  for(int d=0;d<strlen(dt);++d)
    if(!(isalpha(dt[d]) || isdigit(dt[d])))
      dt[d]='_';
char nm[1024];
char nmoutname[1024];
char nmout[1024];
  if(strstr(strupr(uri),strupr("/ACC"))!=0){
    (ChalkBoard::instance)->keyboard('8',0,0);
  }
  else if(strstr(strupr(uri),strupr("/DEC"))!=0){
    (ChalkBoard::instance)->keyboard('2',0,0);
  }
  else if(strstr(strupr(uri),strupr("/LEFT"))!=0){
    (ChalkBoard::instance)->keyboard('4',0,0);
  }
  else if(strstr(strupr(uri),strupr("/RIGHT"))!=0){
    (ChalkBoard::instance)->keyboard('6',0,0);
  }
  if(strstr(strupr(uri),strupr("/RENDER.GIF"))!=0){
strcpy(nm,"ScreenShot"); 
strcat(nm, dt);
strcat(nm,".ppm");
strcpy(nmout,DOCUMENTROOT);
strcat(nmout,"\\ScreenShot");
strcat(nmout, dt);
strcat(nmout,".gif");

strcat(nmoutname,"ScreenShot");
strcat(nmoutname, dt);
strcat(nmoutname,".gif");

#ifdef DEBUG
  printf("Write File %s",nm);
#endif

    (ChalkBoard::instance)->WriteFile(nm);

    char cp[1024];
    char cmd[1024];
    strcat(cmd,"cp frame.gif ");
    strcat(cmd,nmoutname);
    strcat(cmd," ; timeout --kill-after=3s --signal=9 3s convert.exe \"");
strcat(cmd, DOCUMENTROOT);
strcat(cmd,"\\");
strcat(cmd, nm);
strcat(cmd, "\" ");
strcat(cmd ,nmoutname);
strcat(cp,"cp \"");
strcat(cp, nmout);
strcat(cp, "\" frame.gif");

#ifdef DEBUG
    printf("Convert cmd: %s",cmd);
#endif
  system(cmd);
  system(cp);
}
  if(strstr(strupr(uri),strupr("/RENDER.GIF"))!=0){
#ifdef DEBUG
  printf("Load File %s",nmout);
#endif
    loadfile(nmout,&memory_buffer,&buffer_size);  
  }
  else{
  loadfile(request_file,&memory_buffer,&buffer_size);
  }
#ifdef DEBUG
  LogError("Load File suces");
#endif
  sendSocketOutput( socketID,request_file,
		    memory_buffer,&buffer_size);
#ifdef DEBUG
  LogError("Sent Response");
#endif

  shutdown( socketID, 2 );
    if( memory_buffer != NULL )
          free(memory_buffer);

#ifdef DEBUG
    LogError("Free Close");
#endif


      pthread_t thispthread=pthread_self();
        pthread_detach(thispthread);
}

int  mainloop( void ){
  pthread_t connthread;
  int socketID;
  int sock=socketInIDconnect();

#ifdef DEBUG
  LogError("ConnectIn");
#endif

	while( 1 ){
	  struct sockaddr_in clientname;
	  size_t size;
	  
		if( listen (sock, 1) < 0 )
		  LogError( "Error while listening." );

#ifdef DEBUG
		LogError("Listen");
#endif

		size = sizeof (clientname);
		socketID = accept(sock, (struct sockaddr *) &clientname,(socklen_t*) &size);

#ifdef DEBUG
		LogError("Accept");
#endif


		/*waitpid( WAIT_ANY, NULL, WNOHANG );*/
		pthread_create(&connthread, NULL, 
			       startConnection, &socketID);
	}/*while*/
}/*mainloop*/

void signal_handler( int signal ){
	LogError("Exiting...\n");
	exit(1);
}/*signal_handler*/	


extern "C"
{
void* startWebserver(void *arg)
{
	openlog( "webserver", LOG_PID, LOG_MAKEPRI(LOG_DAEMON, LOG_DEBUG) );
	syslog( 0, "Miniserver started." );
	
	signal( SIGINT, signal_handler );
	signal( SIGKILL, signal_handler );
	signal( SIGTERM, signal_handler );
	
	//	daemon( 0, 0 );
	mainloop();	
}
}



