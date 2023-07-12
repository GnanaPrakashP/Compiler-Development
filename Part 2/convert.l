%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    int instruction_number=0;

    struct list{
        char *label;
        int address;
        struct list *next;
    };
    
    struct list *head=NULL;
    bool phase=false;
    FILE *ptr;
%}

label       [L][0-9]+[:]["\n"]|[F][0-9]+[:]["\n"]|[E][0-9]+[:]["\n"]
call        [L][0-9]+|[F][0-9]+|[E][0-9]+
NEWLINE     ["\n"]
CONST       \"(\\.|[^"\\])*\"

%%

{label}         {
                    if(!phase){
                        struct list *new=(struct list*)malloc(sizeof(struct list));
                        new->label=(char*)malloc((yyleng)*sizeof(char));
                        strcpy(new->label,yytext);
                        new->address=2056+2*(instruction_number-8);
                        new->next=NULL;
                        if(head==NULL){
                            head=new;
                        }
                        else{
                            struct list *temp=head;
                            while(temp->next!=NULL){
                                temp=temp->next;
                            }
                            temp->next=new;
                        }
                    }
                    else{
                        continue;
                    }
                }
{call}          {
                    if(phase){
                        struct list *temp=head;
                        char *str=(char*)malloc((yyleng+3)*sizeof(char));
                        for(int i=0;i<yyleng;i++){
                            str[i]=yytext[i];
                        }
                        str[yyleng]=':';
                        str[yyleng+1]='\n';
                        str[yyleng+2]='\0';
                        while(temp!=NULL&&strcmp(temp->label,str)!=0){
                            temp=temp->next;
                        }
                        if(temp!=NULL){
                            printf("%d",temp->address);
                        }
                    }
                }
{NEWLINE}       {
                    instruction_number++;
                    printf("\n");
                }
{CONST}         {printf("%s",yytext);}

%%

int yywrap(){
    return 1;
}

int main(){
    ptr=freopen("final.xsm","w",stdout);
    yyin=fopen("target_file.xsm","r");
    yylex();
    fclose(ptr);
    phase=true;
    ptr=freopen("final.xsm","w",stdout);
    yyin=fopen("target_file.xsm","r");
    yylex();
    fclose(ptr);
    return 0;
}