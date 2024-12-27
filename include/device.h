#ifndef DOMOTIC_DEVICES_DEVICE_H
#define DOMOTIC_DEVICES_DEVICE_H

class device {
    private:
        int starting_time;
        double energy_impact; // ? both input and output energy could be managed by the same class
    public:
        virtual void start() = 0;
        virtual void stop() = 0;
};

#endif