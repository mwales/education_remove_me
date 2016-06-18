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

0x448c = @A
0x4494 = ^+
0x449c = M.
0x44a8 = lm

```
@A^+M.lm
```


