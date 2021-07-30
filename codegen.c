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
  
  //PROGRAM ::= BLOCK
  BLOCK(0,tokens, symbols);
  emit (9, 0, 3);
  
  code[0].m = symbols[0].val * 3;

}

void BLOCK(int lex_level, lexeme *tokens, symbol *symbols)
{
  printf("BLOCK OCCURING\n");
  int proc_index = sym_index - 1;
  int numSym = 0;
  int numVars = 0;

  //printf("Jo: %d\n", proc_index);
  
  //CONST_DECLARATION(tokens, symbols)
  if(tokens[tok_index].type == constsym)
  {
    printf("CONST_DECLARATION\n");
    do
    {
      //printf("Const doing\n");
      tok_index++;
      //printf("One %d\n", tok_index);
      
      numSym++;
      
      symbols[sym_index].mark = 0;
      
      sym_index++;
      tok_index = tok_index + 3;
      //printf("Two %d\n", tok_index);
    
    
    }while (tokens[tok_index].type == commasym);
     
     tok_index = tok_index + 1;
     //printf("three %d\n", tok_index);
  
  }
  
  //VAR_DECLARATION
  if(tokens[tok_index].type == varsym)
  {
    printf("VAR_DECLARATION\n");
    do
    {
      //printf("Var doing\n");
      tok_index++;
      //printf("four %d\n", tok_index);
      
      numVars++;
      numSym++;
      
      //Unmark variable
      symbols[sym_index].mark = 0;
      sym_index++;
      
      tok_index++;
      //printf("five %d\n", tok_index);
      
    
    }while (tokens[tok_index].type == commasym);
    
    tok_index++;
    //printf("six %d\n", tok_index);
  
  
  }
  
  //PROCEDURE_DECLARATION
  if(tokens[tok_index].type == procsym)
  {
    printf("PRODECURE_DECLARATION\n");
    do
    {
      //printf("Doing Procedure\n");
      tok_index++;
      //printf("seven %d\n", tok_index);
      
      numSym++;
      
      //Unmark procedure
      symbols[sym_index].mark = 0;
      sym_index++;
      
      tok_index = tok_index + 2;
      //printf("eight %d\n", tok_index);
      
      BLOCK(lex_level + 1, tokens, symbols);
      
      emit(2, 0, 0);
      
      tok_index++;
      //printf("nine %d\n", tok_index);
    
    }while (tokens[tok_index].type == procsym);
  
  }
  
  //printf("CODE INDEX: %d\n", code_index);
  symbols[proc_index].val = code_index;
  
  //printf("%d\n", code[0].m)
  
  emit(6, 0, 3 + numVars);
  
  //Statement declaration
  STATEMENT(lex_level, tokens, symbols);
  

}

