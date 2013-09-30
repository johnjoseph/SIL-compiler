 #include<stdio.h>
 #include<stdlib.h>
 #include<string.h>
 #include<ctype.h>

int c=0,l=0,cnd;
int if_l,else_l;exit_l;
int wh_stat;
char temp[100];	


struct Node
{
	char *str;
	struct Node *ptr1,*ptr2,*ptr3;
	char * key;
	int cons;	
};

struct Node *makenode(char *str,struct Node *next1,struct Node *next2,struct Node *next3);
void printtree(struct Node *ptr);
char * lookup(struct Node * foo);
void install(struct Node * foo	,char * type,char * loc,int val);

struct symbol
{
	char * name;
	char * type;
	int 	val;
	int 	bind;
	struct symbol * next; 
};

struct  Gqueue
{
	struct symbol * Gfront;
	struct symbol * Grear;	
}*gq;

struct  Lqueue
{
	struct symbol * Lfront;
	struct symbol * Lrear;	
}*lq;

char * dtype;
char * ltype;

struct quadruple
{
	char op[10];
	char oprnd1[10];
	char oprnd2[10];
	char rslt[10];
	struct quadruple * next;
};

struct Qqueue
{
	struct quadruple *Qfront;
	struct quadruple *Qrear;
}*qq;

struct Node * makenode(char *str, struct Node *next1, struct Node *next2, struct Node *next3)
{
	struct Node * node = malloc(sizeof(struct Node));
	node->str=str;
	node->ptr1=next1;
	node->ptr2=next2;
	node->ptr3=next3;

	return node;	
}

void printtree(struct Node * ptr)
{
	if(ptr == NULL)
		return;

	if(ptr->str!=NULL)
	{
		printf("  (");
		printf("%s",ptr->str);
		printtree(ptr->ptr1);
		printtree(ptr->ptr2);
		printtree(ptr->ptr3);
		printf(")  ");
	}	
	else
	{
		printtree(ptr->ptr1);
		printtree(ptr->ptr2);
		printtree(ptr->ptr3);

	}


}

void init()
{
	lq=malloc(sizeof(struct Lqueue));
	lq->Lfront=NULL;
	gq=malloc(sizeof(struct Gqueue));
	gq->Gfront=NULL;
	qq=malloc(sizeof(struct Qqueue));
	qq->Qfront=NULL;
}

void install(struct Node *foo, char * type,char * loc,int val)
{
	char * name;
	name=foo->key;
	struct symbol * temp=malloc(sizeof(struct symbol));
	temp->name=name;
	temp->val=val;
	temp->type=type;
	temp->next=NULL;
	if(loc=="global")
	{
		if((gq->Gfront)==NULL)
		{
			gq->Gfront=temp;
			gq->Grear=temp;
		}
		else 
		{
			(gq->Grear)->next=temp;
			gq->Grear=temp;
		}			
	}
	else
	{
		if((lq->Lfront)==NULL)
		{
			lq->Lfront=temp;
			lq->Lrear=temp;
		}
		else 
		{
			(lq->Lrear)->next=temp;
			lq->Lrear=temp;
		}
	}


}


char * lookup(struct Node *foo)
{
	char * name;
	name=foo->key;
	struct symbol * temp;
	if(!strcmp(name,"integer"))
	{
		return "integer";
	}
	if((!strcmp(name,"true"))||(!strcmp(name,"false")))
	{
		return "boolean";
	}	
	temp=gq->Gfront;	
	while(temp!=NULL)
	{
		if(!strcmp(temp->name,name))
		{
			return temp->type;
		}
		else
		{
			temp=temp->next;
		}
		
	}
	
	if(temp==NULL)
	{
		temp=lq->Lfront;	
		while(temp!=NULL)
		{
			if(!strcmp(temp->name,name))
			{
				return temp->type;
			}
			else
			{
				temp=temp->next;
			}
		}

	}
		printf("\nnot found:%s\n",name);
		exit(0);
		return NULL;

}

