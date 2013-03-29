#ifndef WEBSERVER_H
#define WEBSERVER_H 
#define DOCUMENTROOT "c:\\Users\\Mark\\Desktop\\3dEnvironment\\code"
#define DEBUG
extern "C"
{
  void* startWebserver(void *);
  void* startConnection(void *);
  int loadfile(char *request_file,void **memory_buffer,size_t *buffer_size);
}
#endif
