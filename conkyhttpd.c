
#include <stdio.h>
#include <string.h> /* memset */
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>


#include <lua.h>
#include <lauxlib.h>

int sockfd=-999;
int outfd = -999;
int pipefd[2];
char buff[16 << 10];

static int l_setup(lua_State *L){

    int portno = lua_tonumber(L, -1);
    printf("Starting server on port %d...\n", portno);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    struct sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(portno)
    };

    /* Now bind the host address using bind() call.*/
    bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    listen(sockfd,5);

    outfd = dup(STDOUT_FILENO);
    pipe(pipefd);
    dup2(pipefd[1], STDOUT_FILENO);

}

static int l_cleanup(lua_State *L){
    dup2(outfd, STDOUT_FILENO);
    close(outfd);
    close(pipefd[0]);
    close(pipefd[1]);
    close(sockfd);
}

static int l_listen(lua_State *L)
{
    double interval = lua_tonumber(L, -1);  /* get argument */
    int sec = floor(interval);
    int usec =  (interval - sec) * 1000;
    
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    struct timeval timeout = {
        .tv_sec = sec,
        .tv_usec = usec
    };

    int s = select(FD_SETSIZE, &readfds, 0, 0, &timeout);


    if( s == 0) {
        close(pipefd[0]);
        close(pipefd[1]);
        pipe(pipefd);
        dup2(pipefd[1], STDOUT_FILENO);
        return 0;
    }

    int n = read(pipefd[0], buff, 16 << 10);

    while(s) {
        /* Accept actual connection from the client */

        int newsockfd = accept(sockfd, 0, 0);
        dprintf(newsockfd, "HTTP/1.1 200 OK\r\n");
        dprintf(newsockfd, "Content-Length: %d\r\n", n);
        dprintf(newsockfd, "Access-Control-Allow-Origin: *\r\n");
        dprintf(newsockfd, "Refresh: %d\r\n", sec);
        dprintf(newsockfd, "\r\n");
        write(newsockfd, buff, n);
        close(newsockfd);
        s = select(FD_SETSIZE, &readfds, 0, 0, &timeout);

    }

    return 0;
}

static const struct luaL_Reg conkyhttpd [] = {
    {"listen", l_listen},
    {"setup", l_setup},
    {"cleanup", l_cleanup},
    {NULL, NULL}  /* sentinel */
};

int luaopen_conkyhttpd(lua_State *L)
{
    luaL_openlib(L, "conkyhttpd", conkyhttpd, 0);
    return 1;
}


//int main() {
//int i;
//l_setup(0);
// for(i =0; i < 3; i++) l_listen(0);
//}