void STATEMENT(int lex_level, lexeme *tokens, symbol *symbols)
{
  printf("STATEMENT\n");
 
  if(tokens[tok_index].type == identsym)
  {
    printf("Statement Identity %s\n", tokens[tok_index].name);
    //printf("Lex Level: %d\n", lex_level);
    int sym_temp = 0; 
    
    //THIS FOR LOOP MAY NEED CORRECTION
    for(int i = 0; i < sizeof(symbols); i++)
    {
      
      if(symbols[i].kind == 2 && strcmp(tokens[tok_index].name, symbols[i].name) == 0 && symbols[i].mark == 0)
      {
        //printf("Yahaha\n");
        if(symbols[i].level == lex_level)
        {
          sym_temp = i;
          break;
        }
        
        else if(symbols[i].level < lex_level)
        {
          sym_temp = i;
        
        }
        
        else 
          continue;
        
      }
      
      else
        continue;
      
    }
    
    tok_index = tok_index + 2;
    //printf("ten %d\n", tok_index);
    
    EXPRESSION(tokens, symbols, lex_level);
    
    //Store
    emit(4, lex_level - symbols[sym_temp].level, symbols[sym_temp].addr);
  
   }
    
  //CALL
  else if(tokens[tok_index].type == callsym)
  {
    printf("CALL\n");
    int sym_temp = 0;
    tok_index++;
    
    for(int i = 0; i < sizeof(symbols); i++)
    {
      
      if(symbols[i].kind == 3 && strcmp(tokens[tok_index].name, symbols[i].name) == 0 && symbols[i].mark == 0)
      {
        if(symbols[i].level == lex_level)
        {
          sym_temp = i;
          break;
        }
        
        else if(symbols[i].level < lex_level)
        {
        
          sym_temp = i;
        }
        
        else 
          continue;
        
      }
      
      else
        continue;
    }
    
    tok_index++;
    //printf("Tweleve %d\n", tok_index);
    
    emit(5, lex_level - symbols[sym_temp].level, symbols[sym_temp].val * 3);
    
    
  }
  
  //BEGIN
  else if(tokens[tok_index].type == beginsym)
  {
    printf("Found begin\n");
    tok_index++;
    STATEMENT(lex_level, tokens, symbols);
    BEGIN(lex_level, tokens, symbols);
    tok_index++;
    
    printf("End of begin\n");
    
    
    
    //printf("thirteen %d\n",tok_index);
    
    
  }
  
  //WRITE
  else if(tokens[tok_index].type == writesym)
  {
    printf("Found write\n");
    if(tokens[tok_index].type == writesym)
    {
      tok_index++;
      //printf("Fourteen %d\n", tok_index);

      
      EXPRESSION(tokens, symbols, lex_level);
      emit(9, 0, 1);
    }
  }
  
  //READSYM
  else if(tokens[tok_index].type == readsym)
  {
    printf("Found Read\n");
    tok_index++;
     //printf("Fifteen %d\n", tok_index);

    
    int sym_temp = 0;
    for(int i = 0; i < sizeof(symbols); i++)
    {
      
      if(symbols[i].kind == 2 && strcmp(tokens[tok_index].name, symbols[i].name) == 0 && symbols[i].mark == 0)
      {
        if(symbols[i].level == lex_level)
        {
          sym_temp = i;
          break;
        }
        
        else if(symbols[i].level < lex_level)
        {
          sym_temp = i;
        }
        
        else
          continue;
      
      }
    
    }
    
    tok_index++;
     //printf("sixteen %d\n", tok_index);

    
    //RED
    emit(9, 0, 2);
    
    //STO
    emit(4, lex_level - symbols[sym_temp].level, symbols[sym_temp].addr);
  
  }
  
  ///IF 
  else if(tokens[tok_index].type == ifsym)
  {
    printf("Found an if statement\n");
    
    tok_index++;
    int jpc_index;
    int jmp_index;
    
    CONDITION(lex_level, tokens, symbols);
    
    jpc_index = code_index;
    emit(8, 0, 0);
    tok_index++;
    
    

    STATEMENT(lex_level, tokens, symbols);
    
    printf("TOKENtype %d\n", tokens[tok_index].type);
    printf("down here\n");
    
    if(tokens[tok_index].type == elsesym) 
    {
        tok_index++;
         //printf("eighteen %d\n", tok_index);

    
        jmp_index = code_index;
        emit(7, 0, 0);
        code[jpc_index].m = code_index * 3;
        STATEMENT(lex_level, tokens, symbols);
        code[jmp_index].m = code_index * 3;
    } 
    
    else
        symbols[jpc_index].mark = code_index * 3;
        
    printf("END OF IF\n");
    
    //printf("After if, tok_index is %d\n", tok_index);
  
  }
  
  else if(tokens[tok_index].type == whilesym)
  {
    printf("Found while\n");
    tok_index++;
     //printf("Nineteen %d\n", tok_index);

    
    
    int jmp_index = code_index;
    CONDITION(lex_level, tokens, symbols);
    tok_index++;
     //printf("twenty %d\n", tok_index);

    
    int jpc_index = code_index;
    
    //JPC
    emit(8, 0, 0);
    
    STATEMENT(lex_level, tokens, symbols);
    
    //JMP
    emit(7, 0, jmp_index *3);
    
    code[jpc_index].m = code_index;
  
  }

}


//BEGIN
void BEGIN(int lex_level, lexeme *tokens, symbol *symbols)
{
  printf("Second Begin!\n");
  //printf("Here %d\n", tok_index);
  if(tokens[tok_index].type == semicolonsym)
  {
    printf("Does it trigger?\n");
    tok_index++;
     //printf("Twentyone %d\n", tok_index);
    STATEMENT(lex_level, tokens, symbols);
    BEGIN(lex_level, tokens, symbols);
  }

}

//CONDITION
void CONDITION(int lex_level, lexeme *tokens, symbol *symbols)
{
  printf("CONDITION\n");
  if(tokens[tok_index].type == oddsym)
  {
    tok_index++;
     //printf("twentytwo %d\n", tok_index);

    
    EXPRESSION(tokens, symbols, lex_level);
    emit(2, 0, 6);
  }
  
  else
  {
    printf("COMPARING\n");
    EXPRESSION(tokens, symbols, lex_level);
    int relop;
    
    printf("relop %d\n", tokens[tok_index].type);
    
    
    relop = tokens[tok_index].type;
    tok_index++;
    //printf("twentythree %d\n", tok_index);
    
    printf("CIMP\n");
    EXPRESSION(tokens, symbols, lex_level);
    
    switch(relop)
    {
      case eqlsym:
        emit(2, 0, 8);
        break;
      
      case neqsym:
        emit(2, 0, 9);
        break;
      
      case lessym:
        emit(2, 0, 10);
        break;
      
      case leqsym:
        emit(2, 0, 11);
        break;
      
      case gtrsym:
        emit(2, 0, 12);
        break;
      
      case geqsym:
        emit(2, 0, 13);
        break;
      
    }
  }

}

