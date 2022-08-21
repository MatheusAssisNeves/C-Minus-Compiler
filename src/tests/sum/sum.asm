0       addi $sp, $gp, 4
1       j main
    sum:
2       addi $sp, $sp, 3
3       sw $ra, -2($sp)
4       sw $a0, 0($sp)
5       sw $a1, -1($sp)
6       lw $t0, -1($sp)
7       slti $at, $t0, 1
8       beq $at, $0, L1
9       addi $v0, $0, 0
10      j L0
    L1:
11      lw $t1, 0($sp)
12      add $a0, $t1, $0
13      lw $t2, -1($sp)
14      subi $t3, $t2, 1
15      add $a1, $t3, $0
16      jal sum
17      add $t4, $v0, $0
18      add $v0, $t5, $0
19      j L0
    L0:
20      lw $ra, -2($sp)
21      subi $sp, $sp, 3
22      jr $ra
    main:
23      addi $sp, $sp, 2
24      addi $at, $0, 5
25      sw $at, -1($sp)
26      addi $at, $0, 0
27      sw $at, 0($sp)
    L3:
28      lw $t6, 0($sp)
29      lw $t7, -1($sp)
30      slt $at, $t6, $t7
31      beq $at, $0, L4
32      in $t8
33      lw $t9, 0($sp)
34      add $at, $gp, $t9
35      sw $t8, 0($at)
36      lw $t10, 0($sp)
37      addi $t11, $t10, 1
38      sw $t11, 0($sp)
39      j L3
    L4:
40      addi $t12, $gp, 0
41      add $a0, $t12, $0
42      lw $t13, -1($sp)
43      add $a1, $t13, $0
44      jal sum
45      add $t14, $v0, $0
46      out $t14
    L2:
47      subi $sp, $sp, 2
48      halt
