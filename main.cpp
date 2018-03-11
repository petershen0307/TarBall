#include <iostream>
#include <fstream>
#include <cstdlib>
#include "TarClass.h"
#include <sstream>

int main(int argc, char** argv)
{
    std::stringstream sstream;
    sstream.write("hello\0world", 12);
    std::string str = sstream.str();
    str.size();
    /* open file for writing */
    std::fstream out("archive5.tar", std::ios::out | std::ios::binary);
    if (!out.is_open())
    {
        std::cerr << "Cannot open out" << std::endl;
        return EXIT_FAILURE;
    }
    /* create the tar file */
    LibTar::Tar tarball(out);
    /* add item 1 */
    //tarball.put("item1.txt", "Hello World 1\n");
    /* add item 2 */
    //tarball.put("item2.txt", "Hello World 2\n");
    /* add a file */
    //tarball.putFile("D:/code/TestSLN/x64/Debug/tarFiles.ilk", "tarFiles.ilk");
    //tarball.putFile("tarFiles.pdb", "tarFiles.pdb");
    tarball.putFile("C:/Users/PC/Desktop/code/vs_main/tar/x64/Debug/item1.txt", "item1.txt");
    tarball.putFile("C:/Users/PC/Desktop/code/vs_main/tar/x64/Debug/item2.txt", "item2.txt");
    /* finalize the tar file */
    tarball.finish();
    /* close the file */
    out.close();
    /* we're done */
    return EXIT_SUCCESS;;
}