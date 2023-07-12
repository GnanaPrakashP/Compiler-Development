%{
     #include <stdio.h>
     #include <stdlib.h>
     #include <string.h>
     #include <stdbool.h>
     
     int arr[20];
     int label=-1;                       // For if else and while
     int f_label=1;                      // For function definition everywhere
     int class_label=0;                  // For classes
     int field_index=0;                  // For field position in a class
     int func_index=0;                   // For function position in a class

     struct gsymbol *global_head=NULL;
     struct plist *param_head=NULL;
     struct lsymbol *local_head=NULL;

     struct typetable *typetable_head=NULL;
     struct fieldlist *fieldlist_head=NULL;
     struct memberfunclist *memberfunclist_head=NULL;
     struct typetable *typetable_temp=NULL;

     struct classtable *classtable_head=NULL;
     struct classtable *classtable_temp=NULL;

     int binding_address=4096;
     int local_binding_address=1;
     int flag=1;                         // flag = 1 for declarations and flag = 0 for definitions  
     int decl_flag=1;
     FILE *fptr=NULL;
     int error_line=1;
     char *function_name=NULL;

     int class_flag=1;                   // class_flag = 0 if class definitions are done

     struct typetable* vartype[10];
     int var_index=-1;

     char class_name[30];

     struct typetable *class_type=NULL;

     #include "exprtree.h"
     #include "exprtree.c"
%}

%union{
     struct tnode *ptr;
}

%token begin end READ WRITE
%token NUM ID PLUS MINUS MUL DIV MOD
%token IF then Else ENDIF
%token WHILE DO ENDWHILE
%token LT GT LE GE NE EQ
%token BREAK CONTINUE
%token DECL ENDDECL INT STR
%token CONST
%token MAIN RETURN
%token AND NOT OR
%token TYPE ENDTYPE null
%token CLASS ENDCLASS SELF NEW
%type <ptr> ID NUM READ WRITE CONST

%nonassoc LT GT LE GE NE EQ NOT

%left AND OR '.'
%left PLUS MINUS
%left MUL DIV
%left MOD

%%

Program : TypeDefBlock ClassDefBlock GdeclBlock FdefBlock MainBlock
        | TypeDefBlock ClassDefBlock GdeclBlock MainBlock
        | ClassDefBlock GdeclBlock MainBlock
        ;

TypeDefBlock : TYPE TypeDefList ENDTYPE                            
             |
             ;

TypeDefList : TypeDefList TypeDef
            | TypeDef
            ;

ClassDefBlock : L ClassDefList ENDCLASS                             {class_flag=0;}
              |                                                     {
                                                                        fprintf(fptr, "CALL F0\n");
                                                                        fprintf(fptr, "JMP E0\n");
                                                                    }
              ;

L : CLASS                                                           {
                                                                        fprintf(fptr, "CALL F0\n");
                                                                        fprintf(fptr, "JMP E0\n");
                                                                    }
  ;

ClassDefList : ClassDefList ClassDef
             | ClassDef
             ;

ClassDef : Cname '{' DECL Z ENDDECL MethodDefns '}'                 {                            
                                                                        fieldlist_head=NULL;
                                                                        memberfunclist_head=NULL;
                                                                        field_index=0;
                                                                        func_index=0;
                                                                        flag=1;  // definitions completed next declarations
                                                                    }
         ;

Z : FieldDeclList MethodDecl                                        {
                                                                        struct classtable *temp=classtable_head;
                                                                        while(temp->next!=NULL){
                                                                            temp=temp->next;
                                                                        }
                                                                        temp->memberfield=fieldlist_head;
                                                                        temp->vfuncptr=memberfunclist_head;
                                                                        temp->fieldcount=field_index;
                                                                        temp->methodcount=func_index;
                                                                        flag=0;  // declarations completed next definitions
                                                                    }

Cname : ID                                                          {
                                                                        cinstall($<ptr>1->varname,NULL);
                                                                        strcpy(class_name,$<ptr>1->varname);
                                                                        $<ptr>$=$<ptr>1;
                                                                    }
      ;

