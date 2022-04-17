#include "m2.h"
#include "skin.h"
#include "file.h"

#define LOAD_SKINS false
#define USE_THREADS false
#define CLIENT_PATH "D:\\dev\\wow\\client"
#define EXTRACTS_PATH "extracts"

void process_file(File<M2Header> & m2, std::vector<File<M2SkinHeader>> const& skins)
{
}

void finish(uint32_t fileCount, uint64_t time)
{
    std::cout << "Processed " << fileCount << "files in " << time << "ms\n";
}
