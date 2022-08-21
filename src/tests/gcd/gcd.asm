0       j main
    gcd:
1       addi $sp, $sp, 3
2       sw $ra, -2($sp)
3       sw $a0, 0($sp)
4       sw $a1, -1($sp)
5       lw $t0, -1($sp)
6       addi $at, $0, 0
7       bne $t0, $at, L1
8       lw $t1, 0($sp)
9       add $v0, $t1, $0
10      j L0
11      j L2
    L1:
12      lw $t2, -1($sp)
13      add $a0, $t2, $0
14      lw $t3, 0($sp)
15      lw $t4, 0($sp)
16      lw $t5, -1($sp)
17      div $t6, $t4, $t5
18      lw $t7, -1($sp)
19      mult $t8, $t6, $t7
20      sub $t9, $t3, $t8
21      add $a1, $t9, $0
22      jal gcd
23      add $t10, $v0, $0
24      add $v0, $t10, $0
25      j L0
    L2:
    L0:
26      lw $ra, -2($sp)
27      subi $sp, $sp, 3
28      jr $ra
    main:
29      addi $sp, $sp, 2
30      in $t11
31      sw $t11, 0($sp)
32      in $t12
33      sw $t12, -1($sp)
34      lw $t13, 0($sp)
35      add $a0, $t13, $0
36      lw $t14, -1($sp)
37      add $a1, $t14, $0
38      jal gcd
39      add $t15, $v0, $0
40      out $t15
    L2:
41      subi $sp, $sp, 2
42      halt
