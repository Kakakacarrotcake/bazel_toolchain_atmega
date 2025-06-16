/* This file is sorted such that 'errors' which represent exceptional
 * conditions under which a correct program may fail come first, followed
 * by messages that indicate an incorrect program or system failure. The
 * macro E() along with double-inclusion is used to ensure that ordering
 * of the strings remains synchronized. */

E(EILSEQ,       "Illegal byte sequence")
E(EDOM,         "Domain error")
E(ERANGE,       "Result not representable")
E(EOVERFLOW,    "Value too large for data type")
E(EINVAL,       "Invalid argument")

E(0,            "No error information")
