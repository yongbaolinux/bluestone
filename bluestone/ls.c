#include<sys/stat.h>
#include<dirent.h>
#include "./header/common.h"

typedef struct _stat {
    struct stat fileStat;
    char *fileName;
} fileInfo;

char* type[7] ={"p","c","d","b","-","l","s"};                                                   //文件类型索引表
char* perm[8] ={"---","--x","-w-","-wx","r--","r-x","rw-","rwx"};                //文件访问权限索引表

int main(int argv,char* argc[]){
    if(argv <  2){
//        fprintf(stderr,"usage:%s <filepathname>",argc[0]);
//        exit(-1);
        argv=2;
        argc[1] = ".";
    }
    int i=0;
    fileInfo * fileInfos = (fileInfo*)calloc(1,sizeof(fileInfo));
    fileInfos->fileName = (char*)calloc(strlen(argc[1])+1,sizeof(char));
    
        if(stat(argc[1],&(fileInfos->fileStat))<0){
            fprintf(stderr,"error:%s",strerror(errno));
            exit(-1);
        }

    if(!S_ISDIR(fileInfos->fileStat.st_mode)){
            strcpy(fileInfos->fileName,argc[1]);
            //文件类型
            printf("%s",type[(fileInfos->fileStat.st_mode>>12 & 15)/2]);
            //文件权限
            printf("%s",perm[fileInfos->fileStat.st_mode >>6 & 7]);                    //文件拥有者的权限
            printf("%s",perm[fileInfos->fileStat.st_mode >>3 & 7]);                    //文件拥有者的所在组成员的权限
            printf("%s",perm[fileInfos->fileStat.st_mode & 7]);                           //其他成员的权限
            printf("%s"," ");
            //最后被修改的时间
            printf("%s",ctime(&fileInfos->fileStat.st_mtim.tv_sec));
            printf("%s"," ");
            //文件大小
            printf("%d",fileInfos->fileStat.st_size);
    } else if(S_ISDIR(fileInfos->fileStat.st_mode)){
            if()    
    }
}

char* basename(){
    
}

