/*
	Author: Michael Tsang, Jack Eason
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

instruction *code;
int code_index;
int tok_index;
int sym_index;
int proc_index;

void printcode();
void emit(int op, int L, int M);
void expression();
void PROGRAM(lexeme *tokens, symbol *symbols);
void BLOCK(int lex_level,lexeme *tokens, symbol *symbols);
void STATEMENT(int lex_level, lexeme *tokens, symbol *symbols);
void BEGIN(int lex_level, lexeme *tokens,symbol *symbols);
void CONDITION(int lex_level,lexeme *tokens, symbol *symbols);
void EXPRESSION(lexeme *tokens, symbol *symbols, int lex_level);
void EXPRESSION_PRIME(lexeme *tokens, symbol *symbols, int lex_level);
void TERM(lexeme *tokens, symbol *symbols, int lex_level);
void TERM_PRIME(lexeme *tokens, symbol *symbols, int lex_level);
void FACTOR(lexeme *tokens, symbol *symbols, int lex_level);

instruction *generate_code(lexeme *tokens, symbol *symbols)
{
	code = malloc(500 * sizeof(instruction));
	code_index = 0;
  tok_index = 0;
  sym_index = 0;
  
  
  PROGRAM(tokens, symbols);
  
	printcode();
	return code;
}


void PROGRAM(lexeme *tokens, symbol *symbols)
{
  //Unmark Main
  symbols[sym_index].mark = 0;
  sym_index++;
  
  emit (7, 0, 0);
  BLOCK(0,tokens, symbols);
  emit (9, 0, 3);
  code[0].m = symbols[0].val * 3;

}

void BLOCK(int lex_level, lexeme *tokens, symbol *symbols)
{
  int proc_index = sym_index - 1;
  int numSym = 0;
  int numVars = 0;

  //printf("Jo: %d\n", proc_index);
  
  //CONST_DECLARATION(tokens, symbols)
  if(tokens[tok_index].type == constsym)
  {
    do
    {
      tok_index++;
      numSym++;
      
      symbols[sym_index].mark = 0;
      
      sym_index++;
      tok_index = tok_index + 3;
    
    
    }while (tokens[tok_index].type == commasym);
     
     tok_index = tok_index + 1;
  
  }
  
  //VAR_DECLARATION
  if(tokens[tok_index].type == varsym)
  {
    do
    {
      tok_index++;
      numVars++;
      numSym++;
      
      //Unmark variable
      symbols[sym_index].mark = 0;
      sym_index++;
      
      tok_index++;
      
    
    }while (tokens[tok_index].type == commasym);
    
    tok_index++;
  
  
  }
  
  //PROCEDURE_DECLARATION
  if(tokens[tok_index].type == procsym)
  {
    do
    {
      tok_index++;
      numSym++;
      
      //Unmark procedure
      symbols[sym_index].mark = 0;
      sym_index++;
      
      BLOCK(lex_level + 1, tokens, symbols);
      
      emit(2, 0, 0);
      
      tok_index++;
    
    }while (tokens[tok_index].type == procsym);
  
  }
  
  printf("CODE INDEX: %d\n", code_index);
  symbols[proc_index].val = code_index;
  
  //printf("%d\n", code[0].m)
  
  emit(6, 0, 3 + numVars);
  STATEMENT(lex_level, tokens, symbols);
  

}

void STATEMENT(int lex_level, lexeme *tokens, symbol *symbols)
{
  if(tokens[tok_index].type == identsym)
  {
    //printf("trigger!\n");
    //printf("Lex Level: %d\n", lex_level);
    int sym_temp = 0; 
    
    //THIS FOR LOOP MAY NEED CORRECTION
    for(int i = 0; i < sizeof(symbols); i++)
    {
      //printf("Yolo\n");
      /*
      printf("%d\n", i);
      printf("Kind: %d\n", symbols[i].kind);
      printf("Comparison = %d\n", strcmp(tokens[tok_index].name, symbols[i].name));
      printf("Mark or unmarked? %d\n", symbols[i].mark);
      printf("\n");
      */
      
      if(symbols[i].kind == 2 && strcmp(tokens[tok_index].name, symbols[i].name) == 0 && symbols[i].mark == 0)
      {
        //printf("Yahaha\n");
        if(symbols[i].level == lex_level)
        {
          sym_temp == i;
          break;
        }
      }
      
      else
        continue;
      
    }
    
    tok_index++;
    EXPRESSION(tokens, symbols, lex_level);
    
    //Store
    emit(4, lex_level - symbols[sym_temp].level, symbols[sym_temp].addr * 3);
  
   }
    
  
  else if(tokens[tok_index].type == callsym)
  {
  
    printf("Hello");
  }
  
  
  else if(tokens[tok_index].type == beginsym)
  {
    //printf("yAH\n");
    tok_index++;
    STATEMENT(lex_level, tokens, symbols);
    BEGIN(lex_level, tokens, symbols);
    tok_index++;
    
    
  }
  
  else if(tokens[tok_index].type == writesym)
  {
    printf("hello");
  }
  
  
  else if(tokens[tok_index].type == readsym)
  {
    printf("hello");
  
  }
  
  else if(tokens[tok_index].type == ifsym)
  {
    printf("hello");
  
  }
  
  else if(tokens[tok_index].type == whilesym)
  {
  
    printf("hello");
  
  }

}


