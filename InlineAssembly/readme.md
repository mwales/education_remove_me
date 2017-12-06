# Inline Assembly with GCC Notes

Use __asm__ or asm as "function call" preceding inline assembly.  Both are valid and mean the same
thing, they are both defined incase there is a name collision with one of them.

Example:

```asm
asm("nop");
```

You pretty much have to use AT&T syntax for the assembly.  One stackoverflow post suggested that
you can pass gcc the -masm=intel option and it will then parse the assembly with the intel
assembler instead, but then I later read a post that indicated the substition system that GCC will
use always assumes AT&T syntax, which will provide issues later.

## Extended Inline Assembly

You normally will need to be able to pass arguments in and out of your assembly code, so you will
need to use the extended inline assembly mode.

```c
asm ( "assembly_statements" 
     : output_registers 
     : input_registers 
     : clobbered_registers);
```

The assembly text itself will be parsed for % symbols.  So where you would normally reference a
register %eax, you have to write the assembly as %%eax to escape out the percent symbol.

For input and output registers you can specify many of them with the following register loading
codes:

Loading Code | Register
-------------|---------
a | eax
b | ebx
c | ecx
d | edx
S | esi
D | edi
A | eax and edx as combined 64-bit integer

The loading codes go inside of double-quotes, and the variable that you are using with them goes
int parens:

```
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
```

### Dynamic Constraints / Registers
 
Use the following loading codes to have the compiler dynamically assign what register it wants to
use for passing arguments into and out of the assembly code.

Loading Code | Notes
-------------|------
f | Floating point register value
q | General register, but excluded esi and edi on x86
r | General register
I | Immediate (I do not see many examples to explain why you would do this)

The dynamic registers are numbered %0 - %9 in the assembly.  Numbering starts with the outputs
first, then the inputs, left to right.

```
id example2()
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
```

### Constraint Modifiers
 
The inputs and outputs have the following constraint modifiers

Constraint Modifier | Meaning
--------------------|--------
= | Write only, usually used for output arguments
(blank) | Read only, usually used for input arguments
+ | Read and write operand.  Some sites say it is buggy, some say it just does not work, avoid

### What if you want to have an argument that is both input and output?

Use the loading codes to specify particular register like eax or ebx specifically, and use it for
both an input and output.  Alternatively, you can also use a digit as the loading code, and shrare
a dynamic register.  See the following example for both options:

```
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
```

### Clobbering

If the assembly code clobbers other registers, you need to tell the compiler which ones are
clobbered.  You also have to indicate if the the condition registers "cc" get clobbered, or
memory "memory" was written to.

@todo Memory clobbering example
@todo Flags?  Or is this already covered
@todo Branching / labels in the assembly
@todo Keyword volatile

## References

* http://www.delorie.com/djgpp/doc/brennan/brennan_att_inline_djgpp.html
* http://www.ic.unicamp.br/~celio/mc404-s2-2015/docs/ARM-GCC-Inline-Assembler-Cookbook.pdf
* http://locklessinc.com/articles/gcc_asm/
* https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html
