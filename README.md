Miles Friedman

P-Machine README

The pMachine.c file enclosed in the ZIP contains source code for my P-Machine. The PM/0 is a stack machine with two memory stores and
four registers.

How To Compile and Run: 
. Make sure that within the project file, a text file (mcode.txt) is contained. In this text file, The ISA should be written
  such that each line should contain a single instruction represented as whitespace-seperated 32-bit integer values that 
  represent: opcode l m
. Within the source code of the machine, (pMachine.c) on lines 53 (located the main function) and 306 (located in the countIntsInFile)
  function, replace the first parameter/argument of the fopen statement with the path of your mcode.txt file that contains the 
  ISA instructions. Next replace the first parameter/argument of the fopen statement with the path of your stackTrace.txt or output
  file, this is where the instructions and stack will be displayed as the program is run.
. Now simply run the source code file as you would any other program and your stackTrace.txt or output file should contain the correct 
  data!
  