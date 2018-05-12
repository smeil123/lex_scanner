#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define NO_KEYWOREDS 7
#define ID_LENGTH 12

FILE *fp;

enum tsymbol {
	tnull = -1, tnot, tnotequ,tmod,tmodAssign, tident, tnumber,
	tand, tlparen, trparen, tmul, tmulAssign, tplus,
	tinc, taddAssign, tcomma, tminus, tdec, tsubAssign,
	tdiv, tdivAssign, tsemicolon, tless, tlesse, tassign,
	tequal, tgreat, tgreate, tlbracket, trbracket, teof,
	tconst, telse, tif, tint, treturn, tvoid,
	twhile, tlbrace, tor, trbrace
};

struct tokenType {
	int number;
	union {
		char id[ID_LENGTH];
		int num;
	}value;
};

char *keyword[NO_KEYWOREDS] = {
	"const", "else", "if", "int", "return", "void", "while"
};

enum tsymbol tnum[NO_KEYWOREDS] = {
	tconst, telse, tif, tint, treturn, tvoid, twhile
};

void lexicalError(int n){
	printf(" *** Lexical Error :");
	switch(n){
		case 1: printf("an identifier length must be less than 12.\n");
			break;
		case 2:	printf("next character must be &.\n");
			break;
		case 3: printf("next character must be |.\n");
			break;
		case 4: printf("invalid character!!!\n");
			break;
	}
}

int superLetter(char ch){
	if(isalpha(ch) || ch == '_') return 1;
		else return 0;
}

int superLetterOrDigit(char ch){
	if (isalnum(ch) || ch=='_') return 1;
	else return 0;
}


int hexValue(char ch){
	switch(ch){
		case '0': case '1': case'2': case'3': case'4':
		case '5': case '6': case'7': case '8': case '9':
			return (ch-'0');
		case 'A': case'B': case'C': case 'D': case 'E': case 'F':
			return (ch - 'A' + 10);
		case 'a': case'b': case'c': case 'd': case 'e': case 'f':
			return (ch - 'a' + 10);
		default: return -1;	
	}
}

int getIntNum(char firstCharacter)
{
	int num = 0;
	int value;
	char ch;

	if(firstCharacter != '0'){
		ch = firstCharacter;
		do{
			num = 10*num + (int)(ch - '0');
			ch = fgetc(fp);
		}while(isdigit(ch));
	} else{
		ch = fgetc(fp);
		if((ch >= '0') && (ch <= '7'))
			do{
				num = 8*num + (int)(ch - '0');
				ch = fgetc(fp);
			}while((ch >= '0') || (ch <= '7'));
		else if((ch == 'X') || (ch == 'x')){
			while((value = hexValue(ch = fgetc(fp))) != -1)
				num = 16*num + value;
		}
		else num = 0;
	}
	ungetc(ch, fp);
	return num;
}

struct tokenType scanner(fp){
	struct tokenType token;
	int i, index;
	char ch, id[ID_LENGTH];

	token.number = tnull;

