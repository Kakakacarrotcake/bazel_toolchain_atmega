/* Generated automatically by the program `genconstants'
   from the machine description file `md'.  */

#ifndef GCC_INSN_CONSTANTS_H
#define GCC_INSN_CONSTANTS_H

#define GASISR_Epilogue 2
#define ZERO_REGNO_TINY 17
#define GASISR_Prologue 1
#define REG_Y 28
#define ACC_B 10
#define REG_SP 32
#define TMP_REGNO_TINY 16
#define ZERO_REGNO 1
#define ACC_A 18
#define GASISR_Done 0
#define REG_W 24
#define REG_X 26
#define LPM_REGNO 0
#define REG_Z 30
#define TMP_REGNO 0

enum unspec {
  UNSPEC_STRLEN = 0,
  UNSPEC_MOVMEM = 1,
  UNSPEC_INDEX_JMP = 2,
  UNSPEC_FMUL = 3,
  UNSPEC_FMULS = 4,
  UNSPEC_FMULSU = 5,
  UNSPEC_COPYSIGN = 6,
  UNSPEC_IDENTITY = 7,
  UNSPEC_INSERT_BITS = 8,
  UNSPEC_ROUND = 9,
  UNSPEC_ASHIFT = 10,
  UNSPEC_ASHIFTRT = 11,
  UNSPEC_LSHIFTRT = 12,
  UNSPEC_DIVQI4 = 13,
  UNSPEC_DIVHI4 = 14,
  UNSPEC_DIVSI4 = 15,
  UNSPEC_UDIVQI4 = 16,
  UNSPEC_UDIVHI4 = 17,
  UNSPEC_UDIVSI4 = 18,
  UNSPEC_DIVSF3 = 19,
  UNSPEC_MODQI4 = 20,
  UNSPEC_MODHI4 = 21,
  UNSPEC_MODSI4 = 22,
  UNSPEC_UMODQI4 = 23,
  UNSPEC_UMODHI4 = 24,
  UNSPEC_UMODSI4 = 25
};
#define NUM_UNSPEC_VALUES 26
extern const char *const unspec_strings[];

enum unspecv {
  UNSPECV_PROLOGUE_SAVES = 0,
  UNSPECV_EPILOGUE_RESTORES = 1,
  UNSPECV_WRITE_SP = 2,
  UNSPECV_GASISR = 3,
  UNSPECV_GOTO_RECEIVER = 4,
  UNSPECV_ENABLE_IRQS = 5,
  UNSPECV_MEMORY_BARRIER = 6,
  UNSPECV_NOP = 7,
  UNSPECV_SLEEP = 8,
  UNSPECV_WDR = 9,
  UNSPECV_DELAY_CYCLES = 10
};
#define NUM_UNSPECV_VALUES 11
extern const char *const unspecv_strings[];

#endif /* GCC_INSN_CONSTANTS_H */
