#ifndef _CODE_H_
#define _CODE_H_

#include "quad.h"

//Instruct Address Type
typedef enum {Rtype, Itype, IAtype, Jtype, Ktype} InstrType;

//Instructions
typedef enum {addI = 1, addiI, subI, subiI, multI, multiI, divI, diviI ,
			     sltI = 16, sltiI, beqI, bneI, jrI, jumpI, jalI, loadI, storeI, inputI, outputI, haltI} Instruction;

//registers
typedef enum {$zero, $at, $v0, $a0, $a1, $a2, $a3, $t0, $t1, $t2, $t3, $t4, $t5, $t6, $t7, $t8, $t9, $t10, $t11, $t12, $t13, $t14, $t15, $t16, $t17, $t18, $t19, $t20, $t21, $gp, $sp, $ra} Registers;

// Instruction list
typedef struct InstructListRec
   { InstrType type; 
	 Instruction instr;
    Registers rs, rt, rd;
	 int imediate;
	 char * address;
    struct InstructListRec * next;
   }InstructList;

// Label list
typedef struct LabelListRec
   { InstrType type; 
	 char * name;
    int line;
    struct LabelListRec * next;
   }LabelList;

// Variables list
typedef struct ScopeListRec
   { char * name;
     int size;
     struct ScopeListRec * next;
     struct VarRec * varList;
   }ScopeList;

typedef struct VarRec
   { char * name;
     char * scope;
     int size;
     int memLoc; //mem location relative to scope
     struct VarRec * next;
   }Var;
//Start code generation
InstructList * codeGen( QuadrupleList * quadrupleList);

//return size of an scope
int sizeScope (char * name);

//return memory location of an variable relative to its scope
Var * varInfo (char * scope, char * name);

//Insert an R type Instruction in the Instruction List
void Rtype_insert( Instruction instruction, Registers rs, Registers rt, Registers rd);

//Insert an I (imediate) type Instruction in the Instruction List
void Itype_insert( Instruction instruction, Registers rs, Registers rt, int imediate);

//Insert an I (address) type Instruction in the Instruction List
void IAtype_insert( Instruction instr, Registers rs, Registers rt, char * address);

//Insert an J type Instruction in the Instruction List
void Jtype_insert( Instruction instruction, char * address);

//Insert an Label in the Instruction List
void label_insert(char * labelName);

//return label line number from a label name
int labelNumber(char * labelName);

//Print instruction list in the Instruction List
void instructList_print();

char * toString(const char *format, int a);

#endif