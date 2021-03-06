# WinAVR Sample makefile written by Eric B. Weddington, et al.
# Released to the Public Domain
# Please read the make user manual!
#
#
# On command line:
# make all = Make software.
# make clean = Clean out built project files.
# make coff = Convert ELF to COFF using objtool.
#
# To rebuild project do "make clean" then "make all".
#


# MCU name
MCU = atmega32u4

AVRDUDEMCU = m32u4

# Output format. (can be srec, ihex)
FORMAT = ihex

# Target file name (without extension).
TARGET = $(shell basename "$(CURDIR)")
# Optimization level (can be 0, 1, 2, 3, s) 
# (Note: 3 is not always the best optimization level. See avr-libc FAQ)
OPT = 2


# List C source files here. (C dependencies are automatically generated.)
#SRC=$(wildcard /**/*.c)

SRC=$(shell find src -type f -iname '*.c')

# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC = 




# Optional compiler flags.
CFLAGS = -g -O$(OPT) -funsigned-char -funsigned-bitfields -fpack-struct \
-fshort-enums -Wall -Wa,-ahlms=$(<:.c=.lst)

# Optional assembler flags.
ASFLAGS = -Wa,-ahlms=$(<:.S=.lst),-gstabs 

# Optional linker flags.
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref

# Additional libraries
#
# Minimalistic printf version
#LDFLAGS += -Wl,-u,vfprintf -lprintf_min
#
# Floating point printf version (requires -lm below)
#LDFLAGS +=  -Wl,-u,vfprintf -lprintf_flt
#
# -lm = math library
#LDFLAGS += -lm



# ---------------------------------------------------------------------------

# Define directories, if needed.
DIRAVR = c:/winavr
DIRAVRBIN = $(DIRAVR)/bin
DIRAVRUTILS = $(DIRAVR)/utils/bin
DIRINC = .
DIRLIB = $(DIRAVR)/avr/lib


# Define programs and commands.
SHELL = sh

CC = avr-gcc

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

REMOVE = rm -f
COPY = cp

ELFCOFF = objtool

HEXSIZE = avr-size --target=$(FORMAT) $(TARGET).hex
ELFSIZE = avr-size -A $(TARGET).elf

FINISH = echo Errors: none
BEGIN = echo -------- begin --------
END = echo --------  end  --------




# Define all object files.
OBJ = $(SRC:.c=.o) $(ASRC:.S=.o) 

# Define all listing files.
LST = $(ASRC:.S=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags. Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)



# Default target.
all: clean begin gccversion $(TARGET).elf $(TARGET).hex $(TARGET).eep \
$(TARGET).lss finished end


# Eye candy.
begin:
	@$(BEGIN)

finished:
	@$(FINISH)

end:
	@$(END)



# Display compiler version information.
gccversion : 
	$(CC) --version


# Target: Convert ELF to COFF for use in debugging / simulating in AVR Studio.
coff: $(TARGET).cof end

%.cof: %.elf
	-$(OBJCOPY) --debugging --change-section-address .data-0x800000 --change-section-address .bss-0x800000 --change-section-address .noinit-0x800000 --change-section-address .eeprom-0x810000 -O coff-ext-avr $< $@
#  $(ELFCOFF) loadelf $< mapfile $*.map writecof $@




# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@



# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)


# Compile: create object files from C source files.
%.o : %.c
	$(CC) -c $(ALL_CFLAGS) $< -o $@


# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
%.o : %.S
	$(CC) -c $(ALL_ASFLAGS) $< -o $@






# Target: clean project.
clean: begin clean_list finished end

clean_list :
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(OBJ)
	$(REMOVE) $(LST)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)


# Automatically generate C source code dependencies. 
# (Code originally taken from the GNU make user manual and modified (See README.txt Credits).)
# Note that this will work with sh (bash) and sed that is shipped with WinAVR (see the SHELL variable defined above).
# This may not work with other shells or other seds.
%.d: %.c
	set -e; $(CC) -MM $(ALL_CFLAGS) $< \
	| sed 's,\(.*\)\.o[ :]*,\1.o \1.d : ,g' > $@; \
	[ -s $@ ] || rm $@


# Remove the '-' if you want to see the dependency files generated.
-include $(SRC:.c=.d)



# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion coff clean clean_list

	
writeflash:	
	writeflash $(TARGET)

program:
	avrdude -c usbasp -p $(AVRDUDEMCU) -U flash:w:$(TARGET).hex -U lfuse:w:0xde:m -U hfuse:w:0xd9:m -U efuse:w:0xf3:m -B 3