#ifndef GPS_H
#define GPS_H

#include <QPair>
#include <QVector>

class GPS
{
private:
    QVector< QPair<double, double> > *ubications;
    QPair<double, double> ubication;
public:
    GPS();
    QPair<double, double> getcurubication();
};

#endif // GPS_H
