#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <mysql/mysql.h>
#include "/usr/include/mariadb/mysql.h"

#define PORT 8080
#define MAXSZ 100

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "3083"
#define DB_NAME "container"
#define CHOP(x) x[strlen(x) - 1] = ' '

#define BUF_LEN 128			//buffer length
#define SERV_IP_ADDR "192.168.4.2"		//our IP addr
#define SERV_PORT 9000		//registered port 영역, server socket로 사용하는 영역의 것으로 define
int main(int argc,char *argv[])
{    
	char buffer[BUF_LEN];    
	struct sockaddr_in server_addr;		//구조체 선언
	struct sockaddr_in client_addr;		// struct 태그명 변수명
	char temp[20];    					//
	int server_fd,client_fd;			//file descripter(파일 지정번호)
	int len,msg_size;					//msg_size?ok	//what is the mean of len?
	char test[20];						//test...?

	MYSQL       *connection = NULL;
	MYSQL		conn;
	MYSQL_RES   *sql_result;
	MYSQL_ROW   sql_row;
	int       query_stat;

	mysql_init(&conn);

	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);


	if (connection == NULL)
	{
		printf("%d 에러 : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	if (mysql_select_db(&conn, DB_NAME))
	{
		printf("%d 에러 : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	query_stat = mysql_query(connection, "select * from con0001");
	if (query_stat != 0)
	{
		fprintf(stderr, "2Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	// sql_result = mysql_store_result(connection);
	//  mysql_free_result(sql_result);



	/****
	if(argc !=2)    
	{        
		printf("Usege ./filename [PORT] \n");        
		exit(0);    
	}    
	******/	

	if((server_fd = socket(PF_INET,SOCK_STREAM,0)) == -1) 
	//socket(2)함수를 이용해서 socket생성/파일 지정번호 반환, socket()안 기본설정..그냥 이걸로해 TCP/IP는
	{        
		printf("Server: can not Open Socket\n");        //server socket 생성 error
		exit(0);    
	}       
	memset(&server_addr,0x00,sizeof(server_addr));       //server_addr를 0으로 초기화
	server_addr.sin_family = AF_INET;    				//addr family 설정
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    
	//server_addr.sin_addr.s_addr = inet_addr(SERV_IP_ADDR);
	//server_addr.sin_port = htons(atoi(argv[1]));    
	server_addr.sin_port = htons(SERV_PORT);       
	printf("1\n");
	if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )    //bind 함수를 이용해서 port number 설정
	{        printf("Server:*** cat not bind local addrss\n");        
			 exit(0);            
	}
	printf("2\n");	
	if(listen(server_fd,5) < 0)    		//server port number가 설정된 socket에 client의 접속 queue크기 설정
	{        
		printf("Server:** cat not listen connnect.\n");        
		exit(0);    
	}
	printf("3\n");	
	memset(buffer,0x00,sizeof(buffer));			  	//buffer 초기화
	len = sizeof(client_addr);    					//len에 client addr 사이즈 저장
	//printf("=====[PORT] : %d =====\n",atoi(argv[1]));    
	printf("Server : wating connection request.\n");         


	while(1)    
	{        
		//parent process waiting to accept a new connection
		printf("\n*****server waiting for new client connection:*****\n");
		len = sizeof(client_addr);
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
		printf("connected to client: %s\n", inet_ntoa(client_addr.sin_addr));

		pid_t pid;
		//child process is created for serving each new clients
		pid = fork();
		if (pid == 0)//child process rec and send
		{
			//rceive from client
			while (1)
			{
				msg_size = recv(client_fd, buffer, BUF_LEN, 0);
				if (msg_size == 0)
				{
					close(client_fd);
					break;
				}
				buffer[msg_size] = 0;

				char mes[BUF_LEN];
				strcpy(mes, buffer);
				char sArr[20][20] = { 0 };

				char *token = NULL;
				char str2[] = "|";//

				token = strtok(mes, str2);
				int i = 0;
				while (token != NULL)
				{
					strcpy(sArr[i], token);
					token = strtok(NULL, str2);
					i++;
				}
				char query;
				sprintf(query, "insert into con0001(temp, ,,,) values('%s', '%s', '%s', '%s', '%s', '%s')",
					sArr[0], sArr[1], sArr[2], sArr[3], sArr[4], sArr[5]);
				mysql_query(&conn, query);


				send(client_fd, buffer, msg_size, 0);
				query_stat = mysql_query(connection, query);
				if (query_stat != 0)
				{
					fprintf(stderr, "3Mysql query error : %s", mysql_error(&conn));
					//return 1;
				}

				//printf("Receive and set:%s\n", buffer);
			}//close interior while
			exit(0);
		}
		else
		{
			close(client_fd);//sock is closed BY PARENT
		}
	}        		
	mysql_close(&conn);
	return 0;
}
