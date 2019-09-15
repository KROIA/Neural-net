#include "daten.h"

Daten::Daten()
{
    qDebug()<<"Setup Daten....";
    vector<vector<float>>trainingsSet;
    vector<vector<float>>outputSet;
    trainingsSet.push_back({0,0});   outputSet.push_back({0});
    trainingsSet.push_back({0,1});   outputSet.push_back({1});
    trainingsSet.push_back({1,0});   outputSet.push_back({1});
    trainingsSet.push_back({1,1});   outputSet.push_back({0});
    trainingInput.daten(trainingsSet);
    trainingOutput.daten(outputSet);
}
