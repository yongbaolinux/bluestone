#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>                                          //sockaddr_in
#include<string.h>                                                //strlen
#ifndef  DEBUG
#define DEBUG
#endif
int main(int  argv,char *argc[]){
    int socket_client;
    struct sockaddr_in  server_addr;
    int send_len;
    //initial the system
    if(argv != 4){
        printf("%s","parameter is missing!");
        exit(-1);
    }
    socket_client = socket(AF_INET,SOCK_STREAM,0);              //tcp:SOCK_STREAM               udp:SOCK_DGRAM
    if(socket_client<0){
        exit(-2);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argc[2]));
    server_addr.sin_addr.s_addr = inet_addr(argc[1]);
    //running
    int status;
    status = connect(socket_client,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
#ifdef DEBUG
    if(status<0){
        printf("%s","connect failed");
        exit(-3);
    } else {
        printf("%s","connet the server successfully\n");
    }
#endif
    send_len = strlen(argc[3]);
    //重新定义发送数据包 将数据包的长度转为字符后存到数据包的前部 然后再用一个字节来存放这个字符串的长度
    char s[1024];
    sprintf(s,"%d",send_len);
    int s_len = strlen(s);
    char string[send_len+s_len+2];                                      //开头的1个字节来存放字符串的长度  结尾的1个 '\0'
    string[0] = s_len;
    int i;
    for(i=0;i<s_len;i++){
        string[i+1] = s[i];
    }
    for(i=0;i<send_len;i++){
        string[s_len+1+i] =argc[3][i]; 
    }
    string[s_len+send_len+1] = '\0';
    int new_length = send_len+s_len+2;
    status = send(socket_client,string,new_length,0);                   //发送重新组装的字符串 开头的一个字节和结尾的'\0'也发送过去
#ifdef DEBUG
    if(status != new_length){
        printf("%s","send error!");
        exit(-4);
    } else {
        printf("%s","send success");
        char recv_data[100];
        status = recv(socket_client,recv_data,99,0);
        printf("%d",status);
        close(socket_client);
    }
#endif
}

