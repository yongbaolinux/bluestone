#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>                           //inet_ntoa

#ifndef KEEP_ALIVE
        #define KEEP_ALIVE
#endif
#ifndef DEBUG
        #define DEBUG
#endif
#define BUFFSIZE 1000
#define MAXSIZE 1000

void dump(struct sockaddr_in);
char **explode(char*,char*,int,char,char**);

int main(int argv,char *argc[]){
    if(argv!=3){
        printf("%s","parameter error");
        exit(-1);
    }
    int socket_server;
    socket_server = socket(AF_INET,SOCK_STREAM,0);
    if(socket_server<0){
        printf("%s","create socket failed");
        exit(-2);
    }
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    memset(&server_addr,0,sizeof(server_addr));
    memset(&client_addr,0,sizeof(client_addr));
    //server_addr initial
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argc[1]));
    int status;
    //bind  success return 0  failed  return -1
    status = bind(socket_server,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(status<0){
        printf("%s","bind failed");
        exit(-3);
    }
    //listen success return 0 . failded reurn SOCKET_ERROR
    status = listen(socket_server,atoi(argc[2]));
    if(status !=0 ){
        printf("%s","error listening on socket");
        exit(-4);
    }
    //accept success return  a new socket .  failed return -1
    int client_addr_len = sizeof(client_addr);
    //return the socket contained the request data
    int socket_server_clone;
#ifdef KEEP_ALIVE
    /*while(1){
              socket_server_clone = accept(socket_server,(struct sockaddr*)&client_addr,&client_addr_len);
              if(socket_server_clone ==-1 ){
                  printf("%s","failed to accpet the request of client");
                  exit(-5);
              } else {
                  char buffer[BUFFSIZE];
                  status = recv(socket_server_clone)
              }
    }*/
    socket_server_clone = accept(socket_server,(struct sockaddr*)&client_addr,&client_addr_len);
    if(socket_server_clone == -1){
        printf("%s","failed to accept the request of client");
        exit(-5);
    } else {
        char buffer[BUFFSIZE];
        status = recv(socket_server_clone,buffer,BUFFSIZE,0);
        char temp[BUFFSIZE];
        int i;
        for(i=0;buffer[i]!='\r';i++){
            if(buffer[i] == ' '){
                temp[i] = '\0';
            } else {
                temp[i] = buffer[i];
            }
        }
        temp[i] = '\0';
//        printf("%s",temp+strlen(temp)+1);
//        FILE *fp = fopen(temp+strlen(temp)+1,"r");
//        char buffer_[1024];
//        char send_data[10000];
//        int j=0;
//        while(fgets(buffer_,1024,fp)!=NULL){
//            for(int i=0;i<strlen(buffer_);i++){
//                send_data[j] = buffer_[i];
//                j++;
//            }
//        }
//        send_data[j] = '\0';
        //发送响应头
        char *send_header="HTTP/1.1 200 Ok";
        status = send(socket_server_clone,send_header,strlen(send_header),0);
        printf("%d",status);
    }
#else
             socket_server_clone = accept(socket_server,(struct sockaddr*)&client_addr,&client_addr_len);
              if(socket_server_clone ==-1 ){
                  printf("%s","failed to accpet the request of client");
                  exit(-5); 
              } else {
                  //dump(client_addr);
                  char buffer[BUFFSIZE];
                  char desc[MAXSIZE];                         //接受body字符的容器
                  int position;
                  status = recv(socket_server_clone,buffer,BUFFSIZE,0);                         //status is the length of received data
                  //实时计算收到的字节总数
                  //有两种方法判定是否读取完毕 一是查看字节总数  二是看是否有'\0'字符（第二种有待验证）
                  if(status ==0 ){
#ifdef DEBUG
                      printf("%s","the connection is closed\n");
#endif
                  } else if(status == -1){
#ifdef DEBUG
                      printf("%s","receive error");
#endif
                  } else {
                      int s_len = (int)buffer[0];                                       //字节数 转为字符串后占多少长度
                      if(s_len>0 && status>s_len){                              //在缓冲区极短的情况下 s_len也可能要发送两次才能发送完毕 只要把缓冲区设置长点
                          char temp[s_len+1];
                          int i;
                          for(i=0;i<s_len;i++){
                              temp[i] = buffer[i+1];
                          }
                          temp[i] = '\0';
                          int body = atoi(temp);                        //body为实体字符总数
                          int temp_len = status;                        //temp_len为实时接受到的字符总数
                          int real_len = body+s_len+1;             //数据报的总字符数

                          //copy buffer
                          for(i=s_len+1,position=0;i<status;i++,position++){
                              desc[position] = buffer[i];
                          }
                          while(real_len>temp_len && status){
                              //一次没有接受完 循环recv
                              status = recv(socket_server_clone,buffer,BUFFSIZE,0); 
                              temp_len += status;
                              for(i=0;i<status;i++,position++){
                                  desc[position] = buffer[i];
                              }
                          }
                          desc[position] = '\0';
                      } else {
                          //do something
                      }
                  }
                  //parse the string of request
                  int request_len = strlen(desc);
                  char temp[request_len+1];
                  char *string[1];
                  char** s_;
                  s_ = explode(desc,temp,request_len,' ',string);
                  
                  if(strcmp(s_[0],"GET")==0 || strcmp(s_[0],"POST")==0){
                      if(s_[1] != ""){
                          FILE *fp;
                          fp = fopen(s_[1],"r");
                          char buff[1024];
                          char send_data[10000];
                          int j=0;
                          //copy string from stream
                          while(fgets(buff,1024,fp)!=NULL){
                              for( int i=0;i<strlen(buff);i++){
                                  send_data[j] = buff[i];
                                  j++;
                              }
                          }
                          status = send(socket_server_clone,send_data,strlen(send_data),0);
#ifdef  DEBUG
                          if(status == strlen(send_data)){
                              printf("%s","send success");
                          } else {
                              printf("%s","send failed");
                          }
#endif
                      }
                  }
              }
#endif
}

void dump(struct sockaddr_in addr){
    printf("%s","struct sockaddr_in {\n");
    printf("%d",addr.sin_family);
    printf("%s","\n");
    printf("%s",inet_ntoa(addr.sin_addr));                                      //char  FAR* inet_ntoa(struct in_addr)
    printf("%s","\n");
    printf("%d",addr.sin_port);
    printf("%s","\n");
    printf("%s","}");
}

//explode the string of request via ' \s'
char** explode(char *src,char *desc,int len,char c,char ** string){
    int i,tag=0;
    int children_string_count=0;                        
    for(i=0;i<len+1;i++){
        if(src[i]==c){
            tag=0;
            desc[i] = '\0';
        } else {
            desc[i] = src[i];
            if(tag ==0 ){
                tag = 1;
                string[children_string_count] = &desc[i];
                children_string_count++;
                char ** string_;
                string_ = (char**)malloc(sizeof(char*)*(children_string_count+1));
                int j;
                for(j=0;j<children_string_count;j++){                                   //copy the pointer of string
                    string_[j] = string[j];
                }
                string = string_;  
            }
        }
    }
    return string;
}