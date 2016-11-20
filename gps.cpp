#include "gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

GPS::GPS()
{
    this->ubications = new QVector< QPair<double,double> >();
    this->ubications->append(qMakePair(double(-16.417149893474726), double(-71.51327133178711)));
    this->ubications->append(qMakePair(double(-16.3980480890978), double(-71.53678894042969)));
    this->ubications->append(qMakePair(double(-16.404635130008433), double(-71.58571243286133)));
    this->ubications->append(qMakePair(double(-16.484814448981634), double(-71.88217163085938)));
    this->ubications->append(qMakePair(double(-16.422913000573963), double(-72.33261108398438)));
    this->ubications->append(qMakePair(double(-16.6427708039953), double(-72.63885498046875)));
    this->ubications->append(qMakePair(double(-16.521682507760566), double(-72.91488647460938)));
    this->ubications->append(qMakePair(double(-16.426864746696914), double(-73.11813354492188)));
    this->ubications->append(qMakePair(double(-16.325411207783844), double(-73.32550048828125)));
    this->ubications->append(qMakePair(double(-16.21731189963418), double(-73.61251831054688)));
    this->ubications->append(qMakePair(double(-16.181705218921916), double(-73.75946044921875)));
    this->ubications->append(qMakePair(double(-16.062969971128574), double(-73.95309448242188)));
    this->ubications->append(qMakePair(double(-15.942843367782686), double(-74.0753173828125)));
    this->ubications->append(qMakePair(double(-15.895300855260103), double(-74.16183471679688)));
    this->ubications->append(qMakePair(double(-15.860957606260463), double(-74.25521850585938)));
    this->ubications->append(qMakePair(double(-15.80546769793827), double(-74.36370849609375)));
    this->ubications->append(qMakePair(double(-15.711628749621411), double(-74.4873046875)));
    this->ubications->append(qMakePair(double(-15.653453312049916), double(-74.57931518554688)));
    this->ubications->append(qMakePair(double(-15.58203356165925), double(-74.70291137695312)));
    this->ubications->append(qMakePair(double(-15.511912249339868), double(-74.81689453125)));
    this->ubications->append(qMakePair(double(-15.411319377980995), double(-74.893798828125)));
    this->ubications->append(qMakePair(double(-15.243114837297805), double(-74.937744140625)));
    this->ubications->append(qMakePair(double(-14.9248813652563), double(-74.98992919921875)));
    this->ubications->append(qMakePair(double(-14.83330151553516), double(-74.95697021484375)));
    this->ubications->append(qMakePair(double(-14.683239257519446), double(-75.13275146484375)));
    this->ubications->append(qMakePair(double(-14.537061956768639), double(-75.23025512695312)));
    this->ubications->append(qMakePair(double(-14.44132981391078), double(-75.487060546875)));
    this->ubications->append(qMakePair(double(-14.293662110576578), double(-75.68069458007812)));
    this->ubications->append(qMakePair(double(-14.171197284392946), double(-75.71914672851562)));
    this->ubications->append(qMakePair(double(-14.077973196671586), double(-75.74249267578125)));
    this->ubications->append(qMakePair(double(-13.939398640275055), double(-75.85647583007812)));
    this->ubications->append(qMakePair(double(-13.8887448260805), double(-76.11190795898438)));
    this->ubications->append(qMakePair(double(-13.81274334991904), double(-76.15447998046875)));
    this->ubications->append(qMakePair(double(-13.680681930380013), double(-76.16546630859375)));
    this->ubications->append(qMakePair(double(-13.51917291815341), double(-76.13937377929688)));
    this->ubications->append(qMakePair(double(-13.49246663232042), double(-76.33712768554688)));
    this->ubications->append(qMakePair(double(-13.452401604399885), double(-76.33987426757812)));
    this->ubications->append(qMakePair(double(-13.344192601902693), double(-76.22177124023438)));
    this->ubications->append(qMakePair(double(-13.269353062714407), double(-76.28082275390625)));
    this->ubications->append(qMakePair(double(-13.120942366607927), double(-76.39892578125)));
    this->ubications->append(qMakePair(double(-12.92559913225269), double(-76.49642944335938)));
    this->ubications->append(qMakePair(double(-12.67115659542643), double(-76.6461181640625)));
    this->ubications->append(qMakePair(double(-12.47680542229143), double(-76.75323486328125)));
    this->ubications->append(qMakePair(double(-12.306460605137392), double(-76.84661865234375)));
    this->ubications->append(qMakePair(double(-12.217891205488728), double(-76.96334838867188)));
    this->ubications->append(qMakePair(double(-12.134662629772327), double(-76.97708129882812)));
    this->ubications->append(qMakePair(double(-12.063494999963579), double(-77.03201293945312)));
    this->ubications->append(qMakePair(double(-12.021859884778037), double(-77.06634521484375)));
    this->ubications->append(qMakePair(double(-11.946631743580703), double(-77.09930419921875)));
    this->ubications->append(qMakePair(double(-11.946631743580703), double(-77.0526123046875)));
    this->ubications->append(qMakePair(double(-11.814932269624515), double(-77.10617065429688)));
    this->ubications->append(qMakePair(double(-11.821653169925574), double(-76.99081420898438)));
    this->ubications->append(qMakePair(double(-12.024546215925072), double(-76.91940307617188)));
    this->ubications->append(qMakePair(double(-12.12257889707228), double(-76.81915283203125)));
}

QPair<double, double> GPS::getcurubication()
{
    srand(time(NULL));
    int r = rand() % this->ubications->size();

    return this->ubications->at(r);
}
