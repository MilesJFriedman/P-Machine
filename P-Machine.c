// Miles Friedman


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int op; //opcode
	int l; //L
	int m; //M
	char opString[3]; //the string that corresponds with the opcode;
	int instructionNum; //the implicit instructions number (in the order it was given to you in mcode.txt)
}instruction;

//function prototypes:
int base (int l, int base);
int countIntsInFile ();
void executeStep (instruction IR, int *endFlag, FILE *outfile, int *newARFlag, int *whereToPlace);
void fetchStep (instruction *IRTrace, instruction *IR);
void fillOPStrings (instruction *IRTrace, int i);

//declare global variables:
int SP = -1;
int BP = 0;
int PC = 0;
int stack[2000] = {0};


int main (int argc, char* argv[]) {
	int intsInFile = 0; //the number of integers in the input file (mcode.txt)
	int instructionCount = 0; //the number of instructions in the input file
	int newARFlag = 0; //indicates whether a new activation record is made for purposes of adding a new bracket '|' to the stackTrace.txt file
	int endFlag = 0; //a flag used to tell the VM to terminate if RET is hit and SP = -1
	
	//The array below contains the index of each bracket in the order they appear within the stack. Because the stack can only be 2000 large, and each activation record has a minimum
	//of 3 indices, (static link, dynamic link, return address) the largest amount of brackets (or new activation records possible) would be 2000/3 or 667
	int whereToPlace[667] = {0};
	
	intsInFile = countIntsInFile();
	instructionCount = intsInFile / 3; //There are 3 integer values per instruction, use this fact to determine the instruction count
	
	instruction IR; //The instruction register will hold the next instruction to be executed
	instruction *IRpointer = &IR; //pointer to IR so that the fetch cycle may update the instruction register
	instruction IRTrace[instructionCount]; //this array will hold each instruction in the mcode.txt file in the order given
	
	printf("Output: ");
	
	//open the input and output files:
	FILE *infile;
	FILE *outfile;
	infile = fopen("C:\\Users\\Miles\\Desktop\\P-MACHINE\\HW1\\mcode.txt", "r");
	outfile = fopen("C:\\Users\\Miles\\Desktop\\P-MACHINE\\HW1\\stacktrace.txt", "w");
	
	//error check for the input file
	if (!infile)
		printf("Error, unable to read the file...");
	if (!outfile)
		printf("Error, unable to read the file...");
	
	//--------------------------------------------FILL INSTRUCTION TRACE-------------------------------------------------------------------//
	int i = 0; //looping variable
	for (i = 0; i < instructionCount; i++) {
		//Place all ISA from mcode.txt into the array of instructions (IRTrace):
		fscanf(infile, "%d %d %d", &IRTrace[i].op, &IRTrace[i].l, &IRTrace[i].m);
		
		//fill in the corresponding opString for each code and the current instruction number of the current instruction:
		fillOPStrings(IRTrace, i);
		IRTrace[i].instructionNum = i;
	}
	
	//--------------------------------------------PRINT INSTRUCTION TRACE-----------------------------------------------------------------//
	fprintf(outfile, "Line OP   L   M \n"); //print out the header for the assembly instruction trace that will be printed in stackTrace.txt:
	for (i = 0; i < instructionCount; i++) { //for each instruction, print out the current iteration of the instruction trace:
		fprintf(outfile, "%-4d %-4s %-3d %-3d \n", IRTrace[i].instructionNum, IRTrace[i].opString, IRTrace[i].l, IRTrace[i].m);
	}
	fprintf(outfile, "\n");
	
	fprintf(outfile, "                     pc    bp    sp    stack\n"); //print out the header for the stack trace that will be printed in stackTrace.txt:
	fprintf(outfile, "Initial Values       0     0     -1\n");
	while (endFlag == 0) {
		//---------------------------------------------------FETCH STEP-----------------------------------------------------------------------//
		fetchStep(IRTrace, IRpointer);
		PC++;
		//--------------------------------------------------EXECUTE STEP----------------------------------------------------------------------//
		executeStep(IR, &endFlag, outfile, &newARFlag, whereToPlace);
	}
	
	fclose(infile);
	fclose(outfile);
	return 0;
}

