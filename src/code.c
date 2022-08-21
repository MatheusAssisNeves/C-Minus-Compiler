#include "code.h"
#include "cgen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DISPLAY_LINES TRUE

/* the quadruple List */
InstructList *instructList = NULL;
/* the instruct pointer for the last instruction */
InstructList *instructLast = NULL;

/* the label List */
LabelList *labelList = NULL;
/* the label pointer for the last label */
LabelList *labelLast = NULL;

/* line counter for the curent instreuction line */
static int instructLine = -1;

ScopeList *scopeList = NULL;

void createVarTable (QuadrupleList * quad);
void scope_print();



InstructList * codeGen( QuadrupleList * quadrupleList)
{
    createVarTable (quadrupleList);
    scope_print();
    
    int scopeSize = sizeScope("global");
    if (scopeSize != 0)
        Itype_insert(addiI, $gp, $sp, scopeSize - 1);

    Jtype_insert(jumpI, "main");
    
    cGen(quadrupleList);
   return instructList;
}

//create memory table
void createVarTable (QuadrupleList * quad)
{
    int scopeSize = 0;
    int memLoc = 0;
    ScopeList *scopeLast = NULL;
    ScopeList * scope = (ScopeList*) malloc(sizeof(ScopeList));

    Var *varFirst = NULL;
    Var *varLast = NULL;

    scope->name= "global";
    while(quad->op != FuncQ) //create globlal scope table first
    {
        Var * var = (Var*) malloc(sizeof(Var));
        if(quad->op == AllocQ)
        {   var->name = quad->arg1.attr.name;
            var->scope = "global";
            if(quad->arg3.type == Empty)
            {   scopeSize++;
                var->size = 1;
                var->memLoc = memLoc++;
            }
            else
            {   scopeSize += quad->arg3.attr.val;
                var->size = quad->arg3.attr.val;
                var->memLoc = memLoc;
                memLoc +=quad->arg3.attr.val;
 
            }
            var->next = NULL;
            if(varFirst == NULL)
	        {   varFirst = var;
		        varLast = var;
	        }
	        else
	        {   varLast->next = var;
		        varLast = var;
	        }
        }
        quad = quad->next;
    }
    scope->size = scopeSize;
    scope->next = NULL;
    scope->varList = varFirst;
    varFirst = NULL;
    scopeList = scope;
    scopeLast = scope;

	while(quad!=NULL) //create other scopes tables
	{
        ScopeList * scope = (ScopeList*) malloc(sizeof(ScopeList));
        Var * var = (Var*) malloc(sizeof(Var));
        switch (quad->op)
        {
            case FuncQ:
                scopeSize = 0;
                memLoc = 0;
                break;
            case ParamQ:
                scopeSize++;
                var->name = quad->arg2.attr.name;
                var->scope = quad->arg3.attr.name;
                var->size = 1;
                var->memLoc = memLoc++;
                var->next = NULL;
                if(varFirst == NULL)
	            {
		            varFirst = var;
		            varLast = var;
	            }
	            else
	            {
	            	varLast->next = var;
		            varLast = var;
	            }
                break;
            case AllocQ:
                var->name = quad->arg1.attr.name;
                var->scope = quad->arg2.attr.name;
                if(quad->arg3.type == Empty)
                {
                    scopeSize++;
                    var->size = 1;
                    var->memLoc = memLoc++;
                }
                else
                {
                    scopeSize += quad->arg3.attr.val;
                    var->size = quad->arg3.attr.val;
                    var->memLoc = memLoc + quad->arg3.attr.val - 1;
                    memLoc +=quad->arg3.attr.val;
                }
                var->next = NULL;
                if(varFirst == NULL)
	            {
		            varFirst = var;
		            varLast = var;
	            }
	            else
	            {
	            	varLast->next = var;
		            varLast = var;
	            }
                break;
            case EndQ:
                scope->name=quad->arg1.attr.name;
                scope->size = scopeSize;
                scope->next = NULL;
                scope->varList = varFirst;
                varFirst = NULL;

                scopeLast->next = scope;
                scopeLast = scope;

                break;
        }
        quad = quad->next;
    }
}

int sizeScope (char * name)
{
    ScopeList *scp = scopeList;
    while(scp!=NULL && (strcmp(name,scp->name) != 0))
    {	
        scp = scp->next;
    }
    if(scp==NULL)
        return -1;
    else
        return scp->size;
}

Var * varInfo (char * scope, char * name)
{
    ScopeList *scp = scopeList;
    Var *v;
	while(scp!=NULL && (strcmp(scope,scp->name) != 0))	
        scp = scp->next;

    v = scp->varList;
    while(v!=NULL && (strcmp(name,v->name) != 0))
         v = v->next;
         
    if(v==NULL) // if dint find the variable in any other scope search global scope
    {
        scp = scopeList;
        v = scp->varList;
        while(v!=NULL && (strcmp(name,v->name) != 0))
            v = v->next;
        return v;
    }
    else
        return v;
}


