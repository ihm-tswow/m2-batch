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

struct M2Script
{
    virtual uint32_t flags() = 0;
    virtual void process(File<M2Header>& m2, std::vector<File<M2SkinHeader>> const& skins, synced_stream& stream) {};
    virtual void finish(uint32_t fileCount, synced_stream& stream) {};
};
