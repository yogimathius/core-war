.name "Simple"
.comment "Let's get started"

l2:		   sti  [Par. Des.]   r1  %:live    %1
        ;  0b      68         01   00 0f   00 01     ; Hex. representation
          
         and  [Par. Des.]    r1      %0        r1     
        ;  06       64         01  00 00 00 00   01    ; Hex. representation

live:	live %1
		zjmp %:live
