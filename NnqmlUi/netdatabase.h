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
private:
    void creatTables();
    void saveNet(Net *net);
    void saveNeuron(unsigned int posX, unsigned int posY);
    void saveConnection(Net *net);
    void command(QString command);
    void commandOutput(QString command);
    void openDb();
    void closeDb();
    QSqlDatabase mydb;
    QSqlQuery *query;
    QUrl _savepath;
    QString _dbfilename;
    unsigned int netId;
};

#endif // NETDATABASE_H
