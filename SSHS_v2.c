//SSH app(server)
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include<sys/socket.h>
#include <sys/types.h> 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<unistd.h>

main(int argc,char** argv)
{
	if(argc!=2){
		printf("USAGE: ./s1 port_no\n"); return;
	}

	int fd,fd1,i,j,k,len;
	char ch[100],eval;
	char *arr[5],*temp[5];
	struct sockaddr_in a,b;

void ISR()
{
		printf("ISR\n");
		shutdown(fd,2);
		usleep(500000);
		shutdown(fd1,2);
		exit(0);

}

	fd=socket(AF_INET,SOCK_STREAM,0);	//creating socket for TCP connection

	if(fd<0){
		perror("socket");return;
	}

	//filling structure for TCP

	a.sin_family=AF_INET;
	a.sin_port=htons(atoi(argv[1]));
	a.sin_addr.s_addr=inet_addr("0.0.0.0");			//server is ready to accept any client
	len=sizeof(a);
	if(bind(fd,(struct sockaddr*)&a,len)<0){	//bind it with socket
		perror("bind");return;
	}

	if(listen(fd,3)<0){		//listen for connection with queue size a 3
		perror("listen");return;
	}	

	fd1=accept(fd,(struct sockaddr*)&b,&len);	//accept the connection and get a descriptor for client
	if(fd1<0){
		perror("accept");return;
	}
	signal(SIGINT,ISR);



	for(i=0;i<5;i++){			//create a Command array of strings to to store command and its options
		temp[i]=(char*)calloc(10,1);
	}
	//temp is a array of pointer to store command name and its arguments seperately.


	while(1){
		bzero(ch,100);		//buffer to hold the received string
//		printf("Here i am\n");
		for(i=0;i<5;i++){		//empty the Command array
			for(k=0;k<10;k++)
				temp[i][k]='\0';

			arr[i]=temp[i];		//assign 
		}
		if( ( i=read(fd1,ch,sizeof(ch)) ) )		//blocking read command in ch array
			ch[i]=0;
	//	printf("blocking read command - %s \n",ch);
		//////////////////////////////////////////////////////////////
		if(!strncmp("exit",ch,5))
			break;

		///////////seperate the arguments in command/////////////////////			
		i=0;
		j=0;
		k=0;


		while(ch[i])  {		//if options available go inside 

			if(ch[i]!=' ' ){		//check for options field
				arr[j][k++]=ch[i++];	//put charchter in Command array

				//arr array holds commands and its options seperately.
			}
			else{			//when one option or command name done, terminate the string
				arr[j][k]='\0';			
		//		printf("else arr=%s\n",arr[j]);
				j++;
				k=0;
				i++;

			}
		}//while ends
		arr[j][k]='\0';		//terminate the Last string 
		arr[j+1]='\0';		// terminate the Command array
		//				printf("arg arr=%s\n",arr[j]);
		//			printf("j=%d\n",j);

		////////////////////////pipe implemented////////////////	

		int p[2];
		pipe(p);
		if(fork()==0){		//create child and execute the command and write output to pipe
		//	printf("PIPE\n");
			close(1);
			dup(p[1]);
			close(p[0]);
			dup2(p[1],2);	//duplicate error stream with pipe write end


			i=execvp(arr[0],arr);
			if(i<0){		//if execl fails then send error to client and send signal to parent to restart
				perror("Command:");
			}
			close(p[1]);
			exit(0);
		}
		else{
						
			wait(0);
		//	printf("PIPE PARENT\n");
			close(p[1]);
			
			while( ( i=read(p[0],ch,sizeof(ch)-1) ) >0){		//read the pipe put data in 	socket for client
				ch[i]=0;
				write(fd1,ch,sizeof(ch));	
		//		printf("%s",ch);
				usleep(100000);
			}
	xyz:	write(fd1,"#@#",4);	//when data ends, intimate the client
		close(p[0]);	//close pipe
		usleep(100000);	//100ms delay
		}//else

		
	}//while

	printf("Good Bye\n");
	shutdown(fd,2);	//close socket
	usleep(500000);	//500ms delay
	shutdown(fd1,2);	//close socket

}
