# cat_clone

cat — build & test cheat sheet

Build:  gcc -Wall -Wextra -o mycat mycat.c

Test:
  ./mycat file.txt                       regular file
  ./mycat < file.txt                     stdin (shell opens)
  echo hi | ./mycat                      pipe
  ./mycat /nope; echo $?                 open error -> 1
  ./mycat big | diff - big               silent = byte-perfect
  ./mycat /dev/zero | head -c 10 >/dev/null; echo ${PIPESTATUS[0]}   # 141 = SIGPIPE

Trace:  strace ./mycat < input 2>&1 | tail   # fd 0 = shell; fd 3 = own open

./mycat mycat.c | diff - mycat.c //test run will be silent no diff between the two files


strace ./mycat /etc/hostname 2>&1 | tail -8
strace ./mycat < /etc/hostname 2>&1 | tail -8
echo hi | strace ./mycat 2>&1 | tail -8

cat clone works as in the sense you complie the code and run either with an assigned file
 path or direclty on terminal as it echos any data you input

i used the  fd(file discriptors )  to create the system calls ammm.. 3

because 0, 1, 2 are all taken by the read write close(not in order)

so we create a system call  3 by fd 3 and that systemcall is an open we open a file of the designated path as we do argv[1]  the argv[0] in the main function is the name of the file being run ./mycat compliled code
we check if argc which is > 2 its an array ammm....arguments passed when you call the ./mycat program checks if  we have the file put it else we take the 0  STDIO_FILENINO as our input ..thats whatever you put in in the terminal will be output back...(read and written back to the terminal)
 i then initialzed values amm.. char buf[4096] ..4kb of buffer where its just a stream of empty bites and where we will write the read data from either the terminal or the <  associate file or echoed text  | 

     if(n == -1){
        if(errno == EINTR){
            continue; //system got inturrupted retry
        }
        perror("read");
        exit_code =1;
        break;
    }  this block checks for the system errno ==EINTR for signals that interrupt the systemcall


   if(n > 0){
        //we write to the terminal and continue
        if(write_all(STDOUT_FILENO,buf,(size_t)n) == -1){
            exit_code =1;
            break;
        }
        continue;
    } //here we write to the STDOUT_FILENINO which is the terminal or the assocate file > as placed on the terminal

    #we close all open files //only close when we open or open is successful 

    ./mycat /dev/zero | head -c 10 > /dev/null; echo "pipestatus: ${PIPESTATUS[0]}"

    /dev/zero produces an infinite stream of zero bytes.

    head -c 10 reads 10 bytes and exits.

    Your mycat will read 4096 bytes, write them, read again, write again... and at some point try to write into a pipe whose reader is gone.

    The kernel sends SIGPIPE. Your process dies. Exit status is 141.

The > /dev/null prevents 10 NUL bytes from being printed to your terminal, which would be messy.

If pipestatus: 141, . If it's 0 or something else, tell me and we'll figure out why.

To see the SIGPIPE in the trace:
text

strace -e trace=write ./mycat /dev/zero 2>&1 | head -20

You should see a write(1, ...) that returns -1 EPIPE, or the trace ends as the signal kills the process. Either way, you've witnessed it.
