# LEVEL09

## Analysis

## Enumeration

Once again we found a binary and a token file:
```bash
    level09@SnowCrash:~$ ls -la
    [...]
    -rwsr-sr-x 1 flag09  level09 7640 Mar  5  2016 level09
    ----r--r-- 1 flag09  level09   26 Mar  5  2016 token
```

## Investigation

Unlike the previous challenge, we can read the token file with the binary:
```bash
    $ ./level09 token
    tpmhr
```

Trying to read the content of `token` or give it to `level09` does not prove relevant neither:
```bash
    $ cat token
    f4kmm6p|=�p�n��DB�Du{��
    $ ./level09 $(cat token)
    f5mpq;v�E��{�{��TS�W�����
```


Futher investigations help understand how this program works
```bash
    $ ./level09 token
    tpmhr
    $ ./level09 salut
    sbnxx
    $ ./level09 yoyo
    yp{r
    $ ./level09 1234
    1357
    $ ./level09 123456
    13579;
```

We understand a certain transformation pattern. Indeed, observation of ASCII offsets reveals that each character is shifted by its position index.

## Exploitation

Our strategy is to download the toke to our local machine:

```
    scp -P 4242 level09@192.168.186.130:/home/user/level09/token .
    chmod 777 token
```
and try to reverse the process done by the initial program. For that purpose we wrote a short program (see directory) that substracts each character by its own index.

If we cat the token and give it to our program:
```
    ./a.out $(cat token)
    f3iji1ju5yuevaus41q1afiuq
```

## Result 

```bash
    level09@SnowCrash:~$ su flag09
    Password: 
    Don't forget to launch getflag !
    flag09@SnowCrash:~$ getflag
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXXXX
    
```
