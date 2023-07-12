struct classtable *clookup(char *name){
    if(name==NULL){
        return NULL;
    }
    struct classtable *temp=classtable_head;
    while(temp!=NULL&&strcmp(temp->varname,name)!=0){
        temp=temp->next;
    }
    return temp;
}

void cinstall(char *name, char *parent_class_name){
    if(clookup(name)==NULL){
        struct classtable *new=(struct classtable*)malloc(sizeof(struct classtable));
        new->varname=(char*)malloc(strlen(name)*sizeof(char));
        strcpy(new->varname,name);
        new->parentptr=clookup(parent_class_name);
        new->class_index=class_label;
        class_label++;
        new->next=NULL;
        if(classtable_head==NULL){
            classtable_head=new;
        }
        else{
            struct classtable *temp=classtable_head;
            while(temp->next!=NULL){
                temp=temp->next;
            }
            temp->next=new;
        }
    }
    else{
        printf("Error : The declared class %s is declared multiple times\n",name);
        exit(0);
    }
}

struct memberfunclist *minstall(char *name, struct typetable *type){
    struct memberfunclist *new=(struct memberfunclist*)malloc(sizeof(struct memberfunclist));
    new->varname=(char*)malloc(strlen(name)*sizeof(char));
    strcpy(new->varname,name);
    new->type=type;
    new->paramlist=param_head;
    new->funcposition=func_index;
    func_index++;
    new->flabel=f_label;
    f_label++;
    new->next=NULL;
    return new;
}

void typetablecreate(char *name, int size){
    struct typetable *new=(struct typetable*)malloc(sizeof(struct typetable));
    new->varname=(char*)malloc(strlen(name)*sizeof(char));
    strcpy(new->varname,name);
    new->size=size;
    new->fields=NULL;
    new->next=NULL;
    if(typetable_head==NULL){
        typetable_head=new;
        return;
    }
    struct typetable *temp=typetable_head;
    while(temp->next!=NULL){
        temp=temp->next;
    }
    temp->next=new;
    return;
}

struct typetable *tlookup(char *name){
    struct typetable *temp=typetable_head;
    while(temp!=NULL&&strcmp(temp->varname,name)!=0){
        temp=temp->next;
    }
    return temp;
}

struct typetable *tinstall(char *name, int size, struct fieldlist *fields){
    if(tlookup(name)==NULL){
        struct typetable *new=(struct typetable*)malloc(sizeof(struct typetable));
        new->varname=(char*)malloc(strlen(name)*sizeof(char));
        strcpy(new->varname,name);
        new->size=size;
        new->fields=fields;
        new->next=NULL;
        return new;
    }
    else{
        printf("Error : The declared typetable %s is declared multiple times\n",name);
        exit(0);
    }
    return NULL;
}

struct fieldlist *finstall(struct typetable *type, struct classtable *ctype, char *name){
    struct fieldlist *new=(struct fieldlist*)malloc(sizeof(struct fieldlist));
    new->varname=(char*)malloc(strlen(name)*sizeof(char));
    strcpy(new->varname,name);
    new->fieldindex=field_index;
    field_index++;
    new->type=type;
    new->ctype=ctype;
    new->next=NULL;
    return new;
}

int getSize(struct typetable *type){
    int size=0;
    struct fieldlist *temp=type->fields;
    while(temp!=NULL){
        size++;
        temp=temp->next;
    }
    return size;
}

struct gsymbol *glookup(char *name){
    struct gsymbol *temp=global_head;
    while(temp!=NULL&&strcmp(temp->varname,name)!=0){
        temp=temp->next;
    }
    return temp;
}

void ginstall(char *name, int size){
    if(glookup(name)==NULL){
        struct gsymbol *new=(struct gsymbol*)malloc(sizeof(struct gsymbol));
        new->varname=(char*)malloc(strlen(name)*sizeof(char));
        strcpy(new->varname,name);
        new->type=vartype[var_index];
        new->ctype=classtable_temp;
        new->size=size;
        new->binding=binding_address;
        binding_address+=size;
        new->paramlist=NULL;
        new->flabel=-1;
        new->next=NULL;
        if(param_head!=NULL){
            new->paramlist=param_head;
            param_head=NULL;
            f_label++;
            new->flabel=f_label;
        }
        if(global_head==NULL){
            global_head=new;
        }
        else{
            struct gsymbol *temp=global_head;
            while(temp->next!=NULL){
                temp=temp->next;
            }
            temp->next=new;
        }
    }
    else{
        printf("Error : The variable name %s is declared multiple times\n",name);
        exit(0);
    }
    return;
}

struct plist *plookup(char *name){
    struct plist *temp=param_head;
    while(temp!=NULL&&strcmp(temp->varname,name)!=0){
        temp=temp->next;
    }
    return temp;
}

void pinstall(char *name){
    if(plookup(name)==NULL){
        struct plist *new=(struct plist*)malloc(sizeof(struct plist));
        new->varname=(char*)malloc(strlen(name)*sizeof(char));
        strcpy(new->varname,name);
        new->type=vartype[var_index];
        new->next=NULL;
        if(param_head==NULL){
            param_head=new;
        }
        else{
            struct plist *temp=param_head;
            while(temp->next!=NULL){
                temp=temp->next;
            }
            temp->next=new;
        }
    }
    else{
        printf("Error : The paramater %s has already been declared\n",name);
        exit(0);
    }
    return;
}

struct lsymbol *llookup(char *name){
    struct lsymbol *temp=local_head;
    while(temp!=NULL&&strcmp(temp->varname,name)!=0){
        temp=temp->next;
    }
    return temp;
}

void linstall(char *name, int size, int local_variable){
    if(llookup(name)==NULL){
        struct lsymbol *new=(struct lsymbol*)malloc(sizeof(struct lsymbol));
        new->varname=(char*)malloc(strlen(name)*sizeof(char));
        strcpy(new->varname,name);
        new->type=vartype[var_index];
        new->binding=local_binding_address;
        local_binding_address+=size;
        new->local_variable=local_variable;
        new->next=NULL;
        if(local_head==NULL){
            local_head=new;
        }
        else{
            struct lsymbol *temp=local_head;
            while(temp->next!=NULL){
                temp=temp->next;
            }
            temp->next=new;
        }
    }
    else{
        printf("Error : The variable name %s is declared multiple times\n",name);
        exit(0);
    }
    return;
}

