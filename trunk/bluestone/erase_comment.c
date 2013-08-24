#include<stdio.h>
#include<stdlib.h>

#define PFD(x) printf("%d",x);\
				printf("%s","\n")
#define PFC(x) printf("%c",x);\
				printf("%s","\n")
#define PFS(x) printf("%s",x);\
				printf("%s","\n")

int main(int argv,char *argc[]){
	if(argv<2){
		printf("%s","the parameter error");
		exit(-1);
	}
	int ch=0,count=0,pos_start=0,pos_end=0,status=0,cur_pointer=0,ignore=0;
	char s[1024];
	FILE *fp=fopen(argc[1],"r");
	if(fp==NULL){
		printf("%s","open the file failed");
		exit(-2);
	}
	while((ch=fgetc(fp)) != EOF){
		count++;
		switch(ch){
			case '/':
				if(status==0 || status==4){
					status=1;
				}else if(status==3){
					status=4;
					pos_end = count;
					//PFD(pos_end);
				}
				break;
			case '*':
				if(status==1){
					status=2;
					pos_start = count-1;
					//PFD(pos_start);
					ignore=1;
					cur_pointer--;
				}else if(status==2){
					status=3;
				}
				break;
			default:
				if(status==1){
					status=0;
				}else if(status==3){
					status=2;
				}else if(status==4){
					ignore=0;
					status=0;
				}
				break;		
		}
		if(ignore){
			continue;
		}else{
			s[cur_pointer++]=ch;
		}
	}
	s[cur_pointer]='\0';

	//printf("%d",count);
	PFS(s);
	return 1;
}
