%{
  #include<stdio.h>
  int lines=0;
  int chars=0;
  int words=0;
  int syn[1000];
  int i=0;
%}
%%
">" {syn[i]=21;i++;}
"<" {syn[i]=22;i++;}
(==) {syn[i]=23;i++;}
"=" {syn[i]=24;i++;}
(<=) {syn[i]=25;i++;}
%%
void main(int argv,char* argc[]){
	yylex();
	int j;
	for(j=0;j<i;j++){
	   printf("%d\n",syn[j]);
	}
}
int yywrap(){
	return 1;
}
