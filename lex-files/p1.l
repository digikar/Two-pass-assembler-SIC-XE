%{
	#include<stdio.h>
	#include<fcntl.h>
	#include<unistd.h>
	#include<stdlib.h>
	#include"y.tab.h"
	#include"data_structures.h"
	int lineNum,incr,endF,symVal,strtF,loadAddr,beforeOpCodeF,temp,errorF,reswF=0,resbF=0,byteF=0;
	char char_arr[10];
	char buff[50];
	char *ptr;
	extern FILE *symfile;
%}

%option yylineno

%%
[a-zA-Z]+[ ]+"START"	{
                            write(fileno(yyout),yytext+6,yyleng-6);
                            ptr = strchr(yytext,' ');
                            strncpy(progName,yytext,(ptr-yytext));
                            fprintf(symfile,"char Name[] = \"%s\";struct sym{char *label;long int val;}; struct sym tab[]={",progName);
                            strtF=1;
                            return STR;
                        }
#[0-9]+	{write(fileno(yyout),yytext,yyleng);return NUM;}
[0-9]+	{
           // write(fileno(yyout),yytext,yyleng);
            if(reswF==1){
                write(fileno(yyout),yytext,yyleng);
                LOCCTR += 3*(atoi(yytext));
                reswF=0;
            }
            else if(resbF==1){
               write(fileno(yyout),yytext,yyleng);
                LOCCTR += atoi(yytext);
                resbF=0; 
            }
            else if(strtF==1){
		write(fileno(yyout),yytext,yyleng);
                STARTADDR =atoi(yytext);
                LOCCTR =STARTADDR;
                strtF=0;
                }
            return NUM;
        }
"END"	{
            write(fileno(yyout),yytext,yyleng);
            progLength = LOCCTR-STARTADDR;
            dispSymTab();
            printf("The program size is :\t %ld\n%s\n",progLength,progName);
            endF = 1;
            fprintf(symfile,"};\nlong int progSize = %ld;\nlong int startAddr=%ld;\n int siz=sizeof(tab)/16;\n",progLength,STARTADDR);
            return END;
        }
[ \t]+	{write(fileno(yyout),yytext,yyleng);return WS;}                     
[AXLBSTF]","[AXLBSTF]|[AXLBSTF]|[a-zA-Z0-9]+","[AXLBSTF]    {write(fileno(yyout),yytext,yyleng);return LB;}
"X""'"[a-zA-Z0-9]+"'"|"C""'"[a-zA-Z0-9]+"'" {
                                            write(fileno(yyout),yytext,yyleng);
                                            if(byteF==1){
                                                LOCCTR += (yyleng-3);
                                                byteF=0;
                                            }
                                            return LB;
                                        }
[a-zA-Z0-9\+]+ {   
                if(endF==1){
                    write(fileno(yyout),yytext,yyleng);
                    loadAddr = searchSymTab(yytext);
                    endF=0;
                    return SLB;
                }
                else{
                    if((incr=searchOpTab(yytext))==-1){
                                                     if((symVal = searchSymTab(yytext))==-1 && beforeOpCodeF==0){
                                                                                             insrtSymTab(yytext);
                                                                                             fprintf(symfile,"{\"%s\",%ld},",yytext,LOCCTR);
                                                                                             return LB;
                                                                                             }
                                                     else if((symVal = searchSymTab(yytext))==-1 && beforeOpCodeF==-1){
							printf("\n\n****ERROR: Forward Referencing****\n\n\n");                                                                
								exit(0);
                                                                temp = LOCCTR;
                                                                LOCCTR = 0;
                                                                fflush(stdin);
                                                                write(fileno(yyout),yytext,yyleng);
                                                                insrtSymTab(yytext);
                                                                LOCCTR = temp;
                                                                return  LB;
                                                     }
                                                    else if((symVal = searchSymTab(yytext))>0 && beforeOpCodeF==0){
                                                                errorF = 1;
                                                                printf("Error: Duplicate entry line: %d\n",lineNum);
                                                                return SLB;
                                                    }
                                                    else if((symVal = searchSymTab(yytext))==0 && beforeOpCodeF==0){
                                                                insrtSymTab(yytext);
                                                                fprintf(symfile,"{\"%s\",%ld},",yytext,LOCCTR);
                                                                return SLB;
                                                    
                                                    }
                                                    else if((symVal = searchSymTab(yytext))==0 && beforeOpCodeF==-1){
                                                                write(fileno(yyout),yytext,yyleng);
                                                                return SLB;
                                                                
                                                    
                                                    }
                                                    else if((symVal = searchSymTab(yytext))>0 && beforeOpCodeF==-1 ){
                                                                write(fileno(yyout),yytext,yyleng);
                                                                return SLB;
                                                    }
                                                     else{
                                                                write(fileno(yyout),char_arr,strlen(char_arr));
                                                                return SLB;
                                                            }
                                                       }
                    else{
                            if(beforeOpCodeF == -1){
                                errorF = 1;
                                printf("\nError: %d\n",lineNum);
                                return LB;
                                
                            }
                            else{
                                if(strcmp("RESW",yytext)==0){
                                    write(fileno(yyout),yytext,yyleng);
                                    reswF = 1;
                                    return OP;
                                }
                                else if(strcmp("RESB",yytext)==0){
                                    write(fileno(yyout),yytext,yyleng);
                                    resbF = 1;
                                    return OP;
                                }
                                else if(strcmp("BYTE",yytext)==0){
                                    write(fileno(yyout),yytext,yyleng);
                                    byteF = 1;
                                    return OP;
                                }
                                else{
                                write(fileno(yyout),yytext,yyleng);
                                LOCCTR += incr;
                                beforeOpCodeF = -1;
                                return OP;
                                }
                            }
                      }
                } 
             }
\n	{
        write(fileno(yyout),yytext,yyleng);
        beforeOpCodeF = 0;
        lineNum++;
        return NL;
    }
\.[a-zA-Z0-9 \t]*   ;
.	;
%%
int yywrap(){return 1;}


