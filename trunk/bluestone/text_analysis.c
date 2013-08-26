#ifndef _COMMON_H_
#include "./header/common.h"
#endif

extern void copy_element(void * [],int ,char *,int,int [],char * []);
extern void check_if_key(int [],int i,char*,char * []);

int main(int argv,char *argc[]){
    if(argv<2){
        PFS("the parameter error");
        exit(-1);
    }
    //读取文件
    FILE * fp = fopen(argc[1],"r");
    if(fp==NULL){
        PFS("open the file error");
        exit(-2);
    }
    int cur_pointer=0;    //临时字符的位置游标
    int ch=0;                   //当前读取的字符
    char temp[100];             //临时字符存放
    int status=0;                       //状态分四种 0为空白 1为字符 2为数字 3为其他字符
    int status__=0;                      //当status为3时 还细分多字符操作符和单字符操作符
    void *elements[1000];               //存放各元素的主字符串指针数组(token值)
    int i=0;                                            //主字符串指针数组和syn值数组的当前索引(两数组同步)
    int syn[1000];                              //syn值数组
    char *keys[9] = {"if","else","do","while","for","int","double","float","short"};
                                                           //系统保留关键词数组
    while((ch=fgetc(fp)) != '#'){
        if(ch==' '){
            //状态变化 临时字符串以' \0 '结尾
            if((status == 1) ||(status == 2)){
                temp[cur_pointer] = '\0';
                //结尾后位置归0 将字符串拷贝到主字符串数组作永久保存
                cur_pointer = 0;
                //拷贝字符串 从临时到主
                copy_element(elements,i,temp,status,syn,keys);
                i++;
            }
            status=0;
            continue;
            //如果是字母 可能是变量名、函数名或保留关键字
        } else if((ch>'a' && ch<'z') || (ch>'A' && ch<'Z')){
            if(status == 2){
                temp[cur_pointer] = '\0';
                //位置归0
                cur_pointer = 0;
                //拷贝字符串 从临时到主
                copy_element(elements,i,temp,2,syn,keys);
                i++;
            }
            status=1;
            temp[cur_pointer++] = ch;
        } else if(ch>'0' && ch<'9'){
            if(status == 1){
                temp[cur_pointer] = '\0';
                cur_pointer = 0;
                //拷贝字符串 从临时到主
                copy_element(elements,i,temp,1,syn,keys);
                i++;
            }
            status = 2;
            temp[cur_pointer++] = ch;
        } else {
            //第三种字符就直接存入主字符串数组 不放入临时字符
            if((status == 1) || (status == 2)){
                temp[cur_pointer] = '\0';
                cur_pointer = 0;
                //拷贝字符串 从临时到主
                copy_element(elements,i,temp,status,syn,keys);
                i++;
            }
            status = 3;
            switch(ch){
                case '<':
                    status__=24;
                    elements[i] = "<";
                    syn[i] = 24;
                    i++;
                    break;
                case '>':
                    if(status__==24){
                        //操作符匹配结束 status__置0
                        status__ = 0;
                        elements[--i] = "<>";
                        syn[i] = 19;
                    } else {
                        elements[i] = ">";
                        syn[i] = 23;
                        status__ = 23;
                    }
                    i++;
                    break;
                case '=':
                    if(status__ == 22){
                        status__ = 0;
                        elements[--i] = "==";
                        syn[i] = 18;
                    } else if(status__ == 23){
                        status__ = 0;
                        elements[--i] = ">=";
                        syn[i] = 21;
                    }else if(status__ == 24){
                        status__ = 0;
                        elements[--i] = "<=";
                        syn[i] = 20;
                    } else {
                        status__ = 22;
                        elements[i] = "=";
                        syn[i] = 22;
                    }
                    i++;
                    break;
                case '+':
                    elements[i] = "+";
                    syn[i] = 12;
                    break;
                case '-':
                    elements[i] = "-";
                    syn[i] = 13;
                    break;
                case '*':
                    elements[i] = "*";
                    syn[i] = 14;
                    break;
                case '/':
                    elements[i] = "/";
                    syn[i] = 15;
                    break;
                case '?':
                    elements[i] = "?";
                    syn[i] = 16;
                    break;
                case ':':
                    elements[i] = ":";
                    syn[i] = 17;
                    break;
                case ';':
                    elements[i] = ";";
                    syn[i] = 25;
                    break;
            }
        }
    }
    
    //test
    int m=0;
    for(m=0;m<i;m++){
        printf("%s","(");
        printf("%s",elements[m]);
        printf("%s",",");
        printf("%d",syn[m]);
        printf("%s",")\n");
    }
}

//copy elements and analysis the element and write the syn array
void copy_element(void * elements[],int i,char *temp,int status,int syn[],char *keys[]){
    if(status == 1){
        elements[i] = (char*)calloc(1,strlen(temp)+1);
        strcpy(elements[i],temp);
        check_if_key(syn,i,temp,keys);
    } else if(status == 2){
        int element = atoi(temp);
        elements[i] = calloc(1,sizeof(int));
        *(int*)elements[i] = element;
        syn[i] = 11;
    }
}

//检查字符是否为系统保留关键词
void check_if_key(int syn[],int i,char* temp,char *keys[]){
    int j=0;
    for(j=0;j<9;j++){
        if(strcmp(temp,keys[j])==0){
            syn[i] = j+1;
        } else {
            syn[i] = 10;
        }
    }
}
