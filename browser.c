#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 4096
#define URL_LEN  128
#define LINK_COUNT 128

char domain[URL_LEN];
char path[URL_LEN];
char buffer[BUF_SIZE];
char links[LINK_COUNT][URL_LEN];
char *printBuf;
int linksCountInPage;
int printBufLen = 0;
int printBufSize = BUF_SIZE;
int sock;
struct sockaddr_in serv;
struct hostent *hp;

void urlParse(char* url) {
    int i = 0;
    int j = 0;
    if (url[0] == 'h' && url[1] == 't' && url[2] == 't' && url[3] == 'p' && url[4] == ':' && url[5] == '/' && url[6] == '/')
        i = 7;
    for (j = 0; i < URL_LEN; i++, j++) {
        if (url[i] == '\0' || url[i] == '/' || url[i] == '\n') {
            domain[j] = '\0';
            break;
        }
        domain[j] = url[i];   
    }
    domain[URL_LEN - 1] = '\0';

    if (url[i] == '/') {
        for (j = 0; i < URL_LEN; j++, i++) {
            if (url[i] == '\0' || url[i] == '\n') {
                path[j] = '\0';
                break;
            }
            path[j] = url[i];
        }
    }
    path[j] = '\0';
}

void parseTagA() {
    int pageLen = strlen(printBuf);
    int inTagA = 0;
    int linkNum = 0;
    links[0][0] = '\0'; 
        
    for (int i = 0; i < pageLen && linkNum < LINK_COUNT - 1; i++) {
        if(printBuf[i] == '<' && ( printBuf[i+1] == 'a' || printBuf[i+1] == 'A' || (printBuf[i+1] == ' ' && (printBuf[i+2] == 'a' || printBuf[i+2] == 'A'))) && !inTagA) {
            inTagA = 1;
        }
        if(printBuf[i] == '<' && printBuf[i+1] == '/' && (printBuf[i+2] == 'a' || printBuf[i+2] == 'A') && printBuf[i+3] == '>') {
            inTagA = 0;
        }

        if(inTagA && (printBuf[i] == 'h' || printBuf[i] == 'H') && (printBuf[i+1] == 'r' || printBuf[i+1] == 'R')
        && (printBuf[i+2] == 'e' || printBuf[i+2] == 'E') && (printBuf[i+3] == 'f' || printBuf[i+3] == 'F') && printBuf[i+4] == '='){
            if (printBuf[i+5] == '\'' || printBuf[i+5] == '"')
                i += 6;
            else
                i += 5;
            int k = 0;          
            while (printBuf[i] != '\'' && printBuf[i] != '"' && printBuf[i] != '>' && printBuf[i] != ' ' && k < URL_LEN - 1 && i < pageLen) {           
                links[linkNum][k] = printBuf[i];
                k++;
                i++;
            }
            links[linkNum][k] = '\0';
            linkNum++;
        }
    }
    links[linkNum][0] = '\0'; 
}

void parsePath(char* link) {
    if(link[0] == '#')
        return;
    int ptr = strlen(path);
    while(path[ptr - 1] != '/' && ptr > 0) {
        ptr--;
    }
    memcpy((path + ptr), link, strlen(link));
    path[ptr + strlen(link)] = '\0';
}

void visitLink(char* url) {
    int result;
    if (url[0] == 'h' && url[1] == 't' && url[2] == 't' && url[3] == 'p' && url[4] == ':' && url[5] == '/' && url[6] == '/') {
        memset(path, 0, URL_LEN);
        urlParse(url);
        hp = gethostbyname(domain);
        if (hp == NULL) {
            printf("%s\n", hstrerror(h_errno));
            exit(0);
        }
        bzero((char*)&serv, sizeof(serv));
        bcopy(hp->h_addr_list[0], (char*)&serv.sin_addr, hp->h_length);
        serv.sin_family = AF_INET;
        serv.sin_port = htons(80);
    }
    else {
        parsePath(url);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
        {
            perror( "Error calling socket" );
            exit(0);
        }

    result = connect( sock, ( struct sockaddr * )&serv, sizeof(serv) );
    if( result )
    {
        perror( "Error calling connect" );
        exit(0);
    }
        
    if((printBuf = (char*)calloc(printBufSize, sizeof(char))) == NULL)
        exit(0);

    sprintf(buffer, "GET /%s\nAccept: text/html", path);
    result = send(sock, buffer, strlen(buffer), 0);

    if( shutdown(sock, 1) < 0) {
            perror("Error calling shutdown");
            exit (0);
    }

    int printBufLen = 0;
    while(1) {
        memset(buffer, 0, BUF_SIZE);
        int result = recv( sock, buffer, sizeof(buffer) - 1, 0 );
        if (result < 1)
            break;
        if ((printBufLen + result) >= printBufSize - 1) {
            printBufSize += BUF_SIZE;            
            if((printBuf = (char*) realloc((void *) printBuf, printBufSize)) == NULL)
                exit(0);
        }
        memcpy((printBuf+printBufLen), buffer, result);
        printBufLen += result;
    }
    write(1, printBuf, strlen(printBuf));
    sprintf(buffer, "\nYou on: http://%s/%s\n\n", domain, path);
    write(1, buffer, strlen(buffer));

    parseTagA();

    free(printBuf);
    printBuf = NULL;
    int printBufSize = BUF_SIZE;
    close(sock);
}

void printLinks(char* mainPage) {
    linksCountInPage = 0;
    int k;
    printf("----------%d---------%s\n", 0, mainPage);
    for (k = 0; links[k][0] != '\0'; k++) {
        printf("----------%d---------%s\n", k + 1, links[k]);
    }
    write(1, "\"Ctrl-D\" for exit.\n",19);    
    linksCountInPage = k;
}

int main(int argc, char* argv[]) {

    if (argc < 1) {
        printf("Error\nFormat: %s <url>\n", argv[0]);
        return 0;
    }
    
    if (!(argv[1][0] == 'h' && argv[1][1] == 't' && argv[1][2] == 't' && argv[1][3] == 'p' && argv[1][4] == ':' && argv[1][5] == '/' && argv[1][6] == '/')) {
        printf("Write a protocol\n");
        return 0;
    }



    visitLink(argv[1]);
    printLinks(argv[1]);

    int scanRes;
    int num;
    while((scanRes = scanf("%d", &num)) != EOF) {
        if (scanRes != 1) {
            printf("Wrong input\n");
            while(getchar() != '\n')
                ;
            continue;
        }
        if (num < 0 || num > linksCountInPage) {
            printf("Wrong input: min - %d; max - %d\n", 1, linksCountInPage);
            continue;
        }
        if (num == 0) {
            visitLink(argv[1]);
            printLinks(argv[1]);
        }
        visitLink(links[num - 1]);
        printLinks(argv[1]);
    }
    
    
    return 0;
}