MethodDecl : MethodDecl MDecl
           | MDecl
           |
           ;

MDecl : TypeName ID '(' ParamList ')' ';'                           {
                                                                        struct memberfunclist *new=minstall($<ptr>2->varname,typetable_temp);
                                                                        if(memberfunclist_head==NULL){
                                                                            memberfunclist_head=new;
                                                                        }
                                                                        else{
                                                                            struct memberfunclist *temp=memberfunclist_head;
                                                                            while(temp->next!=NULL){
                                                                                temp=temp->next;
                                                                            }
                                                                            temp->next=new;
                                                                        }
                                                                        param_head=NULL;
                                                                    }
      ;

MethodDefns : MethodDefns Fdef
            | Fdef
            |
            ;

TypeDef : X '{' FieldDeclList '}'                                   {
                                                                        struct typetable *temp=typetable_head;
                                                                        while(temp->next!=NULL){
                                                                            temp=temp->next;
                                                                        }
                                                                        temp->fields=fieldlist_head;
                                                                    }
        ;

X : ID                                                              {
                                                                        typetable_temp=tinstall($<ptr>1->varname,0,NULL);
                                                                        struct typetable *temp=typetable_head;
                                                                        while(temp->next!=NULL){
                                                                            temp=temp->next;
                                                                        }
                                                                        temp->next=typetable_temp;
                                                                        fieldlist_head=NULL;
                                                                    }
  ;

FieldDeclList : FieldDeclList FieldDecl
              | FieldDecl
              |
              ;

FieldDecl : TypeName ID ';'                                         {
                                                                        struct fieldlist *new=finstall(typetable_temp,classtable_temp,$<ptr>2->varname);
                                                                        if(fieldlist_head==NULL){
                                                                            fieldlist_head=new;
                                                                        }
                                                                        else{
                                                                            struct fieldlist *temp=fieldlist_head;
                                                                            while(temp->next!=NULL){
                                                                                temp=temp->next;
                                                                            }
                                                                            temp->next=new;
                                                                        }
                                                                    }

TypeName : INT                                                      {
                                                                        typetable_temp=tlookup("int");
                                                                        classtable_temp=NULL;
                                                                    }
         | STR                                                      {
                                                                        typetable_temp=tlookup("str");
                                                                        classtable_temp=NULL;
                                                                    }
         | ID                                                       {
                                                                        typetable_temp=tlookup($<ptr>1->varname);
                                                                        classtable_temp=clookup($<ptr>1->varname);
                                                                    }
         ;

GdeclBlock : DECL GdeclList ENDDECL                                 {flag=0;}
           | DECL ENDDECL                                           {flag=0;}
           |                                                        {flag=0;}
           ;
           
GdeclList : GdeclList Gdecl
          | Gdecl
          ;
          
Gdecl : Type GidList ';'                                            {var_index--;}
      ;
      
GidList : GidList ',' Gid
        | Gid
        ;
        
Gid : ID                                                            {
                                                                        ginstall($<ptr>1->varname,1);
                                                                    }
    | ID '[' NUM ']'                                                {
                                                                        ginstall($<ptr>1->varname,atoi($<ptr>3->varname));
                                                                    }
    | ID '(' ParamList ')'                                          {
                                                                        ginstall($<ptr>1->varname,0);
                                                                        param_head=NULL;
                                                                    }
    ;
    
FdefBlock : FdefBlock Fdef
          | Fdef
          |
          ;
          
Fdef : Type X3 '(' Y ')' '{' LdeclBlock '}'                         {
                                                                        if(class_flag){
                                                                            class_check_exist_function($<ptr>2->varname);
                                                                            generate_code($<ptr>7,fptr,getlabel($<ptr>2->varname));
                                                                        }
                                                                        else{
                                                                            check_exist_function($<ptr>2->varname);
                                                                            generate_code($<ptr>7,fptr,$<ptr>2->Ggentry->flabel);
                                                                        }
                                                                        local_binding_address=1;
                                                                        local_head=NULL;
                                                                        decl_flag=1;
                                                                        var_index--;
                                                                    }
     ;

