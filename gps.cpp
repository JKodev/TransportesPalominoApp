#include "gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

GPS::GPS()
{

}

QPair<double, double> GPS::getcurubication()
{
    srand(time(NULL));
    this->ubication.first = ((double)rand()/(double)RAND_MAX);
    this->ubication.second = ((double)rand()/(double)RAND_MAX);

    return this->ubication;
}
