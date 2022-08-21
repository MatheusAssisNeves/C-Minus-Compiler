#include "globals.h"
#include "quad.h"
#include "qgen.h"

/* tmpReg temp register counter
   It is incremented each time a temp register is
   used. ONLY increment using "incremtReg()"
*/
static int tmpReg = -1;
/* Increment register counter and return
   its value. Looping back when it reaches
   tempRegSIZE
*/
int incremtReg();
/* Return last used register
   when tmpReg = 0 returns tempRegSIZE
*/
int lastReg();
/* Label is the label counter
   It is incremented each time a label is
   used.
*/
static int Label = -1;
/* Store a label to the end of a function
   for use after calling a return
*/
int LabelEndFunc;
/* inFunct keeps track in which function 
   tree we are in.
*/
char * inFunct = "global";


/* Procedure genStmt generates quadruples at a statement node */
static void genStmt( TreeNode * tree)
{ TreeNode * p1, * p2, * p3;
  Argument f2, f3, f4;
  Argument empty;
  empty.type = Empty;
  int LabelPre;
  switch (tree->kind.stmt) {

      case IfK :
         p1 = tree->child[0];
         p2 = tree->child[1];
         p3 = tree->child[2];
         qGen(p1);
         LabelPre = Label;
         qGen(p2);
         if (p3 != NULL)
         {
            f2.type = Lab;
            f2.attr.val = ++LabelPre;
             quad_insert(GotoQ,f2,empty,empty);
             LabelPre--;
         }
         f2.type = Lab;
         f2.attr.val = LabelPre;
         quad_insert(LabQ,f2,empty,empty);

         if (p3 != NULL) 
         {
            qGen(p3);
            f2.type = Lab;
            f2.attr.val = ++LabelPre;
            quad_insert(LabQ,f2,empty,empty);
         }
         break;
      case WhileK: //ARRUMAR para casos dentro dele
         p1 = tree->child[0];
         p2 = tree->child[1];
         
         f2.type = Lab;
         f2.attr.val = ++Label;
         quad_insert(LabQ,f2,empty,empty);
         qGen(p1);

         LabelPre = Label;
         qGen(p2);

         f2.type = Lab;
         f2.attr.val = --LabelPre;
         quad_insert(GotoQ,f2,empty,empty);

         f2.attr.val = ++LabelPre;
         quad_insert(LabQ,f2,empty,empty);

         break;
      case AssignK:
         p1 = tree->child[0];
         p2 = tree->child[1];
         qGen(p2);
         switch (p2->kind.exp)
         {
         case IdK:
         case OpK:
         case CallK:
            f2.type = Reg;
            f2.attr.val = tmpReg;
            break;
         case ConstK: 
            f2.type = Val;
            f2.attr.val = p2->attr.val;
            break;
         }

         if (p1->child[0] == NULL) //not array
         {
            f3.type = Name;
            f3.attr.name = p1->attr.name;
            quad_insert(StoreQ,f3,f2,empty);
         }
         else //array
         {
            qGen(p1->child[0]);
            f3.type = Name;
            f3.attr.name = p1->attr.name;
            f4.type = Reg;
            f4.attr.val = tmpReg;
            quad_insert(StoreQ,f3,f2,f4);  
         }
         break;
      case ReturnK:
         p1 = tree->child[0];

         if (tree->child[0] == NULL)
         {
            quad_insert(ReturnQ,empty,empty,empty);
            f2.type = Lab;
            f2.attr.val = LabelEndFunc;
            quad_insert(GotoQ,f2,empty,empty);
            break;
         }

         switch (p1->kind.exp)
         {  case IdK:
            case OpK:
            case CallK:
               qGen(p1);
               f2.type = Reg;
               f2.attr.val = tmpReg;
               quad_insert(ReturnQ,f2,empty,empty);
               f2.type = Lab;
               f2.attr.val = LabelEndFunc;
               quad_insert(GotoQ,f2,empty,empty);
               break;
            case ConstK:
               f2.type = Val;
               f2.attr.val = p1->attr.val;
               quad_insert(ReturnQ,f2,empty,empty);
               f2.type = Lab;
               f2.attr.val = LabelEndFunc;
               quad_insert(GotoQ,f2,empty,empty);
               break;
         }
         break;
      default:
         printf("BUG: Unknown statement (%d)",tree->kind.stmt);
         break;
    }
    qGen(tree->sibling);
} /* genStmt */

