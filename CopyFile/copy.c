#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

/*
    Build:
    gcc -o copy copy.c

    Run:
    ./copy [-m] <file_name> <new_file_name>

    Program for copying file content using two different approaches:
        * reading and writing
        * mapping memory
    Attention!
    If destination directory does not exists, copying is impossible.
    If destination file does not exists, it will be created.
*/

int copy_read_write(int fd_from, int fd_to);
int copy_mmap(int fd_from, int fd_to);
void show_manual();
int parameters_error();
int check_if_empty(int size);

int main(int argc, char *argv[]) {
    int m_flag = 0;
    int arg = 0;

    // No paramaters provided.
    if (argc == 1)
        return parameters_error();

    /*
        getopt - takes comand line arguments.
        mh specifies the possible flags: -m and -h.
    */
    while ((arg = getopt(argc, argv, "mh")) != -1) {
        switch (arg) {
            case 'm':
                if (argc != 4)
                    return parameters_error();

                m_flag = 1;
                break;
            case 'h':
                if (argc > 2)
                    return parameters_error();

                show_manual();
                return 0;
            case '?':
                return parameters_error();
        }
    }

    // Check if source file exists.
    if (access(argv[1 + m_flag], F_OK ) != 0 ) {
        printf("Provided source file does not exists.\n");
        return 1;
    }

    /*
        Source file:
        O_RDWR      -    Open for reading only.
    */
    int fp_src = open(argv[1 + m_flag], O_RDONLY);
    if (fp_src == -1) {
        printf("Error opening source file.\n");
        return 1;
    }

    /*
        Destination file:
        The directory in which the destination file is placed must exists!
        If the file does not exists, it will be created.
        O_RDWR      -    Open for reading and writing.
        O_CREAT     -    If files does not exists, it will be created.
        0666        -    read & write permission.
    */
    int fp_dest = open(argv[2 + m_flag], O_CREAT | O_RDWR, 0666);

    if (fp_dest == -1) {
        printf("Error opening destination file.\n");
        return 1;
    }

    if (m_flag == 1)
        copy_mmap(fp_src, fp_dest);
    else
        copy_read_write(fp_src, fp_dest);


    if (close(fp_src) == -1 || close(fp_dest) == -1) {
        printf("Error while closing files.\n");
        return 1;
    }

    return 0;
}

int parameters_error() {
    printf("Wrong number of paramaters! Type -h to see manual.\n");
    return 1;
}

int check_if_empty(int size) {
    if (size == 0) {
        printf("Provided source file is empty.\n");
        return 1;
    }
    return 0;
}

void show_manual() {
    printf("------------------------------------------\n");
    printf("There are two copy methods available.\n");
    printf("1. Read from the source file and write into the destination file:\n");
    printf("\t./copy <source_file_path> <destination_new file_path>\n");
    printf("2. Using memory mapping:\n");
    printf("\t./copy -m <file_path> <new file_path>\n\n");
    printf("To show manual: \n");
    printf("\t./copy -h\n");
}


/*
    struct stat - a system struct that is defined to store information about files.
    fstat(file_descriptor, buffer) -
        gets status information about the object specified by the open descriptor descriptor and stores the information in the area of memory indicated by the buffer argument.
*/
int copy_read_write(int fd_from, int fd_to) {
    struct stat st;
    fstat(fd_from, &st);
    int size = st.st_size;
    char buffer[size];

    if (check_if_empty(size) == 1)
        return 1;

    int res_read = read(fd_from, buffer, size);
    if (res_read == -1) {
        printf("Error reading source file.\n");
        return 1;
    }

    int res_write = write(fd_to, buffer, size);

    if (res_write == -1) {
        printf("Error while copying the file.\n");
        return 1;
    }

    return 0;
}

/*
    mmap(void *addr, size_t length, ...) -
        creates a new mapping in the virtual address space of the
        calling process.  The starting address for the new mapping is
        specified in addr.  The length argument specifies the length of
        the mapping (which must be greater than 0).

        If addr is NULL, then the kernel chooses the (page-aligned)
        address at which to create the mapping

        PROT_READ  - Pages may be read.
        PROT_WRITE - Pages may be written.
        MAP_SHARED - Share this mapping.  Updates to the mapping are visible to other processes mapping the same region.
        https://man7.org/linux/man-pages/man2/mmap.2.html

    memcpy(void *dest, const void *src, size_t n) -
        copies n characters from memory area src to memory area dest.

    munmap(void *addr, size_t len) -
        function shall remove any mappings for those entire pages containing any part of the address space of the process starting at addr and continuing for len bytes.
*/

int copy_mmap(int fd_from, int fd_to) {
    struct stat st;
    char *src_buffer, *dest_buffer;
    fstat(fd_from, &st);
    int size = st.st_size;

    if (check_if_empty(size) == 1)
        return 1;

    src_buffer = mmap(NULL, size,  PROT_READ, MAP_SHARED, fd_from, 0);
    dest_buffer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_to, 0);


    ftruncate(fd_to, size);
    dest_buffer = memcpy(dest_buffer, src_buffer, size);

    if (munmap(src_buffer, size) == -1 || munmap(dest_buffer, size) == -1) {
        printf("Error while mapping the memory.\n");
        return 1;
    }

    return 0;
}