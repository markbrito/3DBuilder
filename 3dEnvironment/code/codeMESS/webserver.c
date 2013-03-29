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
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fnmatch.h>
#include <libgen.h>
#include "ChalkBoard.h"
#include "webserver.h"

#define PORT 8484

pthread_mutex_t    mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void LogError( const char *LogError ){
  syslog( 0, LogError );
  printf(LogError);
}
extern "C"
{
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
}
}

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
  printf("\nFile request: %s\n", request_file );
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
    printf("\nSuccessful Write to File");
  }
  else
    printf( "Error: no data to send.\n" );
 
  return(1);
}/*sendSocketOutput*/

void* startConnection(void *arg)
{
  void *memory_buffer;
  size_t buffer_size;
  char request_file[1024];
  char uri[1024];
  int socketID=*((int *)arg);
  int f=0;
  char file[1024];
#ifdef DEBUG
  LogError("StartConnectionThread");
#endif
  if(strstr(strupr(uri),strupr("/OK.HTML"))!=0)
    {
      strcpy(request_file, DOCUMENTROOT);  
      strcat(request_file,"\\");
      strcat(request_file,"ok.html");
    }
  if(strstr(strupr(uri),strupr("/RENDER.GIF"))!=0 )
    {
    strcpy(request_file, DOCUMENTROOT);
      strcat(request_file,"\\");
      strcat(request_file,"frame.gif");
    }
  if(strstr(strupr(uri),strupr("/FRAME.GIF"))!=0 ){
    strcpy(request_file, DOCUMENTROOT);
      strcat(request_file,"\\");
      strcat(request_file,"frame.gif");
  }

  readSocketInput( socketID,request_file,uri );

  for(f=0;f<1024;++f){
    if(request_file[f]=='/')
      request_file[f]='\\';  
  }

#ifdef debug
  printf("\nread uri %s",uri);
  printf("\nread file %s",request_file);
#endif

  char ok[3]="ok";
  char dt[1024];
  strcpy(dt,__DATE__);  
  strcat(dt,__TIME__);
  char bufrand[128];
  sscanf(bufrand,"%d",rand());
  strcat(dt,bufrand);
  int d=0;
  for(int d=0;d<strlen(dt);++d)
    if(!(isalpha(dt[d]) || isdigit(dt[d])))
      dt[d]='_';
  char nm[1024];
  char nmoutname[1024];
  char nmout[1024];
  if(strstr(strupr(uri),strupr("/transform"))!=0){
    char *xuri=strdup(uri);
    char *pdx=(strstr(xuri,"?dx=")+4);
    char *pda=(strstr(xuri,"&da=")+4);
    *(strstr(xuri,"&r="))=0;
    *(strstr(xuri,"&da="))=0;
    int dx=atoi(pdx);
    int da=atoi(pda);
    (ChalkBoard::instance)->keyboard('t',dx,da);
  }
  if(strstr(strupr(uri),strupr("/acc"))!=0){
    (ChalkBoard::instance)->keyboard('8',0,0);
  }
  else if(strstr(strupr(uri),strupr("/dec"))!=0){
    (ChalkBoard::instance)->keyboard('2',0,0);
  }
  else if(strstr(strupr(uri),strupr("/left"))!=0){
    (ChalkBoard::instance)->keyboard('4',0,0);
  }
  else if(strstr(strupr(uri),strupr("/right"))!=0){
    (ChalkBoard::instance)->keyboard('6',0,0);
  }
  struct timespec mutexwait;
  mutexwait.tv_sec=10;
  mutexwait.tv_nsec=10000;
  pthread_cond_timedwait(&cond,&mutex,&mutexwait);
  if(strstr(strupr(uri),strupr("/render.gif"))!=0 ||
     strstr(strupr(uri),strupr("/ok.html"))!=0 ||
     strstr(strupr(uri),strupr("/frame.gif"))!=0 )
{
    strcpy(nm,"screenshot"); 
    strcat(nm, dt);
    strcat(nm,".ppm");

    strcpy(nmout,DOCUMENTROOT);
    strcat(nmout,"\\screenshot");
    strcat(nmout, dt);
    strcat(nmout,".gif");
    
    strcpy(nmoutname,"screenshot");
    strcat(nmoutname, dt);
    strcat(nmoutname,".gif");

#ifdef debug
    printf("\nwrite file %s",nm);
#endif

    (ChalkBoard::instance)->WriteFile(nm);

#ifdef debug
    printf("\nLoad Generated File %s\n",nmout);
#endif
    loadfile(request_file,&memory_buffer,&buffer_size);
    //        loadfile(nmout,&memory_buffer,&buffer_size);  
    //    memory_buffer=web_frame_buffer;
    //    buffer_size=web_frame_buffer_size;
    
  }
  if(strstr(strupr(uri),strupr("/WEBFRAME.GIF"))!=0){
    memory_buffer=web_frame_buffer;
    buffer_size=web_frame_buffer_size;
  }
  else if(strstr(strupr(uri),strupr("/LOAD"))!=0){
#ifdef DEBUG
   printf("\nLoad File URI: %s\n",request_file);
#endif
    loadfile(request_file,&memory_buffer,&buffer_size);
  }
  sendSocketOutput( socketID,request_file,
		    memory_buffer,&buffer_size);
  pthread_mutex_unlock(&mutex);
#ifdef DEBUG
  printf("\nSent Response %s\n",request_file);
#endif

  shutdown( socketID, 2 );
   if( memory_buffer != NULL )
         free(memory_buffer);
  //  if(web_frame_buffer!=NULL)
  //    free(web_frame_buffer);
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