Y : ParamList                                                       {local_binding_address=1;}
  ;
     
X3 : ID                                                             {
                                                                        function_name=(char*)malloc(strlen($<ptr>1->varname)*sizeof(char));
                                                                        strcpy(function_name,$<ptr>1->varname);
                                                                        $<ptr>$=$<ptr>1;
                                                                    }
   ;

MainBlock : Type X4 '(' ')' '{' LdeclBlock '}'                      {
                                                                        generate_code($<ptr>6,fptr,0);
                                                                        local_binding_address=1;
                                                                        local_head=NULL;
                                                                        decl_flag=1;
                                                                        var_index--;
                                                                    }
          ;

X4 : MAIN                                                           {
                                                                        function_name=(char*)malloc(4*sizeof(char));
                                                                        strcpy(function_name,"MAIN");
                                                                    }
   ;

ParamList : ParamList ',' Param                                     
          | Param                                                   
          | 
          ;
          
Param : Type ID                                                     {
                                                                        if(flag){
                                                                            pinstall($<ptr>2->varname);
                                                                        }
                                                                        else{
                                                                            linstall($<ptr>2->varname,1,0);
                                                                        }
                                                                        var_index--;
                                                                    }
      ;
    
LdeclBlock : X2 LdecList ENDDECL                                    {decl_flag=0;}
           | DECL ENDDECL                                           {decl_flag=0;}
           | LdeclBlock Start                                       {$<ptr>$=$<ptr>2;}
           | Start                                                  {$<ptr>$=$<ptr>1;}
           ;
           
X2 : DECL                                                           {local_binding_address=1;}
   ;
           
LdecList : LdecList Ldecl
         | Ldecl
         ;
         
Ldecl : Type IdList ';'                                             {var_index--;}
    
IdList : IdList ',' ID                                              {linstall($<ptr>3->varname,1,1);}
       | ID                                                         {linstall($<ptr>1->varname,1,1);}
       ;
       
Type : INT                                                          {
                                                                        var_index++;
                                                                        vartype[var_index]=tlookup("int");
                                                                    }
     | STR                                                          {
                                                                        var_index++;
                                                                        vartype[var_index]=tlookup("str");
                                                                    }
     | ID                                                           {
                                                                        var_index++;
                                                                        vartype[var_index]=tlookup($<ptr>1->varname);
                                                                        classtable_temp=clookup($<ptr>1->varname);
                                                                    }
     ;

Start : begin Slist RETURN E ';' end                                {$<ptr>$=makeOperatorNode(3,"return",9,$<ptr>2,$<ptr>4);}
      | begin RETURN E ';' end                                      {$<ptr>$=makeOperatorNode(3,"return",9,NULL,$<ptr>3);}
      | begin Slist end                                             {$<ptr>$=$<ptr>2;}               
      ;

Slist : Slist Stmt                                                  {$<ptr>$=makeOperatorNode(-2,"Connector",3,$<ptr>1,$<ptr>2);}
      | Stmt                                                        {$<ptr>$=$<ptr>1;}
      ;

Stmt : InputStmt                                                    {$<ptr>$=$<ptr>1;}
     | OutputStmt                                                   {$<ptr>$=$<ptr>1;}
     | AsgStmt                                                      {$<ptr>$=$<ptr>1;}
     | Ifstmt                                                       {$<ptr>$=$<ptr>1;}
     | Whilestmt                                                    {$<ptr>$=$<ptr>1;}
     | SkipStmt                                                     {$<ptr>$=$<ptr>1;}
     ;

Ifstmt : IF '(' E ')' then Slist Else Slist ENDIF ';'               {
                                                                        struct tnode *temp=makeOperatorNode(-2,"T/F",6,$<ptr>6,$<ptr>8);
                                                                        $<ptr>$=makeOperatorNode(-1,"if",5,$<ptr>3,temp);
                                                                    }
       | IF '(' E ')' then Slist ENDIF ';'                          {$<ptr>$=makeOperatorNode(-1,"if",5,$<ptr>3,$<ptr>6);}
       ;

