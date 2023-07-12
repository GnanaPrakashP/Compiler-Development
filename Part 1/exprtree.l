%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "exprtree.h"
    #include "y.tab.h"
%}

%%

"begin"                     {return begin;}
"end"                       {return end;}
"read"                      {return READ;}
"write"                     {return WRITE;}
"if"                        {return IF;}
"then"                      {return then;}
"else"                      {return Else;}
"endif"                     {return ENDIF;}
"while"                     {return WHILE;}
"do"                        {return DO;}
"endwhile"                  {return ENDWHILE;}

"break"                     {return BREAK;}
"continue"                  {return CONTINUE;}

"int"                       {return INT;}
"str"                       {return STR;}
"decl"                      {return DECL;}
"enddecl"                   {return ENDDECL;}

"main"                      {return MAIN;}
"return"                    {return RETURN;}

"AND"                       {return AND;}
"NOT"                       {return NOT;}
"OR"                        {return OR;}

"type"                      {return TYPE;}
"endtype"                   {return ENDTYPE;}

"null"                      {return null;}

"class"                     {return CLASS;}
"endclass"                  {return ENDCLASS;}
"self"                      {return SELF;}

"new"                       {return NEW;}

[\n]                        {error_line++;}
[ \t]                       {continue;}

[0-9]+                      {
                                yylval.ptr=makeLeafNode(yytext,inttype,-1);
                                return NUM;  
                            }
[A-Z|a-z|_]+[0-9]*          {
                                yylval.ptr=makeLeafNode(yytext,1,0);
                                return ID;
                            }
"+"                         {return PLUS;}
"-"                         {return MINUS;}
"*"                         {return MUL;}
"/"                         {return DIV;}
"%"                         {return MOD;}
[();=,&{}.]                 {return *yytext;}
"["                         {return *yytext;}
"]"                         {return *yytext;}
"<"                         {return LT;}
">"                         {return GT;}
"<="                        {return LE;}
">="                        {return GE;}
"!="                        {return NE;}
"=="                        {return EQ;}

\"(\\.|[^"\\])*\"           {
                                yylval.ptr=makeLeafNode(yytext,strtype,-2);
                                return CONST;
                            }

%%

int yywrap(){
    return 1;
}