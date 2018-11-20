#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include <pcre.h>

#define VECTOR_MAX 90 // The first max match result number is 30

int main(int argc, char** argv)
{
    int ret = 0;
    int err_pos = -1;
    int offset = 0;
    const char* err_str = NULL;
    const char** sub_str = NULL;
    const char* name_str = NULL;
    int vec[VECTOR_MAX] = {0};
    struct stat file_info;
    size_t file_size = 0;
    char* addr = NULL;
    pcre * re = NULL;

    if (argc < 3) {
        printf("USAGE: %s [file] [regex]  [name_subpattern]\r\n", argv[0]);
        exit(-1);
    }

    int fd = -1;
    fd = open(argv[1], O_RDONLY);
    if (-1 == fd) {
        printf("[ERROR]: file open failed. errno: %d\r\n", errno);
        exit(-1);
    }

    re = pcre_compile(argv[2], 0, &err_str, &err_pos, NULL);
    if (NULL == re) {
        printf("[ERROR]: %s at line: %d\r\n", err_str, err_pos);
        exit(-1);
    }

#if 1
    int captures = 0;
    ret = pcre_fullinfo(re, NULL, PCRE_INFO_CAPTURECOUNT, &captures);
    if (ret >= 0) {
        printf("PCRE_INFO_CAPTURECOUNT = %d\r\n", captures);
    }

    int named_captures = 0;
    ret = pcre_fullinfo(re, NULL, PCRE_INFO_NAMECOUNT, &named_captures);
    if (ret >= 0) {
        printf("PCRE_INFO_NAMECOUNT = %d\r\n", named_captures);
    }

    int name_size = 0;
    ret = pcre_fullinfo(re, NULL, PCRE_INFO_NAMEENTRYSIZE, &name_size);
    if (ret >= 0) {
        printf("PCRE_INFO_NAMEENTRYSIZE = %d\r\n", name_size);
    }

    char* names = 0;
    ret = pcre_fullinfo(re, NULL, PCRE_INFO_NAMETABLE, &names);
    if (ret >= 0) {
        printf("PCRE_INFO_NAMETABLE = %p\r\n", names);
    }

    char* ptr = names;
    for (int i = 0; i < named_captures; i++) {
        printf("Num: %d\r\n", 2 * (ptr[0] << 8) + ptr[1]);
        printf("Name: %s\r\n", ptr+2);
        ptr = ptr + name_size;
    }

    //exit(-1);
#endif

    ret = fstat(fd, &file_info);
    if (-1 == ret) {
        printf("[ERROR]: get file info failed. errno:%d\r\n", errno);
        goto fd_failed;
    }

    file_size = file_info.st_size;

    addr = mmap(NULL, file_size - 1, PROT_READ, MAP_SHARED, fd, 0);
    if (NULL == addr) {
        printf("[ERROR]: mmap error. errno: %d\r\n", errno);
        goto fd_failed;
    }

    do {
        if (offset > file_size) {
            ret = 0;
            break;
        }
        ret = pcre_exec(re, NULL, addr, file_size, offset, 0, vec, VECTOR_MAX);
        if (ret == PCRE_ERROR_NOMATCH) {
            //printf("[WARNING]: No string match\r\n");
            ret = 0;
            break;
        }

        if (ret < 0) {
            printf("[ERROR]: pcre_exec error happened. err: %d\r\n", ret);
            break;
        }

        if (vec[(ret - 1) * 2 + 1] == offset) {
            offset += 1;
            continue;
        }

        offset = vec[(ret - 1) * 2 + 1];

        pcre_get_substring_list(addr, vec, ret, &sub_str);
        for (int i = 0; i < ret; i++) {
            printf("%s\r\n", sub_str[i]);
        }

        pcre_free_substring_list(sub_str); // free

        ret = pcre_get_named_substring(re, addr, vec, ret, argv[3], &name_str);
        if (ret >= 0 && name_str != NULL) {
            printf("NAME: %s\r\n", name_str);
        }
    } while (1);

    pcre_free(re);
    munmap(addr, file_size);

fd_failed:
    close(fd);

    return ret;
}
