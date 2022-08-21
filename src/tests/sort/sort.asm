0       addi $sp, $gp, 9
1       j main
    minloc:
2       addi $sp, $sp, 7
3       sw $ra, -6($sp)
4       sw $a0, 0($sp)
5       sw $a1, -1($sp)
6       sw $a2, -2($sp)
7       lw $t0, -1($sp)
8       sw $t0, -5($sp)
9       lw $t1, -1($sp)
10      lw $at, 0($sp)
11      add $at, $at, $t1
12      lw $t2, 0($at)
13      sw $t2, -4($sp)
14      lw $t3, -1($sp)
15      addi $t4, $t3, 1
16      sw $t4, -3($sp)
    L1:
17      lw $t5, -3($sp)
18      lw $t6, -2($sp)
19      slt $at, $t5, $t6
20      beq $at, $0, L2
21      lw $t7, -3($sp)
22      lw $at, 0($sp)
23      add $at, $at, $t7
24      lw $t8, 0($at)
25      lw $t9, -4($sp)
26      slt $at, $t8, $t9
27      beq $at, $0, L3
28      lw $t10, -3($sp)
29      lw $at, 0($sp)
30      add $at, $at, $t10
31      lw $t11, 0($at)
32      sw $t11, -4($sp)
33      lw $t12, -3($sp)
34      sw $t12, -5($sp)
    L3:
35      lw $t13, -3($sp)
36      addi $t14, $t13, 1
37      sw $t14, -3($sp)
38      j L1
    L2:
39      lw $t15, -5($sp)
40      add $v0, $t15, $0
41      j L0
    L0:
42      lw $ra, -6($sp)
43      subi $sp, $sp, 7
44      jr $ra
    sort:
45      addi $sp, $sp, 7
46      sw $ra, -6($sp)
47      sw $a0, 0($sp)
48      sw $a1, -1($sp)
49      sw $a2, -2($sp)
50      lw $t16, -1($sp)
51      sw $t16, -3($sp)
    L5:
52      lw $t17, -3($sp)
53      lw $t18, -2($sp)
54      subi $t19, $t18, 1
55      slt $at, $t17, $t19
56      beq $at, $0, L6
57      lw $t20, 0($sp)
58      add $a0, $t20, $0
59      lw $t21, -3($sp)
60      add $a1, $t21, $0
61      lw $gp, -2($sp)
62      add $a2, $gp, $0
63      jal minloc
64      add $t0, $v0, $0
65      sw $t0, -4($sp)
66      lw $t1, -4($sp)
67      lw $at, 0($sp)
68      add $at, $at, $t1
69      lw $t2, 0($at)
70      sw $t2, -5($sp)
71      lw $t3, -3($sp)
72      lw $at, 0($sp)
73      add $at, $at, $t3
74      lw $t4, 0($at)
75      lw $t5, -4($sp)
76      lw $at, 0($sp)
77      add $at, $at, $t5
78      sw $t4, 0($at)
79      lw $t6, -5($sp)
80      lw $t7, -3($sp)
81      lw $at, 0($sp)
82      add $at, $at, $t7
83      sw $t6, 0($at)
84      lw $t8, -3($sp)
85      addi $t9, $t8, 1
86      sw $t9, -3($sp)
87      j L5
    L6:
    L4:
88      lw $ra, -6($sp)
89      subi $sp, $sp, 7
90      jr $ra
    main:
91      addi $sp, $sp, 1
92      addi $at, $0, 0
93      sw $at, 0($sp)
    L8:
94      lw $t10, 0($sp)
95      slti $at, $t10, 10
96      beq $at, $0, L9
97      in $t11
98      lw $t12, 0($sp)
99      add $at, $gp, $t12
100     sw $t11, 0($at)
101     lw $t13, 0($sp)
102     addi $t14, $t13, 1
103     sw $t14, 0($sp)
104     j L8
    L9:
105     addi $t15, $gp, 0
106     add $a0, $t15, $0
107     addi $a1, $0, 0
108     addi $a2, $0, 10
109     jal sort
110     add $t16, $v0, $0
111     addi $at, $0, 0
112     sw $at, 0($sp)
    L10:
113     lw $t17, 0($sp)
114     slti $at, $t17, 10
115     beq $at, $0, L11
116     lw $t18, 0($sp)
117     add $at, $gp, $t18
118     lw $t19, 0($at)
119     out $t19
120     lw $t20, 0($sp)
121     addi $t21, $t20, 1
122     sw $t21, 0($sp)
123     j L10
    L11:
    L7:
124     subi $sp, $sp, 1
125     halt
