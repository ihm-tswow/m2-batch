#include "scripts/CameraFinder.h"
#include "scripts/YourScript.h"
#include "script.h"

// The client to extract m2/skins from
#define CLIENT_PATH "D:\\dev\\wow\\client"

// The path to extract m2/skins to
#define EXTRACTS_PATH "extracts"

// What scripts to execute when running the program
M2Script* SCRIPTS[] = {
    new YourScript(),
};
