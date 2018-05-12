%{
#include <stdio.h>
#include <stdlib.h>
enum tnumber{
	TEOF,TIDEN,TNUM, TASSIGN, TADD, TSEMI, TDOT, TBEGIN, TEND, TERROR,
	TCONST, TELSE, TIF, TINT, TRETURN, TVOID, TWHILE, TEQUAL, TNOTQU,
	TLESSES, TGREATE,TAND, TOR, TINC, TDEC, TADDASSIGN, TSUBASSiGN,
	TMULASSIGN,	TDIVASSIGN, TMODASSIGN , TMINUS, TLBRACE, TRBRACE,TLESS,TGREAT,
	TRPAREN, TLPAREN, TLBRACKET,TRBRACKET, TCOMMA
};
%}
L [a-zA-Z_]
D [0-9]
%%
begin		return(TBEGIN);
"end"			return(TEND);
":="|"="	return(TASSIGN);
"+"			return(TADD);
"-"			return(TMINUS);
"const"		return(TCONST);
"else"		return(TELSE);
"if"		return(TIF);
"int"		return(TINT);
"return"	return(TRETURN);
"void"		return(TVOID);
"while"		return(TWHILE);
"=="		return(TEQUAL);
"!="		return(TNOTQU);
"<"			return(TLESS);
">"			return(TGREAT);
"<="		return(TLESSES);
">="		return(TGREATE);
"&&"		return(TAND);
"||"		return(TOR);
"++"		return(TINC);
"--"		return(TDEC);
"+="		return(TADDASSIGN);
"-="		return(TSUBASSiGN);
"*="		return(TMULASSIGN);
"/="		return(TDIVASSIGN);
"%="		return(TMODASSIGN);
"}"			return(TLBRACE);
"{"			return(TRBRACE);
"("			return(TLPAREN);
")"			return(TRPAREN);
"["			return(TLBRACKET);
"]"			return(TRBRACKET);
";"			return(TSEMI);	
","			return(TCOMMA);
\.			return(TDOT);
{L}({L}|{D})*	return(TIDEN);
{D}+		return(TNUM);
[ \t\n]		;
"/*"([^*]|"*"+[^*/])*"*"*+"/"	;
"//".*						;
.			return(yytext[0]);

%%
int yywrap()
{	
	printf("End of Lex\n");
	return 1;
}

char * tokenValue(int tokenNum){
	switch(tokenNum){
		case TBEGIN : return "Begin";
		case TEND : return "End";
		case TIDEN : return yytext;
		case TASSIGN : return "=";
		case TADD: return "+";
		case TNUM : return yytext;
		case TMINUS : return "-";
		case TSEMI : return ";";
		case TDOT : return ".";
		case TCONST: return "const";
		case TELSE: return "else";
		case TIF: return "else";
		case TINT: return "int";
		case TRETURN: return "return";
		case TVOID: return "void";
		case TWHILE: return "while";
		case TEQUAL: return "==";
		case TNOTQU: return "!=";
		case TLESS : return "<";
		case TGREAT : return ">";
		case TLESSES: return "<=";
		case TAND : return "&&";
		case TGREATE: return ">=";		
		case TOR: return "||";
		case TDEC: return "--";
		case TINC: return "++";
		case TADDASSIGN: return "+=";
		case TSUBASSiGN: return "-=";
		case TMULASSIGN: return "*=";
		case TDIVASSIGN: return "/=";
		case TMODASSIGN: return "%=";
		case TLBRACE : return "{";
		case TRBRACE : return "}";
		case TRPAREN : return ")";
		case TLPAREN : return "(";
		case TLBRACKET : return "[";
		case TRBRACKET : return "]";
		case TCOMMA : return ",";
	}
	return 0;
}	

void main()
{
	enum tnumber tn;
	printf("Start of Lex\n");
	while((tn=yylex())!=TEOF){
		if(tn == TNUM){
			char *temp = tokenValue(tn);
			printf("Token ---> %s : (%d,%s)\n",temp,tn,temp);
		}
		else if(tn == TIDEN){
			char *temp = tokenValue(tn);
			printf("Token ---> %s : (%d,%s)\n",temp,tn,temp);
		}
		else{
			printf("Token ---> %s : (%d,0)\n",tokenValue(tn),tn);
		}

	}
	return ;
}