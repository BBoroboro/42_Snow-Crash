# LEVEL13

## Analysis

Again...
```bash
    level13@SnowCrash:~$ ls -la
    total 20
    dr-x------ 1 level13 level13  120 Mar  5  2016 .
    d--x--x--x 1 root    users    340 Aug 30  2015 ..
    -r-x------ 1 level13 level13  220 Apr  3  2012 .bash_logout
    -r-x------ 1 level13 level13 3518 Aug 30  2015 .bashrc
    -rwsr-sr-x 1 flag13  level13 7303 Aug 30  2015 level13
    -r-x------ 1 level13 level13  675 Apr  3  2012 .profile
    level13@SnowCrash:~$ file level13 
    level13: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0xde91cfbf70ca6632d7e4122f8210985dea778605, not stripped
```

When you launch the bin it says UID 2013 started us but we we expect 4242:
```bash
level13@SnowCrash:~$ ./level13 
UID 2013 started us but we we expect 4242
```

If you disass the bin main with GDB, yo can see the program is using getuid:
```
    (gdb) disass main
    Dump of assembler code for function main:
    0x0804858c <+0>:     push   %ebp
    0x0804858d <+1>:     mov    %esp,%ebp
    0x0804858f <+3>:     and    $0xfffffff0,%esp
    0x08048592 <+6>:     sub    $0x10,%esp
    0x08048595 <+9>:     call   0x8048380 <getuid@plt>
    0x0804859a <+14>:    cmp    $0x1092,%eax
```
Getuid returns the real UID of the calling process. That is the info given by the program once we launch it.


We can break at getuid and check the value of eax it says eax = 2013:
```
(gdb) break getuid
Breakpoint 1 at 0x8048380
(gdb) run
Starting program: /home/user/level13/level13 

Breakpoint 1, 0xb7ee4cc0 in getuid () from /lib/i386-linux-gnu/libc.so.6
(gdb) ni
0xb7ee4cc5 in getuid () from /lib/i386-linux-gnu/libc.so.6
(gdb) print $eax
$3 = 2013
```

## Solution

So the solution here is to set this value to 4242 with the command "set $eax=4242":
```
(gdb) set $eax=4242
(gdb) c
Continuing.
your token is XXXXXXXXXXXXXXXXXXXXXXXXXXXX
```
