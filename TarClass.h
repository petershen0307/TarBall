#pragma once
/*
* copy from https://github.com/lindenb/cclindenb/blob/master/src/core/lindenb/io/tarball.h
*/

#ifndef LINDENB_IO_TARBALL_H_
#define LINDENB_IO_TARBALL_H_

#include <iostream>


namespace LibTar
{
    struct PosixTarHeader
    {
        char name[100];
        char mode[8];
        char uid[8];
        char gid[8];
        char size[12];
        char mtime[12];
        char checksum[8];
        char typeflag[1];
        char linkname[100];
        char magic[6];
        char version[2];
        char uname[32];
        char gname[32];
        char devmajor[8];
        char devminor[8];
        char prefix[155];
        char pad[12];
    };

    /**
    *  A Tar Archive
    */
    class Tar
    {
    private:
        bool _finished;
    protected:
        std::ostream& out;
        void _init(void* header);
        void _checksum(void* header);
        void _size(void* header, unsigned long fileSize);
        void _filename(void* header, const char* filename);
        void _endRecord(std::size_t len);
    public:
        Tar(std::ostream& out);
        virtual ~Tar();
        /** writes 2 empty blocks. Should be always called before closing the Tar file */
        void finish();
        void put(const char* filename, const std::string& s);
        void put(const char* filename, const char* content);
        void put(const char* filename, const char* content, std::size_t len);
        void putFile(const char* filename, const char* nameInArchive);
    };

}

#endif /* LINDENB_IO_TARBALL_H_ */
