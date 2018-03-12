#include <iostream>
#include <fstream>
#include <cstdlib>
#include "TarClass.h"
#include <sstream>

int main(int argc, char** argv)
{
    /* open file for writing */

    /* create the tar file */
    std::stringstream sstream;
    LibTar::Tar tarball(sstream);
    tarball.putFile("C:/Users/PC/Desktop/code/vs_main/tar/x64/Debug/item1.txt", "item1.txt");
    tarball.putFile("C:/Users/PC/Desktop/code/vs_main/tar/x64/Debug/item2.txt", "item2.txt");
    /* finalize the tar file */
    tarball.finish();
    /* close the file */
    std::fstream out("archive.tar", std::ios::out | std::ios::binary);
    if (!out.is_open())
    {
        std::cerr << "Cannot open out" << std::endl;
        return EXIT_FAILURE;
    }
    const std::string& content = sstream.str();
    out.write(content.c_str(), content.size());
    out.close();
    /* we're done */
    std::fstream in("archive.tar", std::ios::in | std::ios::binary);
    tarball.readToFile("C:/Users/PC/Desktop/code/vs_main/tar/x64", in);
    return EXIT_SUCCESS;;
}