int bindval(char * name)
{

	struct symbol * temp;

	temp=gq->Gfront;	
	while(temp!=NULL)
	{
		if(!strcmp(temp->name,name))
		{
			return temp->bind;
		}
		else
		{
			temp=temp->next;
		}
		
	}
	
	if(temp==NULL)
	{
		temp=lq->Lfront;	
		while(temp!=NULL)
		{
			if(!strcmp(temp->name,name))
			{
				return temp->bind;
			}
			else
			{
				temp=temp->next;
			}
		}

	}
		printf("\nnot found:%s\n",name);
		return NULL;

}

void displaytable()
{
	struct symbol * temp;
	temp=gq->Gfront;	
	printf("\nglobal\n");
	while(temp!=NULL)
	{
		if(temp->val!=0)
		{
			printf("\n name: %s[%d] type: %s bind:%d",temp->name,temp->val,temp->type,temp->bind);
		}	
		else
		{
			printf("\n name: %s type: %s bind:%d",temp->name,temp->type,temp->bind);			
		}
		temp=temp->next;
	}
	temp=lq->Lfront;	
	printf("\nlocal\n");
	while(temp!=NULL)
	{
		if(temp->val!=0)
		{
			printf("\n name: %s[%d] type: %s bind %d",temp->name,temp->val,temp->type,temp->bind);
		}	
		else
		{
			printf("\n name: %s type: %s bind: %d",temp->name,temp->type,temp->bind);			
		}
		temp=temp->next;		
	}

}

int compare(char foo1[10],char foo2[10])
{
	int i,j;
	if((!strcmp(foo1,"*"))||(!strcmp(foo1,"/")))
	{
		i=1;
	}
	else((!strcmp(foo1,"+"))||(!strcmp(foo1,"-")))
	{	
		i=0;
	}

	if((!strcmp(foo2,"*"))||(!strcmp(foo2,"/")))
	{
		j=1;
		if(i==0)
		{return j;}
		else if(i==1)
		{return (j-1);}
	}
	else((!strcmp(foo2,"+"))||(!strcmp(foo2,"-")))
	{
		j=0;
		return j;
	}

}

