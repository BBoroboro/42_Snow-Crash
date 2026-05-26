# LEVEL02

## Analysis

The provided environment contains a network capture file (`level02.pcap`), indicating that the challenge involves traffic analysis.

## Enumerating

By listing the home directory contents, the presence of a `.pcap` file is identified:
Output:
```txt
    ----r--r-- 1 flag02  level02 8302 Aug 30  2015 level02.pcap
```

This file contains captured network packets that can be analyzed using Wireshark, a packet analysis tool used to inspect network traffic at different protocol layers.

## Exploitation 

Using `scp` we transfer the file locally for offline analysis:
```txt
    scp -P 4242 level02@192.168.186.130:/home/user/level02/level02.pcap .
    chmod 777 level02.pcap
``` 

### Traffic Analysis

The capture is opened in Wireshark, revealing TCP communication streams.
  <!-- ![TCP Stream Analysis](resources/SN_Wireshark3.png) -->

<p align="center">
  <img src="resources/SN_Wireshark1.png" width="750"/>
</p>

Using Follow TCP Stream, the communication is reconstructed into readable text:

<p align="center">
  <img src="resources/SN_Wireshark2.png" width="750"/>
</p>

This capture appears to represent an interactive remote login session over TCP in which a password-like string is observed `Password: "ft_wandr...NDRel.L0L"`.

<p align="center">
  <img src="resources/SN_Wireshark3.png" width="750"/>
</p>

Raw byte inspection shows that the `.` characters correspond to `0x7f` (DEL), indicating deleted characters in the terminal stream.
So if we remove `.` and the same amount of characters preceeding each `.` we get the following string `ft_waNDReL0L`.

## Result

The recovered password is used to authenticate:
```bash
    level02@SnowCrash:~$ su flag02
    Password: 
    Don't forget to launch getflag !
    flag02@SnowCrash:~$ getflag
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```

N.B: `tcpdump` is another tool we could use to succeed this challenge in terminal-only mode.

## Takeaways

- TCP streams can contain full interactive terminal sessions in plaintext.
- Terminal control characters (e.g. 0x7f) can represent deleted input and must be interpreted for accurate reconstruction.
