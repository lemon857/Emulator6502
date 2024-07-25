# Emulator6502
This is a program for emulate some functions 6502 processor  

If you want execute your program slow and see realtime processor instructions, use flags -g or --gui  

As default path to program equal "test.asm6502", if you want change it, use flags -p or --prg and after space relative path to asm6502 program

Source information about instructions you can get [here](http://www.6502.org/tutorials/6502opcodes.html)

6502 have three registers, the size of each one byte: A, X, Y  
Max support memory 64kb

6502 is a little endian processor, this means in word big byte first, little byte end

[custom_asm6502.xml](https://github.com/lemon857/Emulator6502/blob/main/custom_asm6502.xml) is a style for notepad++
# Intoduction
Now I have compiller for custom assembly for 6502 with so small count instructions:   
[lda](#ldaldxldy) [ldx](#ldaldxldy) [ldy](#ldaldxldy) [sta](#stastxsty) [stx](#stastxsty) [sty](#stastxsty) [jmp](#jmp) [jsr](#jsrrts) [rts](#jsrrts)
[tax](#register-instructions) [txa](#register-instructions) [tay](#register-instructions) [tya](#register-instructions) 
[dex](#register-instructions) [dey](#register-instructions) [inx](#register-instructions) [iny](#register-instructions) 
[txs](#stack-instructions) [tsx](#stack-instructions) [pha](#stack-instructions) [pla](#stack-instructions)
[dec](#decinc) [inc](#decinc)
bne beq bcc bcs
adc 
clc sec clv and

Also have some prefixes for comfort compile and for know what every item mean:

- prefix ';' start comment
`; test comment`
- prefix '$' start hexadecemal value, default is address, default mode for commands use as argument
  1. zero page mode ($44) `$44 ; Byte`
  2. absolute mode ($4444) `$4444 ; Word`  
- prefix '#' for place before hex value in command arguments, mean value, not address
  `#$4444`
- prefix '(' with end ')' between this signs value as command argument mean use indirect mode
  `($4444)`
- prefix '.' mark pseudonym, named this pos as address and can used how absolute address in progragram, it isn't a command for cpu, is preprocess sign
`.test`
- *prefix ':'* have two functions, it isn't a command for cpu, is preprocess sign:  
  1. set pos program ex. :$4444 set next commands starts from this value in memory `:$4444`  
  2. set byte in memory ex. :$4444 $44 set byte from $4444 value $44 `:$4444 $44`
  
# Settings
In [memory.hpp](https://github.com/lemon857/Emulator6502/blob/main/include/memory.h)
```c++
// 0x0000 - 0x00FF - Zero Page
// 0x0100 - 0x01FF - Is a stack page
// 0x0200 - 0x02FF - I use as data storage
const Word START_PROGRAM = 0x0300; // 0x0300 - MAX_MEM - our possible using memory
```

# Examples
Using [$value] in comments signfy place in memory at $value

## lda/ldx/ldy
This mnemocodes used for LoaD Accumulator/X/Y from some place in memory or immediate value  
Rules and modes is identity  
- lda use for example, you can also use ldx and ldy  
```asm
; immediate mode
lda #$A0     ; load to A value $A0
```
```asm
; zero page mode
:$000F $A0  ; set memory at $000F value $A0
lda $0F     ; load to A value $A0 from [$0F]
```
```asm
; zero page, x mode
:$000F $A0  ; set memory at $000F value $A0
ldx #$01    ; load $01 to X, next X value use as offset
lda $0E, X  ; load to A value $A0 from [$0E + X]
```
```asm
; absolute mode
:$0322 $A0  ; set memory at $0322 value $A0
lda $0322   ; load to A value $A0 from [$0322]
```
```asm
; absolute, x/y mode
:$00322 $A0      ; set memory at $0F0F value $A0
ldx #$01    ; load $01 to X, next X value use as offset
lda $0321, X/Y  ; load to A value $A0 from [$0321 + X/Y] = [$0322]
```
```asm
; only for A register
; indirect, x/y mode, use Byte as address
:$AE $03        ; set memory at $AE value $0322, use later how address
:$AF $22        ; little byte
:$0322 $A0      ; set memory at $0322 value $A0
ldx #$01        ; load $01 to X, next X value use as offset
lda ($AD), X/Y  ; load to A value $A0 from [from [$AE + X/Y]]
```
## sta/stx/sty 
This mnemocodes used for STore Accumulator/X/Y to some place in memory  
Rules and modes is identity  
- sta use for example, you can also use stx and sty  
```asm
; zero page mode
sta $F0		; store value A to [$F0]
```
```asm
; zero page, x mode
ldx #$01    ; load $01 to X, next X value use as offset
sta $F0, X  ; store value A to [$F0 + X] = [$F1]
```
```asm
; absolute, x/y mode
lda #$A0        ; load $A0 to A
ldx #$01        ; load $01 to X, next X value use as offset
sta $0322, X/Y	; store value A to [$0322 + X/Y] = [$0323]
```
```asm
; indirect, x/y mode, use Byte as address
:$F1 $0D      ; set memory at $F1 value $0DEA, use later how address
:$F2 $EA      ; little byte
lda #$A0      ; load $A0 to A
ldx #$01      ; load $01 to X, next X value use as offset
sta ($F0), X  ; store value A to [from [$F0 + X/Y] = [$F1]]
; mean indrect store value to address, which loaded to address in argument
```
## jmp 

```asm
; absolute address mode, hex
jmp $0322	; jump to $0322
; do something
:$0322 		; mark next code start at $0322 in memory
; do something
```
```asm
; absolute address mode, pseudonym
jmp .test	; jump to .test
; do something
.test 		; mark pseudonym
; do something
```
```asm
; indirect mode, use Word as address
:$0200 $03 	; set memory at $0200 value $0322, use later how address
:$0201 $22
jmp ($0200)	; jump to address, loaded at [$0200]
; do something
:$0322 		; mark next code start at $0322 in memory
; do something
```
## jsr/rts
Jump Sub Routine / ReTurn from Subroutine

```asm
; jsr support only absolute mode, hex
jsr $0322	 ; jump to subroutine
; do something
:$0322		 ; mark next code start at $0322 in memory
; do something
rts        ; return from subroutine
; do something
```
```asm
; jsr support only absolute mode, pseudonym
jsr .testroutine ; jump to subroutine
; do something
.testroutine		
; do something
rts              ; return from subroutine
; do something
```
```asm
; example simple function
jsr .exfunc ; call .exfunc
; do something
jmp .endexfunc   ; protection for exclude possible run code in func without need
.exfunc
; some code
rts              ; reurn from func
.endexfunc       ; mark for protection-round
; do something
```
## register instructions
```asm
tax ; transfer value A to X
txa ; transfer value X to A
tay ; transfer value X to Y
tya ; transfer value Y to A
dex ; decrement X value
dey ; decrement Y value
inx ; increment X value
iny ; increment Y value
```
## stack instructions
```asm
txs ; transfer X to Stack ptr
tsx ; transfer Stack ptr to X
pha ; push A
pla ; pull A
```
## dec/inc
Decrement/increment value from memory  
Rules and modes is identity
```asm
; zero page mode
:$0F $02  ; set value at [$0F] equal $02
dec $0F   ; decrement value at [$0F]
```
```asm
; zero page, x mode
:$0F $02    ; set value at [$0F]equal $02
dec $0E, X  ; decrement value at [$0E + X] = [$0F]
```
```asm
; absolute mode
:$0322 $02  ; set value at [$0322] equal $02
dec $0322   ; decrement value at [$0322]
```
```asm
; absolute, x mode
:$0322 $02    ; set value at [$0322] equal $02
dec $0321, X  ; decrement value at [$0321 + X] = [$0322]
```




