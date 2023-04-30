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


#define SERVER_PORT1 12032
#define SERVER_PORT2 22032
#define MAX_MSG 100

int main (int argc, char *argv[]) {

  int socket1 = 0, socket2 = 0, rc;
  int r1 = 0, r2 = 0;
  char sendBuff[1025], recieveBuff[1025], infor[1025];
  struct sockaddr_in serv_addr1, serv_addr2;
  
  memset(sendBuff, '0', sizeof(sendBuff));
  memset(recieveBuff, '0', sizeof(recieveBuff));
  memset(infor, '0', sizeof(infor));

  if(argc != 2){
    printf("\n Usage: %s <ip of server> \n", argv[0]);
    return 1;
  }

  /* create socket */
  socket1 = socket(AF_INET, SOCK_STREAM, 0);
  if(socket1 < 0) {
    perror("cannot open socket ");
    exit(1);
  }

  memset(&serv_addr1, '0', sizeof(serv_addr1));
  serv_addr1.sin_family = AF_INET;
  serv_addr1.sin_port = htons(SERVER_PORT1);

  if(inet_pton(AF_INET, argv[1], &serv_addr1.sin_addr) <= 0){
    printf("\n inet_pton error occured\n");
    return 1;
  }

  if(connect(socket1, (struct sockaddr *) &serv_addr1, sizeof(serv_addr1)) < 0) {
    printf("cannot connect ");
    return 1;
  }

  char name[100];
  char f_name[100];
  char l_name[100];
  char birthday[8];

  memset(name, '0', sizeof(name));
  memset(f_name, '0', sizeof(f_name));
  memset(l_name, '0', sizeof(l_name));
  memset(birthday, '0', sizeof(birthday));

  printf("Enter your information: ");
  fgets(sendBuff, sizeof(sendBuff), stdin);
  sscanf(sendBuff, "%[^ ] %s %s", f_name, l_name, birthday);

  write(socket1, sendBuff, strlen(sendBuff));

  memset(recieveBuff, 0, sizeof(recieveBuff));
  r1 = read(socket1, recieveBuff, sizeof(recieveBuff)-1);

  if (r1 > 0){
    printf("Reply: %s\n", recieveBuff);
    close(socket1);
  }

  
  /* create socket */
  socket2 = socket(AF_INET, SOCK_STREAM, 0);
  if(socket2<0) {
    perror("cannot open socket ");
    return 1;
  }

  memset(&serv_addr2, '0', sizeof(serv_addr2));
  serv_addr2.sin_family = AF_INET;
  serv_addr2.sin_port = htons(SERVER_PORT2);
	
  if(inet_pton(AF_INET, argv[1], &serv_addr2.sin_addr) <= 0){
    printf("\n inet_pton error occured\n");
    return 1;
  }
  
  if(connect(socket2, (struct sockaddr *) &serv_addr2, sizeof(serv_addr2)) <0) {
    perror("cannot connect ");
    return 1;
  }

  write(socket2, sendBuff + strlen(sendBuff) + 1 + strlen(sendBuff + strlen(sendBuff) + 1), strlen(sendBuff + strlen(sendBuff) + 1));

  memset(recieveBuff, 0, sizeof(recieveBuff));

  r2 = read(socket2, recieveBuff, sizeof(recieveBuff) - 1);

  int year;
  char str[9];
  if (r2 > 0){
    sscanf(recieveBuff, "%d", &year);
    snprintf(str, sizeof(str), "%d", year);
    printf("Year of birth: %c%c%c%c\n", str[4],str[5],str[6],str[7]);
    printf("Birthday: %d\n", year);
  }
  else{
    printf("No data received from server\n");
  }
  
  snprintf(sendBuff, sizeof(sendBuff), "Bye");
  write(socket2, sendBuff, strlen(sendBuff));
  close(socket2);
  return 0;
  
}

