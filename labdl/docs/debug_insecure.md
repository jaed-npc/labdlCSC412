# Debug Insecure Code

### Stack Trace

##### Results with gets()
```
jayflo@Jaytop:~/csc412-devenv/home/labs/labdl/insecure$ export LSAN_OPTIONS="detect_leaks=0"
jayflo@Jaytop:~/csc412-devenv/home/labs/labdl/insecure$ gdb ./main
GNU gdb (Ubuntu 12.1-0ubuntu1~22.04.2) 12.1
Copyright (C) 2022 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.
=============================================================
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./main...
(gdb) break gets
Breakpoint 1 at 0x1190
(gdb) run
Starting program: /home/jayflo/csc412-devenv/home/labs/labdl/insecure/main 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
[Detaching after fork from child process 48600]
Enter some text: ABCDEFGHIJKLMNOPQRSTUVWXYZ
Mem not shared. 9
Mem shared. 5
[Inferior 1 (process 48597) exited normally]
(gdb) print buffer
$1 = '\000' <repeats 29 times>
(gdb) print shared_memory
No symbol "shared_memory" in current context.
(gdb) print *shared_memory
No symbol "shared_memory" in current context.
(gdb) print m
$2 = 9
(gdb) bt
No stack.
(gdb) continue
The program is not being run.
(gdb) print m
$3 = 9
(gdb) continue
The program is not being run.
(gdb) print m
$4 = 9
(gdb) 
```

