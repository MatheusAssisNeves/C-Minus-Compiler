#include "quad.h"
#include "qgen.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

/* the quadruple List */
QuadrupleList *quadList = NULL;
/* the quadruple pointer for the last quadruple */
QuadrupleList *quadLast = NULL;

/* call quadruple generator*/
QuadrupleList * quadGen( TreeNode * tree)
{
   Argument f2, f3, f4;
   Argument empty;
   empty.type = Empty;

   qGen(tree);
   quad_insert(HaltQ,empty,empty,empty);


   return quadList;

}

//Insert Quadruple in the list
void quad_insert( Operation op, Argument arg1,  Argument arg2,  Argument arg3)
{
	QuadrupleList * quad = (QuadrupleList*) malloc(sizeof(QuadrupleList));
	quad->op = op;
	quad->arg1 = arg1;
	quad->arg2 = arg2;
	quad->arg3 = arg3;
	quad->next = NULL;

	if(quadList == NULL)
	{
		quadList = quad;
		quadLast = quad;
	}
	else
	{
		quadLast->next = quad;
		quadLast = quad;
	}
}

//Print an Operation from a quadruple
void print_op(Operation op) 
{
	switch(op)
	{   case HaltQ: printf("HALT"); break;
		case GotoQ: printf("GOTO"); break;
		case LabQ: printf("LAB"); break;
		case LoadQ: printf("LOAD"); break;
		case StoreQ: printf("STORE"); break;
		case ReturnQ: printf("RET"); break;
		case EndQ: printf("END"); break;
		case AddQ: printf("ADD"); break;
		case SubQ: printf("SUB"); break;
		case MultQ: printf("MULT"); break;
		case DivQ: printf("DIV"); break;
		case EqualQ: printf("EQUAL"); break;
		case DifferQ: printf("DIFFER"); break;
		case LtQ: printf("LT"); break;
		case LeqQ: printf("LEQ"); break;
		case MtQ: printf("MT"); break;
		case MeqQ: printf("MEQ"); break;
		case AllocQ: printf("ALLOC"); break;
		case ArgQ: printf("ARG"); break;
		case CallQ: printf("CALL"); break;
		case ParamQ: printf("PARAM"); break;
		case FuncQ: printf("FUNC"); break;
		case InputQ: printf("INPUT"); break;
		case OutputQ: printf("OUTPUT"); break;
	}
}

//print an argument from a quadruple
void print_arg(Argument arg) 
{
	switch(arg.type)
	{
		case Empty: printf("-"); break;
		case Reg: printf("$t%d", arg.attr.val); break;
		case Lab: printf("L%d", arg.attr.val); break;
		case Val: printf("%d", arg.attr.val); break;
		case Name: printf("%s", arg.attr.name); break;
	}
	
}
// print list of quadruples
void quad_print()
{
  QuadrupleList *q = quadList;
		while(q!=NULL)
		{	printf("(");
			print_op(q->op);
			printf(",");
			print_arg(q->arg1);
			printf(",");
			print_arg(q->arg2);
			printf(",");
			print_arg(q->arg3);
			printf(")\n");
			q = q->next;
		}
}

