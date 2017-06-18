#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define SERV_PORT 8000

int main(void)
{
  struct sockaddr_in serv_addr, clie_addr;
  socklen_t clie_addr_len;

  int sockfd;
  char buf[BUFSIZ] = {0};

  char str[INET_ADDRSTRLEN];
  int i, n;

  //watchout SOCK_DGRAM
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(SERV_PORT);

  bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  printf("bind ok, new waitting for recv\n");
  
  while(1){
    clie_addr_len = sizeof(clie_addr);
    n = recvfrom(sockfd, buf, BUFSIZ, 0, (struct sockaddr *)&clie_addr, &clie_addr_len);
    if(n == -1) {
      perror("recvfrom error");
    }
    printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &clie_addr.sin_addr, str, sizeof(str)), 
	   ntohs(clie_addr.sin_port));

    //processing
    for(i=0;i<n;i++){
      buf[i] = toupper(buf[i]);
    }
    //buf[n] = '\0'; //n<BUFSIZ

    n = sendto(sockfd, buf, BUFSIZ, 0, (struct sockaddr*)&clie_addr, sizeof(clie_addr));
    if(n==-1){
      perror("send error");
    }

  }//end of while

  close(sockfd);
  return 0;
}
