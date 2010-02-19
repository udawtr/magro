%{
#include <gc.h>
#include "parser_rdata.h"
%}

%option prefix="rdata"

EXPONENT	[eE][+-][0-9]+
BRACKET         [ \t]*\(

%s RDATA
%x COMMENT
%x SYSTEM

%%

","			return ',';
"["			return '[';
"]"			return ']';
"("			return '(';
")"			return ')';
"="                     return '=';
"<-"			return ARROW;
"*"			return '*';
":"                     return ':';
";"                     return ';';
"`"                     return '`';

"/*"                    BEGIN(COMMENT);
<COMMENT>[^*]*          /* Eat up anything that's not a '*'   */
<COMMENT>"*"+[^*/n]*    /* Eat up '*'s not followed by a '/'  */
<COMMENT>"*"+"/"        BEGIN(RDATA);

[ \t\r]+ /* Eat whitespace */
"#".*\n  /* Eat comments */
[\n]+            /* Eat newlines */

"-"?([0-9]+){EXPONENT}  {
  rdatalval.val = atof(rdatatext); return DOUBLE;
}
"-"?([0-9]+"."[0-9]*){EXPONENT}  {
  rdatalval.val = atof(rdatatext); return DOUBLE;
}
"-"?([0-9]+"."[0-9]*)  {
  rdatalval.val = atof(rdatatext); return DOUBLE;
}
"-"?("."[0-9]+){EXPONENT}  {
  rdatalval.val = atof(rdatatext); return DOUBLE;
}
"-"?("."[0-9]+)  {
  rdatalval.val = atof(rdatatext); return DOUBLE;
}
"-"?[0-9]+  {
  rdatalval.val = atof(rdatatext); return DOUBLE;
}
"-"?[0-9]+"L" {
  rdatalval.val = atof(rdatatext); return DOUBLE;
}


".Dim"                   return DIM;
"NA"		        return NA;
"NULL"		        return R_NULL;
"structure"/{BRACKET}	return STRUCTURE;
"list"/{BRACKET}		return LIST;
"c"/{BRACKET} 		return 'c';
"as.integer"/{BRACKET}   return ASINTEGER;

[a-zA-Z\.]+[a-zA-Z0-9\._\-\\\/]* { 
  	int len,sz;
	len = strlen(rdatatext);
	sz = sizeof(char) * (len+1);
	rdatalval.stringptr = (char*)malloc(sz);
  	memcpy(rdatalval.stringptr, rdatatext, len);
  	rdatalval.stringptr[sz-1] = '\0';
	return NAME;
}

\"[^\"]*\"  {
  	int len, sz;
	len = strlen(rdatatext);
	sz = sizeof(char) * (len+1-2);
	rdatalval.stringptr = (char*)malloc(len);
	memcpy(rdatalval.stringptr, ((char*)rdatatext)+1, sz);
	rdatalval.stringptr[sz-1] = '\0';
	return STRING;
}

\'[^\']*\' {
  	int len, sz;
	len = strlen(rdatatext);
	sz = sizeof(char) * (len+1-2);
	rdatalval.stringptr = (char*)malloc(len);
	memcpy(rdatalval.stringptr, ((char*)rdatatext)+1, sz);
	rdatalval.stringptr[sz-1] = '\0';
	return STRING;
}

<<EOF>> {
	yyterminate();
    return ENDDATA;
}
%%
