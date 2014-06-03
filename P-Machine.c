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


//Function Prototypes:
int findBase (); //finds the base L levels down.
void fetchCycle (); //fetches the instruction from the input file and stores it into the IR
int countIntsInFile (); //counts the number of lines in the input file


int main (int argc, char *argv[]) {
	int SP = 0; //stack pointer
	int BP = 1; //base pointer
	int PC = 0; //program counter
	instruction IR; //instruction register
	int intCount = 0; //the number of ints contained within the file.
		
	//initialize each register in the register file to 0
	int registerFile[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	intCount = countIntsInFile();
	
	printf("intCount: %d", intCount);
	
	return 0;
}


//Function Definitions:
void fetchCycle () {
	
}

/*int findBase (l, base) { //l stands for L in the instruction format.
	int b1; //find base L levels down
	b1 = base;
	while (1 > 0) {
		b1 = stack[b1 + 1];
		l--;
	}
	return b1;
}*/

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