/* Procedure genExp generates quadruples at an expression node */
static void genExp( TreeNode * tree)
{ int loc;
  TreeNode * p1, * p2;
  Argument f2, f3, f4, f5;
  Argument empty;
  empty.type = Empty;
  int i;
  switch (tree->kind.exp) {

    case OpK :
         p1 = tree->child[0];
         p2 = tree->child[1];
         qGen(p1);
         switch (p1->kind.exp)
         {
         case IdK:
         case OpK:
            f3.type = Reg;
            f3.attr.val = tmpReg;
            break;
         case ConstK: 
            f3.type = Val;
            f3.attr.val = p1->attr.val;
            break;
         }
         qGen(p2);
         switch (p2->kind.exp)
         {
         case IdK:
         case OpK:
         case CallK:
            f4.type = Reg;
            f4.attr.val = tmpReg;
            break;
         case ConstK: 
            f4.type = Val;
            f4.attr.val = p2->attr.val;
            break;
         }


         switch (tree->attr.opr) {
            case MAIS :
               f2.type = Reg;
               f2.attr.val = incremtReg();
               quad_insert(AddQ,f2,f3,f4);

               break;
            case SUB :
               f2.type = Reg;
               f2.attr.val = incremtReg();
               quad_insert(SubQ,f2,f3,f4);
               
               break;
            case MULT :
               f2.type = Reg;
               f2.attr.val = incremtReg();
               quad_insert(MultQ,f2,f3,f4);
               
               break;
            case DIV :
               f2.type = Reg;
               f2.attr.val = incremtReg();
               quad_insert(DivQ,f2,f3,f4);
               
               break;
            case IGUAL :
               

               break;
            case IGDAD :
               f5.type = Lab;
               f5.attr.val = ++Label;
               quad_insert(EqualQ,f3,f4,f5);
               break;
            case DIFER :
               f5.type = Lab;
               f5.attr.val = ++Label;
               quad_insert(DifferQ,f3,f4,f5);
               break;
            case MENOR :
               f5.type = Lab;
               f5.attr.val = ++Label;
               quad_insert(LtQ,f3,f4,f5);
               break;
            case MENIG :
               f5.type = Lab;
               f5.attr.val = ++Label;
               quad_insert(LeqQ,f3,f4,f5);
               break;
            case MAIOR :
               f5.type = Lab;
               f5.attr.val = ++Label;
               quad_insert(MtQ,f3,f4,f5);
               break;
            case MAIIG :
               f5.type = Lab;
               f5.attr.val = ++Label;
               quad_insert(MeqQ,f3,f4,f5);
               break;
            default:
               printf("BUG: Unknown operator");
               break;
         }
         break;

    case ConstK :
      //printf("ConstK\n");
    
      break;
    case IdK :
      p1 = tree->child[0];

      if (tree->child[0] == NULL)
      {  f2.type = Reg;
         f2.attr.val = incremtReg();
         f3.type = Name;
         f3.attr.name = tree->attr.name;
         quad_insert(LoadQ,f2,f3,empty);
         break;
      }
      switch (p1->kind.exp)
      {
         case IdK:
            f2.type = Reg;
            f3.type = Name;
            f2.attr.val = incremtReg();
            f3.attr.name = p1->attr.name;
            quad_insert(LoadQ,f2,f3,empty);

            f2.attr.val = incremtReg();
            f3.attr.name = tree->attr.name;
            f4.type = Reg;
            f4.attr.val = lastReg();
            quad_insert(LoadQ,f2,f3,f4);
            break;
         case ConstK:

            f2.type = Reg;
            f2.attr.val = incremtReg();
            f3.type = Val;
            f3.attr.val = p1->attr.val;
            f4.type = Reg;
            f4.attr.val = lastReg();
            quad_insert(LoadQ,f2,f3,f4);
            break;

      }
      break;
    case VarK :
      if (tree->child[0] != NULL)
      {
         f2.type = Name;
         f2.attr.name = tree->attr.name; 
         f3.type = Name;
         f3.attr.name = inFunct;
         f4.type = Val;
         f4.attr.val = tree->child[0]->attr.val; //não precisa, usar size

         quad_insert(AllocQ,f2,f3,f4);

      }else 
      {
         f2.type = Name;
         f2.attr.name = tree->attr.name;
         f3.type = Name;
         f3.attr.name = inFunct;
         quad_insert(AllocQ,f2,f3,empty);
      }
      break;
    case ParamK :
      break;
    case FuncK :
      inFunct = tree->attr.name;
      LabelEndFunc = ++Label;
      p1 = tree->child[0];
      qGen(p1);
      p2 = tree->child[1];
      qGen(p2);
      f2.type = Lab;
      f2.attr.val = LabelEndFunc;
      quad_insert(LabQ,f2,empty,empty);
      f2.type = Name;
      f2.attr.name = tree->attr.name;
      quad_insert(EndQ,f2,empty,empty);

      break;
    case CallK :
      p1 = tree->child[0];

   if ((strcmp(tree->attr.name,"input") != 0) && (strcmp(tree->attr.name,"output") != 0))
   {
      for (i = 0;p1 != NULL; i++)
      {  
         if(p1->kind.exp == ConstK) 
         {

            f2.type = Val;
            f2.attr.val = p1->attr.val;
            quad_insert(ArgQ,f2,empty,empty);
         }else{
            qGen(p1);
            f2.type = Reg;
            f2.attr.val = tmpReg;
            quad_insert(ArgQ,f2,empty,empty);
         }
         p1 = p1->sibling;
      }

      if (tree->type == voidDType)
      {
         f2 = empty;
      }else
      {
         f2.type = Reg;
         f2.attr.val = incremtReg();  
      }
      
      f3.type = Name;
      f3.attr.name = tree->attr.name;
      f4.type = Val;
      f4.attr.val = i;
      quad_insert(CallQ,f2,f3,f4); 
      qGen(tree->sibling);
   }else if(strcmp(tree->attr.name,"input") == 0)
   {
      f2.type = Reg;
      f2.attr.val = incremtReg();
      quad_insert(InputQ,f2,empty,empty);
      qGen(tree->sibling);

   }else if(strcmp(tree->attr.name,"output") == 0) //fazer para constante
   {
      qGen(p1);
      f2.type = Reg;
      f2.attr.val = tmpReg;
      quad_insert(OutputQ,f2,empty,empty);
      qGen(tree->sibling);
   }
      break;
    case TypeK :
      p1 = tree->child[0];
      if (tree->child[0] == NULL) break;

      switch (p1->kind.exp) //não precisa, usar DataType
      {
         case ParamK:
            f2.type = Name;
            f2.attr.name = tree->attr.name;
            f3.type = Name;
            f3.attr.name = p1->attr.name;
            f4.type = Name;
            f4.attr.name = inFunct;
            quad_insert(ParamQ,f2,f3,f4);
            qGen(p1);
            break;
         case FuncK:
            f2.type = Name;
            f2.attr.name = tree->attr.name;
            f3.type = Name;
            f3.attr.name = p1->attr.name;
            quad_insert(FuncQ,f2,f3,empty);
            qGen(p1);
            break;
         case IdK:
         case VarK:
            qGen(p1);
            break;

      }
      qGen(tree->sibling);
      break;   
    default:
      printf("BUG: Unknown expression (%d)",tree->kind.exp);
      break;
  }
}

/* Procedure qGen recursively quadruples by
 * tree traversal
 */
void qGen( TreeNode * tree)
{ if (tree != NULL)
  {
     switch (tree->nodekind)
     {
      case StmtK:
        genStmt(tree);
        break;
      case ExpK:
        genExp(tree);
        break;
      default:
        break;
    }
  }
}

int incremtReg() 
{
 if (tmpReg >= tempRegSIZE) tmpReg = -1;//loop temporary registers counter back to 0 when they reach 32
 return ++tmpReg;
}

int lastReg() 
{
 if (tmpReg == 0) return tempRegSIZE;//loop temporary registers counter back to 0 when they reach 32
 return tmpReg - 1;
}


