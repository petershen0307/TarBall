/*
* copy from https://github.com/lindenb/cclindenb/blob/master/src/core/lindenb/io/tarball.cpp
*/
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <ctime>
#include "TarClass.h"
namespace LOCALNS = LibTar;

#define TARHEADER static_cast<LOCALNS::PosixTarHeader*>(header)
#define THROW(x) std::cout << x


#pragma warning(disable : 4996)

void LOCALNS::Tar::_init(void* header)
{
    std::memset(header, 0, sizeof(PosixTarHeader));
    std::strcpy(TARHEADER->magic, "ustar");
    std::strcpy(TARHEADER->version, " ");
    std::sprintf(TARHEADER->mtime, "%011lo", static_cast<unsigned long>(time(NULL)));
    std::sprintf(TARHEADER->mode, "%07o", 0644);
    std::sprintf(TARHEADER->gname, "%s", "users");
}

void LOCALNS::Tar::_checksum(void* header)
{
    unsigned int sum = 0;
    char *p = (char *)header;
    char *q = p + sizeof(PosixTarHeader);
    while (p < TARHEADER->checksum) sum += *p++ & 0xff;
    for (int i = 0; i < 8; ++i) {
        sum += ' ';
        ++p;
    }
    while (p < q) sum += *p++ & 0xff;

    std::sprintf(TARHEADER->checksum, "%06o", sum);
}

void LOCALNS::Tar::_size(void* header, unsigned long fileSize)
{
    std::sprintf(TARHEADER->size, "%011llo", (long long unsigned int)fileSize);
}

void LOCALNS::Tar::_filename(void* header, const char* filename)
{
    if (filename == NULL || filename[0] == 0 || std::strlen(filename) >= 100)
    {
        THROW("invalid archive name \"" << filename << "\"");
    }
    std::snprintf(TARHEADER->name, 100, "%s", filename);
}

void LOCALNS::Tar::_endRecord(std::size_t len)
{
    char c = '\0';
    while ((len % sizeof(PosixTarHeader)) != 0)
    {
        out.write(&c, sizeof(char));
        ++len;
    }
}


LOCALNS::Tar::Tar(std::iostream& _out)
    : _finished(false)
    , out(_out)
{
    if (sizeof(PosixTarHeader) != 512)
    {
        THROW(sizeof(PosixTarHeader));
    }
}

LOCALNS::Tar::~Tar()
{
    if (!_finished)
    {
        std::cerr << "[warning]tar file was not finished." << std::endl;
    }
}

/** writes 2 empty blocks. Should be always called before closing the Tar file */
void LOCALNS::Tar::finish()
{
    _finished = true;
    //The end of the archive is indicated by two blocks filled with binary zeros
    PosixTarHeader header;
    std::memset((void*)&header, 0, sizeof(PosixTarHeader));
    out.write((const char*)&header, sizeof(PosixTarHeader));
    out.write((const char*)&header, sizeof(PosixTarHeader));
    out.flush();
}

void LOCALNS::Tar::put(const char* filename, const std::string& s)
{
    put(filename, s.c_str(), s.size());
}
void LOCALNS::Tar::put(const char* filename, const char* content)
{
    put(filename, content, std::strlen(content));
}

void LOCALNS::Tar::put(const char* filename, const char* content, std::size_t len)
{
    PosixTarHeader header;
    _init((void*)&header);
    _filename((void*)&header, filename);
    header.typeflag[0] = 0;
    _size((void*)&header, static_cast<unsigned long>(len));
    _checksum((void*)&header);
    out.write((const char*)&header, sizeof(PosixTarHeader));
    out.write(content, len);
    _endRecord(len);
}

void LOCALNS::Tar::putFile(const char* filename, const char* nameInArchive)
{
    char buff[BUFSIZ];
    std::FILE* in = std::fopen(filename, "rb");
    if (in == NULL)
    {
        THROW("Cannot open " << filename << " " << std::strerror(errno));
    }
    std::fseek(in, 0L, SEEK_END);
    long int len = std::ftell(in);
    std::fseek(in, 0L, SEEK_SET);

    PosixTarHeader header;
    _init((void*)&header);
    _filename((void*)&header, nameInArchive);
    header.typeflag[0] = 0;
    _size((void*)&header, len);
    _checksum((void*)&header);
    out.write((const char*)&header, sizeof(PosixTarHeader));


    std::size_t nRead = 0;
    while ((nRead = std::fread(buff, sizeof(char), BUFSIZ, in))>0)
    {
        out.write(buff, nRead);
    }
    std::fclose(in);

    _endRecord(len);
}