struct classtable *classcalculate(struct tnode *head){
    if(head==NULL){
        return clookup(class_name);
    }
    if(head->varname[0]=='.'){
        struct classtable *temp=classcalculate(head->left);
        if(temp==NULL){
            struct fieldlist *ptr1=class_type->fields;
            while(ptr1!=NULL&&strcmp(ptr1->varname,head->right->varname)!=0){
                ptr1=ptr1->next;
            }
            if(ptr1==NULL){
                printf("Error in line %d\n",error_line);
                exit(0);
            }
            class_type=ptr1->type;
            return NULL;
        }
        struct fieldlist *ptr=temp->memberfield;
        while(ptr!=NULL&&strcmp(ptr->varname,head->right->varname)!=0){
            ptr=ptr->next;
        }
        if(ptr==NULL){
            printf("Error in line %d\n",error_line);
            exit(0);
        }
        // Since this function is only called
        // to know obj type in obj.func() so
        // the return type is always classtable
        if(ptr->ctype==NULL){
            class_type=ptr->type;
        }
        return ptr->ctype;
    }
    else{
        // The class objects are defined globally or in a class
        return glookup(head->varname)->ctype;
    }
}

struct typetable *typecalculate(struct tnode *head){
    if(head==NULL){
        return NULL;
    }
    if(head->varname[0]=='.'){
        struct typetable *temp=typecalculate(head->left);
        if(temp==NULL){
            return NULL;
        }
        struct fieldlist *ptr=temp->fields;
        while(ptr!=NULL&&strcmp(ptr->varname,head->right->varname)!=0){
            ptr=ptr->next;
        }
        if(ptr==NULL){
            printf("Error in line %d\n",error_line);
            exit(0);
        }
        return ptr->type;
    }
    else{
        if(llookup(head->varname)!=NULL){
            return llookup(head->varname)->type;
        }
        else{
            return glookup(head->varname)->type;
        }
    }
}

struct tnode *makeLeafNode(char *name, int type, int nodetype){
    struct tnode *new=(struct tnode*)malloc(sizeof(struct tnode));
    new->varname=(char*)malloc(strlen(name)*sizeof(char));
    strcpy(new->varname,name);
    new->type=type;
    new->nodetype=nodetype;
    new->Ggentry=glookup(name);
    new->Lgentry=llookup(name);
    if(new->Ggentry!=NULL){
        new->ctype=new->Ggentry->ctype;
    }
    new->left=new->right=new->next=NULL;
    // Upto stage 6 the functions are defined globally only
    // In stage 7 and stage 8 the functions can be declared globally
    // or in a class or inherited
    int class_fun_flag=0;
    struct classtable *temp=classtable_head;
    while(temp!=NULL){
        struct fieldlist *ptr=temp->memberfield;
        while(ptr!=NULL&&strcmp(ptr->varname,name)!=0){
            ptr=ptr->next;
        }
        if(ptr!=NULL){
            class_fun_flag=1;
            break;
        }
        temp=temp->next;
    }
    if(class_fun_flag==0){
        struct classtable *temp1=classtable_head;
        while(temp1!=NULL){
            struct memberfunclist *ptr1=temp1->vfuncptr;
            while(ptr1!=NULL&&strcmp(ptr1->varname,name)!=0){
                ptr1=ptr1->next;
            }
            if(ptr1!=NULL){
                class_fun_flag=1;
                break;
            }
            temp1=temp1->next;
        }
    }
    if(new->Ggentry==NULL&&new->Lgentry==NULL&&decl_flag==0&&new->nodetype==0&&class_fun_flag==0&&clookup(name)==NULL){
        // Leaf node must be variable
        if(strcmp(name,"initialize")!=0&&strcmp(name,"alloc")!=0&&strcmp(name,"free")!=0&&strcmp(name,"new")!=0){
            printf("Error : The variable %s has not been declared\n",name);
            exit(0);
        }
    }
    return new;
}

