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
void STATEMENT(int lex_leve, lexeme *tokens, symbol *symbols);

instruction *generate_code(lexeme *tokens, symbol *symbols)
{
	code = malloc(500 * sizeof(instruction));
	code_index = 0;
  tok_index = 0;
  sym_index = 0;
  
  
  /*
  for(int i = 0; i < sizeof(symbols); i++)
  {
    printf("%d\n", i);
    printf("Symbol kind %d\n", symbols[i].kind);
    printf("Symbol name %s\n", symbols[i].name);
    printf("Symbol value %d\n", symbols[i].val);
    printf("Symbol level %d\n", symbols[i].level);
    printf("Symbol addr %d\n", symbols[i].addr);
    printf("Symbol mark %d\n\n", symbols[i].mark);
  
  }*/
  
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
  code[0].m= symbols[0].val * 3;

}

void BLOCK(int lex_level, lexeme *tokens, symbol *symbols)
{
  int proc_index = sym_index - 1;
  int numSym = 0;
  int numVars = 0;
  
  //ENDLESSLY LOOPING: MUST FIX!!!! DELETE THIS COMMENT LATER.
  printf("Jo: %d\n", proc_index);
  
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
  
  //printf("The code index is: %d\n", code_index);
  //printf("the proc_index is: %d\n", proc_index);
  
  code[proc_index].m = code_index;
  
  //printf("%d\n", code[0].m)
  
  emit(6, 0, 3 + numVars);
  STATEMENT(lex_level, tokens, symbols);
  

}

void STATEMENT(int lex_level, lexeme *tokens, symbol *symbols)
{
  if(tokens[tok_index].type == identsym)
  {
   
    
  
  }
  
  
  else if(tokens[tok_index].type == callsym)
  {
  
  
  }
  
  
  else if(tokens[tok_index].type == beginsym)
  {
  
  
  
  }
  
  
  else if(tokens[tok_index].type == writesym)
  {
  
  
  }
  
  
  else if(tokens[tok_index].type == readsym)
  {
  
  
  }
  
  else if(tokens[tok_index].type == ifsym)
  {
  
  
  }
  
  else if(tokens[tok_index].type == whilesym)
  {
  
  
  
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
