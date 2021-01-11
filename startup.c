// Create references to symbols defined in the linker script 
extern unsigned int __data_start;
extern unsigned int __data_end;
extern unsigned int __bss_start;
extern unsigned int __bss_end;
extern unsigned int __end;
extern unsigned int __text_start;
extern unsigned int __text_end;
extern unsigned int __rodata_start;
extern unsigned int __rodata_end;


void startup();			// Function prototype (forward declaration) for startup function
int main();			// Function prototype for main function


// The startup function, address was provided in the vector table
void startup()
{
    volatile int *p;
	volatile int *src;

	/* Zero the BSS area */
	for ( p = (int *) &__bss_start; p < (int *) &__bss_end; )
	    *p++ = 0;

	/* Copy initialized data from flash */
	// src = &__rodata_start;
	src = &__text_end;

    for ( p = &__data_start; p < (int *) &__data_end; p++ )
            *p = *src++;

    // Calling the main function
    main();
}
