/*
*	Compile in Win7 x64 + VisualStudio2010
*/


#include "stdafx.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXTOKENS	128
#define MAXTOKENLEN	64

enum TypeTag{
	IDENTIFIER,			/* the variable declared in the text */
	QUIALIFIER,			/* the additional attribute for variable */
	TYPE				
};

struct Token{
	char mType;					
	char mStr[MAXTOKENLEN];		/*	Description string	*/
} ;

Token tokenStack[MAXTOKENS];
Token currToken;

int top = -1;
#define pop		tokenStack[top--]
#define push(s)	tokenStack[++top] = s

TypeTag classify_string(){
	char *s = currToken.mStr;
	if(!strcmp(s,"const")){
		strcpy(s,"read-only");
		return QUIALIFIER;
	}

	if(!strcmp(s,"volatile"))	return QUIALIFIER;
	if(!strcmp(s,"void"))		return TYPE;
	if(!strcmp(s,"char"))		return TYPE;
	if(!strcmp(s,"signed"))		return TYPE;
	if(!strcmp(s,"unsigned"))	return TYPE;
	if(!strcmp(s,"short"))		return TYPE;
	if(!strcmp(s,"int"))		return TYPE;
	if(!strcmp(s,"long"))		return TYPE;
	if(!strcmp(s,"float"))		return TYPE;
	if(!strcmp(s,"double"))		return TYPE;
	if(!strcmp(s,"struct"))		return TYPE;
	if(!strcmp(s,"union"))		return TYPE;
	if(!strcmp(s,"enum"))		return TYPE;

	return IDENTIFIER;
}

void getToken(){
	char* p = currToken.mStr;

	/* skip the blank */
	while ((*p = getchar()) == ' '){};

	if (isalnum(*p)){
		/* copy string until find the next blank */
		while(isalnum( *++p = getchar()));

		ungetc(*p,stdin);
		*p = '\0';
		currToken.mType = classify_string();
		return ; 
	}

	/* the 'type' descriptor is follow with the '*' */
	if(*p == '*'){
		strcpy(currToken.mStr,"pointer to");
		currToken.mType = *p;
		return;	
	}

	currToken.mStr[1] = '\0';
	currToken.mType = *p;
	return;
}

/*	recursion till find the identifier */
void read_to_first_identifier(){
	getToken();
	while(currToken.mType != IDENTIFIER){
		push(currToken);
		getToken();
	}

	printf("%s is ",currToken.mStr);
	getToken();
}

void deal_with_arrays(){
	while(currToken.mType == '['){
		printf("array ");
		getToken();
		if(isdigit(currToken.mStr[0])){
			printf("(length is %d) ", atoi(currToken.mStr) - 1);
			getToken();
		}
		getToken();
		printf("of ");
	}
}

void deal_with_function_args(){
	while (currToken.mType != ')'){
		getToken();
	}
	getToken();
	printf("function, returning ");
}

void deal_with_pointers(){
	while(tokenStack[top].mType == '*'){
		printf("%s", pop.mStr);
	}
}

void deal_with_declarator(){
	//Check if array/func exist
	switch(currToken.mType){
	case '[': 
		deal_with_arrays();
		break;
	case '(':
		deal_with_function_args();
	}

	deal_with_pointers();
	
	while(top >= 0 ){
		if(tokenStack[top].mType == '('){
			pop;
			getToken();	
			deal_with_declarator();
		}else{
			printf("%s ",pop.mStr);
		}
	}
}

int _tmain(int argc, _TCHAR* argv[]){

	read_to_first_identifier();
	deal_with_declarator();
	printf("\n");

	getchar();
	return 0;
}

