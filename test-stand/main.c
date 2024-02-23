typedef unsigned long size_t;

typedef long ssize_t;

typedef long __off_t;

extern int printf(const char *__format, ...);

extern size_t strlen(const char *__s);

extern int open(const char *__file, int __oflag, ...);

extern int close(int __fd);

extern __off_t lseek(int __fd, __off_t __offset, int __whence);

extern ssize_t read(int __fd, void *__buf, size_t __nbytes);

extern ssize_t write(int __fd, const void *__buf, size_t __n);

extern void *malloc(size_t __size);

extern void *realloc(void *__ptr, size_t __size);

extern void free(void *__ptr);

#define S_IRWXU (0400 | 0200 | 0100)
#define O_RDWR 02
#define O_CREAT 0100
#define O_TRUNC 01000
#define SEEK_SET 0

int main() {
    const char *name = "test";
    int fd = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    printf("open(%p, %d)=%d\n", name, O_RDWR | O_CREAT | O_TRUNC, fd);

    const char *data = "test string\n";
    size_t ret = write(fd, data, strlen(data) + 1);
    printf("write(%d, %p, %ld)=%ld\n", fd, data, strlen(data) + 1, ret);

    ret = lseek(fd, 0, SEEK_SET);
    printf("lseek(%d, %d, %d)=%ld\n", fd, 0, SEEK_SET, ret);

    char buff[20];
    ret = read(fd, buff, 20);
    printf("read(%d, %p, %d)=%ld\n", fd, buff, 20, ret);

    ret = close(fd);
    printf("close(%d)=%ld\n", fd, ret);

    void *ptr = malloc(1024);
    printf("malloc(%d)=%p\n", 1024, ptr);

    void *re_ptr = realloc(ptr, 4096);
    printf("realloc(%p, %d)=%p\n", ptr, 4096, re_ptr);

    free(re_ptr);
    printf("free(%p)\n", re_ptr);
}