Whilestmt : WHILE '(' E ')' DO Slist ENDWHILE ';'                   {$<ptr>$=makeOperatorNode(-1,"while",7,$<ptr>3,$<ptr>6);}
          ;

InputStmt : READ '(' ID ')' ';'                                     {$<ptr>$=makeOperatorNode(2,"read",1,$<ptr>3,NULL);}
          | READ '(' e ')' ';'                                      {$<ptr>$=makeOperatorNode(2,"read",1,$<ptr>3,NULL);}
          ;

OutputStmt : WRITE '(' E ')' ';'                                    {$<ptr>$=makeOperatorNode(2,"write",2,$<ptr>3,NULL);}
           ;

AsgStmt : ID '=' E ';'                                              {$<ptr>$=makeOperatorNode(1,"=",4,$<ptr>1,$<ptr>3);}
        | e '=' E ';'                                               {$<ptr>$=makeOperatorNode(1,"=",4,$<ptr>1,$<ptr>3);}
        | Field '=' E ';'                                           {$<ptr>$=makeOperatorNode(1,"=",4,$<ptr>1,$<ptr>3);}
        ;

SkipStmt : BREAK ';'                                                {$<ptr>$=makeOperatorNode(-2,"break",8,NULL,NULL);}
         | CONTINUE ';'                                             {$<ptr>$=makeOperatorNode(-2,"continue",8,NULL,NULL);}
         ;

E : E PLUS E                                                        {$<ptr>$=makeOperatorNode(1,"+",4,$<ptr>1,$<ptr>3);}
  | E MINUS E                                                       {$<ptr>$=makeOperatorNode(1,"-",4,$<ptr>1,$<ptr>3);}
  | E MUL E                                                         {$<ptr>$=makeOperatorNode(1,"*",4,$<ptr>1,$<ptr>3);}
  | E DIV E                                                         {$<ptr>$=makeOperatorNode(1,"/",4,$<ptr>1,$<ptr>3);}
  | E MOD E                                                         {$<ptr>$=makeOperatorNode(1,"%",4,$<ptr>1,$<ptr>3);}
  | E LT E                                                          {$<ptr>$=makeOperatorNode(0,"<",4,$<ptr>1,$<ptr>3);}
  | E GT E                                                          {$<ptr>$=makeOperatorNode(0,">",4,$<ptr>1,$<ptr>3);}
  | E LE E                                                          {$<ptr>$=makeOperatorNode(0,"<=",4,$<ptr>1,$<ptr>3);}
  | E GE E                                                          {$<ptr>$=makeOperatorNode(0,">=",4,$<ptr>1,$<ptr>3);}
  | E NE E                                                          {$<ptr>$=makeOperatorNode(0,"!=",4,$<ptr>1,$<ptr>3);}
  | E EQ E                                                          {$<ptr>$=makeOperatorNode(0,"==",4,$<ptr>1,$<ptr>3);}
  | E AND E                                                         {$<ptr>$=makeOperatorNode(0,"AND",4,$<ptr>1,$<ptr>3);}
  | E OR E                                                          {$<ptr>$=makeOperatorNode(0,"OR",4,$<ptr>1,$<ptr>3);}
  | NOT E                                                           {$<ptr>$=makeOperatorNode(0,"NOT",4,$<ptr>2,NULL);}
  | '(' E ')'                                                       {$<ptr>$=$<ptr>2;}
  | NUM                                                             {$<ptr>$=$<ptr>1;}
  | ID                                                              {$<ptr>$=$<ptr>1;}
  | CONST                                                           {$<ptr>$=$<ptr>1;}
  | e                                                               {$<ptr>$=$<ptr>1;}
  | Field                                                           {$<ptr>$=$<ptr>1;}
  | FieldFunction                                                   {$<ptr>$=$<ptr>1;}
  | null                                                            {$<ptr>$=makeOperatorNode(5,"null",12,NULL,NULL);}
  | NEW '(' ID ')'                                                  {$<ptr>$=makeOperatorNode(6,"new",13,$<ptr>3,NULL);}
  ;

