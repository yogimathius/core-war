.name "Simple"
.comment "Let's get started"

l2:	sti   r1  %:live %1
        ; 0b 68 01 0f 01
          
        and   r1   %0  r1     
        ; 06 64 01 00 01

live:	live %1
                zjmp %:live
