%{
  #include<stdio.h>
  int lines=0;
  int chars=0;
  int words=0;
%}
%%
[a-zA-Z\.\_\'\"]+ {words++;chars+=yyleng;}
\n {lines++;chars++;}
. {chars++;}
%%
void main(int argv,char* argc[]){
	yylex();
	printf("%8d%8d%8d",lines,chars,words);
}
int yywrap(){
	return 1;
}