Field : Field '.' ID                                                {$<ptr>$=makeOperatorNode(4,".",11,$<ptr>1,$<ptr>3);}
      | ID '.' ID                                                   {$<ptr>$=makeOperatorNode(4,".",11,$<ptr>1,$<ptr>3);}
      | SELF '.' ID                                                 {$<ptr>$=makeOperatorNode(4,".",11,NULL,$<ptr>3);}
      ;

FieldFunction : Field '.' G                                         {$<ptr>$=makeOperatorNode(4,".",11,$<ptr>1,$<ptr>3);}
              | ID '.' G                                            {$<ptr>$=makeOperatorNode(4,".",11,$<ptr>1,$<ptr>3);}
              | SELF '.' G                                          {$<ptr>$=makeOperatorNode(4,".",11,NULL,$<ptr>3);}
              ;

G : ID '(' ')'                                                      {
                                                                        $<ptr>1->nodetype=10;
                                                                        $<ptr>$=$<ptr>1;
                                                                    }
    | ID '(' ArgList ')'                                            {
                                                                        $<ptr>1->nodetype=10;
                                                                        $<ptr>1->left=$<ptr>3;
                                                                        $<ptr>$=$<ptr>1;
                                                                    }
    ;

E : ID '(' ')'                                                      {
                                                                        $<ptr>1->nodetype=10;
                                                                        $<ptr>$=$<ptr>1;
                                                                    }
    | ID '(' ArgList ')'                                            {
                                                                        $<ptr>1->nodetype=10;
                                                                        $<ptr>1->left=$<ptr>3;
                                                                        $<ptr>$=$<ptr>1;
                                                                    }
    ;
  
ArgList : ArgList ',' E                                             {
                                                                        if($<ptr>3->type==booltype){
                                                                            printf("ERROR in Line %d\n",error_line);
                                                                            exit(0);
                                                                        }
                                                                        $<ptr>1->next=$<ptr>3;
                                                                    }
        | E                                                         {
                                                                        if($<ptr>1->type==booltype){
                                                                            printf("ERROR in Line %d\n",error_line);
                                                                            exit(0);
                                                                        }
                                                                        $<ptr>$=$<ptr>1;
                                                                    }
        ;
        
e : ID '[' E ']'                                                    {
                                                                        if($<ptr>3->type!=inttype){
                                                                            printf("ERROR in Line %d\n",error_line);
                                                                            exit(0);
                                                                        }
                                                                        $<ptr>1->left=$<ptr>3;
                                                                        $<ptr>$=$<ptr>1;
                                                                    }
  | Field                                                           {$<ptr>$=$<ptr>1;}
  ;
        
%%

yyerror(char const *s){
    printf("yyerror %s",s);
}

int main(int argc, char* argv[]){
    fptr=fopen("target_file.xsm","w");
    fprintf(fptr, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",0,2056,0,0,0,0,0,0);
    if(argc>1){
        yyin=fopen(argv[1],"r");
    }
    typetablecreate("int",1);
    typetablecreate("str",1);
    typetablecreate("boolean",1);
    typetablecreate("void",1);
    yyparse();
    for(int i=0;i<20;i++){
        arr[i]=0;
    }
    fprintf(fptr, "E0:\n");
    int p=getReg();
    fprintf(fptr, "MOV R%d, 10\n",p);
    for(int i=0;i<5;i++){
        fprintf(fptr, "PUSH R%d\n",p);
    }
    fprintf(fptr, "INT 10\n");
    for(int i=0;i<5;i++){
        fprintf(fptr, "POP R%d\n",p);
    }
    freeReg(p);
    fclose(yyin);
    fclose(fptr);
    return 0;
}