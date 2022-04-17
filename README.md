# m2-batch

This is a simple tool to efficiently read all available m2 data from the 3.3.5a client to easily and quickly figure out edge cases with the format.

## Prerequisites
- [Visual Studio](https://visualstudio.microsoft.com/downloads/)
  - If using Visual Studio 2019, make sure you have the latest update to support the C++20 flag.
- [Git](https://git-scm.com/download/win)
- [CMake](https://cmake.org/download/) (version 11+)


## Build (Windows)
- Open up a command-line prompt
- Clone this repository: `git clone https://github.com/ihm-tswow/m2-batch.git`
- Open up the cmake gui
  - Under "Where is the source code", enter the path to where you cloned the repository
  - Under "Where to build the binaries", choose a build directory
  - Press "Configure"
  - When asked what generator to select, make sure Visual Studio 2019 or 2022 is selected, usually this is the preselected option.
  - Press "Generate"
  - Press "Open Project"
  - Build the solution like a normal visual studio project (**Make sure to select Release or RelWithDebInfo mode**)
  
## Usage
This program is used by creating script files and configuring what script files to load when compiling the program.

The first time you run the program, it will extract all .m2 and .skin files from the clients default mpq archives. It does not currently support loading custom mpqs.

### Quickstart
- Set your client / extract path by editing `config.h`
- For a basic script skeleton, just edit `scripts/YourScript.h`, it will be the selected script by default.
- M

### Scripting

Scripts are defined as classes that inherit from `M2Script`. A basic script skeleton is defined in a header file inside the `scripts` directory as follows:

```c++
#pragma once

#include "script.h"

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
```

#### `flags()`
This function allows us to define special performance settings for this script if we want to multithread or skip loading skin files.

- If using multithreading, individual calls to `process` will happen on different threads, so if you want to modify variables outside of this function you need to use mutex locks.
- If at least one loaded script does not specify `M2ScriptFlags::USE_THREADS`, then no loaded script uses multithreading.
- On my machine, running an empty script in release mode with no performance flags takes about 3.6 seconds, and with all performance flags takes about 0.8 seconds.

#### `process(m2s, skins)`
This function is called with a loaded M2 file and any skin files that belongs to it (an empty array if `M2ScriptFlags::SKIP_SKINS` is supplied.

#### `finish(fileCount)`
This function is called when all scripts have finished processing all m2 files found in the client.

### Configuration

Global configurtaion is done in the `config.h` file, allowing you to specify where your wow client is and where to store extracted m2 files.

The `SCRIPTS` property defines what scripts to run by including instances of the script objects. 

- To add a new script to run, first `#include` your script file at the top of the file, and then add an instance of the script class inside this array.
- To stop a script from running, just remove or comment out its `new ScriptName()` line.

_Note: For visual studio to recognize new script files that you add, you need to click `config` and `generate` inside CMake again._
