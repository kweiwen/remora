#pragma once

//
// C
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_MANAGE 500

struct memory_manage
{
    void* mem;
    size_t size;
    const char *fn;
    unsigned line;
    bool valid;
};

struct memory_manage mem_i[MAX_MANAGE];


#ifdef __cplusplus
namespace memoryld {
#endif

    void memory_monitoring(void)
    {
        for (size_t i = 0; i < MAX_MANAGE; ++i)
            mem_i[i].valid = false;
    }

    void memory_check(void)
    {
        int nleaks = 0;
        for (size_t i = 0; i < MAX_MANAGE; ++i) {
            if (mem_i[i].valid) {
                fprintf(stderr, "Memory Leaks Occurs! \n");
                fprintf(stderr, "   Address     : %p\n", mem_i[i].mem);
                fprintf(stderr, "   Size        : %zu\n", mem_i[i].size);
                fprintf(stderr, "   File Nmae   : %s\n", mem_i[i].fn);
                fprintf(stderr, "   Line        : %u\n", mem_i[i].line);
                if (++nleaks)
                    putchar('\n');
            }
        }
        fprintf(stderr, "Total %d Memory Leak Events. \n", nleaks);
    }

#ifdef __cplusplus
} // namespace mld
#endif

void* malloc_(const size_t size, const char* fn, const unsigned line)
{
    for (size_t i = 0; i < MAX_MANAGE; ++i) {
        if (mem_i[i].valid == false) {
            void* mp = malloc(size);
            if (mp) {
                mem_i[i].mem   =   mp;
                mem_i[i].size  = size;
                mem_i[i].fn    =   fn;
                mem_i[i].line  = line;
                mem_i[i].valid = true;
                return mp;
            }
            else
                return NULL;
        }
    }
    fprintf(stderr, "Exceed memory allocation limitation");
    exit(1);
}

void free_(void* release_mem)
{
    for (size_t i = 0; i < MAX_MANAGE; ++i) {
        if (release_mem == mem_i[i].mem)
            mem_i[i].valid = false;
    }
    free(release_mem);
}

#define malloc(size) malloc_(size, __FILE__, __LINE__)
#define free(release_mem) free_(release_mem)

//
// C++
//

#ifdef __cplusplus

#include <new>

const char* alloc_file_name;
std::size_t alloc_line;

void* operator new(const std::size_t sz)
{
    void* const p = malloc_(sz, alloc_file_name, alloc_line);
    if (p)
        return p;
    else
        throw std::bad_alloc();
}

void* operator new[](const std::size_t sz)
{
    void* const p = malloc_(sz, alloc_file_name, alloc_line);
    if (p)
        return p;
    else
        throw std::bad_alloc();
}

void operator delete(void* p) noexcept
{
    free_(p);
}

void operator delete[](void* p) noexcept
{
    free_(p);
}

#define new (alloc_file_name=__FILE__,alloc_line=__LINE__) && 0 ? nullptr : new

extern const char* alloc_file_name;
extern std::size_t alloc_line;

#endif
