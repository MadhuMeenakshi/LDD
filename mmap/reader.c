#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define FILE_NAME "mmap_file"
#define SIZE 1024

int main()
{
    int fd;

    fd = open(FILE_NAME, O_RDWR, 0666);

    if(fd < 0)
    {
        perror("open");
        return -1;
    }

    char *ptr = mmap(NULL,
                     SIZE,
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     fd,
                     0);

    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }

    printf("Message : %s\n", ptr);

    munmap(ptr, SIZE);

    close(fd);

    return 0;
}