void generate(struct Node * ptr)
{

	char string1[10],string2[10],string3[10];

	if(ptr==NULL)
	{
		return;
	}

	if(ptr->str=="CONDREL"||ptr->str=="CONDRELFROMLOG")
	{
		if(ptr->str=="CONDREL")
		{
			ptr=ptr->ptr1;
		}

		struct quadruple *temp=malloc(sizeof(struct quadruple));

		if((qq->Qfront)==NULL)
		{
			qq->Qfront=temp;
			qq->Qrear=temp;
		}
		else 
		{
			(qq->Qrear)->next=temp;
			qq->Qrear=temp;
		}

		strcpy(string1,"temp");
		sprintf(string3,"%d",c);
		strcat(string1,string3);


		strcpy(temp->op,ptr->ptr2->key);

		if(ptr->ptr1->str=="ID")
		{
			strcpy(temp->oprnd1,ptr->ptr1->key);			
		}
		else 
		{
			sprintf(temp->oprnd1,"%d",ptr->ptr1->cons);			
		}
		if(ptr->ptr3->str=="ID")
		{
			strcpy(temp->oprnd2,ptr->ptr3->key);			
		}
		else 
		{
			sprintf(temp->oprnd2,"%d",ptr->ptr3->cons);			
		}

		strcpy(temp->rslt,string1);
		temp->next=NULL;

		c++;
	}

	else if((ptr->str=="READ") || (ptr->str=="WRITE") || (ptr->str=="NOT"))
	{
		struct quadruple *temp=malloc(sizeof(struct quadruple));
		
		strcpy(string1,"temp");
		sprintf(string3,"%d",c);
		strcat(string1,string3);

		if((ptr->str=="WRITE") || ((ptr->str=="NOT")))
		{
			c++;
			generate(ptr->ptr1);
			c--;
			strcpy(string2,"temp");
			sprintf(string3,"%d",c);
			strcat(string2,string3);
			strcpy(temp->oprnd1,string2);		
		}
		else if(ptr->str=="READ")
		{
			strcpy(temp->oprnd1,ptr->ptr1->key);
		}

		if((qq->Qfront)==NULL)
		{
			qq->Qfront=temp;
			qq->Qrear=temp;
		}
		else 
		{
			(qq->Qrear)->next=temp;
			qq->Qrear=temp;
		}



		strcpy(temp->op,ptr->str);

		strcpy(temp->rslt,string1);
		temp->next=NULL;

		c++;		
	}


	else if(ptr->str=="IF"||ptr->str=="CONDLOG"||ptr->str=="WHILE")
	{
		int num;

		struct Node * foo;

		if(ptr->str=="IF"||ptr->str=="WHILE")
		{
			num=c;
			foo=ptr;
			ptr=ptr->ptr1;
		}

		if(ptr->str=="CONDREL")
		{
			generate(ptr);
		}
		else
		{
			struct quadruple *temp=malloc(sizeof(struct quadruple));
			
			strcpy(string1,"temp");
			sprintf(string3,"%d",c);
			strcat(string1,string3);

			strcpy(temp->rslt,string1);		

			
			c++;
			generate(ptr->ptr1);
			c--;	
			strcpy(string1,"temp");
			sprintf(string3,"%d",c);
			strcat(string1,string3);			

			strcpy(temp->oprnd1,string1);
			c++;

			strcpy(temp->op,ptr->ptr2->key);

			strcpy(string1,"temp");
			sprintf(string3,"%d",c);
			strcat(string1,string3);

			strcpy(temp->oprnd2,string1);		
			temp->next=NULL;



			if(ptr->ptr3->str=="CONDLOG")
			{
				generate(ptr->ptr3);
			}
			else
			{
				generate(ptr->ptr3->ptr1);			
				c++;
			}

			if((qq->Qfront)==NULL)
			{
				qq->Qfront=temp;
				qq->Qrear=temp;
			}
			else 
			{
				(qq->Qrear)->next=temp;
				qq->Qrear=temp;
			}			
		}
			



		if( (foo->str=="IF") || (foo->str=="WHILE"))
		{
			struct quadruple * temp=malloc(sizeof(struct quadruple));
	
			strcpy(temp->rslt,"IF");
			strcpy(string1,"temp");
			sprintf(string2,"%d",num);
			strcat(string1,string2);
			strcpy(temp->oprnd1,string1);		
			strcpy(temp->op,"GOTO");	

			strcpy(string1,"label");

			if(foo->str=="IF")
			{
				sprintf(string2,"%d",if_l);
			}
			else if(foo->str=="WHILE")
			{
				sprintf(string2,"%d",wh_stat);
			}

			strcat(string1,string2);
			strcpy(temp->oprnd2,string1);
			temp->next=NULL;

			if((qq->Qfront)==NULL)
			{
				qq->Qfront=temp;
				qq->Qrear=temp;
			}
			else 
			{
				(qq->Qrear)->next=temp;
				qq->Qrear=temp;
			}

			if(foo->str=="IF")
			{
				go(else_l);
			}
			else if(foo->str=="WHILE")
			{
				go(wh_stat+1);
			}			
			emit();	
			
		}

				
	}

	else if(ptr->str=="EXPR")
	{
		int cm;
		struct quadruple *temp=malloc(sizeof(struct quadruple));
		
		strcpy(string1,"temp");
		sprintf(string3,"%d",c);
		strcat(string1,string3);

		c++;

		strcpy(temp->rslt,string1);	
		strcpy(temp->op,ptr->ptr2->str);

		if((ptr->ptr1)->str=="CONSTANT")
		{
			sprintf(string3,"%d",(ptr->ptr1)->cons);
			strcpy(temp->oprnd1,string3);
		}
		else if((ptr->ptr1)->str=="ID")
		{
			strcpy(temp->oprnd1,ptr->ptr1->key);			
		}

		if((ptr->ptr3)->str=="CONSTANT")
		{
			sprintf(string3,"%d",(ptr->ptr3)->cons);
			strcpy(temp->oprnd2,string3);
		}
		else if((ptr->ptr3)->str=="ID")
		{
			strcpy(temp->oprnd2,ptr->ptr3->key);			
		}
		else if((ptr->ptr3)->str=="EXPR")
		{
			cm=compare(ptr->ptr2->str,(ptr->ptr3)->ptr2->str);
			if(cm)
			{}
			ptr=ptr->ptr3;
			strcpy(string1,"temp");
			sprintf(string3,"%d",c);
			strcat(string1,string3);
			generate(ptr);
			strcpy(temp->oprnd2,string1);			
		}

		temp->next=NULL;

		if((qq->Qfront)==NULL)
		{
			qq->Qfront=temp;
			qq->Qrear=temp;
		}
		else 
		{
			(qq->Qrear)->next=temp;
			qq->Qrear=temp;
		}		
		



	}	
	
	else
	{
		
		struct quadruple *temp=malloc(sizeof(struct quadruple));
		
		strcpy(temp->rslt,ptr->ptr1->key);		
		strcpy(temp->op,"=");

		ptr=ptr->ptr3;
		
		if(ptr->str=="CONSTANT"||ptr->str=="ARR")
		{
			sprintf(string2,"%d",ptr->cons);
			strcpy(temp->oprnd1,string2);
		}

		else if(ptr->str=="ID")
		{
			strcpy(temp->oprnd1,ptr->key);
		}

		else
		{
	
			strcpy(string1,"temp");
			sprintf(string3,"%d",c);
			strcat(string1,string3);			
			generate(ptr);
			strcpy(temp->oprnd1,string1);
		}

		temp->next=NULL;

		if((qq->Qfront)==NULL)
		{
			qq->Qfront=temp;
			qq->Qrear=temp;
		}
		else 
		{
			(qq->Qrear)->next=temp;
			qq->Qrear=temp;
		}
			
	}
	
}

