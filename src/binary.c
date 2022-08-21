#include "code.h"
#include <math.h>

#define SPACE_FIELDS TRUE

//convert positive decimal to binary based on a bit size
char * decToBin(int decimal, int bitSize)
{
    if (decimal < 0)
    {
        printf("ERROR: %d is negative and cant be converted to binary %d bits\n", decimal, bitSize);
        return "ERROR";
    }
    else if (decimal >= pow(2,bitSize))
    {
        printf("ERROR: %d is too large to convert to binary %d bits\n", decimal, bitSize);
        return "ERROR";
    }
    int i;
    char *bin = malloc (sizeof (char) * bitSize+1);
    for(i=0; i<bitSize; i++)
    {
        int mask = 1 << (bitSize - 1 - i);
        bin[i] = (decimal & mask) ? '1' : '0';
    }
    bin[bitSize] = '\0';
    return bin;
}
//convert decimal to two's complement based on a bit size
char * decToBin2(int decimal, int bitSize)
{
    if (decimal < (-1*pow(2,bitSize - 1)) || (decimal > pow(2,bitSize - 1) - 1))
    {
        printf("ERROR: %d is too large to convert to binary (two's complement) %d bits\n",decimal, bitSize);
        return "ERROR";
    }
    int i;
    char *bin = malloc (sizeof (char) * bitSize+1);
    for(i=0; i<bitSize; i++)
    {
        int mask = 1 << (bitSize - 1 - i);
        bin[i] = (decimal & mask) ? '1' : '0';
    }
    bin[bitSize] = '\0';
    return bin;
}

//write binary code to .bin file
void instructListToBinary(InstructList * instruct)
{
    int line = -1;
	while(instruct!=NULL)
	{	
        line++;
        fprintf(binary,decToBin(instruct->instr, 6));
        fprintf(binary,"_");
        switch (instruct->type)
        {
            case Rtype:
                fprintf(binary,decToBin(instruct->rs, 5));
                if (SPACE_FIELDS == TRUE) fprintf(binary,"_");
                fprintf(binary,decToBin(instruct->rt, 5));
                if (SPACE_FIELDS == TRUE) fprintf(binary,"_");
                fprintf(binary,decToBin(instruct->rd, 5));
                if (SPACE_FIELDS == TRUE) fprintf(binary,"_");
                fprintf(binary,decToBin(0, 11));
                break;
            case Itype:
                fprintf(binary,decToBin(instruct->rs, 5));
                if (SPACE_FIELDS == TRUE) fprintf(binary,"_");
                fprintf(binary,decToBin(instruct->rt, 5));
                if (SPACE_FIELDS == TRUE) fprintf(binary,"_");
                fprintf(binary,decToBin2(instruct->imediate, 16));
                break;
            case IAtype:
                fprintf(binary,decToBin(instruct->rs, 5));
                if (SPACE_FIELDS == TRUE) fprintf(binary,"_");
                fprintf(binary,decToBin(instruct->rt, 5));
                if (SPACE_FIELDS == TRUE) fprintf(binary,"_");
                fprintf(binary,decToBin2(labelNumber(instruct->address) - line, 16));
                break;
            case Jtype:
                fprintf(binary,decToBin(labelNumber(instruct->address), 26));
                break;
            default:
                fprintf(binary,"Unkown instruction type (%d)",instruct->type);
                break;
        }
        fprintf(binary,"\n");
		instruct = instruct->next;
	}
}