//function definitions:
void executeStep (instruction IR, int *endFlag, FILE *outfile, int *newARFlag, int *whereToPlace) {
	int j = 0; //looping variable
	//Specifies how the program handles the ISA from the input file
	switch (IR.op) {
		case 1: //opCodeString = "LIT";
				SP++;
				stack[SP] = IR.m;
				break;
		case 2: //handles OPR and its possible ISA;
				switch (IR.m) { 
					case 0: //operation RET 
							SP = BP - 1;
							if (SP == -1) {
								*endFlag = 1;
							}
							PC = stack[SP+3];
							BP = stack[SP+2];
							//because RET pops the current activation record off of the stack, there is one less bracket that needs to be displayed on the stack. Update the
							//whereToPlace array to reflect this by setting the furthest index that is not 0 back to 0.
							for (j = 667; j > 0; j--) {
								if (whereToPlace[j] != 0) {
									whereToPlace[j] == 0;
									break;
								}
							}
							break;
					case 1:	//operation NEG
							stack[SP] = -1*stack[SP];
							break;
					case 2: //operation ADD
							SP--;
							stack[SP] = stack[SP] + stack[SP+1];
							break;
					case 3: //operation SUB
							SP--;
							stack[SP] = stack[SP] - stack[SP+1];
							break;
					case 4: //operation MUL
							SP--;
							stack[SP] = stack[SP] * stack[SP+1];
							break;
					case 5: //operation DIV
							SP--;
							stack[SP] = stack[SP] / stack[SP+1];
							break;
					case 6: //operation ODD
							stack[SP] = stack[SP] % 2;
							break;
					case 7: //operation MOD
							SP--;
							stack[SP] = stack[SP] % stack[SP+1];
							break;
					case 8: //operation EQL
							SP--;
							if (stack[SP] == stack[SP+1])
								stack[SP] = 1;
							else
								stack[SP] = 0;
							break;
					case 9: //operation NEQ
							SP--;
							if (stack[SP] != stack[SP+1])
								stack[SP] = 1;
							else
								stack[SP] = 0;
							break;
					case 10: //operation LSS
							 SP--;
							 if (stack[SP] < stack[SP+1])
							 	stack[SP] = 1;
							 else
								stack[SP] = 0;
							 break;
					case 11: //operation LEQ
							 SP--;
							 if (stack[SP] <= stack[SP+1])
								stack[SP] = 1;
							 else
								stack[SP] = 0;
							 break;
					case 12: //operation GTR
							 SP--;
							 if (stack[SP] > stack[SP+1])
								stack[SP] = 1;
							 else
								stack[SP] = 0;
						   	 break;
					case 13: //operation GEQ
							 SP--;
							 if (stack[SP] >= stack[SP+1])
								stack[SP] = 1;
							 else
								stack[SP] = 0;
							 break;
				}
				break;
		case 3: //operation LOD
				SP++;
				stack[SP] = stack[base(IR.l, BP)+IR.m];
				break;
		case 4: //operation STO
				stack[base(IR.l, BP)+IR.m] = stack[SP];
				SP--;
				break;
		case 5: //operation CAL
				stack[SP+1] = base(IR.l, BP);
				stack[SP+2] = BP;
				stack[SP+3] = PC;
				BP = SP + 1;
				PC = IR.m;
				*newARFlag = 1;
				break;
		case 6: //operation INC
				if (*newARFlag == 1) { //This indicates that a CAL was executed last step and that a new activation record has been made
					//This loop is used to iterate through the whereToPlace array. If there is already j active brackets (or new ARs) in the stack, then j will increment until there
					//is an empty index (where 0 is held) so that it may mark the index where the next bracket should be placed in the stack.
					for (j = 0; j < 667; j++) { 
						if (whereToPlace[j] == 0) 
							whereToPlace[j] = SP + 1;
					}
					j = 0; //reset j to 0 so that below when the stack is printed, it will look to place the first bracket at the index specified at whereToPlace[0]. 
				}
				SP = SP + IR.m;
				break;
		case 7: //operation JMP
				PC = IR.m;
				break;
		case 8: //operation JPC
				if (stack[SP] == 0)
					PC = IR.m;
				SP--;
				break;
		case 9: //operation OUT
				printf("%d\n", stack[SP]);
				SP--;
				break;
		case 10: //operation IN
				 SP++;
				 printf("Enter a value to read in: ");
				 scanf("%d", stack[SP]);
				 printf("\n");
				 break;
	}
	
	fprintf(outfile, "%-4d %-4s %-4d %-5d ", IR.instructionNum, IR.opString, IR.l, IR.m);
	fprintf(outfile, "%-5d %-5d %-5d ", PC, BP, SP);
	int i; //looping variable
	for (i = 0; i <= SP; i++) {
		if (i == whereToPlace[j] && whereToPlace[j] != 0) { //This if statement will place a bracket in its correct location before a new AR during the time when INC is executed.
			fprintf(outfile, "| ");
			*newARFlag = 0; //it will then reset newARFlag to 0
			j++; //the j'th bracket has been placed in the stack at the correct location. As the for loop iterates it will look to place the next bracket there is one to place.
		}
		fprintf(outfile, "%d ", stack[i]);
	}
	fprintf(outfile, "\n");
}


