# Snow Crash

## Overview

This repository contains structured writeups for the **Snow Crash** project from 42 School.

Snow Crash is an introductory cybersecurity project focused on:
- system exploitation
- reverse engineering
- privilege escalation
- insecure scripting
- binary analysis
- network inspection

The objective is not only to solve each level, but to understand and document the underlying vulnerability and exploitation process.

---

## 🎯 Technical Focus

The project explores several common vulnerability classes:

- command injection
- unsafe privilege delegation
- race conditions
- environment manipulation
- weak access control
- symlink exploitation
- unsafe scripting practices
- packet inspection and credential recovery
- runtime binary manipulation

---

## 🧠 Methodology

Each level follows a consistent workflow:

1. Enumeration
2. Static / dynamic analysis
3. Vulnerability identification
4. Exploitation
5. Result validation
6. Security takeaways

The goal is to progressively build a practical understanding of offensive security concepts and insecure trust assumptions in UNIX environments.

---

## 🧩 Vulnerability Taxonomy

### Injection Vulnerabilities

| Level | Vulnerability |
|---|---|
| 04 | Perl CGI command injection |
| 06 | PHP code execution via unsafe regex evaluation |
| 07 | Environment variable injection |
| 11 | Lua command injection via `io.popen` |
| 12 | Perl command injection via shell interpolation |

---

### Binary Exploitation & Runtime Manipulation

| Level | Technique |
|---|---|
| 03 | PATH hijacking |
| 13 | Runtime register manipulation |
| 14 | Anti-debug bypass & UID spoofing |

---

### Filesystem & Access Control

| Level | Vulnerability |
|---|---|
| 05 | Unsafe privileged script execution |
| 08 | Symlink / path validation bypass |

---

### Reverse Engineering & Logic Analysis

| Level | Technique |
|---|---|
| 09 | Custom encoding reversal |
| 13 | UID verification reversal |
| 14 | Binary logic analysis |

---

### Network & Traffic Analysis

| Level | Technique |
|---|---|
| 02 | Packet capture credential extraction |

---

### Concurrency Vulnerabilities

| Level | Vulnerability |
|---|---|
| 10 | TOCTOU race condition exploitation |

---

## 🧪 Tools Used

- `gdb`
- `ltrace` / `strace`
- `strings`
- `Wireshark`
- `netcat`
- `scp`
- `John The Ripper`
- `bash`
- `Perl`
- `PHP`
- `Lua`

---

## 📊 Level Progression

| Level | Main Concept |
|---|---|
| 00 | Basic encoding / cipher analysis |
| 01 | Password hash cracking |
| 02 | Network packet analysis |
| 03 | PATH hijacking |
| 04 | CGI command injection |
| 05 | Unsafe scheduled privileged execution |
| 06 | PHP regex evaluation vulnerability |
| 07 | Environment variable injection |
| 08 | Symlink exploitation |
| 09 | Algorithm reversal |
| 10 | TOCTOU race condition |
| 11 | Lua command injection |
| 12 | Shell interpolation injection |
| 13 | Runtime UID manipulation |
| 14 | Anti-debugging bypass |

---

## 🔐 Project Rules

### Accessing the VM

```bash
ssh level00@<IP> -p 4242
```

The initial password for `level00` is:

```txt
level00
```

---

### Progressing Between Levels

Each level requires retrieving either:
- the next user password
- or a flag/token

Typical progression flow:

```bash
su flagXX
getflag
```

Then:

```bash
su levelXX
```

