#include "app/Application.h"

#ifndef ASSET_DIR
#define ASSET_DIR "assets"
#endif

int main() {
    gyaru::Application app(ASSET_DIR);
    return app.run();
}
