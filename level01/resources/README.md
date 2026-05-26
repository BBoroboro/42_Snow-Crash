# LEVEL01

## Analysis

In this challenge we first inspect the `/etc` directory. Historically UNIX stored password hashes directly inside `/etc/passwd`. Nevertheless, modern systems moved actual password hashes into `/etc/shadow` for security reasons. 

## Enumeration

```bash
    ~$ ls -la /etc/passwd && ls -la /etc/shadow
```
Output:
```txt
    -rw-r--r-- 1 root root 2477 Mar  5  2016 /etc/passwd
    -rw-r----- 1 root shadow 4428 Mar  6  2016 /etc/shadow
```

## Investigation

It is not possible to read `/etc/shadow`, though any user can read `/etc/passwd`:
```bash
    $ cat /etc/passwd
```
Output:
```txt
    [...]
    flag00:x:3000:3000::/home/flag/flag00:/bin/bash
    flag01:42hDRfypTqqnw:3001:3001::/home/flag/flag01:/bin/bash
    flag02:x:3002:3002::/home/flag/flag02:/bin/bash
    [...]
```


## Exploitation

The value associated with `flag01` appears to be a password hash. John The Ripper (JTR) is then used to recover the associated plaintext password. We use the dockerfile to create a docker with JTR already installed.

```bash
    $ docker build -t level01 .
    $ docker run -it level01
```

Once inside the container, run the following commands to decode the hashed password:
```bash
    # touch hash.txt
    # echo -n "42hDRfypTqqnw" > hash.txt
    # john hash.txt --show
```
Output:
```txt
[...]
    ?:abcdefg
[...]
```

We then need to submit this decoded flag to get to the next level:
```bash
    level01@SnowCrash:~$ su flag01
    Password: 
    Don't forget to launch getflag !
    flag01@SnowCrash:~$ getflag
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```

## Takeaways

- Improper password hash exposure can lead to credential compromise.
- Separating password hashes into `/etc/shadow` reduces attack surface by restricting access to privileged users only.
- Weak or outdated hashing mechanisms remain vulnerable to password cracking tools such as John The Ripper.