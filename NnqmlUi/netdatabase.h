#ifndef NETDATABASE_H
#define NETDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <backpropnet.h>
#include <QSqlQuery>
#include <QUrl>
class NetDataBase{
public:
    NetDataBase();
    void saveBackpropNet(BackpropNet *backPropNet);
    void deleteBackpropNet(unsigned int id);
    void loadBackPropNet(BackpropNet *Net);
    unsigned int netId;
private:
    void creatTables();
    void saveNet(Net *net);
    void loadNet(unsigned int id, Net *net);
    void loadNeuron(Net *net);
    void saveNeuron(Net *net);
    void saveConnection(Neuron *neuron, unsigned int neuronId);
    void command(QString command);
    void commandOutput(QString command);
    void openDb();
    void closeDb();
    QSqlDatabase mydb;
    QSqlQuery *query;
    QUrl _savepath;
    QString _dbfilename;

};

#endif // NETDATABASE_H
