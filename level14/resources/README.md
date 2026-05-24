# LEVEL14

## Analysis

Nothing much in the directory:
```bash
    level14@SnowCrash:~$ ls -la
    total 12
    dr-x------ 1 level14 level14  100 Mar  5  2016 .
    d--x--x--x 1 root    users    340 Aug 30  2015 ..
    -r-x------ 1 level14 level14  220 Apr  3  2012 .bash_logout
    -r-x------ 1 level14 level14 3518 Aug 30  2015 .bashrc
    -r-x------ 1 level14 level14  675 Apr  3  2012 .profile
```

After many tries, we didn't find much about the flag14. there might be a way to get it from getflag.
Let's check if we can find it:
```bash
    level14@SnowCrash:~$ find / -name getflag 2>/dev/null
    /bin/getflag
    /rofs/bin/getflag
    level14@SnowCrash:~$ ls -ls /bin/getflag
    12 -rwxr-xr-x 1 root root 11833 Aug 30  2015 /bin/getflag
```

Ok we found getflag and we can execute it!

## Solution

If we run it we don't get much:
```bash
    level14@SnowCrash:~$ /bin/getflag
    Check flag.Here is your token : 
    Nope there is no token here for you sorry. Try again :)
```

First let's try to check what's inside with ltrace:
```bash
    level14@SnowCrash:~$ ltrace /bin/getflag
    __libc_start_main(0x8048946, 1, 0xbffff7f4, 0x8048ed0, 0x8048f40 <unfinished ...>
    ptrace(0, 0, 1, 0, 0)                                        = -1
    puts("You should not reverse this"You should not reverse this
    )                          = 28
    +++ exited (status 1) +++
```
And if we try to run the program in GDB we get:
```bash
    (gdb) run
    Starting program: /bin/getflag 
    You should not reverse this
```


After checking online we can see that ptrace is a tool for anti-debugging.
But there is a quick way to bypass this issue.


Let's try to debug again it with GDB:
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

When we disass the main, we can see, like in level13, that the program is using getuid:
```
[...]
   0x08048afd <+439>:   call   0x80484b0 <getuid@plt>
   0x08048b02 <+444>:   mov    %eax,0x18(%esp)
[...]
```


If we run the program and ckeck the value inside eax after getuid we find 2014:
```
(gdb) break getuid
Breakpoint 2 at 0x80484b0
(gdb) run
Starting program: /bin/getflag 

Catchpoint 1 (call to syscall ptrace), 0xb7fdd428 in __kernel_vsyscall ()

Catchpoint 1 (returned from syscall ptrace), 0xb7fdd428 in __kernel_vsyscall ()

Breakpoint 2, 0xb7ee4cc0 in getuid () from /lib/i386-linux-gnu/libc.so.6
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

One again we have to set the eax to the right id for flag14 we found here:
```bash
flag14@SnowCrash:~$ id flag14
uid=3014(flag14) gid=3014(flag14) groups=3014(flag14),1001(flag)
```

So if we do it we get this result:
```
    (gdb) set $eax=3014
    (gdb) c
    Continuing.
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXXXX
    [Inferior 1 (process 3149) exited normally]
```

Seems like we made it!
```
    level14@SnowCrash:~$ su flag14 
    Password: 
    Congratulation. Type getflag to get the key and send it to me the owner of this livecd :)
```