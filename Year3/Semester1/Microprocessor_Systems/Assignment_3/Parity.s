@ Design and write an assembly language program that 
@ will calculate the parity bit for a 7 bit value stored in r0. 
@ 
@ The program should then store the computed parity bit 
@ in bit 7 of r0. Assume even parity.
@ 
@ Using even parity, the parity bit of a value is set such that the 
@ number of set bits (1's) in a value is always even.
@ 
LDR	r0, =0x16 -> (22 in decimal, 10110 in binary)
@ Upload the code that goes between the dashed lines.
@ -------------- start cut ------------------------
MOV     r1, #0          @ Initialize count of 1's to 0
    MOV     r2, #7          @ Initialize loop counter (7 bits)
    MOV     r5, r0          @ Copy r0 to r5 to preserve the original value

count_loop:
    AND     r3, r5, #1      @ Check if the least significant bit is set
    ADD     r1, r1, r3      @ Increment count if bit is 1
    LSR     r5, r5, #1      @ Shift right to check the next bit
    SUBS    r2, r2, #1      @ Decrement loop counter, set flags
    BNE     count_loop      @ Repeat if there are more bits

    ANDS    r1, r1, #1      @ Check if count of 1's is odd
    MOV     r4, #0          @ Default parity bit (1)
    BEQ     set_parity_bit  @ If odd, set parity bit to 1
    MOV     r4, #1          @ If even, set parity bit to 0

set_parity_bit:
    @ Shift r4 left by 7 bits to set it as the parity bit
    LSL     r4, r4, #7         @ Shift r4 left by 7 positions

    @ OR the parity bit with r0 to set it in the 7th position
    ORR     r0, r0, r4         @ Set the parity bit as the MSB (7th bit)

@ -------------- end cut --------------------------
stop:   B	stop
@
@ End of file Parity.s