struct tnode *makeOperatorNode(int type, char *name, int nodetype, struct tnode *left, struct tnode *right){
    if(type==0){
        // Node is boolean operator
        if(strcmp(name,"AND")==0||strcmp(name,"OR")==0){
            if((left->type!=booltype||left->type!=dottype)&&(right->type!=booltype||right->type!=dottype)){
                printf("ERROR in Line %d\n",error_line);
                exit(0);
            }
        }
        else if(strcmp(name,"NOT")==0){
            if(left->type!=booltype||left->type!=dottype){
                printf("ERROR in Line %d\n",error_line);
                exit(0);
            }
        }
        else{
            if(strcmp(name,"==")==0||strcmp(name,"!=")==0){
                if((left->type==inttype||left->type==dottype)&&(right->type==inttype&&right->type==dottype&&right->type==5)){
                    printf("ERROR in Line %d\n",error_line);
                    exit(0);
                }
            }
            else{
                if((left->type==inttype||left->type==dottype)&&(right->type==inttype&&right->type==dottype)){
                    printf("ERROR in Line %d\n",error_line);
                    exit(0);
                }
            }
        }
    }
    else if(type==1){
        // Node is assignment operator
        if(strcmp(name,"=")==0){
            struct typetable *left_type;
            struct fieldlist *left_type1;
            if(left->Lgentry!=NULL){
                left_type=left->Lgentry->type;
            }
            else if(left->Ggentry!=NULL){
                left_type=left->Ggentry->type;
            }
            else{
                // Left is dot operator
                struct tnode *ptr=left;
                while(ptr->left!=NULL){
                    ptr=ptr->left;
                }
                if(ptr->varname[0]=='.'||glookup(ptr->varname)!=NULL&&glookup(ptr->varname)->ctype!=NULL){
                    if(classcalculate(left->left)==NULL){
                        left_type1=class_type->fields;
                    }
                    else{
                        left_type1=classcalculate(left->left)->memberfield;
                    }
                }
                else{
                    left_type1=typecalculate(left->left)->fields;
                }
                while(left_type1!=NULL&&strcmp(left_type1->varname,left->right->varname)!=0){
                    left_type1=left_type1->next;
                }
                if(left_type1==NULL){
                    left_type=tlookup("int");
                }
                else{
                    left_type=left_type1->type;
                }
            }
            if(right->Lgentry!=NULL){
                if(left_type!=right->Lgentry->type){
                    printf("ERROR in Line %d\n",error_line);
                    exit(0);
                }
            }
            else if(right->Ggentry!=NULL){
                if(left_type!=right->Ggentry->type){
                    printf("ERROR in Line %d\n",error_line);
                    exit(0);
                }
            }
            else{
                if(right->nodetype==10){
                    if(strcmp(right->varname,"alloc")==0||strcmp(right->varname,"new")==0){
                        if(strcmp(left_type->varname,"int")==0||strcmp(left_type->varname,"str")==0){
                            printf("ERROR in Line %d\n",error_line);
                            exit(0);
                        }
                    }
                }
                else{
                    if(right->type==inttype){
                        if(strcmp(left_type->varname,"int")!=0){
                            printf("ERROR in Line %d\n",error_line);
                            exit(0);
                        }
                    }
                    else if(right->type==strtype){
                        if(strcmp(left_type->varname,"str")!=0){
                            printf("ERROR in Line %d\n",error_line);
                            exit(0);
                        }
                    }
                    else if(right->type==dottype){
                        // right is dot operator
                        struct tnode *ptr=right;
                        struct fieldlist *left_type1;
                        struct typetable *right_type;
                        while(ptr->left!=NULL){
                            ptr=ptr->left;
                        }
                        if(ptr->varname[0]=='.'||glookup(ptr->varname)!=NULL&&glookup(ptr->varname)->ctype!=NULL){
                            if(classcalculate(right->left)==NULL){
                                left_type1=class_type->fields;
                            }
                            else{
                                left_type1=classcalculate(right->left)->memberfield;
                            }
                        }
                        else{
                            left_type1=typecalculate(right->left)->fields;
                        }
                        while(left_type1!=NULL&&strcmp(left_type1->varname,right->right->varname)!=0){
                            left_type1=left_type1->next;
                        }
                        if(left_type1==NULL){
                            right_type=left_type;
                        }
                        else{
                            right_type=left_type1->type;
                        }
                        if(left_type!=right_type){
                            printf("Return type ERROR in Line %d\n",error_line-1);
                            exit(0);
                        }
                    }
                    else{
                        if(right->type==booltype){
                            printf("ERROR in Line %d\n",error_line);
                            exit(0);
                        }
                    }
                }
            }
        }
        else{
            if(left->type==booltype&&right->type==booltype){
                printf("ERROR in Line %d\n",error_line);
                exit(0);
            }
        }
    }
    else if(type==2){
        // .....Write
        if(strcmp(name,"write")==0){
            if(left->type!=inttype&&left->type!=dottype&&left->type!=strtype){
                printf("ERROR in Line %d\n",error_line);
                exit(0);
            }
        }
    }
    else if(type==3){
        // Return
        if(strcmp(function_name,"MAIN")==0){
            struct typetable *right_type=NULL;
            struct fieldlist *left_type1;
            if(right->Lgentry!=NULL){
                right_type=right->Lgentry->type;
            }
            else if(right->Ggentry!=NULL){
                right_type=right->Ggentry->type;
            }
            else{
                // Dot Operator
                if(right->type==dottype){
                struct tnode *ptr=right;
                while(ptr->left!=NULL){
                    ptr=ptr->left;
                }
                if(ptr->varname[0]=='.'||glookup(ptr->varname)!=NULL&&glookup(ptr->varname)->ctype!=NULL){
                    if(classcalculate(right->left)==NULL){
                        left_type1=class_type->fields;
                    }
                    else{
                        left_type1=classcalculate(right->left)->memberfield;
                    }
                }
                else{
                    left_type1=typecalculate(right->left)->fields;
                }
                while(left_type1!=NULL&&strcmp(left_type1->varname,right->right->varname)!=0){
                    left_type1=left_type1->next;
                }
                if(left_type1==NULL){
                    right_type=tlookup("int");
                }
                else{
                    right_type=left_type1->type;
                }
                }
            }
            if(right_type==NULL){
                if(right->type!=inttype){
                    printf("Return type ERROR in Line %d\n",error_line-1);
                    exit(0);
                }
            }
            else{
                if(strcmp(right_type->varname,"int")!=0){
                    printf("Return type ERROR in Line %d\n",error_line-1);
                    exit(0);
                }
            }
        }
        else{
            struct gsymbol *temp=glookup(function_name);
            if(temp==NULL){
                if(class_flag==0){
                    printf("Function declaration not present in global declarations for %s\n",function_name);
                    exit(0);
                }
                struct memberfunclist *ptr=clookup(class_name)->vfuncptr;
                while(ptr!=NULL&&strcmp(ptr->varname,function_name)!=0){
                    ptr=ptr->next;
                }
                if(ptr==NULL){
                    printf("Function declaration not present in class %s for %s\n",class_name,function_name);
                    exit(0);
                }
                if(right->Lgentry!=NULL){
                    if(ptr!=NULL&&right->Lgentry->type!=ptr->type){
                        printf("Return type ERROR in Line %d\n",error_line-1);
                        exit(0);
                    }   
                }
                else if(right->Ggentry!=NULL){
                    if(ptr!=NULL&&right->Ggentry->type!=ptr->type){
                        printf("Return type ERROR in Line %d\n",error_line-1);
                        exit(0);   
                    }
                }
                else{
                    // The return type (integer|string|null|dot)
                    int gg=1;
                    if(right->type==inttype){
                        if(strcmp(ptr->type->varname,"int")!=0){
                            printf("Return type ERROR in Line %d\n",error_line-1);
                            exit(0);
                        }
                    }
                    else if(right->type==strtype){
                        if(strcmp(ptr->type->varname,"str")!=0){
                            printf("Return type ERROR in Line %d\n",error_line-1);
                            exit(0);
                        }
                    }
                    else if(right->type==dottype){
                        struct tnode *ptr1=right;
                        struct fieldlist *left_type1;
                        struct typetable *right_type;
                        while(ptr1->left!=NULL){
                            ptr1=ptr1->left;
                        }
                        if(ptr1->varname[0]=='.'||glookup(ptr1->varname)!=NULL&&glookup(ptr1->varname)->ctype!=NULL){
                            if(classcalculate(right->left)==NULL){
                                left_type1=class_type->fields;
                            }
                            else{
                                left_type1=classcalculate(right->left)->memberfield;
                            }
                        }
                        else{
                            left_type1=typecalculate(right->left)->fields;
                        }
                        while(left_type1!=NULL&&strcmp(left_type1->varname,right->right->varname)!=0){
                            left_type1=left_type1->next;
                        }
                        if(left_type1==NULL){
                            right_type=tlookup("int");
                        }
                        else{
                            right_type=left_type1->type;
                        }
                        if(ptr->type!=right_type){
                            printf("Return type ERROR in Line %d\n",error_line-1);
                            exit(0);
                        }
                    }
                    else{
                        if(right->type==booltype){
                            printf("Return type ERROR in Line %d\n",error_line-1);
                            exit(0);
                        }
                        if(right->type==5){
                            if(strcmp(ptr->type->varname,"int")==0||strcmp(ptr->type->varname,"void")==0){
                                printf("Return type ERROR in Line %d\n",error_line-1);
                                exit(0);
                            }
                        }
                    }
                }
            }
            else{
                if(right->Lgentry!=NULL){
                    if(temp!=NULL&&right->Lgentry->type!=temp->type){
                        printf("Return type ERROR in Line %d\n",error_line-1);
                        exit(0);
                    }    
                }
                else if(right->Ggentry!=NULL){
                    if(temp!=NULL&&right->Ggentry->type!=temp->type){
                        printf("Return type ERROR in Line %d\n",error_line-1);
                        exit(0);   
                    }
                }
                else{
                    // The return type (integer|string|null|dot)
                    int gg=1;
                    if(right->type==inttype){
                        if(strcmp(temp->type->varname,"int")!=0){
                            printf("Return type ERROR in Line %d\n",error_line-1);
                            exit(0);
                        }
                    }
                    else if(right->type==strtype){
                        if(strcmp(temp->type->varname,"str")!=0){
                            printf("Return type ERROR in Line %d\n",error_line-1);
                            exit(0);
                        }
                    }
                    else if(right->type==dottype){
                        struct tnode *ptr=right;
                        struct fieldlist *left_type1;
                        struct typetable *right_type;
                        while(ptr->left!=NULL){
                            ptr=ptr->left;
                        }
                        if(ptr->varname[0]=='.'||glookup(ptr->varname)!=NULL&&glookup(ptr->varname)->ctype!=NULL){
                            if(classcalculate(right->left)==NULL){
                                left_type1=class_type->fields;
                            }
                            else{
                                left_type1=classcalculate(right->left)->memberfield;
                            }
                        }
                        else{
                            left_type1=typecalculate(right->left)->fields;
                        }
                        while(left_type1!=NULL&&strcmp(left_type1->varname,right->right->varname)!=0){
                            left_type1=left_type1->next;
                        }
                        if(left_type1==NULL){
                            right_type=tlookup("int");
                        }
                        else{
                            right_type=left_type1->type;
                        }
                        if(temp->type!=right_type){
                            printf("Return type ERROR in Line %d\n",error_line-1);
                            exit(0);
                        }
                    }
                    else{
                        if(right->type==booltype){
                            printf("Return type ERROR in Line %d\n",error_line-1);
                            exit(0);
                        }
                        if(right->type==5){
                            if(strcmp(temp->type->varname,"int")==0||strcmp(temp->type->varname,"void")==0){
                                printf("Return type ERROR in Line %d\n",error_line-1);
                                exit(0);
                            }
                        }
                    }
                }
            }
        }
    }
    else{
        if(type==-1){
            // If and while
            if(left->type!=booltype){
                printf("ERROR in Line %d\n",error_line);
                exit(0);
            }
        }
    }
    struct tnode *new=(struct tnode*)malloc(sizeof(struct tnode));
    new->varname=(char*)malloc(strlen(name)*sizeof(char));
    strcpy(new->varname,name);
    new->type=type;
    new->nodetype=nodetype;
    new->left=left;
    new->right=right;
    return new;
}

