#include <xc.h>
#include <stdarg.h>

/* GLOBAL VARIABLS */
volatile __persistent char __DCByte;	// Data capture word for MPLAB to monitor
volatile __persistent char __DCDelay;	// Transmit delay value for MPLAB to init


/* FUNCTION PROTOTYPES */
void __it_trace_dc(char location_id);
void __it_log_dc(char location_id, char data_type, ...);

/*********************************************************************
*
*   location_id:  trace location number 0x40-0x7f
*
**********************************************************************/
#pragma interrupt_level 1
void
__it_trace_dc(char location_id) {
	unsigned char int_state;

	int_state = INTCON;
	di();
	__DCByte = location_id;	// send byte
	_delay3(__DCDelay);	// wait for xmt to complete
	if(int_state & 0x80) {
		ei();
	}
	return;
}

/************************************************************************
*
*   location_id:  log location number 0x0-0x7f
*   data_type:    variable type in encoded format (see Trace16.h)
*   ...:          variable value, 1-4 bytes in length
*
*   Note: Due to bug in compiler, must copy variable argument.
*   Should be able to do the following:
*       va_start(args, data_type);
*       __it_ptr = (char *)args;
*       __it_ptr--;
*   then in loop:
*       __IT_XMT_BYTE(*__it_ptr--);
*
*************************************************************************/
#pragma interrupt_level 1
void
__it_log_dc(char location_id, char data_type, ...) {
	char len;
	long data_var;		// buffer to hold 1-4 bytes of data
	char *__it_ptr;
	va_list args;
	unsigned char int_state;

	int_state = INTCON;
	di();
	len = (data_type & 0x7u) + 1;

	// GET POINTER TO DATA VARIABLE
	va_start(args, data_type);
	data_var = va_arg(args, long);	// copy variable argument
	va_end(args);
	__it_ptr = (char *)&data_var;

	// SEND TYPE AND ID BYTES
	__DCByte = data_type;
	_delay3(__DCDelay);

	__DCByte = location_id;
	_delay3(__DCDelay);

	// SEND DATA VALUE
	do {
		__DCByte = *__it_ptr++;
		_delay3(__DCDelay);
	} while (--len);

	if(int_state & 0x80) {
		ei();
	}
	return;
}

