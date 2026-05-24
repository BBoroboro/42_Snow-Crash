# LEVEL00

## Basic Rules:

###    How to connect to SnowCrash?

To connect to SnowCrash through ssh, you must use ssh (ssh level00@192.168.186.130 -p 4242) then write the first password to access level00,
which is "level00".

###    How to access next level?

To access level(X) you need to find either flag(X-1) or password(X-1). If you find flag(X-1) then write the command "su flag(X-1)", provide the password, then write "getflag" to get the token (to get 
password(X-1)), or just write "su level(X)" and provide password(X-1) to get to the next level.

##    Analysis

Let's use the command ls -la to display all files with their rights.
```
    dr-xr-x---+ 1 level00 level00  100 Mar  5  2016 .
    d--x--x--x  1 root    users    340 Aug 30  2015 ..
    -r-xr-x---+ 1 level00 level00  220 Apr  3  2012 .bash_logout
    -r-xr-x---+ 1 level00 level00 3518 Aug 30  2015 .bashrc
    -r-xr-x---+ 1 level00 level00  675 Apr  3  2012 .profile
```
Nothing very interesting


We can then use the command id to print user and group information for each specified USER (id [OPTION]... [USER]...).
Let's try it with id flag00:
```
    level00@SnowCrash:~$ id flag00
    uid=3000(flag00) gid=3000(flag00) groups=3000(flag00),1001(flag)
```

Here we can see flag00 belongs to uid 3000.

Let's try to use the command find then.
The command "find / -user flag00 2>/dev/null" shows that we have access to 2 files
```
    /usr/sbin/john
    /rofs/usr/sbin/john
```
## Solution

With the commands "ls -la /usr/sbin/john" then "cat /usr/sbin/john" we can see that we have the rights to read the file:
```
    level00@SnowCrash:~$ ls -la /usr/sbin/john
    ----r--r-- 1 flag00 flag00 15 Mar  5  2016 /usr/sbin/john
    level00@SnowCrash:~$ cat /usr/sbin/john
    cdiiddwpgswtgt
```

We found the flag but it doesn't work, it must be encrypted!
```
    level00@SnowCrash:~$ su flag00
    Password: 
    su: Authentication failure
```

Using dcode enable us decoding this flag. the right answer is "nottoohardhere"
```
    level00@SnowCrash:~$ su flag00
    Password: 
    Don't forget to launch getflag !
    flag00@SnowCrash:~$ getflag
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```

And here is our token to level01:
```
    flag00@SnowCrash:~$ su level01
    Password: 
    level01@SnowCrash:~$ 
```