#include "Engine/Engine.h"
#include "Pocket/ServerPocket.h"

int main(int argc, char* argv[])
{
    FEngine ServerEngine;
    ServerEngine.Initialize<FServerPocket>();
    return 0;
}
