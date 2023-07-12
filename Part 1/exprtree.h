struct memberfunclist{
    char *varname;
    struct typetable *type;
    struct plist *paramlist;
    int funcposition;
    int flabel;
    int function_definition_flag;
    struct memberfunclist *next;
};

struct classtable{
    char *varname;                       // Stores class name
    struct fieldlist *memberfield;       // Pointer to memberfield list
    struct memberfunclist *vfuncptr;     // Pointer to memberfunc list
    struct classtable *parentptr;        // Stores NULL(No inheritance)
    int class_index;                     // Stores class count
    int fieldcount;                      // Stores field count
    int methodcount;                     // Stores method count
    struct classtable *next;
};

struct typetable{
    char *varname;
    int size;
    struct fieldlist *fields;
    struct typetable *next;
};

struct fieldlist{
    char *varname;
    int fieldindex;
    struct typetable *type;
    struct classtable *ctype;
    struct fieldlist *next;
};

struct plist{
    char *varname;               // Name of Parameter
    struct typetable *type;      // Type of Parameter
    struct plist *next;
};

struct gsymbol{
    char *varname;               // Name of variable  
    int size;                    // Size of the variable
    int binding;                 // Variable address in the stack
    struct plist *paramlist;     // Pointer to Paramlist
    int flabel;                  // Function number
    struct typetable *type;
    struct classtable *ctype;
    int function_definition_flag;
    struct gsymbol *next;
};

struct lsymbol{
    char *varname;               // Name of local variable
    int binding;                 // Stack address
    int local_variable;
    struct typetable *type;
    struct lsymbol *next;
};

struct tnode{
    char *varname;                  // Stores the name of node
    int type;                       // Stores weather inttype or booltype
    int nodetype;                   // Information of non-leaf nodes
    struct gsymbol *Ggentry;        // Pointing to respective Gsymbol
    struct lsymbol *Lgentry;        // Pointing to respective Lsymbol
    struct classtable *ctype;
    struct tnode *left,*right;
    struct tnode *next;
};

/*

For nodetype,

-2 - String constant
-1 - Interger constant
0 - Leaf node variable
1 - Read
2 - Write
3 - Connector
4 - Operators (both assignment and boolean)
5 - IF
6 - IF Else
7 - While
8 - Break and Continue
9 - return
10 - function call
11 - '.'
12 - null
13 - new

For type,

0 - Logical Operators
1 - Assignment Operators
2 - Read, Write
3 - Return
4 - '.'
5 - null
6 - new
-1 - If, While
-2 - Connector, T/F

*/

#define booltype 0
#define inttype 1
#define strtype 2
#define dottype 4

extern int yyleng;
extern FILE *yyin;
extern int error_line;

void ginstall(char *name, int size);
void pinstall(char *name);
void linstall(char *name, int size, int local_variable);


struct tnode *makeLeafNode(char *name, int type, int nodetype);
struct tnode *makeOperatorNode(int type, char *name, int nodetype, struct tnode *left, struct tnode *right);

/*

vartype - 1 for integers
vartype - 2 for strings

flag - 1 paramlist goes to ginstall
flag - 0 paramlist goes to linstall

decl_flag would be 1 when ID is in declaration part
          would be 0 when ID is used inside functions


Leaf Node

Variables ---> Symbol Table ---> Typetable
Integer Constants ---> inttype
String Constants ---> strtype

*/