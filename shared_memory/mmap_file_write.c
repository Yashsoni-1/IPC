#include <stdio.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>



typedef struct student_
{
    int roll;
    int marks;
    char name[128];
    char city[128];
} student_t;



int main(int argc, const char * argv[]) {
    
    

    if(argc < 2)
    {
        printf("File path not mentioned\n");
        exit(0);
    }
    
    const char *filepath = argv[1];
    
    int fd = open(filepath, O_RDWR);
    
    if(fd < 0)
    {
        printf("Couln't open the file\n");
        exit(1);
    }
    
    struct stat statbuf;
    int err = fstat(fd, &statbuf);
    
    if(err < 0)
    {
        printf("Couln't open the file\n");
        exit(2);
    }
    
    char *ptr = mmap(NULL,
                     statbuf.st_size,
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     fd,
                     0);
    
    if(ptr == MAP_FAILED)
    {
        printf("Mapping failed, errno = %d\n", errno);
        return 1;
    }
    
    close(fd);

    
    student_t stud = {222, 93, "Milkyway", "Utopia"};
    
    memcpy(ptr, &stud, sizeof(stud));
    
    msync(ptr, sizeof(stud), MS_SYNC);
    
    err = munmap(ptr, statbuf.st_size);
    
    if(err != 0)
    {
        printf("Unmapping failed\n");
        return 1;
    }
    


    return 0;
}

