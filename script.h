#pragma once

#include "file.h"
#include "m2.h"
#include "skin.h"

#include <vector>
#include <map>
#include <string>
#include <cstdint>

enum M2ScriptFlags
{
    LOAD_SKINS = 0x1,
    USE_THREADS = 0x2
};

struct M2Script
{
    virtual uint32_t flags() = 0;
    virtual void process(File<M2Header>& m2, std::vector<File<M2SkinHeader>> const& skins) {};
    virtual void finish(uint32_t fileCount) {};
};
