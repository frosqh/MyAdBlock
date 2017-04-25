#include "MyAdBlock.h"

void showError(char* error){
    perror(error);
    exit(852);
}

int main(int argc, char* argv[]){
    pid_t pid;
    int sockfd, newsockfd;
    struct sockaddr_in addr_in, cli_addr, serv_addr;
    struct hostent* host;

    if (argc<2)
        showError("Usage : ./proxy <port>");

    parse("liste.txt", "listout.txt", "listoutex.txt", "listoutexacte.txt", "listouteex.txt");

    printf("La liste a été parsée :D \n");

    bzero((char*)&serv_addr, sizeof(serv_addr));
    bzero((char*)&cli_addr, sizeof(cli_addr));

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if ((sockfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        showError("socket()");

    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        showError("bind()");

    listen(sockfd, 50);
    int len = sizeof(cli_addr);

    labela:

    if ((newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &len)) < 0)
            showError("Accept()");

    //printf("COUCOU\n");
    if ((pid=fork())==0){
        struct sockaddr_in host_addr;
        int flag=0, newsockfd1,n, port=0, i, sockfd1;
        char buffer[510], t1[300], t2[300], t3[10];
        char *temp = NULL;
        bzero((char*) buffer, 500);
        recv(newsockfd, buffer, 500, 0);

        //printf("%s \n", (buffer));
        sscanf(buffer, "%s %s %s", t1, t2, t3);
       	//printf("%s\n", t2);
       /*if (isFiltered(t2, "listout.txt", "listoutex.txt", "listoutexacte.txt", "listouteex.txt")){
        	printf("%s is Filtered !", t2);
        	bzero((char*) buffer, 500);
        	send(newsockfd, buffer, n, 0);
        	exit(0);
        }*/
        /*printf("%s \n", t1);
        printf("%s \n", t2);
        printf("%s \n", t3);*/
        if(((strncmp(t1,"GET",3)==0))&&((strncmp(t3,"HTTP/1.1",8)==0)||(strncmp(t3,"HTTP/1.0",8)==0))&&(strncmp(t2,"http://",7)==0)) {
            
            //printf("Après le IF\n");
            strcpy(t1,t2);
            flag = 0;

            for (i=7; i<strlen(t2);i++){
                if(t2[i]==':'){
                    flag=1;
                    break;
                }
            }

            //printf("Flag : %d\n", flag);

            temp = strtok(t2, "//");
            if (flag==0){
                port=80;
                temp = strtok(NULL,"/");
            }
            else{
                temp = strtok(NULL,":");
            }

            sprintf(t2, "%s", temp);
            //printf("host = %s \n", t2);
            host = gethostbyname(t2);
            //printf("Couocu\n");

            if (flag == 1){
                temp = strtok(NULL, "/");
                port = atoi(temp);
            }
            //printf("%d\n", port);
            strcat(t1, "^]");
            temp = strtok(t1, "//");
            temp = strtok(NULL, "/");
            //printf("%s\n", temp);
            if (temp!=NULL){
                temp = strtok(NULL, "^]");
            }
            //printf("temp\n");
           //printf("%spath = %s %sPort = %d%s \n", CRLF, temp, CRLF, port, CRLF);
            

            bzero((char*) &host_addr, sizeof(host_addr));
            host_addr.sin_port=htons(port);
            host_addr.sin_family=AF_INET;
            bcopy((char*)host->h_addr, (char*) &host_addr.sin_addr.s_addr, host->h_length);

            sockfd1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            //printf("Avant connect remote\n");
            newsockfd1 = connect(sockfd1, (struct sockaddr*) &host_addr, sizeof(struct sockaddr));
            //printf("Après connect remote\n");
            sprintf(buffer,"\nConnected to %s  IP - %d\n",t2,inet_ntoa(host_addr.sin_addr));
            //printf("Après sprintf");
            if (newsockfd1 < 0){
                showError("Connect() remote");
            }

            //printf("Coucou\n");
            //printf("ABC : %s%s%s\n", CRLF, buffer, CRLF);

            bzero((char*)buffer, sizeof(buffer));
            if (temp!=NULL)
                sprintf(buffer, "GET /%s %s%sHost: %s%sConnection: close%s%s", temp, t3, CRLF, t2, CRLF, CRLF, CRLF);
            else{
                 sprintf(buffer, "GET / %s%sHost: %s%sConnection: close%s%s", t3, CRLF, t2, CRLF, CRLF, CRLF);         
            }

            n = send(sockfd1, buffer, strlen(buffer), 0);
           //printf("TOLO %s \n ", buffer);
            //printf("%d\n",n);
            if (n < 0){
                showError("send()");
            }
            else{
                do{
                    //printf("Coucou1?\n");
                    bzero((char*) buffer, 500);
                    n=recv(sockfd1, buffer, 500, 0);
                    //printf("Buffer dans while : %s\n", buffer);
                    if (!(n<=0)){
                        send(newsockfd, buffer, n, 0);
                    }
                    //printf("%d\n",n);
                }
                while (n>0);
                }
            }
            else{
                send (newsockfd, "400 : BAD REQUEST \n ONLY HTTP REQUEST ALLOWED", 18, 0);
            }
            close(sockfd1);
            close(newsockfd);
            close(sockfd);
            exit(0);
        }  
        else{
            close(newsockfd);
            goto labela;
        }
        return 0;
    }