//EXPRESSION
void EXPRESSION(lexeme *tokens, symbol *symbols, int lex_level)
{
  printf("EXPRESSION!\n");
  if(tokens[tok_index].type == plussym)
  {
    printf("Found Plus\n");
    tok_index++;
     //printf("twentyfour %d\n", tok_index);

    
    TERM(tokens, symbols, lex_level);
    EXPRESSION_PRIME(tokens, symbols, lex_level);
  
  }
  
  else if(tokens[tok_index].type == minussym)
  {
    printf("Found minus\n");
    tok_index++;
    
    printf("Token name %s\n", tokens[tok_index].name);
    
    //printf("twentyfive %d\n", tok_index);

    
    emit(2, 0, 1);
    EXPRESSION_PRIME(tokens, symbols, lex_level);
  
  }
  
  else
  {
    printf("NO PLUS OR MINUS BEFORE TERM\n");
    TERM(tokens, symbols, lex_level);
    EXPRESSION_PRIME(tokens, symbols, lex_level);
  
  }

}

//EXPRESSION_PRIME
void EXPRESSION_PRIME(lexeme *tokens, symbol *symbols, int lex_level)
{
  if(tokens[tok_index].type == plussym)
  {
    printf("Found Prime Plus\n");
    tok_index++;
     //printf("twentysix %d\n", tok_index);

    
    TERM(tokens, symbols, lex_level);
    
    //ADD
    emit(2, 0, 2);
    EXPRESSION_PRIME(tokens, symbols, lex_level);
  }
  
  if(tokens[tok_index].type == minussym)
  {
    printf("Found Prime Minus\n");
    tok_index++;
     //printf("twentyseven %d\n", tok_index);

    TERM(tokens, symbols, lex_level);
    
    //SUB
    emit(2, 0, 3);
    EXPRESSION_PRIME(tokens, symbols, lex_level);
  
  }

}

//TERM
void TERM(lexeme *tokens, symbol *symbols, int lex_level)
{
  printf("TERM\n");
  FACTOR(tokens, symbols, lex_level);
  
  printf("Activating term prime from term\n");
  TERM_PRIME(tokens, symbols, lex_level);

}

void TERM_PRIME(lexeme *tokens, symbol *symbols, int lex_level)
{
  //printf("Activating term prime\n");
  if(tokens[tok_index].type == multsym)
  {
    printf("Found termP multsym\n");
    tok_index++;
     //printf("twentyeight %d\n", tok_index);

    
    FACTOR(tokens, symbols, lex_level);
    
    emit(2, 0, 4);
    TERM_PRIME(tokens, symbols, lex_level);
  }
  
  else if(tokens[tok_index].type == slashsym)
  {
    printf("Found termP slashsym");
    tok_index++;
     //printf("twentynine %d\n", tok_index);

    
    FACTOR(tokens, symbols, lex_level);
    
    emit(2, 0, 5);
    TERM_PRIME(tokens, symbols, lex_level);
    
  }
  
  else if(tokens[tok_index].type == modsym)
  {
    printf("Found MOD\n");
    tok_index++;
     //printf("thirty %d\n", tok_index);

    
    FACTOR(tokens, symbols, lex_level);
    
    emit(2, 0, 7);
    TERM_PRIME(tokens, symbols, lex_level);
    
  }

}

void FACTOR(lexeme *tokens, symbol *symbols, int lex_level)
{
  printf("FACTOR\n");
  
  printf("The token index is %d\n", tok_index);
  printf("type: %d\n", tokens[tok_index].type);
  
  if(tokens[tok_index].type == identsym)
  {
    
    printf("IDENTI %s\n", tokens[tok_index].name);
    
    int temp = 0;
     for(int i = 0; i < sizeof(symbols); i++)
     {
        if((symbols[i].kind == 2 || symbols[i].kind == 1) && strcmp(tokens[tok_index].name, symbols[i].name) == 0 && symbols[i].mark == 0)
        {
          if(symbols[i].level == lex_level)
          {
            //printf("Check me: %d\n", i);
            temp = i;
            break;
          }
          
          else if(symbols[i].level < lex_level)
          {
            temp = i;
          
          }
          
          else
            continue;
        }
        
        else
          continue;
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
      //printf("thirtyone %d\n", tok_index);

    
  }
  
  else if(tokens[tok_index].type == numbersym)
  {
    printf("IT'S A NUMBER\n");
    printf("And it's %d\n", tokens[tok_index].value);
    emit(1, 0, tokens[tok_index].value);
    tok_index++;
    
    printf("Kind: %d\n", tokens[tok_index].type);
    
    printf("thirtytwo %d\n", tok_index);

  }
  
  else
  {
    printf("Something else!!!!!!!!!!!!!\n");
    tok_index++;
     //printf("thirtythree %d\n", tok_index);

    
    EXPRESSION(tokens, symbols, lex_level);
    printf("After SOMETHINGEXPRESSION\n");
    
    //May need to uncomment down below.
    tok_index++;
    //printf("thirtyfour %d\n", tok_index);

  
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
