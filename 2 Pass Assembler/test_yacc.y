%{
	#include<stdio.h>
	#include<unistd.h>
	extern int yylex();
	extern FILE *yyin;
	extern FILE *yyout;
	FILE *datafile;
        extern char* yytext;
          extern int yylineno;
%}
%token LB STR NUM WS NL OP END SLB RB RW
%%	
STMT:  X  	{printf("\n\nValid\n\n");}
S : STR WS NUM 
 
 ;
B : LB WS OP WS LB
 | LB WS OP WS NUM
 | OP WS LB
 | OP WS NUM
 | LB WS OP WS SLB
 | OP WS SLB
 | END WS LB
 | END
 | END WS SLB
 | RW
 | RB
 
 
 ;
X: S NL X
 | B NL X
 | 
;
%%
int main(){
FILE *myfile = fopen("in1.txt","r");
FILE *outfile = fopen("out.txt","w");
datafile = fopen("data.dat","w");
yyin = myfile;
yyout = outfile;
yyparse();
fclose(myfile);
fclose(outfile);
fclose(datafile);
execl("/media/akshay/New Volume1/RVCE/Seminar/6th sem/sscd/ours/pass2","pass2",NULL);
}
int yyerror(){

printf("error. %s, %d\n",yytext,yylineno);
}
