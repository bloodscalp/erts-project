#=============================================================================#
# ARM makefile for the uCOS-II os
# 
# author      : Xavier Meyer
# last change : 2010-01-08
#
#=============================================================================#

# C headers
DIR_HEADERS += ./uCOS-II/Source/
DIR_HEADERS += ./uCOS-II/Ports/ARM-Cortex-M3/Generic/GCC/

# C sources
LIST_SOURCES += ./uCOS-II/Ports/ARM-Cortex-M3/Generic/GCC/os_cpu_c.c
LIST_SOURCES += ./uCOS-II/Ports/ARM-Cortex-M3/Generic/GCC/os_dbg.c
LIST_SOURCES += ./uCOS-II/Source/os_core.c
LIST_SOURCES += ./uCOS-II/Source/os_flag.c
LIST_SOURCES += ./uCOS-II/Source/os_mbox.c
LIST_SOURCES += ./uCOS-II/Source/os_mem.c
LIST_SOURCES += ./uCOS-II/Source/os_mutex.c
LIST_SOURCES += ./uCOS-II/Source/os_q.c
LIST_SOURCES += ./uCOS-II/Source/os_sem.c
LIST_SOURCES += ./uCOS-II/Source/os_task.c
LIST_SOURCES += ./uCOS-II/Source/os_time.c
LIST_SOURCES += ./uCOS-II/Source/os_tmr.c

# Assembler files
LIST_ASM += ./uCOS-II/Ports/ARM-Cortex-M3/Generic/GCC/os_cpu_a.asm

#vpath %.c ./uCOS-II/Ports/ARM-Cortex-M3/Generic/GCC/
#vpath %.c ./uCOS-II/Source/
#vpath %.asm ./uCOS-II/Ports/ARM-Cortex-M3/Generic/GCC/