#include<sys/stat.h>
#include "./header/common.h"

typedef struct _stat {
    struct stat fileStat;
    char *fileName;
} fileInfo;

char* perm[8] ={"---","--x","-w-","-wx","r--","r-x","rw-","rwx"};

int main(int argv,char* argc[]){
    if(argv <  2){
        fprintf(stderr,"usage:%s <filepathname>",argc[0]);
        exit(-1);
    }
    int i=0;
    fileInfo * fileInfos = (fileInfo*)calloc(argv,sizeof(fileInfo));
    memset(fileInfos,0,sizeof(fileInfo)*argv);
    for(i=1;i<argv;i++){
        if(stat(argc[i],&(fileInfos[i-1].fileStat))<0){
            fprintf(stderr,"error:%s",strerror(errno));
            continue;
        }
//        if(S_ISREG(fileInfos[i-1].fileStat.st_mode)){
//            
//        }
        fileInfos[i-1].fileName = (char*)calloc(strlen(argc[i])+1,sizeof(char));
        strcpy(fileInfos[i-1].fileName,argc[i]);
        printf("%s",perm[fileInfos[i-1].fileStat.st_mode >>6 & 7]);                    //文件拥有者的权限
        printf("%s",perm[fileInfos[i-1].fileStat.st_mode >>3 & 7]);                    //文件拥有者的所在组成员的权限
        printf("%s",perm[fileInfos[i-1].fileStat.st_mode & 7]);                           //其他成员的权限
        PFS(ctime(&fileInfos[i-1].fileStat.st_ctim.tv_sec));
    }
}