void fetchStep (instruction *IRTrace, instruction *IR) {
	IR->op = IRTrace[PC].op;
	IR->l = IRTrace[PC].l;
	IR->m = IRTrace[PC].m;
	strcpy(IR->opString, IRTrace[PC].opString);
	IR->instructionNum = IRTrace[PC].instructionNum;
}

void fillOPStrings(instruction *IRTrace, int i) {
	//this switch fills the opCodeString
	switch (IRTrace[i].op) {
		case 1: //opCodeString = "LIT";
				strcpy(IRTrace[i].opString, "lit");
				break;
		case 2: //opCodeString = "OPR"
				strcpy(IRTrace[i].opString, "opr");
				break;
		case 3: //opCodeString = "LOD";
				strcpy(IRTrace[i].opString, "lod");
				break;
		case 4: //opCodeString = "STO";
				strcpy(IRTrace[i].opString, "sto");
				break;
		case 5: //opCodeString = "CAL";
				strcpy(IRTrace[i].opString, "cal");
				break;
		case 6: //opCodeString = "INC";
				strcpy(IRTrace[i].opString, "inc");
				break;
		case 7: //opCodeString = "JMP";
				strcpy(IRTrace[i].opString, "jmp");
				break;
		case 8: //opCodeString = "JPC";
				strcpy(IRTrace[i].opString, "jpc");
				break;
		case 9: //opCodeString = "OUT";
				strcpy(IRTrace[i].opString, "out");
				break;
		case 10: //opCodeString = "IN";
				 strcpy(IRTrace[i].opString, "in");
				 break;
	}
}

int countIntsInFile () {
	char integer[2];
	int c = 0; //holds the current character returned by getChar
	int numOfInts = 0;
	int index = 0;
	
	FILE* fp;
	fp = fopen ("C:\\Users\\Miles\\Desktop\\P-MACHINE\\HW1\\mcode.txt", "r");
	//fp = fopen(cmd.argv[1], "r");
	
	//check to make sure the file exists.
	if (fp == NULL) {
		printf("Error, unable to read the file...");
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

int base(int l, int base) {
	int b1; //find base L levels down
	b1 = base;
	while (l > 0) {
		b1 = stack[b1];
		l--;
	}
	return b1;
}

