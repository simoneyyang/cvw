# fir.s
# Simone Yang simyang@hmc.edu
# Feb 13, 2025

# designed echoing the approach to 2.14 in the textbook question sets
# a0 = x[], a1 = c[], a2 = y[], a3 = n, a4 = m

fir: 
    li t6, 0        # j = 0
    lw t3, 0(a3)      # n
    lw t4, 0(a4)      # m

L1:
    beq t6, t3, exit    # termination check j = n
    li t0, 0		# sum = 0
    li t5, 0       # initialize i = 0
    j L2

L2:
    beq t5, t4, L1ret    # termination check i = m

    sub t1, t6, t5     # temp1 = j - i
    addi t2, a4, -1     # temp2 = m - 1
    add t1, t1, t2      # temp1 = temp1 + temp2

    slli t1, t1, 4      # convert temp1 to bytes
    add t1, a0, t1    # t1 = &x[j - i + (m - 1)] = x[temp1]

    slli t2, t5, 4  # put i in bytes
    add t2, a1, t5  # c[i]
    # fancy multiplication time
    mul t1, t2, t1      # t1 = c[i] + x[j - i + (m - 1)]
    # that's gonna be 64 bits (bad)
    slli t1, t1, 0x1
    srai t1, t1, 32

    add t0, t0, t1       # t0 = t0 + t1 (multiplication result)

    addi t5, t5, 1    # increment i
    j L2

L1ret:
    sw t0, 0(a2)       # y[j] = sum
    addi t6, t6, 1     # increment j
    addi a2, a2, 4      # increment y[j]
    j L1

exit:
    ret