#include <iostream>
#include <stdio.h>

void example1()
{
  int cVarA = 10;
  int cVarB = 20;
  int cVarC = 0;

  __asm__("add %%ebx, %%eax;"             // Assembly.  % symbols have to be escaped in extended assembly mode
          "mov %%eax, %%ecx;"
          : "=c"(cVarC)                   // Outputs.  The = means to set variable c = register value
          : "a"(cVarA), "b"(cVarB)        // Inputs
          : );                            // Clobbered registers.

  printf("a + b = c ==> %d + %d = %d\n", cVarA, cVarB, cVarC);
}

void example2()
{
  int a = 10;
  int b = 5;
  int c = 4;

  __asm__("add %2, %1 ;\n"        // Adds a to b, and stores in register where b was assigned
          "mov %1, %0 ;\n"        // Moves result to the register where c will be stored
          :"=r"(c)
          :"r"(a), "r"(b)
          : );

  printf("Example 2, a + b = c  ===>    %d + %d = %d\n", a, b, c);

}

void example3()
{
  int a = 10;

  __asm__("inc %%eax;"
          : "=a"(a)
          : "a" (a)
          : );

  printf("Example 3, variable A was 10 before calling assembly to increment it, now it is %d\n", a);

  __asm__("inc %0;"
          : "=r" (a)
          : "0" (a)
          : );

  printf("After another increment, the value is now %d\n", a);
}

void example4(int varA)
{
  printf("Example 4, A before = %d\n", varA);

  int* aPtr = &varA;
  int B = 0;

  __asm__ __volatile__ ("mov %1, %%rax;\n"
                        "mov (%%rax), %%ecx;\n"
                        "and $1, %%ecx;\n"
                        "jnz skip_increment;\n"
                        "incq (%%rax);\n"
                        "skip_increment:\n"
                        "mov %%ecx, %0;\n"
                        : "=r" (B)
                        : "r" (aPtr)
                        : "%eax", "%ecx", "memory", "cc" );

  printf("  After, A = %d, and B = %d\n", varA, B);
}

int main(int argc, char** argv)
{


  printf("Size of int = %lu\n", sizeof(int));
  printf("Size of int* = %lu\n", sizeof(int*));

  example1();
 
  example2();

  example3();

  example4(9);
  example4(14);

  return 0;
}
