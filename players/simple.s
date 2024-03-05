.name "Made in house"
.comment "Let's get started"

simple:	
		ld %34, r3
		; op  params      values
		; 02 | 90 |	00 00 00 22 03
		st r4, %34
		; op  params      values
		; 03 | 70 | 04 00 00 00 22
		add r1 r1, r3
		; op  params      values
		; 04 | 54 |	01 01 03
		sub r4, r5, r6
		st %34, r4
		; op  params      values
		; 03 | 70 | 04 00 00 00 22
		add r1 r1, r3
		; op  params      values
		; 04 | 54 |	01 01 03
		sub r4, r5, r6
		live r1
		st r4, %34
		; op  params      values
		; 03 | 70 | 04 00 00 00 22
		add r1 r1, r3
		; op  params      values
		; 04 | 54 |	01 01 03
		sub r4, r5, r6
		; op  params      values
		; ; 05 | 54 |	04 05 06
		and r7, r8, r9
		; ; op  params      values
		; ; 06 | 54 |	04 05 06
		or r10, r11, r12
		; ; op  params      values
		; ; 07 | 54 |	06 07 12
		xor r13, r14, r15
		; ; op  params      values
		; ; 08 | 54 |	12 14 15
		zjmp %23
		; ; op  params      values
		; ; 09 | 80 |	00 00 00 13
		ldi 3, %4, r1
		; ; op  params      values
		; ; 0a | 64 |	3 00 00 00 00 04 01
		sti r2, %4, %5
		; ; op  params      values
		; ; 0b | 90 |	00 00 00 22 03
		fork %1
		; ; op  params      values
		; ; 0c | 80 |	00 00 00 1
		lld 34, r3
		; ; op  params      values
		; ; 0d | D0 01 |	1C 03
		lldi %3, %4, r1
		; ; op  params      values
		; ; 0e | A4 00 |	00 00 00 03 00 00 00 04 01
		lfork %2
		; ; op  params      values
		; ; 0f | 80 |	00 00 00 02
		aff r3
		; ; op  params      values
		; ; 010 | 40 |	01