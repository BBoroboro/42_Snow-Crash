# LEVEL13

## Analysis

## Enumeration

We identify a setuid binary:
```bash
    [...]
    -rwsr-sr-x 1 flag13  level13 7303 Aug 30  2015 level13
```
```bash
    level13@SnowCrash:~$ file level13 
    level13: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0xde91cfbf70ca6632d7e4122f8210985dea778605, not stripped
```

Running the program displays the following message:
```bash
level13@SnowCrash:~$ ./level13 
UID 2013 started us but we we expect 4242
```
The program appears to verify the caller UID before revealing the token.

## Investigation
Disassembling the main function with GDB reveals the following logic: , gives another clue. The program calls `getuid`, compares its returned value with `0x1092` and jumps if equal:
```
    (gdb) disass main
    [...]
    0x08048595 <+9>:     call   0x8048380 <getuid@plt>
    0x0804859a <+14>:    cmp    $0x1092,%eax
    0x0804859f <+19>:    je     0x80485cb <main+63>
```
The program calls `getuid()`, compares its returned value with `0x1092` and jumps if equal.
Indeed, getuid returns the real UID of the calling process. Though `0x1092` in decimal is equal to 4242, the value expected by the program.

This confirms the program only proceeds if the real UID equals 4242.
We can verify the current UID at runtime:
```
(gdb) break getuid
Breakpoint 1 at 0x8048380
(gdb) run
(gdb) ni
0xb7ee4cc5 in getuid () from /lib/i386-linux-gnu/libc.so.6
(gdb) print $eax
$3 = 2013
```

## Exploitation

Since the binary performs the UID verification client-side, we can modify the register value during execution using GDB.
Before the comparison executes, we overwrite eax with the expected UID. This can be done in gdb with the command `set $eax=4242`

```txt
    (gdb) set $eax=4242
    (gdb) c
    Continuing.
    your token is XXXXXXXXXXXXXXXXXXXXXXXXXXXX
```

## Takeaways

- Client-side security checks can be bypassed through runtime manipulation
- GDB allows direct modification of registers and execution flow
- Authentication logic embedded in binaries should never rely solely on local UID comparisons