//BEGIN
void BEGIN(int lex_level, lexeme *tokens, symbol *symbols)
{
  if(tokens[tok_index].type == semicolonsym)
  {
    tok_index++;
    STATEMENT(lex_level, tokens, symbols);
    BEGIN(lex_level, tokens, symbols);
  }

}

//CONDITION
void CONDITION(int lex_level, lexeme *tokens, symbol *symbols)
{
  if(tokens[tok_index].type == oddsym)
  {
    tok_index++;
    EXPRESSION(tokens, symbols, lex_level);
    emit(2, 0, 6);
  }
  
  else
  {
    EXPRESSION(tokens, symbols, lex_level);
    int relop;
    
    relop == tokens[tok_index].type;
    tok_index++;
    
    EXPRESSION(tokens, symbols, lex_level);
    
    switch(relop)
    {
      case eqlsym:
        emit(2, 0, 8);
      
      case neqsym:
        emit(2, 0, 9);
      
      case lessym:
        emit(2, 0, 10);
      
      case leqsym:
        emit(2, 0, 11);
      
      case gtrsym:
        emit(2, 0, 12);
      
      case geqsym:
        emit(2, 0, 13);
      
    }
  }

}

//EXPRESSION
void EXPRESSION(lexeme *tokens, symbol *symbols, int lex_level)
{
  //printf("Here we go!\n");
  if(tokens[tok_index].type == plussym)
  {
    tok_index++;
    TERM(tokens, symbols, lex_level);
    EXPRESSION_PRIME(tokens, symbols, lex_level);
  
  }
  
  else if(tokens[tok_index].type == minussym)
  {
    tok_index++;
    emit(2, 0, 1);
    EXPRESSION_PRIME(tokens, symbols, lex_level);
  
  }
  
  else
  {
    //printf("On track\n");
    TERM(tokens, symbols, lex_level);
    EXPRESSION_PRIME(tokens, symbols, lex_level);
  
  }

}

//EXPRESSION_PRIME
void EXPRESSION_PRIME(lexeme *tokens, symbol *symbols, int lex_level)
{
  if(tokens[tok_index].type == plussym)
  {
    tok_index++;
    TERM(tokens, symbols, lex_level);
    emit(2, 0, 2);
    EXPRESSION_PRIME(tokens, symbols, lex_level);
  }

}

//TERM
void TERM(lexeme *tokens, symbol *symbols, int lex_level)
{
  //printf("Factoring\n");
  FACTOR(tokens, symbols, lex_level);
  TERM_PRIME(tokens, symbols, lex_level);
}

