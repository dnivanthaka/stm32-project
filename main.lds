
MEMORY
{
    rom : ORIGIN = 0x08000000, LENGTH = 512K 
    ram : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{

   .text :
   {
        __text_start = .;
       *(.vectors*)
       *(.text*)
       . = ALIGN(4);
       __text_end = .;
   } > rom

   .bss  :
   {
       . = ALIGN(4);
       __bss_start = .;
       *(.bss*)
       *(COMMON)
       . = ALIGN(4);
       __bss_end = .;
   } > ram

   /* Will work, acts like .rodata */
   /*
   .data :
   {
       . = ALIGN(4);
       __data_start = .;
       *(.data*)
       . = ALIGN(4);
       __data_end = .;
   } > rom 
   */

   .data :
   {
       . = ALIGN(4);
       __data_start = .;
       *(.data*)
       . = ALIGN(4);
       __data_end = .;
       __end = .;
   } > ram AT> rom 

   .rodata :
   {
       . = ALIGN(4);
       __rodata_start = .;
       *(.rodata*)
       . = ALIGN(4);
       __rodata_end = .;
   } > rom 
	
} 


