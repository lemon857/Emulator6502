# Emulate6502
This is a program for  emulate some functions 6502 processor

Source information I get here: http://www.6502.org/tutorials/6502opcodes.html#RTS

custom_asm6502.xml is a style for notepad++

Now I have compiller custom asm6502 with so small count instructions: 
lda ldx ldy sta stx sty rts jmp jsr and 
tax txa tay tya dex dey inx iny bne beq 
txs tsx pha pla dec inc adc bcc bcs clc 
sec clv 

Also have some prefixes for comfort compile and know what it do:

- prefix '.' named this pos as address and can used how absolute address
- prefix ';' start comment
- prefix '$' start hexadecemal value, default is address, default mode for commands use zero page ($44) or absolute ($4444)
- prefix '#' for place before hex value in command, need value, not address
- prefix '\*' for args for branch type commands to set \+ or \- offset in bytes for jump if ...
- prefix '(' with end ')' between this signs value as command argument singfy use indirect mode
* prefix ':' *
have two functions, it isn't a command for cpu, is preprocess sign: 
first - set pos program ex. :$4444 set next commands from this value in memory 
second - set byte in memory ex. :$4444 $44 set byte from $4444 value $44 