	do{
		while(isspace(ch = fgetc(fp)));
		if(superLetter(ch)){
			i = 0;
			do{
				if (i < ID_LENGTH) id[i++] = ch;
				ch = fgetc(fp);
			} while(superLetterOrDigit(ch));

			if (i >= ID_LENGTH)	lexicalError(1);
			id[i] = '\0';
			ungetc(ch, fp);

			for(index = 0; index < NO_KEYWOREDS; index++)
				if(!strcmp(id,keyword[index])) break;
			if(index < NO_KEYWOREDS)
				token.number = tnum[index];

			else{
				token.number = tident;
				strcpy(token.value.id,id);
			}
		}
		else if (isdigit(ch)){
			token.number = tnumber;
			token.value.num = getIntNum(ch);
		}
		else switch(ch){
			case '/':
				ch = fgetc(fp);
				if (ch == '*')
					do{
						while(ch!='*')	ch = fgetc(fp);
						ch = fgetc(fp);
					} while(ch!= '/');

				else if (ch =='/')
					while(ch = fgetc(fp) !='\n');
				else if (ch == '=') token.number = tdivAssign;
				else {
					token.number = tdiv;
					ungetc(ch,fp);
				}
				break;

			case '!':
				id[0] = ch;
				id[1] = '\0';
				ch = fgetc(fp);
				if (ch == '=') {token.number = tnotequ;	id[1] = ch; id[2] = '\0';}
				else {
					token.number = tnot;
					ungetc(ch,fp);
				}
				break;
			case '%':				
				id[0] = ch;
				id[1] = '\0';
				ch = fgetc(fp);
				if(ch == '='){
					token.number = tmodAssign;
					id[1] = ch; id[2] = '\0';
				}
				else {
					token.number = tmod;
					ungetc(ch,fp);
				}
				break;
			case '&':
				id[0] = ch;
				id[1] = '\0';
				ch = fgetc(fp);
				if (ch == '&'){
					token.number = tand;
					id[1] = ch; id[2] = '\0';
				}
				else {
					lexicalError(2);
					ungetc(ch,fp);
				}
				break;
			case '*':
				id[0] = ch;
				id[1] = '\0';
				ch = fgetc(fp);
				if(ch == '=') {
					token.number = tmulAssign;
					id[1] = ch; id[2] = '\0';
				}
				else{
					token.number = tmul;
					ungetc(ch,fp);
				}
				break;
			case '+':			
				id[0] = ch;
				id[1] = '\0';
				ch = fgetc(fp);
				if (ch == '+')	{
					token.number = tinc;					
					id[1] = ch; id[2] = '\0';
				}
				else if(ch == '='){
					token.number = taddAssign;					
					id[1] = ch; id[2] = '\0';
				}
				else{
					token.number = tplus;
					ungetc(ch,fp);
				}
				break;
			case '-':			
				id[0] = ch;
				id[1] = '\0';
				ch = fgetc(fp);
				if(ch == '-'){
					token.number = tdec;				
					id[1] = ch; id[2] = '\0';
				}
				else if(ch == '='){
					token.number = tsubAssign;				
					id[1] = ch; id[2] = '\0';
				}
				else {
					token.number = tminus;
					ungetc(ch,fp);
				}
				break;
			case '<':
				id[0] = ch;
				id[1] = '\0';
				ch = fgetc(fp);
				if(ch == '=') {
					token.number = tlesse;		
					id[1] = ch; id[2] = '\0';
				}
				else{
					token.number = tless;
					ungetc(ch,fp);
				}
				break;
			case '=':
				id[0] = ch;
				id[1] = '\0';
				ch = fgetc(fp);
				if (ch == '='){
					token.number = tequal;
					id[1] = ch; id[2] = '\0';
				}
				else{
					token.number = tassign;
					ungetc(ch,fp);
				}
				break;
			case '>':
				id[0] = ch;
				id[1] = '\0';
				ungetc(ch,fp);
				if (ch == '=')	{
					token.number = tgreate;					
					id[1] = ch; id[2] = '\0';
				}
				else{
					token.number = tgreat;
					ungetc(ch,fp);
				}
				break;
			case '|':
				id[0] = ch;
				id[1] = '\0';
				ungetc(ch,fp);
				if(ch == '|'){
					token.number = tor;
					id[0] = ch;
					id[1] = '\0';
				}
				else{
					id[0]='\0';
					lexicalError(3);
					ungetc(ch,fp);
				}
				break;
			case '(':	
				id[0] = ch;
				id[1] = '\0';
				token.number = tlparen;	break;
			case ')':	
				id[0] = ch;
				id[1] = '\0';
				token.number = trparen;	break;
			case ',':	
				id[0] = ch;
				id[1] = '\0';
				token.number = tcomma;	break;
			case ';':	
				id[0] = ch;
				id[1] = '\0';
				token.number = tsemicolon;	break;
			case '[':	
				id[0] = ch;
				id[1] = '\0';
				token.number = tlbracket;	break;
			case ']':	
				id[0] = ch;
				id[1] = '\0';
				token.number = trbracket;	break;
			case '{':	
				id[0] = ch;
				id[1] = '\0';
				token.number = tlbrace;	break;
			case '}':	
				id[0] = ch;
				id[1] = '\0';
				token.number = trbrace;	break;
			case EOF:	token.number = teof;	break;

			default: {
				printf("Current character : %c",ch);
				lexicalError(4);
				break;
			}


		}
	}while(token.number == tnull);

	if(token.number == tnumber){
		printf("Token ---> %d : (%d,%d)\n",token.value.num,token.number,token.value.num);
	}
	else if(token.number == tident){
		printf("Token ---> %s : (%d,%s)\n",token.value.id,token.number,token.value.id);
	}
	else if(token.number == teof){		
		printf("Token ---> End~ : (%d,0)\n",token.number);
	}
	else{
		printf("Token ---> %s : (%d,0)\n",id,token.number);	
	}

	return token;
}

void main(){

	fp = fopen("perfect.mc","r");

	struct tokenType token;
	char ch;

	do{
		token = scanner(fp);

	}while(token.number != teof);

	fclose(fp);
}
