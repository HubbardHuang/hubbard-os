CPP_SOURCES = $(shell find .. -name "*.cpp")
CPP_OBJECTS = $(patsubst %.cpp, %.o, $(CPP_SOURCES))
S_SOURCES = $(shell find .. -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

ADDITIONAL_INCLUDE =								\
	-I ../source/interface/type						\
	-I ../source/interface/string					\
	-I ../source/kernel/entry						\
	-I ../source/kernel/device_drivers/console		\
	-I ../source/kernel/device_drivers/interrupt	\
	-I ../source/kernel/device_drivers/memory 		\
	-I ../source/kernel/device_drivers/timer		\
	-I ../source/kernel/device_drivers				\
	-I ../test/console_test
CC = g++
LD = ld
ASM = nasm

CPP_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-threadsafe-statics -fno-stack-protector $(ADDITIONAL_INCLUDE)
LD_FLAGS = -T hubbard_os.ld -m elf_i386 -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: $(S_OBJECTS) $(CPP_OBJECTS) link update_image clean qemu

.cpp.o:
	@echo Compiling $< ...
	@$(CC) $(CPP_FLAGS) $< -o $@

.s.o:
	@echo Compiling $< ...
	@$(ASM) $(ASM_FLAGS) $<

link:
	@echo Linking object files to generate an executable file...
	@$(LD) $(LD_FLAGS) $(S_OBJECTS) $(CPP_OBJECTS) -o hubbard_os

.PHONY:clean
clean:
	@echo Removing all object files...
	@$(RM) $(S_OBJECTS) $(CPP_OBJECTS)

.PHONY:update_image
update_image:
	@echo Updating the image...
	@sudo mount disk.img /mnt/kernel
	@sudo cp hubbard_os /mnt/kernel/hubbard_os
	@sleep 1
	@sudo umount /mnt/kernel

.PHONY:mount_image
mount_image:
	sudo mount disk.img /mnt/kernel

.PHONY:umount_image
umount_image:
	sudo umount /mnt/kernel

.PHONY:qemu
qemu:
	@echo ----------------------------------------------
	@echo Now booting HubbardOS kernel with Qemu ...
	@qemu -fda disk.img -boot a

.PHONY:bochs
bochs:
	bochs -f scripts/bochsrc.txt

.PHONY:debug
debug:
	qemu -S -s -fda disk.img -boot a &
	sleep 1
	cgdb -x scripts/gdbinit