int emit()
{
	l++;
	char string1[10],string2[10];
	struct quadruple *temp=malloc(sizeof(struct quadruple));
	temp->next=NULL;
	strcpy(string1,"label");
	sprintf(string2,"%d",l);
	strcat(string1,string2);
	strcpy(temp->rslt,string1);	
	if((qq->Qfront)==NULL)
	{
		qq->Qfront=temp;
		qq->Qrear=temp;
	}
	else 
	{
		(qq->Qrear)->next=temp;
		qq->Qrear=temp;
	}
	return l;
}

void em(int l)
{
	char string1[10],string2[10];
	struct quadruple *temp=malloc(sizeof(struct quadruple));
	temp->next=NULL;
	strcpy(string1,"label");
	sprintf(string2,"%d",l);
	strcat(string1,string2);
	strcpy(temp->rslt,string1);	
	if((qq->Qfront)==NULL)
	{
		qq->Qfront=temp;
		qq->Qrear=temp;
	}
	else 
	{
		(qq->Qrear)->next=temp;
		qq->Qrear=temp;
	}
}

void go(int x)
{
	char foo1[10],foo2[10];
	struct quadruple *temp2=malloc(sizeof(struct quadruple));

	strcpy(temp2->rslt,"GOTO");

	strcpy(foo1,"label");
	sprintf(foo2,"%d",x);
	strcat(foo1,foo2);
	strcpy(temp2->oprnd1,foo1);

	temp2->next=NULL;
	if((qq->Qfront)==NULL)
	{
		qq->Qfront=temp2;
		qq->Qrear=temp2;
	}
	else 
	{
		(qq->Qrear)->next=temp2;
		qq->Qrear=temp2;
	}	
}

char * assembly(struct quadruple * temp)
{
	char foo[10];
	strcpy(foo,temp->op);

	if(!strcmp(foo,"+"))
	return "ADD";		

	else if(!strcmp(foo,"-"))
	return "SUB";		

	else if(!strcmp(foo,"*"))
	return "MUL";

	else if(!strcmp(foo,"/"))
	return "DIV";

	else if(!strcmp(foo,"<"))
	return "LT";

	else if(!strcmp(foo,">"))
	return "GT";

	else if(!strcmp(foo,"=="))
	return "EQ";

	else if(!strcmp(foo,"!="))
	return "NE";

	else if(!strcmp(foo,">="))
	return "GE";

	else if(!strcmp(foo,"<="))
	return "LE";

	else if(!strcmp(foo,"AND"))
	return "AND";

	else if(!strcmp(foo,"OR"))
	return "OR";

	else
	return NULL;


}

