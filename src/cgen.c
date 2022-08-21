#include "quad.h"
#include "code.h"
#include "cgen.h"
#include "symtab.h"
#include <stdio.h>

int $t = $t0; //position of first temporary registrator
int $a = $a0; //position of first argument registrator


char * inFun = "global";

/* arg register counter
   It is incremented each time a arg register is
   used and resets when a function call happens"
*/
static int argReg = -1;
/*  control argument registrators counter increment
    to avoid incrementing more than 4
*/
int incremtArg();

/* param register counter
   It is incremented each time a param register is
   used and resets when a function ends"
*/
static int paramReg = -1;
/*  control paramters registrators counter increment
    to avoid incrementing more than 4
*/
int incremtParam();

void cGen(QuadrupleList * quad)
{
/*HaltQ, IffQ, GotoQ, LabQ, ReturnQ, EndQ, LoadQ, StoreQ, AddQ, SubQ, MultQ, DivQ, EqualQ, DifferQ, LtQ, LeqQ, MtQ, MeqQ, 
AllocQ, ArgQ, CallQ, ParamQ, FuncQ*/
        int line = 0;
        while(quad!=NULL)
		{	
            Argument arg1 = quad->arg1;
            Argument arg2 = quad->arg2;
            Argument arg3 = quad->arg3;
            Var * var;
            int memLoc;
            int scopeSize;
            switch (quad->op)
            {
                case HaltQ:
                Rtype_insert(haltI,0,0,0);
                    break;
                case AddQ:
                    if(arg2.type == Reg && arg3.type == Reg)
                    {
                        Rtype_insert(addI, $t+arg2.attr.val, $t+arg3.attr.val, $t+arg1.attr.val);
                    }
                    else if (arg2.type == Reg && arg3.type == Val)
                    {
                        Itype_insert(addiI, $t+arg2.attr.val, $t+arg1.attr.val, arg3.attr.val);
                    }
                    else if (arg2.type == Val && arg3.type == Reg)
                    {
                        Itype_insert(addiI, $t+arg3.attr.val, $t+arg1.attr.val, arg2.attr.val);
                    }
                    else if (arg2.type == Val && arg3.type == Val)
                    {
                        Itype_insert(addiI, $zero, $t+arg1.attr.val, arg2.attr.val);
                        Itype_insert(addiI, $t+arg1.attr.val, $t+arg1.attr.val, arg3.attr.val);
                    }
                    break;

                case SubQ:
                    if(arg2.type == Reg && arg3.type == Reg)
                    {
                        Rtype_insert(subI, $t+arg2.attr.val, $t+arg3.attr.val, $t+arg1.attr.val);
                    }
                    else if (arg2.type == Reg && arg3.type == Val)
                    {
                        Itype_insert(subiI, $t+arg2.attr.val, $t+arg1.attr.val, arg3.attr.val);
                    }
                    else if (arg2.type == Val && arg3.type == Reg)
                    {
                        Itype_insert(addiI, $zero, $t+arg1.attr.val, arg2.attr.val);
                        Rtype_insert(subI, $t+arg1.attr.val, $t+arg3.attr.val, $t+arg1.attr.val);
                    }
                    else if (arg2.type == Val && arg3.type == Val)
                    {
                        Itype_insert(addiI, $zero, $t+arg1.attr.val, arg2.attr.val);
                        Itype_insert(subiI, $t+arg1.attr.val, $t+arg1.attr.val, arg3.attr.val);
                    }
                    break;

                case MultQ:
                    if(arg2.type == Reg && arg3.type == Reg)
                    {
                        Rtype_insert(multI, $t+arg2.attr.val, $t+arg3.attr.val, $t+arg1.attr.val);
                    }
                    else if (arg2.type == Reg && arg3.type == Val)
                    {
                        Itype_insert(multI, $t+arg2.attr.val, $t+arg1.attr.val, arg3.attr.val);
                    }
                    else if (arg2.type == Val && arg3.type == Reg)
                    {
                        Itype_insert(multI, $t+arg3.attr.val, $t+arg1.attr.val, arg2.attr.val);
                    }
                    else if (arg2.type == Val && arg3.type == Val)
                    {
                        Itype_insert(addiI, $zero, $t+arg1.attr.val, arg2.attr.val);
                        Itype_insert(multI, $t+arg1.attr.val, $t+arg1.attr.val, arg3.attr.val);
                    }
                    break;

                case DivQ:
                    if(arg2.type == Reg && arg3.type == Reg)
                    {
                        Rtype_insert(divI, $t+arg2.attr.val, $t+arg3.attr.val, $t+arg1.attr.val);
                    }
                    else if (arg2.type == Reg && arg3.type == Val)
                    {
                        Itype_insert(diviI, $t+arg2.attr.val, $t+arg1.attr.val, arg3.attr.val);
                    }
                    else if (arg2.type == Val && arg3.type == Reg)
                    {
                        Itype_insert(addiI, $zero, $t+arg1.attr.val, arg2.attr.val);
                        Rtype_insert(divI, $t+arg1.attr.val, $t+arg3.attr.val, $t+arg1.attr.val);
                    }
                    else if (arg2.type == Val && arg3.type == Val)
                    {
                        Itype_insert(addiI, $zero, $t+arg1.attr.val, arg2.attr.val);
                        Itype_insert(diviI, $t+arg1.attr.val, $t+arg1.attr.val, arg3.attr.val);
                    }
                    break;

                case EqualQ:
                    if (arg1.type == Reg && arg2.type == Val)
                    {
                        Itype_insert(addiI, $zero, $at, arg2.attr.val);
                        IAtype_insert(bneI, $t+arg1.attr.val, $at, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Reg && arg2.type == Reg)
                    {
                        IAtype_insert(bneI,$t+arg1.attr.val,$t+arg2.attr.val,toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Val && arg2.type == Reg)
                    {
                        Itype_insert(addiI, $zero, $at, arg1.attr.val);
                        IAtype_insert(bneI, $at, $t+arg2.attr.val, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Val && arg2.type == Val) 
                    {
                        if (arg1.attr.val != arg2.attr.val)
                        {
                            Jtype_insert(jumpI,toString("L%d",quad->arg3.attr.val));
                        }
                    }
                    break;

                case DifferQ:
                        if (arg1.type == Reg && arg2.type == Val)
                        {
                            Itype_insert(addiI, $zero, $at, arg2.attr.val);
                            IAtype_insert(beqI, $t+arg1.attr.val, $at, toString("L%d",quad->arg3.attr.val));
                        }
                        else if (arg1.type == Reg && arg2.type == Reg)
                        {
                            IAtype_insert(beqI,$t+arg1.attr.val,$t+arg2.attr.val,toString("L%d",quad->arg3.attr.val));
                        }
                        else if (arg1.type == Val && arg2.type == Reg)
                        {
                            Itype_insert(addiI, $zero, $at, arg1.attr.val);
                            IAtype_insert(beqI, $t+arg2.attr.val, $at, toString("L%d",quad->arg3.attr.val));
                        }
                        else if (arg1.type == Val && arg2.type == Val) 
                        {
                        if (arg1.attr.val == arg2.attr.val)
                        {
                            Jtype_insert(jumpI,toString("L%d",quad->arg3.attr.val));
                        }
                        }
                    break;

                case LtQ:
                    if (arg1.type == Reg && arg2.type == Val)
                    {
                        Itype_insert(sltiI, $t+arg1.attr.val, $at, arg2.attr.val);
                        IAtype_insert(beqI, $at, $zero, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Reg && arg2.type == Reg)
                    {
                        Rtype_insert(sltI, $t+arg1.attr.val, $t+arg2.attr.val, $at);
                        IAtype_insert(beqI, $at, $zero, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Val && arg2.type == Reg)
                    {
                        Itype_insert(sltiI, $t+arg2.attr.val, $at, arg1.attr.val+1);
                        IAtype_insert(bneI, $at, $zero, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Val && arg2.type == Val) 
                    {
                        if (arg1.attr.val >= arg2.attr.val)
                        {
                            Jtype_insert(jumpI,toString("L%d",quad->arg3.attr.val));
                        }
                    }
                    break;

                case LeqQ:
                    if (arg1.type == Reg && arg2.type == Val)
                    {
                        Itype_insert(sltiI, $t+arg1.attr.val, $at, arg2.attr.val+1);
                        IAtype_insert(beqI, $at, $zero, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Reg && arg2.type == Reg)
                    {
                        Rtype_insert(sltI, $t+arg1.attr.val, $t+arg2.attr.val, $at);
                        IAtype_insert(bneI, $at, $zero, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Val && arg2.type == Reg)
                    {
                        Itype_insert(sltiI, $t+arg2.attr.val, $at, arg1.attr.val);
                        IAtype_insert(bneI, $at, $zero, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Val && arg2.type == Val) 
                    {
                        if (arg1.attr.val > arg2.attr.val)
                        {
                            Jtype_insert(jumpI,toString("L%d",quad->arg3.attr.val));
                        }
                    }
                    break;

                case MtQ:
                    if (arg1.type == Reg && arg2.type == Val)
                    {
                        Itype_insert(sltiI, $t+arg1.attr.val, $at, arg2.attr.val+1);
                        IAtype_insert(bneI, $at, $zero, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Reg && arg2.type == Reg)
                    {
                        Rtype_insert(sltI, $t+arg1.attr.val, $t+arg2.attr.val, $at);
                        IAtype_insert(beqI, $at, $zero, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Val && arg2.type == Reg)
                    {
                        Itype_insert(sltiI, $t+arg2.attr.val, $at, arg1.attr.val);
                        IAtype_insert(beqI, $at, $zero, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Val && arg2.type == Val) 
                    {
                        if (arg1.attr.val <= arg2.attr.val)
                        {
                            Jtype_insert(jumpI,toString("L%d",quad->arg3.attr.val));
                        }
                    }
                    break;

                case MeqQ:
                    if (arg1.type == Reg && arg2.type == Val)//
                    {
                        Itype_insert(sltiI, $t+arg1.attr.val, $at, arg2.attr.val);
                        IAtype_insert(bneI, $at, $zero, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Reg && arg2.type == Reg)
                    {
                        Rtype_insert(sltI, $t+arg1.attr.val, $t+arg2.attr.val, $at);
                        IAtype_insert(bneI, $at, $zero, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Val && arg2.type == Reg)
                    {
                        Itype_insert(sltiI, $t+arg2.attr.val, $at, arg1.attr.val+1);
                        IAtype_insert(beqI, $at, $zero, toString("L%d",quad->arg3.attr.val));
                    }
                    else if (arg1.type == Val && arg2.type == Val) 
                    {
                        if (arg1.attr.val < arg2.attr.val)
                        {
                            Jtype_insert(jumpI,toString("L%d",quad->arg3.attr.val));
                        }
                    }
                    break;

                case GotoQ:
                    Jtype_insert(jumpI, toString("L%d",quad->arg1.attr.val));
                    break;

                case LabQ:
                    label_insert(toString("L%d",quad->arg1.attr.val));
                    break;

                case FuncQ:
                    argReg = -1;
                    inFun = arg2.attr.name;
                    label_insert(arg2.attr.name);
                    if(strcmp(inFun,"main") == 0)
                    {
                        scopeSize = sizeScope(inFun);
                        Itype_insert(addiI, $sp, $sp, scopeSize); //$sp=$sp+scopeSize
                    }else
                    {
                        scopeSize = sizeScope(inFun) + 1;
                        Itype_insert(addiI, $sp, $sp, scopeSize); //$sp=$sp+scopeSize+1
                        Itype_insert(storeI,$sp, $ra, -1*(scopeSize-1)); //mem[$sp - (scopeSize-1)] <- $ra
                    }
                    break;

                case ArgQ:
                    if (arg1.type == Reg)
                        Rtype_insert(addI, $t+arg1.attr.val, $zero, $a+incremtArg());
                    else if (arg1.type == Val)
                        Itype_insert(addiI, $zero, $a+incremtArg(), arg1.attr.val);
                    break;

                case CallQ:
                    argReg = -1;
                    Jtype_insert(jalI, arg2.attr.name);
                    Rtype_insert(addI, $v0, $zero, $t+arg1.attr.val);
                    break;

                case ParamQ:
                    var = varInfo(inFun,arg2.attr.name);
                    Itype_insert(storeI,$sp,$a+incremtParam(),-1*var->memLoc);
                    break;

                case ReturnQ:
                    if (arg1.type == Reg)
                        Rtype_insert(addI, $t+arg1.attr.val, $zero, $v0);
                    else if (arg1.type == Val)
                        Itype_insert(addiI, $zero, $v0, arg1.attr.val);
                    break;

                case EndQ:
                    paramReg = -1;
                    inFun = quad->arg1.attr.name;
                    if(strcmp(inFun,"main") == 0)
                    {
                        scopeSize = sizeScope(inFun);
                        Itype_insert(subiI, $sp, $sp, scopeSize); //$sp=$sp+scopeSize
                    }else
                    {
                        scopeSize = sizeScope(inFun) + 1;
                        Itype_insert(loadI,$sp, $ra, -1*(scopeSize-1)); //mem[$sp - (scopeSize-1)] -> $ra
                        Itype_insert(subiI, $sp, $sp, scopeSize); //$sp=$sp+scopeSize+1
                        Rtype_insert(jrI,$ra,0,0);
                    }
                    break;

                case AllocQ:
                    break;

                case InputQ:
                    Rtype_insert(inputI,$t+arg1.attr.val,0,0);
                    break;

                case OutputQ:
                    if (arg1.type == Reg)
                        Rtype_insert(outputI,$t+arg1.attr.val,0,0);
                    if (arg1.type == Val)
                    {
                        Itype_insert(addiI, $zero, $at, arg1.attr.val);
                        Rtype_insert(outputI,$at,0,0);
                    }
                    break;

                case LoadQ:
                    var = varInfo(inFun,arg2.attr.name);
                    if (strcmp(var->scope,"global") != 0) // its not a global variable
                    {
                        if (arg3.type == Empty)
                        {
                            if (var->size == 1)// not an array (pass value)
                                Itype_insert(loadI,$sp,$t+arg1.attr.val,-1*var->memLoc);

                            else //Array (pass address of array instead of value array[0] )
                                Itype_insert(addiI, $sp, $t+arg1.attr.val, -1*var->memLoc);
                        }
                        else                     //is an array
                        {   
                            if (var->size > 1) // not poiter (pass array[$at])
                            {
                                Rtype_insert(addI, $sp, $t+arg3.attr.val, $at); // $at = $sp + $t(position inside array)
                                Itype_insert(loadI,$at,$t+arg1.attr.val,-1*var->memLoc); // load array[$at] value on temporary
                            }
                            else               //accessing a pointer
                            {
                                Itype_insert(loadI,$sp,$at,-1*var->memLoc); //load array[0] adress in $at
                                Rtype_insert(addI, $at, $t+arg3.attr.val, $at); // $at = $at + $t(position inside array)
                                Itype_insert(loadI,$at,$t+arg1.attr.val,0);     // load array[$at] value on temporary
                            }
                        }
                    }
                    else // is global variable
                    {
                        if (arg3.type == Empty)
                        {
                            if (var->size == 1)// not an array (pass value)
                                Itype_insert(loadI,$gp,$t+arg1.attr.val,var->memLoc);
                            else //global array (get address instead of value array[0] )
                                Itype_insert(addiI, $gp, $t+arg1.attr.val, var->memLoc);
                        }
                        else                     //is an array
                        {
                            Rtype_insert(addI, $gp, $t+arg3.attr.val, $at); // $at = $gp + $t(position inside array)
                            Itype_insert(loadI,$at,$t+arg1.attr.val,var->memLoc); // load array[$at] value on temporary
                        }
                    }
                    break;

                case StoreQ:
                    var = varInfo(inFun,arg1.attr.name);
                    if (strcmp(var->scope,"global") != 0) // its not a global variable
                    {
                        if (arg3.type == Empty) // not an array
                        {
                            if (arg2.type == Reg)
                                Itype_insert(storeI,$sp,$t+arg2.attr.val,-1*var->memLoc);
                            if (arg2.type == Val)
                            {
                                Itype_insert(addiI, $zero, $at, arg2.attr.val);
                                Itype_insert(storeI,$sp,$at,-1*var->memLoc);
                            }
                        }
                        else                     //is an array
                        {
                            if (var->size > 1) // not poiter
                            {
                                Rtype_insert(addI, $sp, $t+arg3.attr.val, $at); // $at = $sp + $t(position inside array)
                                Itype_insert(storeI,$at,$t+arg2.attr.val,-1*var->memLoc); // load array[$at] value on temporary
                            }
                            else// is a pointer
                            {
                                Itype_insert(loadI,$sp,$at,-1*var->memLoc); //load array[0] adress in $at
                                Rtype_insert(addI, $at, $t+arg3.attr.val, $at); // $at = $at + $t(position inside array)
                                Itype_insert(storeI,$at,$t+arg2.attr.val,0); // store temporary to array[$at]

                            }
                        }
                    }
                    else // is global variable
                    {
                        if (arg3.type == Empty) // not an array
                        {
                            if (arg2.type == Reg)
                                Itype_insert(storeI,$gp,$t+arg2.attr.val,var->memLoc);
                            if (arg2.type == Val)
                            {
                                Itype_insert(addiI, $zero, $at, arg2.attr.val);
                                Itype_insert(storeI,$gp,$at,var->memLoc);
                            }
                        }
                        else                     //is an array
                        {
                            Rtype_insert(addI, $gp, $t+arg3.attr.val, $at);
                            Itype_insert(storeI,$at,$t+arg2.attr.val,var->memLoc);
                        }
                    }
                    break;

                default:
                    printf("Unkown Quadruple Operation (%d)",quad->op);
                    break;
            }
			quad = quad->next;
		}
}

int incremtArg() 
{
    if (argReg >= 3)
    {
        printf("ERROR: Can't pass more than 4 arguments to a function call.\n");
        argReg = 0;
        return argReg;
    }else
    {
        return ++argReg;
    }
}
int incremtParam() 
{
    if (paramReg >= 3)
    {
        printf("ERROR: A function can't receive more than 4 paramters.\n");
        paramReg = 0;
        return paramReg;
    }else
    {
        return ++paramReg;
    }
}