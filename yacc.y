%{
	#include <stdio.h>
	#include "code_old.h"

	int yylex(void);

%}

%union
{
	struct Node *ptr;	
};

%token <ptr> ID ASSIGNOP CONSTANT SEMICOLON RIGHTPAR LEFTPAR INTEGER BOOLEAN TRUE FALSE NOT
%token <ptr> LEFTBRKT RIGHTBRKT DECL ENDDECL COMMA BEG END IF ELSE THEN ENDIF READ WRITE DIVIDE
%token <ptr> GT LT GE LE EQ NE WHILE DO ENDWHILE LEFTCURL RIGHTCURL RET MAN AND OR PLUS MINUS MULTIPLY

%type <ptr> mainprog strt declaration endcl dclr declare id datatype program assgn 
%type <ptr> rtbrkt nd assign while asgn condition else endf ndwhile thn done identity 
%type <ptr> condtn cond logop relop red wrt expr consid rightpr op asgncond 

%left PLUS MINUS
%left MULTIPLY DIVIDE
%left OR
%left AND

%start start


%%

start
	: {init();} mainprog {show();mips();}  
	| {init();ltype="global";}declaration mainprog {show();mips();}
	;

mainprog
	: datatype MAN LEFTPAR RIGHTPAR LEFTCURL strt RIGHTCURL 
	;

strt
	: {ltype="local";}declaration program 
	;

declaration
	: DECL dclr endcl
	;
	
endcl
	: ENDDECL
	| dclr endcl
	;
	
dclr
    : datatype declare SEMICOLON
    ;
    
declare
	: id
	| id COMMA declare
	;
	
id
	: ID 									{$$=makenode("ID",NULL,NULL,NULL);install($1,dtype,ltype,0);$$->key=$1->key;}
	| ID LEFTBRKT CONSTANT RIGHTBRKT		{$$=makenode("ID",NULL,NULL,NULL);if(dtype=="integer"){install($1,dtype,ltype,$3->cons);}else{type_error();}$$->key=$1->key;}
	;
	
datatype
	: INTEGER 								{dtype="integer";}
	| BOOLEAN 								{dtype="boolean";}
	;
	
program
	: BEG asgncond nd {$$=makenode(NULL,$2,$3,NULL);}
	;

nd	
	: RET expr SEMICOLON END {$$=makenode(NULL,$2,NULL,NULL);}
	| asgncond nd 			 {$$=makenode(NULL,$1,$2,NULL);}
	;

asgncond
	: assign 			{$$=makenode(NULL,$1,NULL,NULL);}
	| condition 		{$$=makenode(NULL,$1,NULL,NULL);}
	| while 			{$$=makenode(NULL,$1,NULL,NULL);}
	| red 				{$$=makenode(NULL,$1,NULL,NULL);}
	| wrt 				{$$=makenode(NULL,$1,NULL,NULL);}
	;

while
	: WHILE {cnd=emit();}LEFTPAR condtn RIGHTPAR done {$$=makenode("WHILE",$3,$5,NULL);generate($$);}
	;

done
	: DO {go(cnd);wh_stat=emit();} asgn  							 {$$=makenode("DO",$2,NULL,NULL);}
	;

asgn
	: assign ndwhile 					 {$$=makenode(NULL,$1,$2,NULL);}
	| assign asgn 						 {$$=makenode(NULL,$1,$2,NULL);}
	| red asgn 							 {$$=makenode(NULL,$1,$2,NULL);}
	| wrt asgn							 {$$=makenode(NULL,$1,$2,NULL);}

ndwhile
	: ENDWHILE SEMICOLON				 {$$=makenode("ENDWHILE",NULL,NULL,NULL);}
	;

condition
	: IF LEFTPAR condtn RIGHTPAR thn 	 {$$=makenode("IF",$3,$5,NULL);generate($$);}
	;	
	
thn
	: THEN {cnd=++l;go(cnd);if_l=emit();exit_l=if_l+2;} assign {go(exit_l);} else 					 {$$=makenode("THEN",$2,$3,NULL);}	
	;

else
	: endf {em(cnd);else_l=if_l+2;l++;}															 {$$=makenode(NULL,$1,NULL,NULL);}
	| ELSE {else_l=emit();exit_l=else_l+1;} assign {go(exit_l);em(cnd);} endf 					 {$$=makenode("ELSE",$2,NULL,NULL);}
	;

endf
	: ENDIF SEMICOLON 					 {$$=makenode("ENDIF",NULL,NULL,NULL);}
	;	

condtn
	: cond    							 				 {$$=makenode("CONDREL",$1,NULL,NULL);}
	| LEFTPAR cond RIGHTPAR logop condtn   				 {$$=makenode("CONDLOG",$2,$4,$5);}
	| LEFTPAR cond RIGHTPAR   			 				 {$$=makenode("CONDREL",$2,NULL,NULL);}	
	;
	
	
