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
void displayAssemblyInstructions (); //displays the instructions as assembly
void setAssemblyInstructions (); //sets the assembly instructions


int SP = 0; //stack pointer
int BP = 1; //base pointer
int PC = 0; //program counter
//create and initialize the stack
int stack[MAX_STACK_HEIGHT] = {0};


int main (int argc, char *argv[]) {

	//instruction IR; //instruction register
	int intCount = 0; //the number of ints contained within the file.
	int instructionCount = 0; //the number of instructions that must be fetched and executed.
	
	//initialize each register in the register file to 0
	int registerFile[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	intCount = countIntsInFile();
	instructionCount = intCount/4;
	
	//toBeExecuted = (instruction*)malloc(instructionCount*sizeof(instruction));
	//create the instruction register array
	instruction IR[instructionCount];
	
	//fetches the instruction from the input file
	fetchCycle(IR, instructionCount);
	//printIRArray(IR, instructionCount);
	
	//print out the header for the assembly that will be printed after each instruction is read in during the
	//fetch cycle.
	printf("Line OP R L M \n");
	
	char opCodeString[3]; //a string representing the instruction that corresponds to the opCode.
	displayAssemblyInstructions(IR, instructionCount);
	
	//printf("\n\nInitial Values PC BP SP \n");
	//executeCycle (IR, instructionCount, registerFile);
	
	//printf("intCount: %d \n", intCount);
	//printf("instructionCount: %d \n", instructionCount);
	
	return 0;
}


//Function Definitions:
void setAssemblyInstructions (instruction *IR, int instructionCount, char *opCodeString, int i) {
		//this switch fills the opCodeString
		switch (IR[i].op) {
			case 1: //opCodeString = "LIT";
					strcpy(opCodeString, "LIT");
					break;
			case 2: //opCodeString = "RTN";
					strcpy(opCodeString, "RTN");
					break;
			case 3: //opCodeString = "LOD";
					strcpy(opCodeString, "LOD");
					break;
			case 4: //opCodeString = "STO";
					strcpy(opCodeString, "STO");
					break;
			case 5: //opCodeString = "CAL";
					strcpy(opCodeString, "CAL");
					break;
			case 6: //opCodeString = "INC";
					strcpy(opCodeString, "INC");
					break;
			case 7: //opCodeString = "JMP";
					strcpy(opCodeString, "JMP");
					break;
			case 8: //opCodeString = "JPC";
					strcpy(opCodeString, "JPC");
					break;
			case 9: //opCodeString = "SIO";
					strcpy(opCodeString, "SIO");
					break;
			case 10: //opCodeString = "SIO";
					 strcpy(opCodeString, "SIO");
					 break;
			case 11: //opCodeString = "SIO";
					 strcpy(opCodeString, "SIO");
					 break;
			case 12: //opCodeString = "NEG";
					 strcpy(opCodeString, "NEG");
					 break;
			case 13: //opCodeString = "ADD";
					 strcpy(opCodeString, "ADD");
					 break;
			case 14: //opCodeString = "SUB";
					 strcpy(opCodeString, "SUB");
					 break;
			case 15: //opCodeString = "MUL";
					 strcpy(opCodeString, "MUL");
					 break;
			case 16: //opCodeString = "DIV";
					 strcpy(opCodeString, "DIV");
					 break;
			case 17: //opCodeString = "ODD";
					 strcpy(opCodeString, "ODD");
					 break;
			case 18: //opCodeString = "MOD";
					 strcpy(opCodeString, "MOD");
					 break;
			case 19: //opCodeString = "EQL";
					 strcpy(opCodeString, "EQL");
					 break;
			case 20: //opCodeString = "NEQ";
					 strcpy(opCodeString, "NEQ");
					 break;
			case 21: //opCodeString = "LSS";
					 strcpy(opCodeString, "LSS");
					 break;
			case 22: //opCodeString = "LEQ";
					 strcpy(opCodeString, "LEQ");
					 break;
			case 23: //opCodeString = "GTR";
					 strcpy(opCodeString, "GTR");
					 break;
			case 24: //opCodeString = "GEQ";
					 strcpy(opCodeString, "GEQ");
					 break;
		}
}

void displayAssemblyInstructions (instruction *IR, int instructionCount, char *opCodeString) {	
	int i = 0;
	for (i = 0; i < instructionCount; i++) {
		setAssemblyInstructions (IR, instructionCount, opCodeString, i);
		printf("%d %s %d %d %d \n", i, opCodeString, IR[i].r, IR[i].l, IR[i].m);
	}
}

void executeCycle (instruction *IR, int instructionCount, int *registerFile) {
	int halt = 0;
	int i = 0;
	for (i = 0; i < instructionCount; i++) {
		switch (IR[i].op) {
			//LIT R, 0, M
			case 1: registerFile[IR[i].r] = IR[i].m;
					break;
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
			case 19: registerFile[IR[i].r] = (registerFile[IR[i].l] == registerFile[IR[i].m]);
					 break;
			//NEQ
			case 20: registerFile[IR[i].r] = (registerFile[IR[i].l] != registerFile[IR[i].m]);
					 break;
			//LSS
			case 21: registerFile[IR[i].r] = (registerFile[IR[i].l] < registerFile[IR[i].m]);
					 break;
			//LEQ
			case 22: registerFile[IR[i].r] = (registerFile[IR[i].l] <= registerFile[IR[i].m]);
					 break;
			//GTR
			case 23:  registerFile[IR[i].r] = (registerFile[IR[i].l] > registerFile[IR[i].m]);
					 break;
			//GEO
			case 24: registerFile[IR[i].r] = (registerFile[IR[i].l] >= registerFile[IR[i].m]);
					 break;
		}
		
		//setAssemblyInstructions
		//printf("%d %s %d %d %d %d %d %d | %d %d %d %d %d %d \n", i, opCodeString, IR[i].r, IR[i].l, IR[i].m, PC, BP, SP, stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
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

int base (l, base) { //l stands for L in the instruction format.
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

