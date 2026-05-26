# LEVEL00

##  Analysis

We begin by enumerating the files in the home directory. This will reveal standard user configuration files:
```bash
    $ ls -la
```
Output:
```bash
    dr-xr-x---+ 1 level00 level00  100 Mar  5  2016 .
    d--x--x--x  1 root    users    340 Aug 30  2015 ..
    -r-xr-x---+ 1 level00 level00  220 Apr  3  2012 .bash_logout
    -r-xr-x---+ 1 level00 level00 3518 Aug 30  2015 .bashrc
    -r-xr-x---+ 1 level00 level00  675 Apr  3  2012 .profile
```
No immediate sensitive information is present in the directory listing. 

##  Enumeration

To identify potential targets belonging to flag00, we will inspect system users:
```bash
    level00@SnowCrash:~$ id flag00
    uid=3000(flag00) gid=3000(flag00) groups=3000(flag00),1001(flag)
```

We then search for files owned by this user:
```Bash
    find / -user flag00 2>/dev/null
```

Result:
```
    /usr/sbin/john
    /rofs/usr/sbin/john
```
## Investigation

We inspect the file permissions `ls -la /usr/sbin/john`:
```
    level00@SnowCrash:~$ ls -la /usr/sbin/john
    ----r--r-- 1 flag00 flag00 15 Mar  5  2016 /usr/sbin/john

```
The file is readable by other users, allowing direct content extraction:
```
    level00@SnowCrash:~$ cat /usr/sbin/john
    cdiiddwpgswtgt
```

##  Exploitation

The extracted string does not appear to be usable as a password and appears to be encoded.
```
    level00@SnowCrash:~$ su flag00
    Password: 
    su: Authentication failure
```

Using dcode, the string can be decoded using a Caesar cipher analysis. The plaintext is: <nottoohardhere>
```
    level00@SnowCrash:~$ su flag00
    Password: 
    Don't forget to launch getflag !
    flag00@SnowCrash:~$ getflag
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```

## Takeaways

- Simple encoding (e.g: Caeser cipher, etc) are not enough to protect sensitive information like passwords.