int getReg(){
    for(int i=0;i<20;i++){
        if(arr[i]==0){
            arr[i]=1;
            return i;
        }
    }
}

void freeReg(int x){
    arr[x]=0;
    return;
}

int getLabel(){
    label++;
    return label;
}

int calculate(struct tnode *head){
    struct fieldlist *temp;
    struct tnode *ptr=head;
    while(ptr->left!=NULL){
        ptr=ptr->left;
    }
    if(ptr->varname[0]=='.'||glookup(ptr->varname)!=NULL&&glookup(ptr->varname)->ctype!=NULL){
        if(classcalculate(head->left)==NULL){
            temp=class_type->fields;
        }
        else{
            temp=classcalculate(head->left)->memberfield;
        }
    }
    else{
        temp=typecalculate(head->left)->fields;
    }
    int num=1;
    while(temp!=NULL&&strcmp(temp->varname,head->right->varname)!=0){
        num++;
        temp=temp->next;
    }
    if(temp==NULL){
        printf("Error %s\n",head->right->varname);
        exit(0);
    }
    return num;
}

int getlabel(char *name){
    struct classtable *temp=classtable_head;
    while(temp!=NULL){
        struct memberfunclist *ptr=temp->vfuncptr;
        while(ptr!=NULL&&strcmp(ptr->varname,name)!=0){
            ptr=ptr->next;
        }
        if(ptr!=NULL){
            return ptr->flabel;
        }
        temp=temp->next;
    }
    return -1;
}

