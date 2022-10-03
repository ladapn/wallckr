#ifndef IDISTANCESENSOR_H
#define IDISTANCESENSOR_H

class IDistanceSensor {
public:
    virtual bool trigger_measurement() = 0;
    virtual unsigned long get_distance_raw_cm() = 0;
    virtual unsigned long get_distance_filtered_cm() = 0;
};

#endif // IDISTANCESENSOR_H