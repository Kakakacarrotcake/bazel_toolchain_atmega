/*
 * Profiling and Data Capture data.
 * Identifiers with exrernal linkage are defined by the compiler's 
 * code-generator in the access bank.
 */
volatile __persistent char __DCByte;
extern volatile __persistent char __DCDelay;
/* Profiling functions save WREG context here */
extern volatile __persistent char __flp_WREG_temp;