int codegen(struct tnode *head, FILE *fptr){
    if(head==NULL){
        return -1;
    }
    if(head->left==NULL&&head->right==NULL&&head->nodetype!=10){
        // This represents a leaf node and possible leaf node are
        // 1) Integer Constant
        // 2) String Constant
        // 3) Declared Variable
        // 4) NULL node
        int p=getReg();
        if(head->Lgentry==NULL&&head->Ggentry==NULL){
            // NULL node or Integer or String Constant
            if(strcmp(head->varname,"null")==0){
                // NULL node
                fprintf(fptr, "MOV R%d, %d\n",p,0);
            }
            else if(head->varname[0]>=48&&head->varname[0]<=57){
                // Integer Constant
                fprintf(fptr, "MOV R%d, %d\n",p,atoi(head->varname));
            }
            else{
                // String Constant
                fprintf(fptr, "MOV R%d, %s\n",p,head->varname);
            }
        }
        else{
            // Declared Variable
            // For declared variables the address is stored
            // in a register and register index is returned
            if(head->Lgentry!=NULL){
                // Scope is available inside the function either 
                // declared locally or
                // sent through argument
                if(head->Lgentry->local_variable){
                    // Local variable (declared locally)
                    fprintf(fptr, "MOV R%d, BP\n",p);
                    int x=getReg();
                    fprintf(fptr, "MOV R%d, %d\n",x,head->Lgentry->binding);
                    fprintf(fptr, "ADD R%d, R%d\n",p,x);
                    freeReg(x);
                }
                else{
                    // Sent argument
                    int number_of_arguments=0;
                    struct lsymbol *temp=local_head;
                    while(temp!=NULL){
                        if(!temp->local_variable){
                            number_of_arguments++;
                        }
                        temp=temp->next;
                    }
                    fprintf(fptr, "MOV R%d, BP\n",p);
                    int x=getReg();
                    fprintf(fptr, "MOV R%d, %d\n",x,number_of_arguments+3-head->Lgentry->binding);
                    fprintf(fptr, "SUB R%d, R%d\n",p,x);
                    freeReg(x);
                }
            }
            else{
                // Scope is available globally i.e. declared globally
                fprintf(fptr, "MOV R%d, %d\n",p,head->Ggentry->binding);
            }
        }
        return p;
    }
    else if(head->left!=NULL&&head->right==NULL||head->nodetype==10){
        if(head->nodetype==13){
            // NEW keyword
            // Same as alloc function
            int i=0;
            while(arr[i]==0){
                arr[i]=1;
                i++;
            }
            int x=getReg();
            arr[x]=0;
            int temp;
            // Pushing the live registers
            for(int i=0;i<20;i++){
                if(arr[i]==1){
                    fprintf(fptr, "PUSH R%d\n",i);
                    arr[i]=0;
                }
                else{
                    temp=i;
                    break;
                }
            }
            fprintf(fptr, "MOV R%d, \"alloc\"\n",x);
            // What about size of alloc?
            for(int i=0;i<5;i++){
                fprintf(fptr, "PUSH R%d\n",x);
            }
            fprintf(fptr, "CALL 0\n");
            for(int i=0;i<=x;i++){
                arr[i]=1;
            }
            fprintf(fptr, "POP R%d\n",x);
            int y=getReg();
            for(int i=0;i<4;i++){
                fprintf(fptr, "POP R%d\n",y);
            }
            freeReg(y);
            for(int i=temp-1;i>=0;i--){
                fprintf(fptr, "POP R%d\n",i);
                arr[i]=1;
            }
            return x;
        }
        else if(head->nodetype==4){
            // NOT boolean operator
            int p=codegen(head->left,fptr);
            int z=getReg();
            fprintf(fptr, "MOV R%d, 0\n",z);
            fprintf(fptr, "EQ R%d, R%d\n",p,z);
            freeReg(z);
            return p;
        }
        else if(head->nodetype==10){
            // Function Call and they can be
            // 1) Initialise function
            // 2) Alloc function
            // 3) Free function
            // 4) Normal function call
            if(strcmp(head->varname,"initialize")==0){
                int x=getReg();
                arr[x]=0;
                int temp;
                // Pushing the live registers
                for(int i=0;i<20;i++){
                    if(arr[i]==1){
                        fprintf(fptr, "PUSH R%d\n",i);
                        arr[i]=0;
                    }
                    else{
                        temp=i;
                        break;
                    }
                }
                fprintf(fptr, "MOV R%d, \"%s\"\n",x,head->varname);
                for(int i=0;i<5;i++){
                    fprintf(fptr, "PUSH R%d\n",x);
                }
                fprintf(fptr, "CALL 0\n");
                for(int i=0;i<=x;i++){
                    arr[i]=1;
                }
                fprintf(fptr, "POP R%d\n",x);
                int y=getReg();
                for(int i=0;i<4;i++){
                    fprintf(fptr, "POP R%d\n",y);
                }
                freeReg(y);
                for(int i=temp-1;i>=0;i--){
                    fprintf(fptr, "POP R%d\n",i);
                    arr[i]=1;
                }
                return x;
            }
            else if(strcmp(head->varname,"alloc")==0){
                int x=getReg();
                arr[x]=0;
                int temp;
                // Pushing the live registers
                for(int i=0;i<20;i++){
                    if(arr[i]==1){
                        fprintf(fptr, "PUSH R%d\n",i);
                        arr[i]=0;
                    }
                    else{
                        temp=i;
                        break;
                    }
                }
                fprintf(fptr, "MOV R%d, \"%s\"\n",x,head->varname);
                // What about size of alloc?
                for(int i=0;i<5;i++){
                    fprintf(fptr, "PUSH R%d\n",x);
                }
                fprintf(fptr, "CALL 0\n");
                for(int i=0;i<=x;i++){
                    arr[i]=1;
                }
                fprintf(fptr, "POP R%d\n",x);
                int y=getReg();
                for(int i=0;i<4;i++){
                    fprintf(fptr, "POP R%d\n",y);
                }
                freeReg(y);
                for(int i=temp-1;i>=0;i--){
                    fprintf(fptr, "POP R%d\n",i);
                    arr[i]=1;
                }
                return x;
            }
            else if(strcmp(head->varname,"free")==0){
                int x=getReg();
                arr[x]=0;
                int temp;
                // Pushing the live registers
                for(int i=0;i<20;i++){
                    if(arr[i]==1){
                        fprintf(fptr, "PUSH R%d\n",i);
                        arr[i]=0;
                    }
                    else{
                        temp=i;
                        break;
                    }
                }
                fprintf(fptr, "MOV R%d, \"%s\"\n",x,head->varname);
                fprintf(fptr, "PUSH R%d\n",x);
                int z=codegen(head->left,fptr);
                fprintf(fptr, "MOV R%d, R%d\n",x,z);
                freeReg(z);
                for(int i=0;i<4;i++){
                    fprintf(fptr, "PUSH R%d\n",x);
                }
                fprintf(fptr, "CALL 0\n");
                for(int i=0;i<=x;i++){
                    arr[i]=1;
                }
                fprintf(fptr, "POP R%d\n",x);
                int y=getReg();
                for(int i=0;i<4;i++){
                    fprintf(fptr, "POP R%d\n",y);
                }
                freeReg(y);
                for(int i=temp-1;i>=0;i--){
                    fprintf(fptr, "POP R%d\n",i);
                    arr[i]=1;
                }
                return x;
            }
            else{
                // Normal Function Call
                int temp;

                int g=getReg();
                fprintf(fptr, "POP R%d\n",g);
                fprintf(fptr, "PUSH R%d\n",g);
                freeReg(g);

                // Pushing the live registers

                for(int i=0;i<20;i++){
                    if(arr[i]==1){
                        fprintf(fptr, "PUSH R%d\n",i);
                        arr[i]=0;
                    }
                    else{
                        temp=i;
                        break;
                    }
                }

                fprintf(fptr, "PUSH R%d\n",g);

                // Pushing the arguments
                struct tnode *arg_ptr=head->left;
                while(arg_ptr!=NULL){
                    int x=codegen(arg_ptr,fptr);
                    if((arg_ptr->Lgentry!=NULL||arg_ptr->Ggentry!=NULL||arg_ptr->nodetype==11)&&arg_ptr->nodetype!=10){
                        if(!(arg_ptr->right!=NULL&&arg_ptr->right->nodetype==10)){
                            fprintf(fptr, "MOV R%d, [R%d]\n",x,x);
                        }
                    }
                    fprintf(fptr, "PUSH R%d\n",x);
                    freeReg(x);
                    arg_ptr=arg_ptr->next;
                }

                // Memory address for return value
                fprintf(fptr, "ADD SP, 1\n");

                // Function call
                if(head->Ggentry==NULL){
                    fprintf(fptr, "CALL F%d\n",getlabel(head->varname));
                }
                else{
                    fprintf(fptr, "CALL F%d\n",head->Ggentry->flabel);
                }
                
                for(int i=0;i<temp;i++){
                    arr[i]=1;
                }

                // Poping ans accessing return value
                int p=getReg();
                fprintf(fptr, "POP R%d\n",p);
            
                // Poping arguments
                arg_ptr=head->left;
                int z=getReg();
                while(arg_ptr!=NULL){
                    fprintf(fptr, "POP R%d\n",z);
                    arg_ptr=arg_ptr->next;
                }
                // Poping self address
                fprintf(fptr, "POP R%d\n",z);
                freeReg(z);

                // Poping registers
                for(int i=temp-1;i>=0;i--){
                    fprintf(fptr, "POP R%d\n",i);
                    arr[i]=1;
                }

                return p;
            }
        }
        else{
            // Single Dimentional Array used
            // which was initially declared globally
            int x=codegen(head->left,fptr);
            if((head->left->Lgentry!=NULL||head->left->Ggentry!=NULL||head->left->nodetype==11)&&head->left->nodetype!=10){
                fprintf(fptr, "MOV R%d, [R%d]\n",x,x);
            }
            int p=getReg();
            fprintf(fptr, "MOV R%d, %d\n",p,head->Ggentry->binding);
            fprintf(fptr, "ADD R%d, R%d\n",x,p);
            freeReg(p);
            return x;
        }
    }
    else{
        int x1=codegen(head->left,fptr);
        if(head->varname[0]=='.'){
            // DOT Operator
            if(head->left==NULL){
                // SELF keyword
                if(head->right->nodetype==10){
                    // DOT operator with function call
                    // PUSH address for SELF
                    int x=getReg();
                    int number_of_arguments=0;
                    struct lsymbol *temp=local_head;
                    while(temp!=NULL){
                        if(!temp->local_variable){
                            number_of_arguments++;
                        }
                        temp=temp->next;
                    }
                    int y=getReg();
                    fprintf(fptr, "MOV R%d, BP\n",y);
                    fprintf(fptr, "SUB R%d, %d\n",y,number_of_arguments+3);
                    fprintf(fptr, "MOV R%d, [R%d]\n",x,y);
                    freeReg(y);
                    fprintf(fptr, "PUSH R%d\n",x);
                    // PUSH object type
                    int z=codegen(head->right,fptr);
                    // POP object type
                    // POP address for SELF
                    fprintf(fptr, "POP R%d\n",x);
                    freeReg(x);
                    return z;
                }
                else{
                    int x=getReg();
                    int y=getReg();
                    int number_of_arguments=0;
                    struct lsymbol *temp=local_head;
                    while(temp!=NULL){
                        if(!temp->local_variable){
                            number_of_arguments++;
                        }
                        temp=temp->next;
                    }
                    int z=getReg();
                    fprintf(fptr, "MOV R%d, BP\n",z);
                    fprintf(fptr, "SUB R%d, %d\n",z,number_of_arguments+3);
                    fprintf(fptr, "MOV R%d, [R%d]\n",x,z);
                    freeReg(z);
                    fprintf(fptr, "MOV R%d, [R%d]\n",y,x);
                    fprintf(fptr, "MOV R%d, %d\n",x,calculate(head));
                    fprintf(fptr, "ADD R%d, R%d\n",y,x);
                    freeReg(x);
                    return y;
                }
            }
            else{
                if(head->right->nodetype==10){
                    // DOT operator with function call
                    // PUSH address for SELF
                    fprintf(fptr, "PUSH R%d\n",x1);
                    freeReg(x1);
                    // PUSH object type
                    int z=codegen(head->right,fptr);
                    // POP object type
                    int q=getReg();
                    fprintf(fptr, "POP R%d\n",q);
                    freeReg(q);
                    return z;
                }
                else{
                    int x=getReg();
                    fprintf(fptr, "MOV R%d, [R%d]\n",x1,x1);
                    fprintf(fptr, "MOV R%d, %d\n",x,calculate(head));
                    fprintf(fptr, "ADD R%d, R%d\n",x1,x);
                    freeReg(x);
                    return x1;
                }
            }
        }
        int x2=codegen(head->right,fptr);
        int y1=getReg();
        int y2=getReg();
        fprintf(fptr, "MOV R%d, R%d\n",y1,x1);
        fprintf(fptr, "MOV R%d, R%d\n",y2,x2);
        // Check for the variables present on the
        // left or right of the assignment operator
        if((head->left->Lgentry!=NULL||head->left->Ggentry!=NULL||head->left->nodetype==11)&&head->left->nodetype!=10){
            if(!(head->left!=NULL&&head->left->right!=NULL&&head->left->right->nodetype==10)){
                fprintf(fptr, "MOV R%d, [R%d]\n",y1,y1);
            }
        }
        if((head->right->Lgentry!=NULL||head->right->Ggentry!=NULL||head->right->nodetype==11)&&head->right->nodetype!=10){
            if(!(head->right!=NULL&&head->right->right!=NULL&&head->right->right->nodetype==10)){
                fprintf(fptr, "MOV R%d, [R%d]\n",y2,y2);
            }
        }
        // Rx1 and Rx2 stores the address/values of variables(if any exists)
        // Ry1 and Ry2 stores the values of variables(if any exists)
        if(strlen(head->varname)==1){
            switch(head->varname[0]){
                case '+':
                    fprintf(fptr, "ADD R%d, R%d\n",y1,y2);
                    break;
                case '-':
                    fprintf(fptr, "SUB R%d, R%d\n",y1,y2);
                    break;
                case '*':
                    fprintf(fptr, "MUL R%d, R%d\n",y1,y2);
                    break;
                case '/':
                    fprintf(fptr, "DIV R%d, R%d\n",y1,y2);
                    break;
                case '=':
                    fprintf(fptr, "MOV [R%d], R%d\n",x1,y2);
                    break;
                case '<':
                    fprintf(fptr, "LT R%d, R%d\n",y1,y2);
                    break;
                case '>':
                    fprintf(fptr, "GT R%d, R%d\n",y1,y2);
                    break;
                case '%':
                    int m=getReg();
                    fprintf(fptr, "MOV R%d, R%d\n",m,y1);
                    fprintf(fptr, "DIV R%d, R%d\n",y1,y2);
                    fprintf(fptr, "MUL R%d, R%d\n",y1,y2);
                    fprintf(fptr, "SUB R%d, R%d\n",m,y1);
                    fprintf(fptr, "MOV R%d, R%d\n",y1,m);
                    freeReg(m);
                    break;
            }
        }
        else{
            if(strcmp(head->varname,"<=")==0){
                fprintf(fptr, "LE R%d, R%d\n",y1,y2);
            }
            else if(strcmp(head->varname,">=")==0){
                fprintf(fptr, "GE R%d, R%d\n",y1,y2);
            }
            else if(strcmp(head->varname,"!=")==0){
                fprintf(fptr, "NE R%d, R%d\n",y1,y2);
            }
            else if(strcmp(head->varname,"==")==0){
                fprintf(fptr, "EQ R%d, R%d\n",y1,y2);
            }
            else if(strcmp(head->varname,"AND")==0){
                // AND boolean operator
                fprintf(fptr, "ADD R%d, R%d\n",y1,y2);
                int z=getReg();
                fprintf(fptr, "MOV R%d, 1\n",z);
                fprintf(fptr, "GT R%d, R%d\n",y1,z);
                freeReg(z);
            }
            else{
                // OR boolean operator
                fprintf(fptr, "ADD R%d, R%d\n",y1,y2);
                int z=getReg();
                fprintf(fptr, "MOV R%d, 0\n",z);
                fprintf(fptr, "GT R%d, R%d\n",y1,z);
                freeReg(z);
            }
        }
        freeReg(x1);
        freeReg(x2);
        freeReg(y2);
        return y1;
    }
}

