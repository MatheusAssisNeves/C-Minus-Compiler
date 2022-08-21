0       j main
    fatorial:
1       addi $sp, $sp, 2
2       sw $ra, -1($sp)
3       sw $a0, 0($sp)
4       lw $t0, 0($sp)
5       addi $at, $0, 1
6       bne $t0, $at, L1
7       addi $v0, $0, 1
8       j L0
9       j L2
    L1:
10      lw $t1, 0($sp)
11      lw $t2, 0($sp)
12      subi $t3, $t2, 1
13      add $a0, $t3, $0
14      jal fatorial
15      add $t4, $v0, $0
16      mult $t5, $t1, $t4
17      add $v0, $t5, $0
18      j L0
    L2:
    L0:
19      lw $ra, -1($sp)
20      subi $sp, $sp, 2
21      jr $ra
    main:
22      addi $sp, $sp, 2
23      in $t6
24      sw $t6, 0($sp)
25      lw $t7, 0($sp)
26      add $a0, $t7, $0
27      jal fatorial
28      add $t8, $v0, $0
29      sw $t8, -1($sp)
30      lw $t9, -1($sp)
31      out $t9
    L2:
32      subi $sp, $sp, 2
33      halt
