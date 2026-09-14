#include <fcntl.h>      // open, O_RDONLY
#include <unistd.h>     // read, write, close, STDIN_FILENO, STDOUT_FILENO
#include <stdio.h>      // perror
#include <stdlib.h> 
#include <errno.h>

static int write_all( int fd,const char *buf, size_t n){
    size_t  written  = 0;
    ssize_t w;
    while(written < n){
       w = write(fd,buf,n);
       if(w > 0){
        written += w;
        continue;
       }
       if(w == -1){
         if(errno == EINTR){
            continue;
         }else{
            break;
            return -1;
         }
       }
       if( w == 0){
        break; //rarely happens
        return 0;
       }
       if (written == n){
        return 0;
       }

    }

    return 0;

}

int main(int argc , char *argv[]){
    int fd;
    int exit_code = 0;
    if (argc < 2) {
        fd = STDIN_FILENO;     // 0, already open, nothing to open
    } else {
         fd = open(argv[1], O_RDONLY);
         if (fd == -1) { perror(argv[1]); return 1; }
     }
    
    //declare two variables 
    char buf[4096];
   for(;;){
    ssize_t n = read(fd,buf,sizeof buf);

    if(n > 0){
        //we write to the terminal and continue
        if(write_all(STDOUT_FILENO,buf,(size_t)n) == -1){
            exit_code =1;
            break;
        }
        continue;
    }
    if(n == 0){
        break;
    }

    if(n == -1){
        if(errno == EINTR){
            continue; //system got inturrupted retry
        }
        perror("read");
        exit_code =1;
        break;
    }

   }
    close(fd);
    return exit_code;
}