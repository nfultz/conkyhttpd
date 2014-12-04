
#include <stdio.h>
#include <string.h> /* memset */
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>

int newstdoutfd = -999;
int sockfd;

static int l_setup(lua_State *L){
    printf("Setting up web conky...\n");

    int portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset((void*) &serv_addr,1, sizeof(serv_addr));
    portno = 5001;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    listen(sockfd,5);

    newstdoutfd  = dup(STDOUT_FILENO);

}

static int l_listen(lua_State *L)
{

    dup2(newstdoutfd, STDOUT_FILENO);
    close(newstdoutfd);

    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    printf("Listening... ");
    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, 
            &clilen);


    printf("Accepted\n");
    newstdoutfd  = dup(STDOUT_FILENO);
    dup2(newsockfd, STDOUT_FILENO);
    close(newsockfd);

    return 0;
}

static const struct luaL_Reg webconky [] = {
    {"listen", l_listen},
    {"setup", l_setup},
    {NULL, NULL}  /* sentinel */
};

int luaopen_webconky(lua_State *L)
{
    luaL_openlib(L, "webconky", webconky, 0);
    return 1;
}

int main(){

  int i;
  for(i = 0; i < 3; i++){

      l_listen(0);
      printf("Hello world\n");
  }


}
