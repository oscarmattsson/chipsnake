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

.global enable_interrupt
enable_interrupt:
  ei
  jr $ra
  nop
