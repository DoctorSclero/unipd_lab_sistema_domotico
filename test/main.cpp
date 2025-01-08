#include <iostream>

#include "home.h"
#include "device.h"
#include "cycledevice.h"
#include "manualdevice.h"

int main() {
    domoticdevices::Home home{3.5, "test.log"};
    
    return EXIT_SUCCESS;
}