//สิทธิภาคย์ ศรีสวัสดิ์ 6410742032

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 


#define SUCCESS 0
#define ERROR   1

#define SERVER_PORT1 12032
#define SERVER_PORT2 22032


int main (int argc, char *argv[]) {
  
  int socket1, newSd1;
  int socket2, newSd2;

  struct sockaddr_in serv_addr1, serv_addr2;

  char sendBuff[1025];
  char recieveBuff[1025];
  char infor[20];
  char name[20];
  char year[9];
 
  memset(&serv_addr1, '0', sizeof(serv_addr1));
  memset(&serv_addr2, '0', sizeof(serv_addr2));
  memset(&sendBuff, '0', sizeof(sendBuff));
  memset(&recieveBuff, '0', sizeof(recieveBuff));
  memset(&infor, '0', sizeof(infor));

  /* create socket */
  socket1 = socket(AF_INET, SOCK_STREAM, 0);
  socket2 = socket(AF_INET, SOCK_STREAM, 0);
   if(socket1 == -1 || socket2 == -1)  {
    perror("cannot open socket ");
    return ERROR;
  }
  
  /* bind server port */
  serv_addr1.sin_family = AF_INET;
  serv_addr1.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr1.sin_port = htons(SERVER_PORT1);
  
  serv_addr2.sin_family = AF_INET;
  serv_addr2.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr2.sin_port = htons(SERVER_PORT2);

  if(bind(socket1, (struct sockaddr *) &serv_addr1, sizeof(serv_addr1))<0 || bind(socket2, (struct sockaddr *) &serv_addr2, sizeof(serv_addr2))<0) {
    perror("cannot bind port ");
    return ERROR;
  }

  listen(socket1,5);
  listen(socket2,5);

  printf("Server is running on port: %d and port: %d \n", SERVER_PORT1, SERVER_PORT2);

  while(1) {

    printf("%s: waiting for data on port TCP %u\n",argv[0],SERVER_PORT1);
   
    newSd1 = accept(socket1, (struct sockaddr *) NULL, NULL);
    if(newSd1<0) {
      perror("cannot accept connect port 12032");
      continue;;
    }

    memset(recieveBuff, 0, sizeof(recieveBuff));
    read(newSd1, recieveBuff, sizeof(recieveBuff) - 1);
    sscanf(recieveBuff, "%[^\n] %s", infor, year);

    strncpy(name, &infor[0], strlen(infor) - 8);
    strncpy(year, &infor[strlen(infor) - 8], 8);
    year[8] = '\0';

    printf("Name: %s\n", name);

    snprintf(sendBuff, sizeof(sendBuff), "Server Recieved");
    write(newSd1, sendBuff, strlen(sendBuff));
    close(newSd1);

    printf("%s: waiting for data on port TCP %u\n",argv[0],SERVER_PORT2);

    newSd2 = accept(socket2, (struct sockaddr *) NULL, NULL);
    if(newSd2<0) {
      perror("cannot accept connect port 22032");
      continue;
    }

    int year_int = (atoi(year) - 543);
    snprintf(sendBuff, sizeof(sendBuff), "%d", year_int);
    write(newSd2, sendBuff, strlen(sendBuff));

    memset(recieveBuff, 0, sizeof(recieveBuff));
    int n = read(newSd2, recieveBuff, sizeof(recieveBuff)-1);
    while(1) {
      memset(recieveBuff, 0, sizeof(recieveBuff));
      int n = read(newSd2, recieveBuff, sizeof(recieveBuff)-1);
      if (n > 0) {
        recieveBuff[n] = '\0';
        printf("Recieved message: %s\n", recieveBuff);
        if (strcmp(recieveBuff, "Bye") == 0){
          break;
        }
      }
    }
    close(newSd2);
    sleep(1);

 
}

}