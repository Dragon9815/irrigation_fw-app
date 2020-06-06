/* Weigl WST Coprocessor */
/* TrueSTUDIO generated code, modified by Stefan Gr�nzinger */

/* Entry Point */
ENTRY(Reset_Handler)

/* Generate a link error if heap and stack don't fit into RAM */
_Heap_Size  = 0xB000;      /* required amount of heap  */
_Stack_Size = 0x1000; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
    FLASH_BL          (rx)  : ORIGIN = 0x08000000, LENGTH = 32K
    FLASH_APP         (rx)  : ORIGIN = 0x08008200, LENGTH = 220K
    
    
 	RAM_RESERVED      (rw)  : ORIGIN = 0x20000000, LENGTH = 1k
    RAM_WORK          (rw)  : ORIGIN = 0x20000400, LENGTH = 79k    /* we have a total of 128k RAM, 256 bytes are reserved at */
                                                                   /* the beginning for BL - APP interaction                 */
   	RAM_HEAP          (rw)  : ORIGIN = 0x20014000, LENGTH = _Heap_Size
    RAM_KERNEL_STACK  (rw)  : ORIGIN = 0x2001F000, LENGTH = _Stack_Size
}

/* end of stack, the stack grows downwards */
_estack = ORIGIN( RAM_KERNEL_STACK ) + LENGTH( RAM_KERNEL_STACK ) - 16;  /* 16 bytes padding for safety */

/* Define output sections */
SECTIONS
{
    .reset :
    {
        . = ALIGN( 4 );
        KEEP(*(.__RESET_vector))
        LONG(0xAAAAAAAA)                /* Dummy value, needed for correct operation of ld (?) */
        . = ALIGN( 4 ); 
    } > FLASH_BL
  
  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    _sapp = .;
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH_APP

  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH_APP

  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH_APP

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH_APP
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH_APP

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH_APP
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH_APP
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH_APP

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >RAM_WORK AT> FLASH_APP

  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM_WORK

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap :
  {
    . = ALIGN(4);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    PROVIDE ( _heap_start = . );
    . = . + _Heap_Size;
    PROVIDE ( _heap_end = . );
    . = ALIGN(4);
  } >RAM_HEAP
  
  ._kernel_stack :
  {
    . = ALIGN(4);
    PROVIDE ( _stack_start = . );
    . = . + _Stack_Size;
    PROVIDE ( _stack_end = . );
    . = ALIGN(4);
  } >RAM_KERNEL_STACK

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
