#ifndef ERROR_H
#define ERROR_H
//      Autor   Alex Krieg
//      Version 00.00.00
//      Datum   29.01.2020

#include <QObject>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QList>
#include <iostream>

class Error;

typedef QList<Error> ErrorList;

class Error : public QObject
{
    Q_OBJECT
    public:
       Error(const Error *err = nullptr);
       Error(const Error &err);
       Error(const QString __namespace,const QStringList __messageList);
       Error(const QString __namespace,const QString __message);
       Error(const QString __message);
       Error(const QStringList __messageList);

       ~Error();
       Error &operator=(const Error &e);

       void setNamespace(const QString &space);
       QString getNamespace() const ;

       void addMessage(const QString &message);
       void addMessageList(const QStringList &messageList);
       QStringList getMessageList() const;

       QString toQString() const;
       QStringList toQStringList() const;

      /*
            comparisonLower Must be '[' or ']'
            '[' -> range including the value "minValue"
            ']' -> range excluding the value "minValue"

            same for upper limit but reverse.
            General: When the bracket faces away from the number, the number will be excluted
        */
        static void print(Error &e);
        void print();

    private:
        QString _namespace; // where is the error occured?
        QStringList _messageList;
};
namespace ErrorMessage{
    template<typename  T>
    QString outOfRange(char comparisonLower,T minValue,T value,T maxValue,char comparisonHigher){
        return " "+QString::number(value)+" Is out of range. Value must be in Range: "+
                comparisonLower+QString::number(minValue)+":"+QString::number(maxValue)+comparisonHigher;
    }

    extern QString listIsEmpty(QString varName);
    extern QString updateNetFirst();
}
#endif // ERROR_H