cond
	: consid relop consid {$$=makenode("CONDRELFROMLOG",$1,$2,$3);$$->key=lookup($1);if($$->key==lookup($3)){$$->key="boolean";}else{type_error();};}
	| NOT LEFTPAR cond RIGHTPAR	{$$=makenode("NOT",$3,NULL,NULL);$$->key=$3->key;}
	; 

logop
	: AND {$$=makenode("AND",NULL,NULL,NULL);$$->key=$1->key;}
	| OR {$$=makenode("OR",NULL,NULL,NULL);$$->key=$1->key;}
	;

relop
	: GT	{$$=makenode("GT",NULL,NULL,NULL);$$->key=$1->key;}
	| LT 	{$$=makenode("LT",NULL,NULL,NULL);$$->key=$1->key;}
	| GE 	{$$=makenode("GE",NULL,NULL,NULL);$$->key=$1->key;}
	| LE 	{$$=makenode("LE",NULL,NULL,NULL);$$->key=$1->key;}
	| EQ 	{$$=makenode("EQ",NULL,NULL,NULL);$$->key=$1->key;}
	| NE 	{$$=makenode("NE",NULL,NULL,NULL);$$->key=$1->key;}
	;	
		
assign
	: identity assgn expr SEMICOLON {$$=makenode("ASSIGN",$1,$2,$3);$$->key=lookup($1);if(($$->key)==lookup($3)){$$->key=$3->key;$$->cons=$3->cons;}else{type_error();};generate($$);}
	| identity LEFTBRKT CONSTANT rtbrkt {$$=makenode(NULL,$1,$3,$4);$$->key=lookup($1);if(($$->key)==lookup($4)){$$->key=$4->key;$$->cons=$4->cons;}else{type_error();};generate($$);}
	;

identity
	: ID {$$=makenode("ID",NULL,NULL,NULL);$$->key=$1->key;}
	;

rtbrkt
	: RIGHTBRKT assgn expr SEMICOLON {$$=makenode("ARR",$2,$3,NULL);$$->key=$3->key;$$->cons=$3->cons;}
	;

assgn
	: ASSIGNOP {$$=makenode("=",NULL,NULL,NULL);$$->key=$1->key;$$->cons=$1->cons;}
	;	

red
	: READ LEFTPAR id RIGHTPAR SEMICOLON {$$=makenode("READ",$3,NULL,NULL);generate($$);}
	;

wrt
	: WRITE LEFTPAR expr RIGHTPAR SEMICOLON {$$=makenode("WRITE",$3,NULL,NULL);generate($$);}
	;

expr
	: TRUE					{$$=makenode("TRUE",NULL,NULL,NULL);$$->key="true";}
	| FALSE					{$$=makenode("FALSE",NULL,NULL,NULL);$$->key="false";}
	| CONSTANT 				{$$=makenode("CONSTANT",NULL,NULL,NULL);$$->key="integer";$$->cons=$1->cons;}
	| ID   				    {$$=makenode("ID",NULL,NULL,NULL);$$->key=$1->key;$$->cons=0;}
	| consid op expr  		{$$=makenode("EXPR",$1,$2,$3);$$->key=lookup($1);if(($$->key)==lookup($3)){$$->key=$1->key;$$->cons=$3->cons;}else{type_error();};}
	| LEFTPAR expr rightpr  {$$=makenode(NULL,NULL,$2,$3);$$->key=$2->key;}
	;

consid
	: CONSTANT {$$=makenode("CONSTANT",NULL,NULL,NULL);$$->key="integer";$$->cons=$1->cons;}
	| ID       {$$=makenode("ID",NULL,NULL,NULL);$$->key=$1->key;$$->cons=0;}
	;

	
rightpr	
	: RIGHTPAR  			{$$=makenode(NULL,NULL,NULL,NULL);}
	| RIGHTPAR op expr  	{$$=makenode(NULL,$2,$3,NULL);}
	;
	
op
	: PLUS   		{$$=makenode("+",NULL,NULL,NULL);$$->key=$1->key;}
	| MINUS   		{$$=makenode("-",NULL,NULL,NULL);$$->key=$1->key;}
	| MULTIPLY  	{$$=makenode("*",NULL,NULL,NULL);$$->key=$1->key;}
	| DIVIDE  		{$$=makenode("/",NULL,NULL,NULL);$$->key=$1->key;}
	;
	
%%
int main(void)
{
	return yyparse();
}

yyerror(char *s)
{
	fprintf(stderr,":%s",s);
}

yywrap()
{
	return(1);
}

void type_error()
{
	printf("\ntype error\n");
	exit(0);
}

