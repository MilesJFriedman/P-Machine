#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3


typedef struct {
	int op; //opcode
	int r; //register
	int l; //L
	int m; //M
} instruction;


//Declare a pointer to the array of instructions to be executed structs:
//instruction *toBeExecuted;


//Function Prototypes:
int findBase (); //finds the base L levels down.
void executeCycle (); //executes every direction in the IR array.
void fetchCycle (); //fetches the instruction from the input file and stores it into the IR.
void printIRArray (); //prints the instruction register array that holds each instruction register.
int countIntsInFile (); //counts the number of lines in the input file.


int main (int argc, char *argv[]) {
	int SP = 0; //stack pointer
	int BP = 1; //base pointer
	int PC = 0; //program counter
	//instruction IR; //instruction register
	int intCount = 0; //the number of ints contained within the file.
	int instructionCount = 0; //the number of instructions that must be fetched and executed.
		
	//create and initialize the stack
	int stack[MAX_STACK_HEIGHT] = {0};
	
	//initialize each register in the register file to 0
	int registerFile[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	intCount = countIntsInFile();
	instructionCount = intCount/4;
	
	//toBeExecuted = (instruction*)malloc(instructionCount*sizeof(instruction));
	//create the instruction register array
	instruction IR[instructionCount];
	
	//fetches the instruction from the input file
	fetchCycle(IR, instructionCount);
	printIRArray(IR, instructionCount);
	
	//printf("intCount: %d \n", intCount);
	//printf("instructionCount: %d \n", instructionCount);
	
	return 0;
}


//Function Definitions:
void executeCycle (instruction *IR, int instructionCount, registerFile, stack) {
	int halt = 0;
	int i = 0;
	for (i = 0; i < instructionCount; i++) {
		switch (IR[i].op) {
			//LIT R, 0, M
			case 1: registerFile[IR[i].r] = IR[i].m;
					break
			//RTN 0, 0, 0
			case 2: SP = BP - 1;
					BP = stack[SP + 3];
					PC = stack[SP + 4];
					break;
			//LOD R, L, M
			case 3: registerFile[IR[i].r] = stack[base(IR[i].l, BP) + IR[i].m];
					break;
			//STO R, L, M
			case 4: stack[base(IR[i].l, BP) + IR[i].m] = registerFile[IR[i].r];
					break;
			//CAL 0, L, M
			case 5: stack[SP + 1] = 0;						//space to return value
					stack[SP + 2] = base(IR[i].l, BP);		//static link (SL)
					stack[SP + 3] = BP;						//dynamic link (DL)
					stack[SP + 4] = PC;						//return address (RA)
					BP = SP + 1;
					PC = IR[i].m;
					break;
			//INC 0, 0, M
			case 6: SP = SP + IR[i].m;
					break;
			//JMP 0, 0, M
			case 7: PC = IR[i].m;
					break;
			//JPC R, 0, M
			case 8: if (registerFile[IR[i].r] == 0)
						PC = IR[i].m;
					break;
			//SIO R, 0, 1
			case 9: printf("%d", IR[i].r);
					break;
			//SIO R, 0, 2
			case 10: scanf("%d", registerFile[IR[i].r]);
					 break;
			//SIO R, 0, 3
			case 11: halt = 1;
					 break;
			//NEG
			case 12: registerFile[IR[i].r] = registerFile[IR[i].l];
					 break;
			//ADD
			case 13: registerFile[IR[i].r] = registerFile[IR[i].l] + registerFile[IR[i].m];
					 break;
			//SUB
			case 14: registerFile[IR[i].r] = registerFile[IR[i].l] - registerFile[IR[i].m];
					 break;
			//MUL
			case 15: registerFile[IR[i].r] = registerFile[IR[i].l] * registerFile[IR[i].m]; 
					 break;
			//DIV
			case 16: registerFile[IR[i].r] = registerFile[IR[i].l] / registerFile[IR[i].m];
					 break;
			//ODD
			case 17: registerFile[IR[i].r] = (registerFile[IR[i].r] % 2);
					 break;
			//MOD
			case 18: registerFile[IR[i].r] = registerFile[IR[i].l] % registerFile[IR[i].m];
					 break;
			//EQL
			case 19: registerFile[IR[i].r] = (registerFile[IR[i].l] == registerFile[IR[i].m];
					 break;
			//NEQ
			case 20: registerFile[IR[i].r] = (registerFile[IR[i].l] != registerFile[IR[i].m];
					 break;
			//LSS
			case 21: registerFile[IR[i].r] = (registerFile[IR[i].l] < registerFile[IR[i].m];
					 break;
			//LEQ
			case 22: registerFile[IR[i].r] = (registerFile[IR[i].l] <= registerFile[IR[i].m];
					 break;
			//GTR
			case 23:  registerFile[IR[i].r] = (registerFile[IR[i].l] > registerFile[IR[i].m];
					 break;
			//GEO
			case 24: registerFile[IR[i].r] = (registerFile[IR[i].l] >= registerFile[IR[i].m];
					 break;
		}
	}
}

void fetchCycle (instruction *IR, int instructionCount) {
	int i = 0;

	//loop through each instruction in the input file storing each into the instruction register and then
	//transfering to the toBeExecuted array.
	for (i = 0; i < instructionCount; i++) {
		scanf("%d", &IR[i].op);
		scanf("%d", &IR[i].r);
		scanf("%d", &IR[i].l);
		scanf("%d", &IR[i].m);
	}
}

void printIRArray (instruction *IR, int instructionCount) {
	int i = 0;
	
	for (i = 0; i < instructionCount; i++) {
		printf("%d ", IR[i].op);
		printf("%d ", IR[i].r);
		printf("%d ", IR[i].l);
		printf("%d ", IR[i].m);
		printf("\n");
	}
}

int findBase (l, base) {} //l stands for L in the instruction format
	int b1; //find base L levels down
	b1 = base;
	while (1 > 0) {
		b1 = stack[b1 + 1];
		l--;
	}
	return b1;
}

int countIntsInFile () {
	char integer[2];
	int c = 0; //holds the current character returned by getChar
	int numOfInts = 0;
	int index = 0;
	
	FILE* fp;
	fp = fopen ("C:/Users/Miles/Desktop/Program Files/C Programs/P-Machine/input.txt", "r");
	//fp = fopen(cmd.argv[1], "r");
	
	//check to make sure the file exists.
	if (fp == NULL) {
		printf("The file could not be opened, make sure the file path is written correctly.");
	} else {
		
		//printf("c: %d \n", c);
		c = getc(fp);
		//printf("c: %d \n", c);
		
		//while their is still a character to be read in the file.
		while (c != EOF) {
			//char integer[2] = {'\0', '\0'}; //an integer in the file will either have one or two digits
			integer[0] = '\0';
			integer[1] = '\0';
			index = 0;
			//c = getchar();
			
			//while c is a valid integer, add c to the integer string until whitespace is found.
			while (c < 58 && c > 47) {
				integer[index] = c;
				index++; //incriment index incase the integer in the file as two digits
				c = getc(fp);
				//printf("c: %d \n", c);
			}
		
			//if the integer array has been filled at all, meaning that a valid int has been found and has not
			//been acounted for, incriment the numOfInts counter.
			if (integer[0] != '\0')
				numOfInts++;
			
			c = getc(fp);
			//printf("c: %d \n", c);	
		}
		//printf(" test ");
	}

	fclose(fp);
	return numOfInts;
}

