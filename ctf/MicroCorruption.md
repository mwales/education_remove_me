# Microcorruption CTF

I had completed the CTF stages through Algiers and then got distracted and started working on other
things.  So I decided to come back and resolve them and then see if I can get farther along.  This
time as I solve I will write up my solution.

## New Orleans

Put a breakpoint in the check_password function.  Saw that the first comparison was pointing at a 
memory location (0x2400) with ASCII data.  Let us try that as a possible solution.

```
~LM4iFg
```

## Sydney

Check password now has a bunch of comparisons in sequence, and the hardcoded values for the
comparison look like they might be ASCII as well...

- 0x448c = @A
- 0x4494 = ^+
- 0x449c = M.
- 0x44a8 = lm

```
@A^+M.lm
```

## Hanoi

I have no idea what the code was actually trying to do, but I found that the global memory buffer
can be overwritten, and that there was a state variable immediately following the buffer that we
could change.

This input is hexadecimal.

```
555555555555555555555555555555557b
```

## Cusco 

Call convention is 1st argument is put in r15, second arg is r14...
16-byte password buffer is put on the stack in login function

Code calls getsn(&buf, 0x30) which will allow a buffer overrun on the stack.  We should be able to 
change return address of login to call unlock_door().

Return address is on stack @ 0x43fe, points to 0x443c, need to change it to 0x4446

```
0102030405060708090a0b0c0d0e0f104644
```

## Reykjavik

## Whitehorse

## Montevideo

## Johannesburg

## Santa Cruz

## Jakarta

## Addis Ababa

## Novosibirsk

## Algiers

## Vladivostok

## Lagos


