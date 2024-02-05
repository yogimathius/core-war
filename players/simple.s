.name "Simple"
.comment "Let's get started"

simple:	live 1
		ld %34, r3
		; op  params      values
		; 02 | 90 |	00 00 00 22 03
		st 04, %34
		; op  params      values
		; 03 | 70 | 04 00 00 00 22
		add 01 01, 03
		; op  params      values
		; 04 | 54 |	01 01 03
		sub 04, 05, 06
		; op  params      values
		; 05 | 54 |	04 05 06
		and 07, 08, 09
		; op  params      values
		; 06 | 54 |	04 05 06
		or 10, 11, 12
		; op  params      values
		; 07 | 54 |	06 07 12
		xor 13, 14, 15
		; op  params      values
		; 08 | 54 |	12 14 15
		zjmp %23
		; op  params      values
		; 09 | 80 |	00 00 00 13
		ldi 3, %4, r1
		; op  params      values
		; 0a | 64 |	3 00 00 00 00 04 01
		sti r2, %4, %5
		; op  params      values
		; 0b | 90 |	00 00 00 22 03
		fork %1
		; op  params      values
		; 0c | 80 |	00 00 00 1
		lld 34, r3
		; op  params      values
		; 0d | D0 01 |	1C 03
		lldi %3, %4, r1
		; op  params      values
		; 0e | A4 00 |	00 00 00 03 00 00 00 04 01
		lfork %2
		; op  params      values
		; 0f | 80 |	00 00 00 02
		aff r3
		; op  params      values
		; 010 | 40 |	01