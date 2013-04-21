#
#                                               uC/CPU
#                                    CPU CONFIGURATION & PORT LAYER
#
#                                            CPU PORT FILE
#
#                                            ARM-Cortex-M3
#                                            GCC Compiler
#
# Filename      : cpu_a.asm
# Version       : V1.0
# Programmer(s) : Xavier Meyer
# Miscellaneous : This script is an adapted version of the uC/CPU IAR script from Micrium
# Toolchain 	: arm-elf
#
#********************************************************************************************************
#                                           PUBLIC FUNCTIONS
#********************************************************************************************************

.global CPU_IntDis
.global CPU_IntEn

.global CPU_SR_Save
.global CPU_SR_Restore

.global CPU_CntLeadZeros
.global CPU_RevBits


#********************************************************************************************************
#                                      CODE GENERATION DIRECTIVES
#********************************************************************************************************

.cpu cortex-m3
.fpu softvfp
.syntax unified
.thumb
.text



#$PAGE
#********************************************************************************************************
#                                    DISABLE and ENABLE INTERRUPTS
#
# Description: Disable/Enable interrupts.
#
# Prototypes : void  CPU_IntDis(void)#
#              void  CPU_IntEn (void)#
#********************************************************************************************************
.thumb_func
CPU_IntDis:
        CPSID   I
        BX      LR

.thumb_func
CPU_IntEn:
        CPSIE   I
        BX      LR


#********************************************************************************************************
#                                      CRITICAL SECTION FUNCTIONS
#
# Description : Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking, the
#               state of the interrupt disable flag is stored in the local variable 'cpu_sr' & interrupts
#               are then disabled ('cpu_sr' is allocated in all functions that need to disable interrupts).
#               The previous interrupt state is restored by copying 'cpu_sr' into the CPU's status register.
#
# Prototypes  : CPU_SR  CPU_SR_Save   (void);
#               void    CPU_SR_Restore(CPU_SR cpu_sr);
#
# Note(s)     : (1) These functions are used in general like this:
#
#                   void  Task (void *p_arg)
#                   {
#                                                               /* Allocate storage for CPU status register */
#                   #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
#                       CPU_SR  cpu_sr;
#                   #endif
#
#                            :
#                            :
#                       CPU_CRITICAL_ENTER();                   /* cpu_sr = CPU_SR_Save();                  */
#                            :
#                            :
#                       CPU_CRITICAL_EXIT();                    /* CPU_SR_Restore(cpu_sr);                  */
#                            :
#                            :
#                   }
#********************************************************************************************************
.thumb_func
CPU_SR_Save:

        MRS     R0, PRIMASK                     @ Set prio int mask to mask all (except faults)
        CPSID   I
        BX      LR

.thumb_func
CPU_SR_Restore:                                  @ See Note #2.
        MSR     PRIMASK, R0
        BX      LR


#$PAGE
#********************************************************************************************************
#                                         COUNT LEADING ZEROS
#
# Description : Counts the number of binary zero bits before the first binary one bit in the argument.
#               If the argument is zero, the value 32 is returned.
#
# Prototypes : CPU_INT32U  CPU_CntLeadZeros (CPU_INT32U  val)
#
# Argument(s) : val     variable to count leading zeros
#********************************************************************************************************

.thumb_func
CPU_CntLeadZeros:
        CLZ     R0, R0                          @ Count leading zeros
        BX      LR


#********************************************************************************************************
#                                             REVERSE BITS
#
# Description : Reverses the bits in the argument.
#
# Prototypes : CPU_INT32U  CPU_RevBits (CPU_INT32U  val)
#
# Argument(s) : val     variable to reverse
#********************************************************************************************************

.thumb_func
CPU_RevBits:
        RBIT    R0, R0                          @ Reverse bits
        BX      LR


#$PAGE
#********************************************************************************************************
#                                     CPU ASSEMBLY PORT FILE END
#********************************************************************************************************

.end

