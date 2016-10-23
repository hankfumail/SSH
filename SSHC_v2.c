//SSH client
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include<sys/socket.h>
#include <sys/types.h> 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<signal.h>

main(int argc,char **argv)
{

	int fd,fd1,i,len;
	char ch[100];
	struct sockaddr_in a;



void my()
{
	printf("IN ISR\n");
	write(fd,"exit",5);
	usleep(300000);
	shutdown(fd,2);
	exit(0);
}




	if(argc<2){
		printf("USAGE: ./a.out command");return;
	}


	
	fd=socket(AF_INET,SOCK_STREAM,0);	//create TCP socket
	if(fd<0){
		perror("socket");return;
	}

	a.sin_family=AF_INET;			
	a.sin_port=htons(atoi(argv[1]));
	a.sin_addr.s_addr=inet_addr("192.168.1.2");	//loopback
	len=sizeof(a);	
	if(connect(fd,(struct sockaddr*)&a,len)<0){	//connect with server
		perror("connect");return;
	}
	signal(SIGINT,my);		//handle terminating interrupt
	while(1){
		printf("\n$ ");		//take commands
		scanf(" %[^\n]s",ch);
		if(!strcmp(ch,"exit")){

			write(fd,"exit",5);		//if exit is given, close the other side
			break;
		}
		putchar('\n');
		write(fd,ch,sizeof(ch));	//write to socket and send
	while( ( i=read(fd,ch,sizeof(ch) ) ) >0 ){	//read the output from server
		if(!strcmp(ch,"#@#"))		//terminating string, if provided, come out of this loop
			break;
		ch[i]=0;
		printf("%s",ch);
		bzero(ch,100);
	}
}

	printf("good bye...\n");
	usleep(300000);
	shutdown(fd,2);		//close the socket

}
