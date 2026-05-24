# LEVEL02

## Analysis

With the command "ls -la" we can identify a ".pcap" file:
level02@SnowCrash:~$ ls -la
```bash
    total 24
    dr-x------ 1 level02 level02  120 Mar  5  2016 .
    d--x--x--x 1 root    users    340 Aug 30  2015 ..
    -r-x------ 1 level02 level02  220 Apr  3  2012 .bash_logout
    -r-x------ 1 level02 level02 3518 Aug 30  2015 .bashrc
    ----r--r-- 1 flag02  level02 8302 Aug 30  2015 level02.pcap
    -r-x------ 1 level02 level02  675 Apr  3  2012 .profile
```

These files contain packet data of a network and are used to analyze the network characteristics. They also 
contribute to controlling the network traffic and determining network status.


## Solution
We'll have to download this file on the local machine with the command "scp".
```
    full cmd: scp -P <PORT> <NAME@IP>:<PATH_TO_FILE> <DESTINATION>
    e.g  scp -P 4242 level02@192.168.186.130:/home/user/level02/level02.pcap .
```

Then we need to level02.pcap file's rights to be able to read it.
cmd: chmod 777 level02.pcap

This allows use to use wireshark, a powerful network protocol analyzer. It lets you capture, inspect, and filter packets sent and received over a network (or stored in a .pcap file). But we could also use tcpdump -r file.pcap in terminal-only mode.

For TCP conversations (e.g., HTTP, FTP, Telnet, etc.): Right-click a packet → Follow → TCP Stream (or UDP stream). 
Wireshark reconstructs the conversation in plain text. Great for extracting credentials, messages, or flags in CTFs.

Doing so we find this line Password: "ft_wandr...NDRel.L0L". Trying it as password doesn't work, but if we show the
data as RAW, we can see that the character "." is represented by "7F" which is the equivalent of DEL in the ASCII table.
So if "delete" the "." and the same amount of characters preceding each "." we get "ft_waNDReL0L".

Let's try it:
```bash
    level02@SnowCrash:~$ su flag02
    Password: 
    Don't forget to launch getflag !
    flag02@SnowCrash:~$ getflag
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```

Level Passed!
