#pragma once

#include "script.h"
#include <mutex>

class YourScript : public M2Script
{
    uint32_t flags()
    {
        return 0
            | M2ScriptFlags::USE_THREADS
            | M2ScriptFlags::SKIP_SKINS
        ;
    }

    void process(File<M2Header>& m2, std::vector<File<M2SkinHeader>> const& skins) override {
    };

    void finish(uint32_t fileCount) override {
        std::cout << "Your script just finished processing " << fileCount << " files\n";
    };
};