void TERM_PRIME(lexeme *tokens, symbol *symbols, int lex_level)
{
  if(tokens[tok_index].type == multsym)
  {
    tok_index++;
    FACTOR(tokens, symbols, lex_level);
    
    emit(2, 0, 4);
    TERM_PRIME(tokens, symbols, lex_level);
  }
  
  else if(tokens[tok_index].type == slashsym)
  {
    tok_index++;
    FACTOR(tokens, symbols, lex_level);
    
    emit(2, 0, 5);
    TERM_PRIME(tokens, symbols, lex_level);
    
  }
  
  else if(tokens[tok_index].type == modsym)
  {
    tok_index++;
    FACTOR(tokens, symbols, lex_level);
    
    emit(2, 0, 7);
    TERM_PRIME(tokens, symbols, lex_level);
    
  }

}

void FACTOR(lexeme *tokens, symbol *symbols, int lex_level)
{
  if(tokens[tok_index].type == identsym)
  {
    //printf("name: %s\n", tokens[tok_index].name);
    
    int temp = 0;
     for(int i = 0; i < sizeof(symbols); i++)
     {
       //printf("HiHi\n");
        if((symbols[i].kind == 2 || symbols[i].kind == 1) && strcmp(tokens[tok_index].name, symbols[i].name) == 0 && symbols[i].mark == 0)
        {
          /*
          printf("HOLD IT!\n");
          printf("LEVEL: %d\n", lex_level);
          printf("AT: %d\n", symbols[i].level);
          */
          if(symbols[i].level == lex_level)
          {
            //printf("Check me: %d\n", i);
            temp = i;
            break;
          }
          
        }
    }
    
    //printf("Index: %d\n", temp);
    //printf("Kind %d\n", symbols[temp].kind);
     if(symbols[temp].kind == 1)
     {
       //LIT
       emit(1, 0, symbols[temp].val);
     }
       
     else if(symbols[temp].kind == 2)
     {
       //LOD
       emit(3, lex_level - symbols[temp].level, symbols[temp].addr);
     }
     
     tok_index++;
    
  }
  
  else if(tokens[tok_index].type == numbersym)
  {
    emit(1, 0, tokens[tok_index].value);
    tok_index++;
  }
  
  else
  {
    tok_index++;
    EXPRESSION(tokens, symbols, lex_level);
    tok_index++;
  
  }
   
  
  
  
}


void emit(int op, int L, int M)
{
  if(code_index > 500)
  {
    printf("error EDIT ME!\n");
  }  
  
  code[code_index].opcode = op;
  code[code_index].l = L;
  code[code_index].m = M;
  code_index++;

}


void printcode()
{
	int i;
	printf("Line\tOP Code\tOP Name\tL\tM\n");
	for (i = 0; i < code_index; i++)
	{
		printf("%d\t", i);
		printf("%d\t", code[i].opcode);
		switch (code[i].opcode)
		{
			case 1:
				printf("LIT\t");
				break;
			case 2:
				switch (code[i].m)
				{
					case 0:
						printf("RTN\t");
						break;
					case 1:
						printf("NEG\t");
						break;
					case 2:
						printf("ADD\t");
						break;
					case 3:
						printf("SUB\t");
						break;
					case 4:
						printf("MUL\t");
						break;
					case 5:
						printf("DIV\t");
						break;
					case 6:
						printf("ODD\t");
						break;
					case 7:
						printf("MOD\t");
						break;
					case 8:
						printf("EQL\t");
						break;
					case 9:
						printf("NEQ\t");
						break;
					case 10:
						printf("LSS\t");
						break;
					case 11:
						printf("LEQ\t");
						break;
					case 12:
						printf("GTR\t");
						break;
					case 13:
						printf("GEQ\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			case 3:
				printf("LOD\t");
				break;
			case 4:
				printf("STO\t");
				break;
			case 5:
				printf("CAL\t");
				break;
			case 6:
				printf("INC\t");
				break;
			case 7:
				printf("JMP\t");
				break;
			case 8:
				printf("JPC\t");
				break;
			case 9:
				switch (code[i].m)
				{
					case 1:
						printf("WRT\t");
						break;
					case 2:
						printf("RED\t");
						break;
					case 3:
						printf("HAL\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			default:
				printf("err\t");
				break;
		}
		printf("%d\t%d\n", code[i].l, code[i].m);
	}
}
