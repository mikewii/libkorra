#include "Tools/Utils.hpp"
#include <string.h>

#ifdef __linux__
    #include <stdio.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <string.h>
#else
//
#endif


namespace Utils {

    // Alignment safe copy
extern void* copybytes(void* _dest, const void* _src, size_t _size)
{
    int isSafe = reinterpret_cast<u64>(_dest) % 4 ;

    if ( isSafe == 0 )
        return memcpy(_dest, _src, _size);

    return memmove(_dest, _src, _size);
}

int makedir(const char* dir)
{
    int res = 0;

#ifdef __linux__

    char tmp[256];
    char *p = NULL;
    int perm = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // 0755
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    if(tmp[len - 1] == '/')
            tmp[len - 1] = 0;
    for(p = tmp + 1; *p; p++)
            if(*p == '/') {
                    *p = 0;
                    res = mkdir(tmp, perm);
                    *p = '/';
            }
    res = mkdir(tmp, perm);
   #else
       res = _mkdir(dir);
   #endif

    return res;
}

bool FileToCC(const char* fname, CContainer* cc)
{
    FILE* f = nullptr;
    u32 fsize = 0;
    u32 rsize = 0;

#ifdef __linux__

    if ( access(fname, F_OK | R_OK ) == 0 )
    {
        f = fopen(fname, "rb");

        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        rewind(f);

        cc->allocate(fsize);

        rsize = fread(cc->data(), sizeof(u8), fsize, f);
        fclose(f);
    }

#else
    //
#endif

    if (rsize > 0) return true;
    return false;
}

bool CCtoFile(const char* fname, CContainer* cc, bool makedir)
{
    FILE* f = nullptr;
    u32 wsize = 0;

    if (makedir)
    {
        std::string path = fname;
        path.resize(path.find_last_of("/"));
        Utils::makedir(path.c_str());
    }

    f = fopen(fname, "wb");

    if (f)
    {
        wsize = fwrite(cc->data(), sizeof(u8), cc->size(), f);
        fclose(f);
    }

    if (wsize > 0) return true;
    return false;
}

} // Utils