void Rtype_insert( Instruction instr, Registers rs, Registers rt, Registers rd)
{
	InstructList * instruct = (InstructList*) malloc(sizeof(InstructList));
    instruct->type = Rtype;
	instruct->instr = instr;
    instruct->rs = rs;
    instruct->rt = rt;
	instruct->rd = rd;
	instruct->next = NULL;

	if(instructList == NULL)
	{
		instructList = instruct;
		instructLast = instruct;
	}
	else
	{
		instructLast->next = instruct;
		instructLast = instruct;
	}
    instructLine++;
}

void Itype_insert( Instruction instr, Registers rs, Registers rt, int imediate)
{
	InstructList * instruct = (InstructList*) malloc(sizeof(InstructList));
    instruct->type = Itype;
	instruct->instr = instr;
    instruct->rs = rs;
    instruct->rt = rt;
	instruct->imediate = imediate;
	instruct->next = NULL;

	if(instructList == NULL)
	{
		instructList = instruct;
		instructLast = instruct;
	}
	else
	{
		instructLast->next = instruct;
		instructLast = instruct;
	}
    instructLine++;
}

void IAtype_insert( Instruction instr, Registers rs, Registers rt, char * address)
{
	InstructList * instruct = (InstructList*) malloc(sizeof(InstructList));
    instruct->type = IAtype;
	instruct->instr = instr;
    instruct->rs = rs;
    instruct->rt = rt;
	instruct->address = address;
	instruct->next = NULL;

	if(instructList == NULL)
	{
		instructList = instruct;
		instructLast = instruct;
	}
	else
	{
		instructLast->next = instruct;
		instructLast = instruct;
	}
    instructLine++;
}

void Jtype_insert( Instruction instr, char * address)
{
	InstructList * instruct = (InstructList*) malloc(sizeof(InstructList));
    instruct->type = Jtype;
	instruct->instr = instr;
	instruct->address = address;
	instruct->next = NULL;

	if(instructList == NULL)
	{
		instructList = instruct;
		instructLast = instruct;
	}
	else
	{
		instructLast->next = instruct;
		instructLast = instruct;
	}
    instructLine++;
}

void label_insert(char * labelName)
{
    LabelList * label = (LabelList*) malloc(sizeof(LabelList));
    label->name = labelName;
    label->line = instructLine + 1;
    label->next = NULL;

    if(labelList == NULL)
	{
		labelList = label;
		labelLast = label;
	}
	else
	{
		labelLast->next = label;
		labelLast = label;
	}
}

int labelNumber(char * labelName)
{
  LabelList *lab =  labelList;
  while ((lab != NULL) && (strcmp(labelName,lab->name) != 0))
    lab = lab->next;
  if (lab == NULL) return -1;
  else return lab->line;
}

//print instruction name
void instruct_print(Instruction instr) {
    switch (instr)
    {
        /*nopI, addI, addiI, subI, subiI, multI, multiI, divI, diviI, notI, andI, andiI, orI, oriI,
			     slI, srI, sltI, sltiI, beqI, bnqI, jI, jiI, loadI, loadiI, storeI, haltI, inI, outI*/    
        case loadI: fprintf(code,"lw"); break;
        case storeI: fprintf(code,"sw"); break;
        case addI: fprintf(code,"add"); break;
        case addiI: fprintf(code,"addi"); break;
        case subI: fprintf(code,"sub"); break; 
        case subiI: fprintf(code,"subi"); break; 
        case multI: fprintf(code,"mult"); break; 
        case multiI: fprintf(code,"multi"); break;
        case divI: fprintf(code,"div"); break;
        case diviI: fprintf(code,"divi"); break;
        case jumpI: fprintf(code,"j"); break;
        case jalI: fprintf(code,"jal"); break;
        case jrI: fprintf(code,"jr"); break;
        case bneI: fprintf(code,"bne"); break;
        case beqI: fprintf(code,"beq"); break;
        case sltI: fprintf(code,"slt"); break;
        case sltiI: fprintf(code,"slti"); break;
        case haltI: fprintf(code,"halt"); break;
        case inputI: fprintf(code,"in"); break;
        case outputI: fprintf(code,"out"); break;

        default:
            fprintf(code,"Unkown instruction (%d)",instr);
    }
}

