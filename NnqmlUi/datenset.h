#ifndef DATENSET_H
#define DATENSET_H

#include <iostream>
#include <QObject>
#include <QDebug>
#include <QString>

using namespace std;

class DatenSet
{
public:
    DatenSet();
    vector<vector<float>> daten();
    vector<float> daten(unsigned int id);
    float daten(unsigned int idX, unsigned int idY);

    void daten(vector<vector<float>> i);
    void daten(vector<float> i);
    void daten(vector<float> i,unsigned int id);
    void daten(vector<vector<qreal>> i);
    void daten(vector<qreal> i);
    void daten(vector<qreal> i,unsigned int id);
private:
    vector<vector<float>> _daten;
};

#endif // DATENSET_H