void read(struct tnode *head, FILE *fptr){
    int index=codegen(head,fptr);
    int p=getReg();
    fprintf(fptr, "MOV R%d, 7\n",p);
    fprintf(fptr, "PUSH R%d\n",p);
    fprintf(fptr, "MOV R%d, -1\n",p);
    fprintf(fptr, "PUSH R%d\n",p);
    fprintf(fptr, "PUSH R%d\n",index);
    fprintf(fptr, "PUSH R%d\n",p);
    fprintf(fptr, "PUSH R%d\n",p);
    fprintf(fptr, "INT 6\n");
    for(int i=0;i<5;i++){
        fprintf(fptr, "POP R%d\n",p);
    }
    freeReg(p);
    return;
}

void write(struct tnode *head, FILE *fptr){
    int index=codegen(head,fptr);
    int p=getReg();
    fprintf(fptr, "MOV R%d, 5\n",p);
    fprintf(fptr, "PUSH R%d\n",p);
    fprintf(fptr, "MOV R%d, -2\n",p);
    fprintf(fptr, "PUSH R%d\n",p);
    if(head->Lgentry!=NULL||head->Ggentry!=NULL||(head->nodetype==11&&head->right->nodetype!=10)){
        fprintf(fptr, "MOV R%d, [R%d]\n",index,index);
    }
    fprintf(fptr, "PUSH R%d\n",index);
    fprintf(fptr, "PUSH R%d\n",p);
    fprintf(fptr, "PUSH R%d\n",p);
    fprintf(fptr, "INT 7\n");
    for(int i=0;i<5;i++){
        fprintf(fptr, "POP R%d\n",p);
    }
    freeReg(p);
    freeReg(index);
    return;
}