//print register name
void reg_print (Registers reg)
{
    if ((reg >= $t0) && (reg < ($t0+tempRegSIZE)))
    {
        fprintf(code,"$t%d",reg-$t0);
    }else
    {
        switch (reg)
        {   
            case $zero: fprintf(code,"$0"); break;
            case $at: fprintf(code,"$at"); break;
            case $v0: fprintf(code,"$v0"); break;
            case $a0: fprintf(code,"$a0"); break;
            case $a1: fprintf(code,"$a1"); break;
            case $a2: fprintf(code,"$a2"); break;
            case $a3: fprintf(code,"$a3"); break;
            case $gp: fprintf(code,"$gp"); break;
            case $sp: fprintf(code,"$sp"); break;
            case $ra: fprintf(code,"$ra"); break;
        
            default:
                fprintf(code,"$ERROR[%d]",reg); break;
                printf("Unkown registrator (%d)",reg);
                break;
        }
    }
}

//write assembly code to .asm file
void instructList_print() {
  InstructList *instruct = instructList;
  LabelList *label = labelList;
  int line = 0;
  int skip = 0;
		while(instruct!=NULL && label!=NULL)
		{	
                while ((label->line == line) && (label->next != NULL) && (label!=NULL))
                {
                    if(DISPLAY_LINES == TRUE) fprintf(code,"    ");
                    fprintf(code,"%s",label->name);
                    fprintf(code,":\n",label->line);
                    label = label->next;
                }
                if (label->line == line && (label->next == NULL) && (label!=NULL)) {
                    if(DISPLAY_LINES == TRUE) fprintf(code,"    ");
                    fprintf(code,"%s",label->name);
                    fprintf(code,":\n",label->line);
                }
            if(DISPLAY_LINES == TRUE) fprintf(code,"%-4d",line);
            line++;


            switch (instruct->type)
            {
                case Rtype:
                    switch (instruct->instr)
                    {
                        case haltI:
                            fprintf(code,"    ");
                            instruct_print(instruct->instr);
                            fprintf(code,"\n");
                            break;
                        case jrI:
                        case inputI:
                        case outputI:
                            fprintf(code,"    ");
                            instruct_print(instruct->instr);
                            fprintf(code," ");
                            reg_print(instruct->rs);
                            fprintf(code,"\n");
                            break;
                        default:
                            fprintf(code,"    ");
                            instruct_print(instruct->instr);
                            fprintf(code," ");
                            reg_print(instruct->rd);
                            fprintf(code,", ");
                            reg_print(instruct->rs);
                            fprintf(code,", ");
                            reg_print(instruct->rt);
                            fprintf(code,"\n");
                            break;
                    }
                    break;
                case Itype:
                    switch (instruct->instr)
                    {
                        case loadI:
                        case storeI:
                            fprintf(code,"    ");
                            instruct_print(instruct->instr);
                            fprintf(code," ");
                            reg_print(instruct->rt);
                            fprintf(code,", ");
                            fprintf(code,"%d",instruct->imediate);
                            fprintf(code,"(");
                            reg_print(instruct->rs);
                            fprintf(code,")\n");
                            break;
                        default:
                            fprintf(code,"    ");
                            instruct_print(instruct->instr);
                            fprintf(code," ");
                            reg_print(instruct->rt);
                            fprintf(code,", ");
                            reg_print(instruct->rs);
                            fprintf(code,", ");
                            fprintf(code,"%d",instruct->imediate);
                            fprintf(code,"\n");
                            break;
                    }
                    break;
                case IAtype:
                    fprintf(code,"    ");
                    instruct_print(instruct->instr);
                    fprintf(code," ");
                    reg_print(instruct->rs);
                    fprintf(code,", ");
                    reg_print(instruct->rt);
                    fprintf(code,", ");
                    fprintf(code,"%s",instruct->address);
                    fprintf(code,"\n");
                    break;
                case Jtype:
                    fprintf(code,"    ");
                    instruct_print(instruct->instr);
                    fprintf(code," ");
                    fprintf(code,"%s",instruct->address);
                    fprintf(code,"\n");
                    break;
                default:
                    fprintf(code,"ERROR\n");
                    printf("Unkown instruction type (%d)",instruct->type);
                    break;
            }
			instruct = instruct->next;
		}
}

//print memory table
void scope_print()
{
  ScopeList *scp = scopeList;
  Var *v;
        printf("\nMemory table:\n\n");
		while(scp!=NULL)
		{	
            printf(">Scope: %s \n  Size:  %d\n",scp->name,scp->size);
            v = scp->varList;
            if (v != NULL)
            {
                printf("              ------------------------\n");
                printf("              Name       memLoc   Size\n");
                printf("              -------    ------   ----\n");
            }
            while(v!=NULL)
            {
                printf("              %-10s %-8d %-8d\n",v->name,v->memLoc,v->size);
                v = v->next;
            }
            printf("\n");
			scp = scp->next;
		}
}

//return string from a print format
char * toString(const char *format, int a){
  char *srtReturn = malloc(sizeof(char)*(10+1));
  sprintf(srtReturn, format, a);
  return srtReturn;
}
