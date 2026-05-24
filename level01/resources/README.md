# LEVEL01

## Analysis

Since we don't get much with the commands used in level00. Let's try to inspect the /etc directory. Historically UNIX stored 
user account records and password hashes in /etc/passwd, but modern systems move the actual password hashes into /etc/shadow
for security. To fix that, modern UNIX-like systems (Linux, many BSDs) use /etc/shadow to store the actual password hashes and
related password aging info

```
    level01@SnowCrash:~$ ls -la /etc/passwd && ls -la /etc/shadow
    -rw-r--r-- 1 root root 2477 Mar  5  2016 /etc/passwd
    -rw-r----- 1 root shadow 4428 Mar  6  2016 /etc/shadow
```

We can't read the shadow file:
```bash
    level01@SnowCrash:~$ cat /etc/shadow
    cat: /etc/shadow: Permission denied
```

Though we can read the /etc/passwd file and it gives us the following:
```bash
    level01@SnowCrash:~$ cat /etc/passwd
    [...]
    flag00:x:3000:3000::/home/flag/flag00:/bin/bash
    flag01:42hDRfypTqqnw:3001:3001::/home/flag/flag01:/bin/bash
    flag02:x:3002:3002::/home/flag/flag02:/bin/bash
    [...]
```
## Solution
We found flag01 but it is encryted... The solution is to use John The Ripper!
Download JTR and create a file.txt then use "john" on the file.txt with the --show flag.
```
    > In a different shell
    cmds:
    - echo <hashed_pwd> > hash.txt
    - <john_exec> --show hash.txt
```

We then gets the flag01 decrypted: ?:abcdefg. And if we submit it we get the token for level02.
```bash
    level01@SnowCrash:~$ su flag01
    Password: 
    Don't forget to launch getflag !
    flag01@SnowCrash:~$ getflag
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```
