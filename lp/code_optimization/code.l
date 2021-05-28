%{
    #include "code.hh"
%}

%%

"if"									{ strcpy(yylval.str,yytext); return IF; }
"goto"									{ strcpy(yylval.str,yytext); return GOTO; }
[a-zA-Z_][a-zA-Z0-9_]* 					{ strcpy(yylval.str,yytext); return VAR; }
[0-9]+                           		{ strcpy(yylval.str,yytext); return NUM; }
(<)|(>)|(<=)|(>=)|(==)|(!=)				{ strcpy(yylval.str,yytext); return RELOP; }
"+"|"-"|"*"|"/"                         { strcpy(yylval.str,yytext); return OP; }
"="										{ strcpy(yylval.str,yytext); return EQ; }
\n                                      { return NL; } 
[ \t]+								    ;

%%