void solve_expr(struct tnode *head, FILE *fptr, int top_label, int bottom_label){
    if(head!=NULL){
        if(head->nodetype==1){
            read(head->left,fptr);
        }
        else if(head->nodetype==2){
            write(head->left,fptr);
        }
        else if(head->nodetype==3){
            solve_expr(head->left,fptr,top_label,bottom_label);
            solve_expr(head->right,fptr,top_label,bottom_label);
        }
        else if(head->nodetype==4){
            int x=codegen(head,fptr);
            freeReg(x);
        }
        else if(head->nodetype==5){
            int label_1=getLabel();
            int label_2=getLabel();
            int x=codegen(head->left,fptr);
            if(head->right->nodetype==6){
                fprintf(fptr, "JZ R%d, L%d\n",x,label_1);
                freeReg(x);
                solve_expr(head->right->left,fptr,top_label,bottom_label);
                fprintf(fptr, "JMP L%d\n",label_2);
                fprintf(fptr, "L%d:\n",label_1);
                solve_expr(head->right->right,fptr,top_label,bottom_label);
                fprintf(fptr, "L%d:\n",label_2);
            }
            else{
                fprintf(fptr, "JZ R%d, L%d\n",x,label_1);
                freeReg(x);
                solve_expr(head->right,fptr,top_label,bottom_label);
                fprintf(fptr, "L%d:\n",label_1);
            }
        }
        else if(head->nodetype==8){
            if(strcmp(head->varname,"break")==0){
                fprintf(fptr, "JMP L%d\n",bottom_label);
            }
            else{
                fprintf(fptr, "JMP L%d\n",top_label);
            }
        }
        else if(head->nodetype==9){
            solve_expr(head->left,fptr,top_label,bottom_label);
            // Calculating number of local variables
            int number_of_local_variables=0;
            struct lsymbol *temp=local_head;
            while(temp!=NULL){
                if(temp->local_variable){
                    number_of_local_variables++;
                }
                temp=temp->next;
            }
            int y=getReg();
            fprintf(fptr, "MOV R%d, BP\n",y);
            fprintf(fptr, "SUB R%d, 2\n",y);
            int x=codegen(head->right,fptr);
            if(head->right->Lgentry!=NULL||head->right->Ggentry!=NULL||head->right->nodetype==11){
                fprintf(fptr, "MOV R%d, [R%d]\n",x,x);
            }
            fprintf(fptr, "MOV [R%d], R%d\n",y,x);
            freeReg(x);
            freeReg(y);
            fprintf(fptr, "SUB SP, %d\n",number_of_local_variables);
            // Updation of Base Pointer
            fprintf(fptr, "POP BP\n");
            fprintf(fptr, "RET\n");
        }
        else if(head->nodetype==10){
            int x=codegen(head,fptr);
            freeReg(x);
        }
        else{
            int label_1=getLabel();
            int label_2=getLabel();
            fprintf(fptr, "L%d:\n",label_1);
            int x=codegen(head->left,fptr);
            fprintf(fptr, "JZ R%d, L%d\n",x,label_2);
            freeReg(x);
            solve_expr(head->right,fptr,label_1,label_2);
            fprintf(fptr, "JMP L%d\n",label_1);
            fprintf(fptr, "L%d:\n",label_2);
        }
    }
    return;
}

