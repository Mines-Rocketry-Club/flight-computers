#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

class Accelerometer {
public:
    Accelerometer();
    void update();
    float getAcceleration();
private:
    float m_acceleration;
};

#endif
