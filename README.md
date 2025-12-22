# InjectByte-OS

**Developing a Multithreaded Kernel from Scratch**

InjectByte-OS is a low-level operating system project built from the ground up in **C** and **x86 Assembly**, designed to explore kernel development concepts such as multitasking, memory management, and hardware interaction. This repository contains the source code and resources used to develop the kernel.

---

## About

Just Curious About how The OS works. So why not make one Yourself. 

### Key Concepts Covered
- Boot process and real mode setup
- Interrupt handling
- Memory management and paging
- Multithreading and task scheduling
- Disk I/O and filesystem basics
- ELF loader implementation
- Virtual filesystem design
- Kernel debugging in emulators like QEMU

---

## Prerequisites

- Knowledge of **C programming**
- Basic understanding of **x86 Assembly**
- Familiarity with command-line tools (optional but helpful)
- Download a C Cross Compiler "Must Find how? if You wanna Learn"


---

## Features

- Custom bootloader
- Protected mode kernel
- Interrupt Descriptor Table (IDT) implementation
- Paging and heap memory management
- Simple multitasking and scheduling
- Disk and keyboard drivers
- ELF file loading support

---

## Getting Started

1. Cross Compiler
  ``` bash
Edit build.sh File according to Compiler's Path
  ```
2. Clone the repository:
   ```bash
   git clone https://github.com/IJBaig/InjectByte-OS.git
   cd InjectByte-OS
   ```
3. Build the kernel:
  ```bash
make clean
./build.sh
  ```
4. Run in QEMU:
  ```bash
qemu-system-i386 -kernel bin/os.bin
  ```
## Learning Outcomes

  ### By exploring this project, you will gain hands-on experience in:

  - Building a kernel from scratch
  
  - Understanding memory and CPU architecture
  
  - Differentiating between kernel space and user space
  
  - Implementing basic multitasking
  
  - Designing low-level drivers and system components
  
  - Debugging using emulators and GDB

## Credits
Special thanks to my mentor and instructor, **Daniel McCarthy** , whose guidance and course materials were invaluable in developing this kernel project.
## License

This project is licensed under the GNU GENERAL PUBLIC LICENSE



