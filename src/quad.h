#ifndef _QUAD_H_
#define _QUAD_H_

#include "globals.h"
/* number of temporary registrators */
#define tempRegSIZE 22

/* quadruple structures*/
//Quadruple Operations
typedef enum {HaltQ, GotoQ, LabQ, ReturnQ, EndQ, LoadQ, StoreQ, AddQ, SubQ, MultQ, DivQ,
			  EqualQ, DifferQ, LtQ, LeqQ, MtQ, MeqQ, AllocQ, ArgQ, CallQ, ParamQ, FuncQ, InputQ, OutputQ	} Operation;

//Quadruple Argument Type
typedef enum {Empty, Reg, Lab, Val, Name} AtrType;

//Quadruple Argument Structure
typedef struct{
	AtrType type;   //type of argument
	union{
		int val;    //stores a value (register number, label or imediate)
		char *name; //stores name (variable, function or scope)
	}attr;
}Argument;

//Quadruple Structure
typedef struct QuadrupleListRec
   { Operation op;
     Argument arg1, arg2, arg3;
     struct QuadrupleListRec * next;
   }QuadrupleList;

/* call quadruple generator*/
QuadrupleList * quadGen( TreeNode * tree);

//Insert Quadruple in the list
void quad_insert( Operation op, Argument arg1,  Argument arg2,  Argument arg3);

//Print quadruple list
void quad_print();



#endif
