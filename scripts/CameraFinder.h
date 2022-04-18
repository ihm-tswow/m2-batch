#pragma once

#include "script.h"
#include <mutex>

class CameraFinder : public M2Script
{
    uint32_t flags()
    {
        return 0
            | M2ScriptFlags::USE_THREADS
            | M2ScriptFlags::SKIP_SKINS
        ;
    }

    std::mutex camera_lock;
    int camera_count = 0;

    void process(File<M2Header>& m2, std::vector<File<M2SkinHeader>> const& skins) override {
        if (m2.header.cameras.count > 0)
        {
            std::cout << "Found camera: " << m2.path << "\n";
            std::scoped_lock(camera_lock);
            camera_count++;
        }
    };

    void finish(uint32_t fileCount) override {
        std::cout << "Found " << camera_count << " cameras\n";
    };
};