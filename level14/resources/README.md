# LEVEL14

## Analysis

## Enumeration

Enumerating the home directory and looking for files related to `flag14` proved irrelevant, 

The remaining attack surface is the binary responsible for validating flags: `getflag`
We first locate the executable:
```bash
    $ find / -name getflag 2>/dev/null
    /bin/getflag
    /rofs/bin/getflag
```
```
    level14@SnowCrash:~$ ls -ls /bin/getflag
    12 -rwxr-xr-x 1 root root 11833 Aug 30  2015 /bin/getflag
```

Running the program directly does not provide a valid token:
```bash
    level14@SnowCrash:~$ /bin/getflag
    Check flag.Here is your token : 
    Nope there is no token here for you sorry. Try again :)
```

## Investigation

Using `ltrace` immediately reveals suspicious behavior:
```bash
    level14@SnowCrash:~$ ltrace /bin/getflag
    __libc_start_main(0x8048946, 1, 0xbffff7f4, 0x8048ed0, 0x8048f40 <unfinished ...>
    ptrace(0, 0, 1, 0, 0)                                        = -1
    puts("You should not reverse this"You should not reverse this
    )                          = 28
    +++ exited (status 1) +++
```
Running the binary inside GDB produces the same message:
```bash
    (gdb) run
    Starting program: /bin/getflag 
    You should not reverse this
```

The binary uses `ptrace` as an anti-debugging mechanism.

## Exploitation

### Step 1: Bypass Ptrace

Ptrace is easily bypassed by setting its return value to 0.

This can be done before running the program in gdb:
```bash
(gdb) catch syscall ptrace
Catchpoint 1 (syscall 'ptrace' [26])
(gdb) commands 1
Type commands for breakpoint(s) 1, one per line.
End with a line saying just "end".
>set ($eax) = 0
>continue
>end
```

### Step 2: Identify the UID Validation

Disassembling the main function proved the program usies getuid():
```
[...]
   0x08048afd <+439>:   call   0x80484b0 <getuid@plt>
   0x08048b02 <+444>:   mov    %eax,0x18(%esp)
[...]
```

The value inside EAX after getuid() we is 2014:
```
    (gdb) break getuid
    Breakpoint 2 at 0x80484b0
    (gdb) run
    (gdb) step
    Single stepping until exit from function getuid,
    which has no line number information.
    0x08048b02 in main ()
    (gdb) info registers
    eax            0x7de    2014
    ecx            0xb7fda000       -1208115200
    edx            0x20     32
    ebx            0xb7fd0ff4       -1208152076
    esp            0xbffff610       0xbffff610
    ebp            0xbffff738       0xbffff738
    esi            0x0      0
    edi            0x0      0
    eip            0x8048b02        0x8048b02 <main+444>
    eflags         0x200296 [ PF AF SF IF ID ]
    cs             0x73     115
    ss             0x7b     123
    ds             0x7b     123
    es             0x7b     123
    fs             0x0      0
    gs             0x33     51
```

Now we have to find to whom belongs `flag14`:
```bash
flag14@SnowCrash:~$ id flag14
uid=3014(flag14) gid=3014(flag14) groups=3014(flag14),1001(flag)
```

### Step 3: Patch the Runtime Register

Now if we switch the value returned by getuid() to the proper owner UID:
```
    (gdb) set $eax=3014
    (gdb) c
    Continuing.
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXXXX
    [Inferior 1 (process 3149) exited normally]
```

## Result
```
    level14@SnowCrash:~$ su flag14 
    Password: 
    Congratulation. Type getflag to get the key and send it to me the owner of this livecd :)
```