void generate_code(struct tnode *head, FILE *fptr, int function_label){
    fprintf(fptr, "F%d:\n",function_label);
    if(function_label==0){
        fprintf(fptr, "MOV SP, %d\n",binding_address-1);
    }
    fprintf(fptr, "PUSH BP\n");
    fprintf(fptr, "MOV BP, SP\n");
    fprintf(fptr, "ADD SP, %d\n",local_binding_address-1);
    solve_expr(head,fptr,-1,-1);
    return;
}

void check_exist_function(char *name){
    struct gsymbol *temp=global_head;
    while(temp!=NULL&&strcmp(temp->varname,name)!=0){
        temp=temp->next;
    }
    if(temp==NULL){
        printf("Error : The %s function has not been defined\n",name);
        exit(0);
    }
    struct plist *ptr1=temp->paramlist;
    struct lsymbol *ptr2=local_head;
    if(temp->type!=vartype[var_index]){
        printf("Error : The function %s declaration and definition conflict in return types\n",name);
        exit(0);
    }
    while(ptr1!=NULL&&ptr2!=NULL&&ptr2->local_variable==0&&strcmp(ptr1->varname,ptr2->varname)==0&&ptr1->type==ptr2->type){
        ptr1=ptr1->next;
        ptr2=ptr2->next;
    }
    if(ptr1!=NULL||ptr2!=NULL&&ptr2->local_variable==0){
        printf("Error : The %s function arguments are not declared correctly\n",name);
        exit(0);
    }
    return;
}

void class_check_exist_function(char *name){
    struct memberfunclist *temp=clookup(class_name)->vfuncptr;
    while(temp!=NULL&&strcmp(temp->varname,name)!=0){
        temp=temp->next;
    }
    if(temp==NULL){
        printf("Error : The %s function has not been defined\n",name);
        exit(0);
    }
    struct plist *ptr1=temp->paramlist;
    struct lsymbol *ptr2=local_head;
    if(temp->type!=vartype[var_index]){
        printf("Error : The function %s declaration and definition conflict in return types\n",name);
        exit(0);
    }
    while(ptr1!=NULL&&ptr2!=NULL&&ptr2->local_variable==0&&strcmp(ptr1->varname,ptr2->varname)==0&&ptr1->type==ptr2->type){
        ptr1=ptr1->next;
        ptr2=ptr2->next;
    }
    if(ptr1!=NULL||ptr2!=NULL&&ptr2->local_variable==0){
        printf("Error : The %s function arguments are not declared correctly\n",name);
        exit(0);
    }
    return;
}