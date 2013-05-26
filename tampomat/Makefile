#=============================================================================#
# ARM makefile
# 
# author: Xavier Meyer
# last change: 2010-01-07
#
#=============================================================================#


#=============================================================================#
# toolchain configuration
#=============================================================================#
TOOLCHAIN = arm-elf-

COMPILER = $(TOOLCHAIN)gcc
OBJCOPY = $(TOOLCHAIN)objcopy
ASSEMBLER = $(TOOLCHAIN)gcc -x assembler-with-cpp
OBJDUMP = $(TOOLCHAIN)objdump
SIZE = $(TOOLCHAIN)size
REMOVE = rm


#=============================================================================#
# project configuration
#=============================================================================#

# project name
PROJECT = NTRT

# Output format. (can be srec, ihex, binary)
FORMAT = binary

# core type
CORE = cortex-m3
FP = vfp

# linker script
LINKER_SCRIPT = ./STM32F103ZE_NTRT.ld

# project structure folders
# output binary folder
DIR_OUTPUT = ./out/


#=============================================================================#
# Application files
#=============================================================================#

# user software folder
DIR_APPLICATION_INC = ./src/application/
LIST_APPLICATION_SRC = ./src/application/app.c
LIST_APPLICATION_SRC += ./src/application/getset.c
#LIST_APPLICATION_SRC += ./src/application/model.c
LIST_APPLICATION_SRC += ./src/application/regulation.c
#LIST_APPLICATION_SRC += ./src/application/thread_car_model.c
LIST_APPLICATION_SRC += ./src/application/thread_ihm.c
#vpath %.c ./src/application/

# user BSP (Board Support Package) folder
DIR_BSP_INC = ./src/bsp/
LIST_BSP_SRC  = ./src/bsp/bsp.c
LIST_BSP_SRC += ./src/bsp/bsp_globals.c
LIST_BSP_SRC += ./src/bsp/bsp_utils.c
LIST_BSP_SRC += ./src/bsp/app_int.c
LIST_BSP_SRC += ./src/bsp/app_vect.c
LIST_BSP_SRC += ./src/bsp/stm32f10x_startup.c
LIST_BSP_SRC += ./src/bsp/stm32f10x_syscall.c
LIST_BSP_SRC += ./src/bsp/sdcard.c

# user USB
DIR_USB_INC = ./src/usb/
LIST_USB_SRC  = ./src/usb/usb_desc.c
LIST_USB_SRC += ./src/usb/usb_endp.c
LIST_USB_SRC += ./src/usb/usb_istr.c
LIST_USB_SRC += ./src/usb/usb_prop.c
LIST_USB_SRC += ./src/usb/usb_pwr.c
LIST_USB_SRC += ./src/usb/hw_config.c


# user USB
DIR_RAM_INC = ./src/sram/
LIST_RAM_SRC  = ./src/sram/fsmc_sram.c


#=============================================================================#
# External libs
#=============================================================================#

# Path to dependencies
LIB_PATH 			= ./lib/
LIB_CMSIS   		= CMSIS
LIB_STM32_USB   	= STM32_USB-FS-Device_Driver
LIB_STM32_STD   	= STM32F10x_StdPeriph_Driver
LIB_UC_CPU  		= uC-CPU
LIB_UCOS_II 		= uCOS-II
LIBS  = $(LIB_CMSIS) $(LIB_STM32_STD) $(LIB_UC_CPU) $(LIB_UCOS_II) $(LIB_STM32_USB)

# Include each depencies makefile into this one
ifdef LIBS
  LIBS_PATH = $(foreach LIB, $(LIBS), ./$(LIB_PATH)$(LIB))
  -include $(foreach LIB, $(LIBS_PATH), ./$(LIB)/Makefile)
  EXTRAINCDIRS = $(patsubst %,$(LIB_PATH)%,$(DIR_HEADERS))
  SOURCES = $(patsubst %,$(LIB_PATH)%,$(LIST_SOURCES))
  ASM_FILES = $(patsubst %,$(LIB_PATH)%,$(LIST_ASM))  
