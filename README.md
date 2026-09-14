# cat_clone

cat — build & test cheat sheet

Build:  gcc -Wall -Wextra -o mycat mycat.c

Test:
  ./mycat file.txt                       # regular file
  ./mycat < file.txt                     # stdin (shell opens)
  echo hi | ./mycat                      # pipe
  ./mycat /nope; echo $?                 # open error -> 1
  ./mycat big | diff - big               # silent = byte-perfect
  ./mycat /dev/zero | head -c 10 >/dev/null; echo ${PIPESTATUS[0]}   # 141 = SIGPIPE

Trace:  strace ./mycat < input 2>&1 | tail   # fd 0 = shell; fd 3 = own open
