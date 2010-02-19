%{
#include <gc.h>
#include "parser_bugs.h"
%}

%option prefix="bugs"
%option yylineno

EXPONENT	[eE][+-][0-9]+
BRACKET		[ \t]*\(

%x COMMENT

%option yylineno

%%
"var"			return VAR;
"data"                  return DATA;
"model"			return MODEL;
"length"/{BRACKET}      return LENGTH;
"dim"/{BRACKET}         return DIM;
"for"/{BRACKET}        	return FOR;
"in"/{BRACKET}		return IN;
"in"                    return IN;
";"			return ';';
","			return ',';
":"			return ':';
"["			return '[';
"]"			return ']';
"("			return '(';
")"			return ')';
"{"			return '{';
"}"			return '}';
"<="                    return LE;
"<"                     return LT;
">="                    return GE;
">"                     return GT;
"&&"                    return AND;
"||"                    return OR;
"!="                    return NE;
"!"                     return NOT;
"=="                    return EQ;
"="			return '=';
"~"			return '~';
"<-"			return ARROW;
"+"			return '+';
"-"			return '-';
"^"                     return '^';
"**"                    return '^';
"*"			return '*';
"/"			return '/';

"%"+[^% \t\r\n]*"%" {
    /* Special operators, e.g. %*% for matrix multiplication */
	int len = strlen(bugstext);
	int sz = sizeof(char) * (len+1);
	bugslval.stringptr = malloc(sz);
	memcpy(bugslval.stringptr, bugstext, sz);
    bugslval.stringptr[sz-1] = '\0';
	return SPECIAL;
}

"/*"           BEGIN(COMMENT);
<COMMENT>[^*]*          /* Eat up anything that's not a '*'   */
<COMMENT>"*"+[^*/n]*    /* Eat up '*'s not followed by a '/'  */
<COMMENT>"*"+"/"        BEGIN(INITIAL);

[ \t\r\n]+              /* Eat whitespace */
"#".*\n                 /* Eat comments */

([0-9]+){EXPONENT} {
  bugslval.val = atof(bugstext); return DOUBLE;
}
([0-9]+"."[0-9]*){EXPONENT} {
  bugslval.val = atof(bugstext); return DOUBLE;
}
([0-9]+"."[0-9]*) {
  bugslval.val = atof(bugstext); return DOUBLE;
}
("."[0-9]+){EXPONENT} {
  bugslval.val = atof(bugstext); return DOUBLE;
}
("."[0-9]+) {
  bugslval.val = atof(bugstext); return DOUBLE;
}
([0-9]+) {
  bugslval.val = atof(bugstext); return DOUBLE;
}


([a-zA-Z]+)/{BRACKET} {
	int len, sz;
	len = strlen(bugstext);
	sz = sizeof(char) * (len+1);
	bugslval.stringptr = malloc(sz);
	memcpy(bugslval.stringptr, bugstext, sz);
	bugslval.stringptr[sz-1] = '\0';
  	return FUNC;
}

[a-zA-Z]+[a-zA-Z0-9\.]*	 {
	int len, sz;
	len = strlen(bugstext);
	sz = sizeof(char) * (len+1);
	bugslval.stringptr = malloc(sz);
	memcpy(bugslval.stringptr, bugstext, sz);
	bugslval.stringptr[sz-1] = '\0';
  	return NAME;
}

<<EOF>>	yyterminate();

%%

