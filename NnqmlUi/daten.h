#ifndef DATEN_H
#define DATEN_H

#include <iostream>
#include <QObject>
#include <datenset.h>

using namespace std;

class Daten
{
    //Q_OBJECT
public:
    Daten();
    DatenSet trainingInput;
    DatenSet trainingOutput;
    int i;
private:

};

#endif // DATEN_H
