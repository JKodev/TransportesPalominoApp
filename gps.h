#ifndef GPS_H
#define GPS_H

#include <QPair>

class GPS
{
private:
    QPair<double, double> ubication;
public:
    GPS();
    QPair<double, double> getcurubication();
};

#endif // GPS_H
