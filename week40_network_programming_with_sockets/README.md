# Week 40 — Network Programming with Sockets

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_40_Network_Programming_with_Sockets.html)** — read these first; the code here is the worked example from that page.

> **Platform:** POSIX only. This week's subject *is* the operating-system interface
> (`fork`, signals, sockets, `epoll`), which Windows does not provide in a form that would
> teach the same lesson. On Windows, run it under WSL — see week 01 of the notes.

## Files

- `echoclient.c`
- `echoserver.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -std=c17 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -g -o echoserver echoserver.c
gcc -std=c17 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -g -o echoclient echoclient.c

./echoserver 8080 &
printf 'hello\nworld\nquit\n' | ./echoclient localhost 8080
```

```bash
./echoclient localhost 9999            # nothing listening
./echoclient no.such.host.invalid 8080 # resolution fails
nc localhost 8080                      # connect with netcat, type, Ctrl-C
head -c 1000000 /dev/zero | tr '\0' 'x' | ./echoclient localhost 8080
```

```bash
sudo tcpdump -i lo -A 'port 8080'
```
