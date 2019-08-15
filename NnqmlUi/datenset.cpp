#include "datenset.h"

DatenSet::DatenSet()
{

}
vector<vector<float>> DatenSet::daten(){
    return _daten;
}
void DatenSet::daten(vector<vector<float>> i){
    //qDebug()<<i;
    _daten = i;
}
vector<float> DatenSet::daten(unsigned int id){
    if(id<_daten.size()){
      return _daten[id];
    }
    else {
        return _daten[0];
    }
}

float DatenSet::daten(unsigned int idX, unsigned int idY){
    if(idX>_daten.size()){
      return _daten[0][0];
    }
    else {
        if(idY>_daten[0].size()){
            return _daten[idX][0];
        }
        else {
            return _daten[idX][idY];
        }
    }
}

void DatenSet::daten(vector<float> i){
    daten(i,unsigned(_daten.size()+1));
}

void DatenSet::daten(vector<float> i,unsigned int id){
    qDebug()<<"add "<<i;
    if(id>_daten.size())
        _daten.push_back(i);
    else
        _daten[id]=i;
}
void DatenSet::daten(vector<vector<qreal>> i){
    for (unsigned j=0;j<i.size();++j) {
        daten(i[j],j);
    }
}
void DatenSet::daten(vector<qreal> i){
    daten(i,unsigned(_daten.size()+1));
}

void DatenSet::daten(vector<qreal> i,unsigned int id){
    vector<float> transferDaten;
    //QString text= QString::number(id)+":";
    for(unsigned j=0; j<i.size();++j){
        //text = text + QString::number(i[j])+" ";
        transferDaten.push_back(float(i[j]));
    }
    //qDebug()<<text;
    if(id>_daten.size())
        _daten.push_back(transferDaten);
    else
        _daten[id]=transferDaten;
}
