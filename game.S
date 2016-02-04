# basework.S
# Written 2015 by F Lundevall
# Modified 2016 by Oscar Mattsson
# The macros PUSH and POP are in the public domain.

.macro	PUSH reg
  addi	$sp,$sp,-4
  sw    \reg,0($sp)
.endm

.macro	POP reg
  lw	  \reg,0($sp)
  addi	$sp,$sp,4
.endm

# hexasc: Converting integers 0-9 and A-F (10-15) to corresponding ASCII-values.
# Written 2016-01-23 by Oscar Mattsson
.global hexasc
hexasc:
  andi	$v0,$a0,0xf			# only read 4 least significant bits
  slti	$t0,$v0,10			# check if less than 10
  addi	$v0, $v0, 0x30	# add 0x30, offset ASCII-value for 0
  bnez	$t0,ehexasc			# if 0-9, jump to end
  nop
  addi	$v0,$v0,7				# if 10-15, add offset from ASCII-value from 9 to A
ehexasc:
  jr    $ra
  nop

# delay: Delay the program by $a0 milliseconds
# Written 2016-01-23 by Oscar Mattsson
.global delay
delay:
  slti	$t0,$a0,1		    # if the delay is 0 or less, jump to end
  bnez	$t0,edelay
  nop

l1:								      # while($a0 > 0)
  li	  $t0,0					  # reset loop count
  li	  $t1,20000			  # set counts per ms

  l2: 						      # for($t0 = 0; $t0 < $t1; $t0++)
    addi	$t0,$t0,1	    # increase loop count
    slt	  $t2,$t0,$t1	  # check if target count is reached
    bnez	$t2,l2		    # if not, keep looping
    nop

  addi	$a0,$a0,-1	    # $a0--
  slti	$t0,$a0,1		    # check if delay target is reached
  beqz	$t0,l1			    # if not, keep looping
  nop

edelay:						      # end delay
  jr    $ra
  nop
