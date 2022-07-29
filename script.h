#pragma once

#include "file.h"
#include "m2.h"
#include "skin.h"

#include "thread_pool.hpp"

#include <vector>
#include <map>
#include <string>
#include <cstdint>

enum M2ScriptFlags
{
    SKIP_SKINS = 0x1,
    USE_THREADS = 0x2
};

enum M2Errors
{
    SUCCESS = 0,
    BAD_M2 = 0x1,
    BAD_SKIN_00 = 0x2,
    BAD_SKIN_01 = 0x4,
    BAD_SKIN = BAD_SKIN_00 | BAD_SKIN_01
};

struct M2Script
{
    virtual uint32_t flags() = 0;
    virtual void process(File<M2Header>& m2, std::vector<File<M2SkinHeader>> & skins, synced_stream& stream) {};
    virtual void process_bad(File<M2Header>& m2, std::vector<File<M2SkinHeader>> & skins, uint32_t errors, synced_stream& stream) {};
    virtual void finish(uint32_t fileCount, synced_stream& stream) {};
};
