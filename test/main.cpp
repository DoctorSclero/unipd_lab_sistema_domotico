#include <iostream>

#include "home.h"
#include "manualdevice.h"
#include "cycledevice.h"

int main() {
    domoticdevices::Home home{3.5, "test.log"};
    home.subscribe(domoticdevices::ManualDevice("Impianto fotovoltaico", -1.5, 0));
    home.subscribe(domoticdevices::CycleDevice("Lavatrice", 2.0, 0, 110));
    home.subscribe(domoticdevices::ManualDevice("Pompa di calore", 2, 0));
    home.subscribe(domoticdevices::CycleDevice("Lavastoviglie", 1.5, 0, 195));

    home.start_device("Impianto fotovoltaico");

    return EXIT_SUCCESS;
}