endif

#=============================================================================#
# Includes, Sources, Asm, Objects files
#=============================================================================#

# Include dir
EXTRAINCDIRS += ${DIR_APPLICATION_INC}
EXTRAINCDIRS += ${DIR_BSP_INC}
EXTRAINCDIRS += ${DIR_USB_INC}  
EXTRAINCDIRS += ${DIR_RAM_INC}

# Sources list
SOURCES += ${LIST_APPLICATION_SRC}
SOURCES += ${LIST_BSP_SRC} 
SOURCES += ${LIST_USB_SRC}
SOURCES += ${LIST_RAM_SRC}

# Object files
OBJECTS_C = $(SOURCES:.c=.o)
OBJECTS_ASM = $(ASM_FILES:.asm=.o) 
LIST_OBJECT  = $(OBJECTS_C)
LIST_OBJECT += $(OBJECTS_ASM)


#=============================================================================#
# Compilation, linking, assembler, object flags
#=============================================================================#

# Compiler flags
CFLAGS = -mcpu=${CORE} -mthumb -Wall
CFLAGS += -O0
CFLAGS += -Wno-cast-align -Wcast-qual -Wimplicit #-Wno-cast-align -Wno-cast-qual -Wimplicit
CFLAGS += -Wpointer-arith -Wswitch -Wpointer-sign
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -Wstrict-prototypes -Wmissing-declarations #-Wno-strict-prototypes -Wmissing-declarations
CFLAGS += -Wmissing-prototypes -Wextra -Wnested-externs 
#CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -mfpu=${FP}
CFLAGS += -c -g $(patsubst %,-I%,$(EXTRAINCDIRS)) #-I${DIR_APPLICATION_INC} -I${DIR_STM32_STD_PERIPH_LIB_INC} #-I${DIR_CM3_STD_CMSIS_CORE_LIB_INC}

AFLAGS = -mcpu=$(CORE) -mthumb -Wall -c -g -Dgcc
AFLAGS += -mfpu=${FP}

# Linker flags
#LFLAGS = $(CFLAGS) -nostartfiles -T$(LINKER_SCRIPT) -Wl,-Map=$(DIR_OUTPUT)$(PROJECT).map,--cref
LFLAGS = -mcpu=${CORE} -mthumb -Wall
LFLAGS += -Wno-cast-align -Wcast-qual -Wimplicit
LFLAGS += -Wpointer-arith -Wswitch
LFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
LFLAGS += -Wstrict-prototypes -Wmissing-declarations #-Wno-strict-prototypes -Wmissing-declarations
LFLAGS += -Wmissing-prototypes -Wextra -Wnested-externs 
LFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS)) #-I${DIR_APPLICATION_INC} -I${DIR_STM32_STD_PERIPH_LIB_INC} -I${DIR_CM3_STD_CMSIS_CORE_LIB_INC}
LFLAGS += -mfpu=${FP}
LFLAGS += -nostartfiles -T$(LINKER_SCRIPT) -Wl,-Map=$(DIR_OUTPUT)$(PROJECT).map,--cref



# Object dump flags
OFLAGS = -x --syms


#=============================================================================#
# Output Files
#=============================================================================#

OUT_ELF_FILE = ${PROJECT}.elf
OUT_ELF = $(DIR_OUTPUT)$(OUT_ELF_FILE)
OUT_HEX_FILE = ${PROJECT}.hex
OUT_HEX = $(DIR_OUTPUT)$(OUT_HEX_FILE)
OUT_BIN_FILE = ${PROJECT}.bin
OUT_BIN = $(DIR_OUTPUT)$(OUT_BIN_FILE)
OUT_DMP_FILE = ${PROJECT}.dmp
OUT_DMP = $(DIR_OUTPUT)$(OUT_DMP_FILE)
OUT_LSS_FILE = ${PROJECT}.lss
OUT_LSS = $(DIR_OUTPUT)$(OUT_LSS_FILE)


#=============================================================================#
# Compilation messages
#=============================================================================#

MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_ARM = "Compiling C (ARM-only):"
MSG_COMPILINGCPP = Compiling C++:
MSG_COMPILINGCPP_ARM = "Compiling C++ (ARM-only):"
MSG_ASSEMBLING = Assembling:
MSG_ASSEMBLING_ARM = "Assembling (ARM-only):"
MSG_CLEANING = Cleaning project:


#=============================================================================#
# Targets
#=============================================================================#

all :  begin gccversion sizebefore build sizeafter finished end

build : $(LIST_APPLICATION_SRC) elf hex bin dmp lss 

elf: $(OUT_ELF)
hex: $(OUT_HEX)
bin: $(OUT_BIN)
lss: $(OUT_LSS)
dmp: $(OUT_DMP)

clean : begin clean_list finished end

clean_list :
	$(REMOVE) -f ${OBJECTS_C}
	$(REMOVE) -f ${OBJECTS_ASM}
	$(REMOVE) -f $(DIR_OUTPUT)$(PROJECT).elf
	$(REMOVE) -f ${DIR_OUTPUT}$(PROJECT).map
	$(REMOVE) -f $(DIR_OUTPUT)$(PROJECT).hex
	$(REMOVE) -f ${DIR_OUTPUT}$(PROJECT).bin
	$(REMOVE) -f ${DIR_OUTPUT}$(PROJECT).dmp
	$(REMOVE) -f ${DIR_OUTPUT}$(PROJECT).lss


#=============================================================================#
# Eye candy
#=============================================================================#

begin:
	@echo
	@echo $(MSG_BEGIN)

finished:
	@echo $(MSG_ERRORS_NONE)

end:
	@echo $(MSG_END)
	@echo


#=============================================================================#
# Informations
#=============================================================================#

# Display size of file.
# Modif YS: base hexa pour afficher la taille
HEXSIZE = $(SIZE) --radix=16 --target=$(FORMAT) $(DIR_OUTPUT)$(PROJECT).hex
ELFSIZE = $(SIZE) -A --radix=16 $(DIR_OUTPUT)$(PROJECT).elf

sizebefore:
	@if [ -f $(DIR_OUTPUT)$(PROJECT).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(DIR_OUTPUT)$(PROJECT).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi


# Display compiler version information.
gccversion : 
	@$(COMPILER) --version

#-----------------------------------------------------------------------------#
# linking - objects -> elf
#-----------------------------------------------------------------------------#

$(OUT_ELF) : $(LIST_OBJECT)
	@echo 'Linking target: $(OUT_ELF_FILE)'
	$(COMPILER) $(LFLAGS) $(LIST_OBJECT) -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# compiling - C source -> objects
#-----------------------------------------------------------------------------#

%.o : %.c
	@echo 'Compiling file: $<'
	$(COMPILER) -c $(CFLAGS) -I. $< -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# assembling - ASM source -> objects
#-----------------------------------------------------------------------------#

%.o : %.asm
	@echo 'Assembling file: $<'
	$(ASSEMBLER) -c $(AFLAGS) -I. $< -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# memory images - elf -> hex, elf -> bin, elf -> dmp, elf -> lss
#-----------------------------------------------------------------------------#

$(OUT_HEX) : $(OUT_ELF)
	@echo 'Creating IHEX image: $(OUT_HEX_FILE)'
	$(OBJCOPY) -O ihex $< $@
	@echo ' '

$(OUT_BIN) : $(OUT_ELF)
	@echo 'Creating binary image: $(OUT_BIN_FILE)'
	$(OBJCOPY) -O binary $< $@
	@echo ' '

$(OUT_DMP) : $(OUT_ELF)
	@echo 'Creating memory dump: $(OUT_DMP_FILE)'
	$(OBJDUMP) $(OFLAGS) $< > $@
	@echo ' '

$(OUT_LSS) : $(OUT_ELF)
	@echo 'Creating extended listing: $(OUT_LSS_FILE)'
	$(OBJDUMP) -S $< > $@
	@echo ' '	

#-----------------------------------------------------------------------------#
# end Makefile
#-----------------------------------------------------------------------------#