void mips()
{
	int b=0;
	struct symbol * foo;

	foo=gq->Gfront;	
	while(foo!=NULL)
	{
		b++;
		foo->bind=b;			
		foo=foo->next;
		
	}
	
	if(foo==NULL)
	{
		foo=lq->Lfront;	
		while(foo!=NULL)
		{
			b++;
			foo->bind=b;
			foo=foo->next;

		}

	}

	
	int r=-1,tmp=-1,nxt=0;
	FILE *fp;
	fp=fopen("sim-2/mips","w");
	if(fp==NULL)
	{
		printf("\ncould not open the file\n");
		exit(0);
	}
	struct quadruple * temp;
	temp=qq->Qfront;
	fprintf(fp,"START\n");
	while(temp!=NULL)
	{
		if(!strcmp(temp->op,"="))
		{
			r=-1;
			if(tmp!=-1)
			{
				fprintf(fp,"MOV [%d],R%d\n",bindval(temp->rslt),tmp);
				nxt=0;				
				tmp=0;
			}
			else
			{
				fprintf(fp,"MOV [%d],%s \n",bindval(temp->rslt),temp->oprnd1);		
			}
		}
		else if(assembly(temp))
		{	
			if(nxt)
			r=nxt;
			else	
			r++;

			if( atoi(temp->oprnd1) || (!strcmp(temp->oprnd1,"0")) )
			{
				fprintf(fp,"MOV R%d,%d\n",r,atoi(temp->oprnd1));
			}
			else if(!strncmp(temp->oprnd1,"temp",4))
			{
				fprintf(fp,"MOV R%d,R%d\n",r,tmp);
				nxt=tmp;				
			}
			else if(bindval(temp->oprnd1))
			{
				fprintf(fp,"MOV R%d,[%d]\n",r,bindval(temp->oprnd1));
			}
			
			r++;
			if( atoi(temp->oprnd2) || (!strcmp(temp->oprnd2,"0")) )
			{
				fprintf(fp,"MOV R%d,%d\n",r,atoi(temp->oprnd2));
			}
			else if(!strncmp(temp->oprnd2,"temp",4))
			{
				fprintf(fp,"MOV R%d,R%d\n",r,tmp);
				nxt=tmp;				
			}
			else if(bindval(temp->oprnd2))
			{
				fprintf(fp,"MOV R%d,[%d]\n",r,bindval(temp->oprnd2));
			}			
			
			fprintf(fp,"%s R%d,R%d \n",assembly(temp),r-1,r);									
			
			tmp=r-1;
			nxt=r;
		}	
		
		else if((!strcmp(temp->op,"READ")) || (!strcmp(temp->op,"WRITE")))
		{
			if(!strcmp(temp->op,"READ"))
			{   
				r=0;
				fprintf(fp,"IN R%d\n",r);				
				fprintf(fp,"MOV [%d],R%d\n",bindval(temp->oprnd1),r);
			}		
			else if(!strcmp(temp->op,"WRITE"))
			{
				fprintf(fp,"MOV R%d,R%d\n",r,tmp);
				fprintf(fp,"OUT R%d\n",r);
			}
		}	

		else if((!strcmp(temp->rslt,"GOTO")) || (!strncmp(temp->rslt,"label",5)) || (!strcmp(temp->rslt,"IF")))
		{
			r=0;
			if((!strcmp(temp->rslt,"GOTO")))
			{
				fprintf(fp,"JMP %s\n",temp->oprnd1);				
			}
			else if((!strcmp(temp->rslt,"IF")))
			{
				fprintf(fp,"JNZ R%d %s\n",tmp,temp->oprnd2);
			}
			else 
			{
				fprintf(fp,"%s:\n",temp->rslt);				
			}

		}	
			
		temp=temp->next;

	}
	fprintf(fp,"HALT\n");
	fclose(fp);
	exit(0);

}


void show()
{
	struct quadruple * temp;
	temp=qq->Qfront;
	while(temp!=NULL)
	{
			printf("\n result: %s oprnd1: %s op: %s oprnd2: %s",temp->rslt,temp->oprnd1,temp->op,temp->oprnd2);
			temp=temp->next;
	}

}