##### Results with fgets()
```
jayflo@Jaytop:~/csc412-devenv/home/labs/labdl/insecure$ gdb ./main
GNU gdb (Ubuntu 12.1-0ubuntu1~22.04.2) 12.1
Copyright (C) 2022 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./main...
(gdb) break main
Breakpoint 1 at 0x13c8: file main.c, line 15.
(gdb) run
Starting program: /home/jayflo/csc412-devenv/home/labs/labdl/insecure/main 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Breakpoint 1, main (argc=1, argv=0x7fffffffdd18) at main.c:15
15          uint8_t *shared_memory = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
(gdb) print buffer
$1 = "\352\264W\367\377\177\000\000\005\000\000\000p`\000"
(gdb) print m
$2 = 9
(gdb) bt
#0  main (argc=1, argv=0x7fffffffdd18) at main.c:15
(gdb) print shared_memory
$3 = (uint8_t *) 0x607000000020 "\001"
(gdb) print buffer
$4 = "\352\264W\367\377\177\000\000\005\000\000\000p`\000"
(gdb) print m
$5 = 9
(gdb) print shared_memory
$6 = (uint8_t *) 0x607000000020 "\001"
(gdb) bt
#0  main (argc=1, argv=0x7fffffffdd18) at main.c:15
(gdb) print buffer
$7 = "\352\264W\367\377\177\000\000\005\000\000\000p`\000"
(gdb) print m
$8 = 9
(gdb) print shared_memory
$9 = (uint8_t *) 0x607000000020 "\001"
(gdb) continue
Continuing.
[Detaching after fork from child process 52410]
Enter some text: ABCDEFGHIJKLMNOPQRSSTUV
Mem not shared. 9
Mem shared. 5
[Inferior 1 (process 51625) exited normally]
(gdb) print buffer
$10 = '\000' <repeats 29 times>
(gdb) print m
$11 = 9
(gdb) print shared_memoru
No symbol "shared_memoru" in current context.
(gdb) print shared_memory
No symbol "shared_memory" in current context.
(gdb) continue
The program is not being run.
(gdb) 
```

##### Describe the differences between the stack traces

### With gets()
- When I used the bt command it outputs "No stack" after execution.  
  - I believe this may suggest that an overflow may have disrupted the stack, or caused the program to crash suddenly 
- The program detaches after forking, which may have prevented a complete trace.  
- The buffer appears empty (\000 repeated), which is probably stack corruption?  
- shared_memory is not accessible in GDB, which I likely believe is due to memory corruption.  

### With fgets()
- The stack trace remains intact (bt shows main at line 15).  
- buffer, m, and shared_memory retain valid values, suggesting memory integrity is maintained.  
- There is no evidence of a buffer overflow... data is properly constrained within the buffer.  


## Memory Corruption
### With gets()
- The program's behavior seems unpredictable due to an overflow.  
- The buffer is filled with '\000' repeated values, suggesting that overflow may have written over.
- shared_memory is inaccessible, possibly due to stack corruption affecting memory mapping.  

### With fgets()
- The buffer contains valid values, and shared_memory remains accessible.  
- The stack frame seems stable, and the program continues execute continuously  

##  Program Stability
### gets() causes uncontrolled execution
- The lack of bounds and edge cases checking leads to stack corruption from possibly overflow.   
- Important variables (shared_memory, m) become unreliable as it randomly changes from malallocation.  

### fgets() has more controlled execution
- The input is limited by the buffer size, preventing overflow and strict contstraints.
  
- The program runs without unexpected termination.  
- Stack is easy to follow and remains intact.  

## Overall contrast
I believe the key difference is that gets() introduces a buffer overflow, leading to memory corruption, stack overflow/corruption, and unpredictable behavior.  
On the other hand, fgets() prevents overflow, allowing memory safety and program stability.  




### Develop your Intuition

Answer the questions below as a group.

##### Describe steps on how to recreate to a buffer overflow when gets() is used.

## How does it work?
gets() is an unsafe function because it doesn't check the size of the buffer you're writing to. This makes it vulnerable to buffer overflow attacks, where too much data is written into a buffer and overflows into any memory, thus corrupting it. This vulnerability can be recreated.

## Steps to Recreate a Buffer Overflow with gets()

 Prepare a Program with gets()
    - Make sure to write a program that uses gets() to read input from the user:
    example: 
      #include <stdio.h>
      #include <stdlib.h>

      int main() {
          char buffer[20]; 
 // A small buffer to hold user input
          printf("Input: ");
          gets(buffer);  
// Unsafe function that doesn't check buffer size
          printf("Input given: %s\n", buffer);
          return 0;
      }
 

 Compile the Program with flags
    - Compile the program without any security protections, or buffer overflow protections:
     
      gcc -g -o program program.c
      

 To recreate the over flow make sure to provide input larger than the Buffer
    - Run the program and enter a string that exceeds the size of the buffer. For example, if the buffer is 20 bytes, so you can try entering a string that is 30 characters long:
     
      Input : ABCDEFGHIJKLMNOPQRSTUVWXYZ1234
  

 Observe the Overflow in ACTION
    - The program should crash, or there might be unusual behavior because gets doesn't limit the input size, causing the input to overflow into adjacent memory, creating a stack overflow error
    - In the case of a crash, it is likely because the overflow overwrote important memory, which could be the return address or local variables.

 Result
    - The buffer overflow can result in:
        - Corruption of adjacent memory (such as other variables, control structures, or even the program's return address).
    
## Why This Happens?
The problem with gets() is that it doesn't check the size of the input before writing it into the buffer. If the user inputs more data than the buffer can hold, the extra data will overwrite memory randomly, which recreates buffer overflow.

By using gets(), you expose your program to serious vulnerabilities. This is why gets() is considered unsafe and has been deprecated. It's always safer to use fgets() because it respects buffer boundaries and prevent such overflows.


##### Describe the change in values of buffer, shared_memory, and m as a result of overflow.
In the program that was vulnerable to buffer overflow by using get(), the overflow caused unintended modifications to memory, corrupting variables that are located close to the buffer. This can lead to changes in the values of critical variables like buffer, shared_memory, and m.

## Key Variables in the Program
- buffer: A local character array that stores user input.
- shared_memory: A pointer to a memory region shared between processes.
- m: integer variable used for counting or holding some state.

When a buffer overflow occurs, input that exceeds the capacity of var buffer will spill over into nearby memory,  corrupting the values of these variable.

## Changes in Values Due to Buffer Overflow

### Buffer
   - Before the overflow: buffer is initialized to hold a fixed number of characters, such as 20 bytes. It will store whatever the user inputs, as long as the input does not exceed this size.
   - During the overflow: When the input exceeds the buffer's capacity, the excess data will overwrite into nearby memory. The buffer will still contain the original user input.... up to its capacity, but the remaining portion of the input will corrupt memory areas beyond it.
   - After the overflow: The buffer's value will likely be a mix of the intended user input and any excess data that overflowed into it casuing random output. This could result in incomplete or corrupted data being stored in buffer.

   Example:
   buffer = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

### shared_memory
- Before the overflow: shared_memory is allocated to point to a region of memory that is accessible by multiple processes. It is typically used to store data that needs to be shared.
 - During the overflow: If the buffer is located next to shared_memory in memory, the overflow can overwrite the address or contents of shared_memory. This could lead to it pointing to incorrect memory locations or holding corrupted data.
- After the overflow: The value of shared_memory may now point to an unexpected location in memory, or it might contain incorrect data. This could cause crashes or unexpected behavior when other processes attempt to access it.

### m

- Before the overflow: m is an int that holds some value . It’s allocated in memory just like any other local variable.
- During the overflow: The overflow from buffer can overwrite the memory used by m, changing its value. Since m is a int, the overflow might disrupt it with random values, depending on the input size.
After the overflow: The value of m will likely be altered to some random or garbage value, which could affect the logic of the program if m is used in calculations or conditions later in the program.


A buffer overflow can result in unintended changes to memory variables like buffer, shared_memory, and m. The values of these variables become corrupted due to the overflow of data into nearby memory locations. This can lead to unpredictable behavior, crashes, or even or data leakage. Proper bounds checking is improtant to prevent these types of overflows and protect program memory.

##### Provide a short intuitive explanation of how gets() causes an error and how fgets() fixes this. 

## The issue with gets()
The function gets() is notorious for being dangerous because it simply does not check the size of the input it reads. Meaning, that if you try to store more data than the allocated space in a buffer, gets() will overflow the buffer, leading to corrupted memory. This can lead to several serious issues:

- Buffer Overflow: When more data is provided than the buffer can handle,the data is prone to spill over into nearby memory, corrupting other variables any causing memory leaks.
- Security Vulnerabilities: A buffer overflow could allow an attacker to inject malicious data into the program's memory, potentially leading to other exploits.
  
For example, if we define a buffer of size 10 and input more than 10 characters with gets(), it will overwrite the memory that follows the buffer, which can cause unpredictable behavior.


char buffer[10];
gets(buffer); 
 // This is bad practice!

### How fgets() fixes the problem
Unlike gets(), fgets() is much safer because it allows you to specify the maximum number of characters to read. This prevents the function from reading more data than the buffer can handle, preventing buffer overflow.

##### How It Works:
fgets() takes three arguments: the buffer, the size of the buffer, and the input source , usually stdin.
It will only read up to size-1 characters, reserving space for the null terminator and will stop if it reaches the end of the buffer or if it comes across a newline character.
By setting a limit on the number of characters it reads, fgets() ensures that the buffer cannot be overflowed, making it a safer alternative to gets().

Example:

char buffer[10];
fgets(buffer, sizeof(buffer), stdin);  
// Safe and good practice!

- In this case, fgets() will read up to 9 characters (leaving space for the null terminator) and prevent any overflow.

In short, gets() causes errors because it does not check the size of the buffer and allows excessive data to overwrite memory, creating potential security risks. On the other hand, fgets() fixes this by limiting the number of characters it reads based on the buffer size, preventing overflows and making it a much safer choice.



##### Provide a short intuitive explanation of how GDB to analyze programs with parent/child processes and shared_memory.

### Parent and child processes
When a program calls fork(), it creates a child process that is a copy of the parent. From this point on, both processes run in parallel. They each have their own memory space, but they can share certain resources like memory if it's set up to do so....

### Shared memory
Shared memory is a way for multiple processes to access the same region of memory. This can be useful when you want both the parent and child to share information efficiently. When using shared memory, both processes can read from and write to the same memory area, but one process's changes might affect the other.

## How GDB helps analyze these scenarios

### ** Following Parent and Child Processes**

By default, when you run a program in GDB and it forks, the debugger will ONLY follow the parent process. To track both the parent and child processes, you can use the set follow-fork-mode command:

- set follow-fork-mode parent : GDB will follow the parent process after a fork.
- set follow-fork-mode child: GDB will follow the child process after a fork.

This lets you control which process you want to debug after a fork.

example: 
(gdb) set follow-fork-mode child

In this case it was useful in analyzing the behavior of the child process.

### Analyzing Shared Memory
 To inspect or modify shared memory while debugging:

Use GDB's print command to examine variables that reference shared memory.
If you know the address of the shared memory region, you can directly inspect the memory content by using commands like to examine memory.
For example, if shared memory is mapped to a specific address, you can inspect it like this:

(gdb) print *shared_memory

This will print the value at the address of shared_memory. If the child process modifies it, you can observe those changes in the parent process as well, since both processes share that memory.

### Using Breakpoints
You can set breakpoints in both the parent and child processes to pause execution at certain points. If the child process hits the breakpoint, you can switch between the parent and child contexts using GDB's commands.

Setting breakpoints in the parent:

(gdb) break main  
# Break at the main function

Setting breakpoints in the child process:

(gdb) break some_child_function  
# Break at a function called in the child process

### Stack Traces and Process Switching
GDB allows you to switch between processes during debugging. After a fork(), you can see the list of processes and switch between them using commands. This helps in viewing stack traces for both the parent and child process independently.
example: 

(gdb) info inferiors
(gdb) inferior 2  
# Switch to the second process (child)

### Overview

- Parent/Child Process Debugging: GDB lets you decide whether to follow the parent or the child process after a fork. This is useful to analyze the behavior of each process independently.

- Shared Memory Analysis: GDB allows you to inspect and modify shared memory regions used by both the parent and child. This is common practice when working with a program that consists of both processes interacting with the same memory.

- Breakpoints and Process Switching: Set breakpoints in both processes and switch between them to analyze the execution flow.

By using these tools, GDB makes it easier to debug complex programs that involve multiple processes and shared memory.
