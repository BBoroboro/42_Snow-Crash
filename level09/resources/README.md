# LEVEL09

## Analysis

Once again let's check what we have in out directory:
```bash
    level09@SnowCrash:~$ ls -la
    total 24
    dr-x------ 1 level09 level09  140 Mar  5  2016 .
    d--x--x--x 1 root    users    340 Aug 30  2015 ..
    -r-x------ 1 level09 level09  220 Apr  3  2012 .bash_logout
    -r-x------ 1 level09 level09 3518 Aug 30  2015 .bashrc
    -rwsr-sr-x 1 flag09  level09 7640 Mar  5  2016 level09
    -r-x------ 1 level09 level09  675 Apr  3  2012 .profile
    ----r--r-- 1 flag09  level09   26 Mar  5  2016 token
```

Like level08, we have a binary ./level09 and a file token but this time we can read it.
```bash
    level09@SnowCrash:~$ ./level09 token
    tpmhr
```

If we use cat on "token" we don't get much...:
```bash
    level09@SnowCrash:~$ cat token
    f4kmm6p|=�p�n��DB�Du{��
```

same if we try to use the exec on the value inside "token":
```bash
    level09@SnowCrash:~$ ./level09 $(cat token)
    f5mpq;v�E��{�{��TS�W�����
```

Mmmmmh that's a rather strange result, maybe we should try to change the input we give to the exec
and see what happens:
```bash
    level09@SnowCrash:~$ ./level09 token
    tpmhr
    level09@SnowCrash:~$ ./level09 salut
    sbnxx
    level09@SnowCrash:~$ ./level09 yoyo
    yp{r
    level09@SnowCrash:~$ ./level09 1234
    1357
    level09@SnowCrash:~$ ./level09 123456
    13579;
```

We can see a pattern here, if we analyse it with our small knowledge of ASCII it seems
the program adds to each charactere its own index. (see last two commands).

## Solution
the next step is to download on our local machine the token to try to decrypt it:
```
    scp -P 4242 level09@192.168.186.130:/home/user/level09/token .
    chmod 777 token
    > cmd: scp -P <PORT> <NAME@IP>:<PATH> <DESTINATION> 
```

If we cat again same result so let's try to write a script to decrypt the token (see file):
```
    ./a.out $(cat token)
    f3iji1ju5yuevaus41q1afiuq
```

Now let's try to get the flag:
```bash
    level09@SnowCrash:~$ su flag09
    Password: 
    Don't forget to launch getflag !
    flag09@SnowCrash:~$ getflag
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXXXX
    
```
