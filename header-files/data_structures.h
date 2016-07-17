#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SYMBSIZE 10
#define SYMBLIMIT 200

long int LOCCTR=0;
long int STARTADDR;
long int progLength;
char progName[15];
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
{"MUL "   ,0X20, 0, 3}, {"MULR"  ,0X98, 0, 2}, 
{"NOBASE",0   ,21, 0}, {"NOP"   ,0   ,70, 0}, 
{"OR"    ,0X44, 0, 3},
{"ORG"   ,0   ,61, 0}, 
{"RD"    ,0XD8, 0, 3}, 
{"RESB"  ,0   , 2, 1}, {"RESW"  ,0   , 3, 3}, 
{"RMO"   ,0XAC, 0, 2}, {"RSUB"  ,0X4C, 0, 3}, 
{"SHIFTL",0XA4, 0, 2}, {"SHIFTR",0XA8, 0, 2}, {"STA"   ,0X0C, 0, 3},
{"STB"   ,0X78, 0, 3}, {"STCH"  ,0X54, 0, 3}, {"STL"   ,0X14, 0, 3},
{"STS"   ,0X7C, 0, 3}, {"STT"   ,0X84, 0, 3}, {"STX"   ,0X10, 0, 3}, 
{"SUB"   ,0X1C, 0, 3}, {"SUBR"  ,0X94, 0, 2}, {"TD"    ,0XE0, 0, 3},
{"TIX"   ,0X2C, 0, 3}, {"TIXR"  ,0XB8, 0, 2}, 
{"USE"   ,0   ,60, 0}, 
{"WD"    ,0XDC, 0, 3}, 
{"WORD"  ,0   , 4, 3},
{"+JSUB" ,0X48, 0, 4},
{"+LDT"  ,0X74, 0, 4},
};

int optabsize = sizeof(opcodetab)/sizeof(struct tablemem);

int searchOpTab(char s[]){
int i;
	for(i=0;i<58;i++){

		if(strcmp(s,opcodetab[i].symbol)==0)
			return opcodetab[i].symbdata.info;

	}
	
	return -1;

}

int sOpTab(char s[]){
int i;
	for(i=0;i<58;i++){

		if(strcmp(s,opcodetab[i].symbol)==0)
			return opcodetab[i].symbdata.val;

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
	for(i=0;i<symbIndex;i++)
		if(strcmp(symbtab[i].symbol,s)==0)
			return symbtab[i].value;

	return -1;

}
void insrtSymTab(char *s){
int i;
    for(i=0;i<symbIndex;i++)
        if(strcmp(symbtab[i].symbol,s)==0)
	        {symbtab[i].value = LOCCTR;return;}

	strcpy(symbtab[symbIndex].symbol,s);
	symbtab[symbIndex].value = LOCCTR;
	symbIndex++;	

}
void dispSymTab(){
int i;
	printf("The symbol table is:\n");
	for(i=0;i<symbIndex;i++)
		printf("%s %04d\n",symbtab[i].symbol,symbtab[i].value);

}

/*=====================================REGTAB==============================================================*/
char REGTAB[][2] = {
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
        if(s==REGTAB[i][0])
            return REGTAB[i][1];
    }
    return -1;
}
/*======================================================hex====================================================*/
char toHexDig(int a)
{
    if(a==0) return '0';
    if(a==1) return '1';
    if(a==2) return '2';
    if(a==3) return '3';
    if(a==4) return '4';
    if(a==5) return '5';
    if(a==6) return '6';
    if(a==7) return '7';
    if(a==8) return '8';
    if(a==9) return '9';
    if(a==10) return 'A';
    if(a==11) return 'B';
    if(a==12) return 'C';
    if(a==13) return 'D';
    if(a==14) return 'E';
    if(a==15) return 'F';
}
char hex[15];
void toHex(int n)
{
    
    int i = 0;
    while(n) {
        hex[i++] = toHexDig(n % 16);// + '0';
        n /= 16;
    }
    if(i<=3){
        while(i!=3){
            hex[i++] = '0';
        }
    }
}
void reverse(char * hex)
{
  if (hex)
  {
    char * end = hex + strlen(hex) - 1;
#   define XOR_SWAP(a,b) do\
    {\
      a ^= b;\
      b ^= a;\
      a ^= b;\
    } while (0)
    while (hex < end)
    {
      XOR_SWAP(*hex, *end);
      hex++;
      end--;
    }
#   undef XOR_SWAP
  }
}
