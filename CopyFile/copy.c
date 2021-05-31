#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int read_write(int src, int dest);
int map_memory(int src, int dest);
void show_manual();

int main(int argc, char *argv[]) {
    int m_flag = 0;
    int arg = 0;

    while ((arg = getopt(argc, argv, "mh")) != -1) {
        switch (arg) {
            case 'm':
                m_flag = 1;
                break;
            case 'h':
                show_manual();
                break;
        }
    }


    int fp_src = open(argv[1 + m_flag], O_RDONLY);
    int fp_dest = open(argv[2 + m_flag], O_RDWR | O_CREAT, 0666);

    if (m_flag == 1)
        map_memory(fp_src, fp_dest);
    else
        read_write(fp_src, fp_dest);


    close(fp_src);
    close(fp_dest);
}

void show_manual() {
    printf("------------------------------------------\n");
    printf("There are two copy methods available.");
    printf("\t1. Read-write\n");
    printf("\tcopy <file_path> <new file_path>\n");
    printf("\t1. Memory mapping\n");
    printf("copy -m <file_path> <new file_path>\n");
}

int read_write(int fp_src, int fp_dest) {
    struct stat st;
    fstat(fp_src, &st);
    int size = st.st_size;
    char buffer[size];

    int res_read = read(fp_src, buffer, size);
    int res_write = write(fp_dest, buffer, size);

    if (res_read == -1 || res_write == -1) {
        printf("Error while copying the memory.");
        exit(1);
    }

    exit(0);
}

int map_memory(int fp_src, int fp_dest) {
    struct stat st;
    char *src_buffer, *dest_buffer;
    int size = 0;

    fstat(fp_src, &st);
    size = st.st_size;

    src_buffer = mnap(NULL, size,  PROT_READ,MAP_SHARED, fp_src, 0);

    ftruncate(fp_dest, size);
    dest_buffer = mnap(dest_buffer, src_buffer, size);

    int res_src = munmap(src_buffer, size);
    int res_dest = munmap(dest_buffer, size);

    if (res_src == -1 || res_dest == -1) {
        printf("Error while mapping the memory.");
        exit(1);
    }

    exit(0);
}