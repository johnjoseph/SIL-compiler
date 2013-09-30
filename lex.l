
/* lex generation */
delim [ \t]
ws {delim}+
letter [A-Za-z]
digit [0-9]
number {digit}+(\.{digit}+)?(E[+-]?{digit}+)?
id {letter}({letter}|{digit})*
comment1 \/\/[^\n]*
comment2 \/\*[^\*\/]*\*\/ 
newline [\n] 
%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

	struct Node
	{
		char *str;
		struct Node *ptr1,*ptr2,*ptr3;
		char * key;
		int cons;	
	};

void count();

%}

%%
{delim} /* printing nothing */
";" {count();return(SEMICOLON);}
"=" {count();return(ASSIGNOP);}
"+" {count();return(PLUS);}
"-" {count();return(MINUS);}
"/" {count();return(DIVIDE);}
"*" {count();return(MULTIPLY);}
"(" {count();struct Node *foo=malloc(sizeof(struct Node));char * temp=malloc(yyleng*sizeof(char));strcpy(temp,yytext);foo->key=temp;yylval.ptr=foo;return(LEFTPAR);}
")" {count();struct Node *foo=malloc(sizeof(struct Node));char * temp=malloc(yyleng*sizeof(char));strcpy(temp,yytext);foo->key=temp;yylval.ptr=foo;return(RIGHTPAR);}
"[" {count();return(LEFTBRKT);}
"]" {count();return(RIGHTBRKT);}
"{" {count();return(LEFTCURL);}
"}" {count();return(RIGHTCURL);}
"," {count();return(COMMA);}
{comment1} /* comment */
{comment2} /* comment */
"decl" {count();return(DECL);}
"enddecl" {count();return(ENDDECL);}
"integer" {count();return(INTEGER);}
"boolean" {count();return(BOOLEAN);}
"begin" {count();return(BEG);}
"end" {count();return(END);}
"if" {count();return(IF);}
"then" {count();return(THEN);}
"else" {count();return(ELSE);}
"endif" {count();return(ENDIF);}
"AND" {count();struct Node *foo=malloc(sizeof(struct Node));char * temp=malloc(yyleng*sizeof(char));strcpy(temp,yytext);foo->key=temp;yylval.ptr=foo;return(AND);}
"OR" {count();struct Node *foo=malloc(sizeof(struct Node));char * temp=malloc(yyleng*sizeof(char));strcpy(temp,yytext);foo->key=temp;yylval.ptr=foo;return(OR);}
"NOT" {count();return(NOT);}
"while" {count();return(WHILE);}
"do" {count();return(DO);}
"endwhile" {count();return(ENDWHILE);}
"return" {count();return(RET);}
"read" {count();return(READ);}
"write" {count();return(WRITE);}
"main" {count();return(MAN);}
"TRUE" {count();return(TRUE);}
"FALSE" {count();return(FALSE);}
">" {count();struct Node *foo=malloc(sizeof(struct Node));char * temp=malloc(yyleng*sizeof(char));strcpy(temp,yytext);foo->key=temp;yylval.ptr=foo;return(GT);}
"<" {count();struct Node *foo=malloc(sizeof(struct Node));char * temp=malloc(yyleng*sizeof(char));strcpy(temp,yytext);foo->key=temp;yylval.ptr=foo;return(LT);}
"==" {count();struct Node *foo=malloc(sizeof(struct Node));char * temp=malloc(yyleng*sizeof(char));strcpy(temp,yytext);foo->key=temp;yylval.ptr=foo;return(EQ);}
"!=" {count();struct Node *foo=malloc(sizeof(struct Node));char * temp=malloc(yyleng*sizeof(char));strcpy(temp,yytext);foo->key=temp;yylval.ptr=foo;return(NE);}
">=" {count();struct Node *foo=malloc(sizeof(struct Node));char * temp=malloc(yyleng*sizeof(char));strcpy(temp,yytext);foo->key=temp;yylval.ptr=foo;return(GE);}
"<=" {count();struct Node *foo=malloc(sizeof(struct Node));char * temp=malloc(yyleng*sizeof(char));strcpy(temp,yytext);foo->key=temp;yylval.ptr=foo;return(LE);}
{id} {count();struct Node *foo=malloc(sizeof(struct Node));char * temp=malloc(yyleng*sizeof(char)+1);strcpy(temp,yytext);temp[yyleng+1]='\0';foo->key=temp;yylval.ptr=foo;return(ID);}
{number} {count();struct Node *foo=malloc(sizeof(struct Node));int temp;temp=atoi(yytext);foo->cons=temp;yylval.ptr=foo;return(CONSTANT);}

%%

int column=0;

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	ECHO;
}