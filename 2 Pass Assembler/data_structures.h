#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#define SYMBSIZE 10
#define SYMBLIMIT 200

long int LOCCTR=0;
long int STARTADDR;
long int progLength;
char progName[7];
/*==========================================OPTAB==============================================================*/
typedef struct tabinfo{
	int val;
	int type;
	int info;
} tabdata;

struct tablemem{
	char *symbol;//CHANGED	
	tabdata symbdata;
};

struct tablemem opcodetab[] = {
	{"ADD"   ,0X18, 0, 3}, {"ADDR"  ,0X90, 0, 2}, {"AND"   ,0X40, 0, 3}, 
{"BASE"  ,0   ,20, 0}, {"BYTE"  ,0   , 1, 0}, 
{"CLEAR" ,0XB4, 0, 2}, {"COMP"  ,0X28, 0, 3}, {"COMPR" ,0XA0, 0, 2}, 
{"CSECT" ,0   ,50, 0},
{"DIV "   ,0X24, 0, 3}, {"DIVR"  ,0X9C, 0, 2},
{"END"   ,0   ,11, 0},
{"EQU"   ,0   ,80, 0}, {"EXTDEF",0   ,40, 0}, {"EXTREF",0   ,41, 0}, 
{"J"     ,0X3C, 0, 3}, {"JEQ"   ,0X30, 0, 3}, {"JGT"   ,0X34, 0, 3},
{"JLT"   ,0X38, 0, 3}, {"JSUB"  ,0X48, 0, 3}, {"LDA"   ,0X00, 0, 3},
{"LDB"   ,0X68, 0, 3}, 
{"LDCH"  ,0X50, 0, 3}, {"LDL"   ,0X08, 0, 3},
{"LDS"   ,0X6C, 0, 3}, {"LDT"   ,0X74, 0, 3}, {"LDX"   ,0X04, 0, 3}, 
{"LTORG" ,0   ,30, 0}, 
{"MUL"   ,0X20, 0, 3}, {"MULR"  ,0X98, 0, 2}, 
{"NOBASE",0   ,21, 0}, {"NOP"   ,0   ,70, 0}, 
{"OR"    ,0X44, 0, 3},
{"ORG"   ,0   ,61, 0}, 
{"RD"    ,0XD8, 0, 3}, 
{"RESB"  ,0   , 2, 1}, {"RESW"  ,0   , 2, 1}, 
{"RMO"   ,0XAC, 0, 2}, {"RSUB"  ,0X4C, 0, 3}, 
{"SHIFTL",0XA4, 0, 2}, {"SHIFTR",0XA8, 0, 2}, {"STA"   ,0X0C, 0, 3},
{"STB"   ,0X78, 0, 3}, {"STCH"  ,0X54, 0, 3}, {"STL"   ,0X14, 0, 3},
{"STS"   ,0X7C, 0, 3}, {"STT"   ,0X84, 0, 3}, {"STX"   ,0X10, 0, 3}, 
{"SUB"   ,0X1C, 0, 3}, {"SUBR"  ,0X94, 0, 2}, {"TD"    ,0XE0, 0, 3},
{"TIX"   ,0X2C, 0, 3}, {"TIXR"  ,0XB8, 0, 2}, 
{"USE"   ,0   ,60, 0}, 
{"WD"    ,0XDC, 0, 3}, 
{"WORD"  ,0   , 4, 3},
};

int optabsize = sizeof(opcodetab)/sizeof(struct tablemem);

int searchOpTab(char s[]){
int i;
	for(i=0;i<56;i++){

		if(strcmp(s,opcodetab[i].symbol)==0)
			return opcodetab[i].symbdata.info;

	}
	
	return -1;

}




int sOpTab(char s[]){
int j;
	for(j=0;j<58;j++){

		if(strcmp(s,opcodetab[j].symbol)==0)
			return opcodetab[j].symbdata.val;

	}
	
	return -1;
}
/*=====================================SYMBTAB==========================================================*/
typedef struct{
	char symbol[SYMBSIZE];
	int value;
	//int casenmbr;
	//int otherinfo;
} tabletype;

tabletype symbtab[SYMBLIMIT];
static int symbIndex=0;
int searchSymTab(char s[]){
int i;
char str[10];

/*if(s[0]=='@')
{	char* token;
			token=strtok(s,"@");

sprintf(str,"%s",token);
//strcpy(str,s+1);
printf("data.h->%s",str);
}
else
	strcpy(str,s);*/
	for(i=0;i<symbIndex;i++)
		if(strcmp(s,symbtab[i].symbol)==0)
			return symbtab[i].value;

	return -1;

}
void updateSymTab(char s[]){
int i;
	for(i=0;i<symbIndex;i++)
		if(strcmp(symbtab[i].symbol,s)==0)
			symbtab[i].value=LOCCTR;

	

}
void insrtSymTab(char *s){

	strcpy(symbtab[symbIndex].symbol,s);
	symbtab[symbIndex].value = LOCCTR;
	symbIndex++;	

}
void RegisterUpdate()
{
int i;
for(i=0;i<symbIndex;i++)
{
if(strcmp(symbtab[i].symbol,"A")==0) symbtab[i].value=0;
if(strcmp(symbtab[i].symbol,"X")==0) symbtab[i].value=1;
if(strcmp(symbtab[i].symbol,"L")==0) symbtab[i].value=2;
if(strcmp(symbtab[i].symbol,"B")==0) symbtab[i].value=3;
if(strcmp(symbtab[i].symbol,"S")==0) symbtab[i].value=4;
if(strcmp(symbtab[i].symbol,"T")==0) symbtab[i].value=5;
if(strcmp(symbtab[i].symbol,"F")==0) symbtab[i].value=6;
}
}
void dispSymTab(){
int i,f,ssize;
RegisterUpdate();
	
f=open("info.h",O_TRUNC|O_WRONLY);
if(f<0)
	printf("error");

	close(f);

f=open("info.h",O_WRONLY);
	lseek(f,0,2);
char str[100]={'\0'};
	
sprintf(str,"char PN[7]=\"%s\";\nint PS=%ld;\nint SA=%ld;\nint labelCount=%d;\n",progName,progLength,STARTADDR,symbIndex);
ssize=strlen(str);
write(f,str,ssize);

sprintf(str,"struct sym{char *label;int val;};\nstruct sym tab[]={");
ssize=strlen(str);
write(f,str,ssize);
for(i=0;i<symbIndex;i++)
{
char str[100]={'\0'};
sprintf(str,"{\"%s\",%d},\n",symbtab[i].symbol,symbtab[i].value);
ssize=strlen(str);
/*write(f,symbtab[i].symbol,sizeof(symbtab[i].symbol));
write(f," ",sizeof(" "));
*/
write(f,str,ssize);

}
sprintf(str,"};");
ssize=strlen(str);
write(f,str,ssize);
	close(f);



printf("The symbol table is:\n");
	for(i=0;i<symbIndex;i++)
		printf("%s %d\n",symbtab[i].symbol,symbtab[i].value);


}


//========regtab============

char regtab[][2] = {
                        {'A',00},
                        {'X',01},
                        {'L',02},
                        {'B',03},
                        {'S',04},
                        {'T',05},
                        {'F',06}
                  };
int searchRegTab(char s){
int i;
    for(i=0;i<7;i++){
        if(s==regtab[i][0])
            return regtab[i][1];
    }
    return -1;
}
