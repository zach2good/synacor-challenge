##== Synacor Challenge ==
https://challenge.synacor.com/

This is my attempt at taking on the Synacor Challenge. I have created the virtual machine and used ImGui to create an interface in which to interact with it.

[You can test the current build here](http://zachtoogood.com/synacor/index.html)

###Screenshot
![image](https://raw.githubusercontent.com/zach2good/synacor-challenge/master/img/anim.gif "ScreenShot1")

###Features
- C++ to Emscripten toolchain
- Live viewing of Register, Stack and Memory status
- Dumping and Restoring of machine-state. (Dumping unsupported in browser)
- Single-step and free-running of the VM.
- Debug output on a per-opcode basis

###Todo
- Fix input editor
- Make all fields editable
- Write new programs from scratch in editor
- Dumping & saving state from web
- Optimisation and general speed-up (Self-check currently takes ~2+ hours)

####Original Challenge Text
##== Synacor Challenge ==

In this challenge, your job is to use this architecture spec to create a
virtual machine capable of running the included binary.  Along the way,
you will find codes; submit these to the challenge website to track
your progress.  Good luck!


###== architecture ==
- three storage regions
  - memory with 15-bit address space storing 16-bit values
  - eight registers
  - an unbounded stack which holds individual 16-bit values
- all numbers are unsigned integers 0..32767 (15-bit)
- all math is modulo 32768; 32758 + 15 => 5

###== binary format ==
- each number is stored as a 16-bit little-endian pair (low byte, high byte)
- numbers 0..32767 mean a literal value
- numbers 32768..32775 instead mean registers 0..7
- numbers 32776..65535 are invalid
- programs are loaded into memory starting at address 0
- address 0 is the first 16-bit value, address 1 is the second 16-bit value, etc

###== execution ==
- After an operation is executed, the next instruction to read is immediately after the last argument of the current operation.  If a jump was performed, the next operation is instead the exact destination of the jump.
- Encountering a register as an operation argument should be taken as reading from the register or setting into the register as appropriate.

###== hints ==
- Start with operations 0, 19, and 21.
- Here's a code for the challenge website: KvdzsuspbBGU
- The program "9,32768,32769,4,19,32768" occupies six memory addresses and should:
  - Store into register 0 the sum of 4 and the value contained in register 1.
  - Output to the terminal the character with the ascii code contained in register 0.

###== opcode listing ==

| OP | Number | Args | Description |  
|----|--------|-----|-------------|
| halt | 0 | N/A | Stop execution and terminate the program |
| set | 1  | a b | Set register a to the value of b |
| push | 2 | a | push a onto the stack |
| pop | 3 | a | Remove the top element from the stack and write it into a; empty stack = error |
| eq | 4 | a b c | Set a to 1 if b is equal to c; set it to 0 otherwise |
| gt | 5 | a b c | Set a to 1 if b is greater than c; set it to 0 otherwise |
| jmp | 6 | a | Jump to a |
| jt | 7 | a b | If a is nonzero, jump to b |
| jf | 8 | a b | If a is zero, jump to b |
| add | 9 | a b c | Assign into a the sum of b and c (modulo 32768) |
| mult | 10 | a b c | Store into a the product of b and c (modulo 32768) |
| mod | 11 | a b c | Store into a the remainder of b divided by c |
| and | 12 | a b c | Stores into a the bitwise and of b and c |
| or | 13 | a b c | Stores into a the bitwise or of b and c |
| not | 14 | a b | Stores 15-bit bitwise inverse of b in a |
| rmem | 15 | a b | Read memory at address b and write it to a |
| wmem | 16 | a b | Write the value from b into memory at address a |
| call | 17 | a | Write the address of the next instruction to the stack and jump to a |
| ret | 18 | N/A  | Remove the top element from the stack and jump to it; empty stack = halt |
| out | 19 | a | Write the character represented by ascii code a to the terminal |
| in | 20 | a | Read a character from the terminal and write its ascii code to a; it can be assumed that once input starts, it will continue until a newline is encountered; this means that you can safely read whole lines from the keyboard and trust that they will be fully read |
| noop | 21 | N/A | No operation | 
  
