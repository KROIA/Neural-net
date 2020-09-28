#ifndef ERROR_H
#define ERROR_H
//      Autor   Alex Krieg
//      Version 00.01.00
//      Datum   25.09.2020

#include <vector>
#define QT_APP

#ifdef QT_APP
  #include <QDebug>
  #include <QObject>
#endif

#ifdef QDEBUG_H
#ifndef CONSOLE
#define CONSOLE qDebug()
#endif
#else
#include <iostream>
#include <stdio.h>
#ifndef CONSOLE
#define CONSOLE std::cout
#endif
#endif

class Error;

typedef std::vector<Error> ErrorList;

#ifdef QT_APP
class Error : public QObject
#else
class Error
#endif
{
#ifdef QT_APP
    Q_OBJECT
#endif
    public:
       Error(const Error *err = nullptr);
       Error(const Error &err);
       Error(const std::string __namespace,const std::vector<std::string> __messageList);
       Error(const std::string __namespace,const std::string __message);
       Error(const std::string __message);
       Error(const std::vector<std::string> __messageList);

       ~Error();
       Error &operator=(const Error &e);

       void setNamespace(const std::string &space);
       std::string getNamespace() const ;

       void addMessage(const std::string &message);
       void addMessageList(const std::vector<std::string> &messageList);
       std::vector<std::string> getMessageList() const;

       std::string toString() const;
       std::vector<std::string> toStringList() const;

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
        std::string _namespace; // where is the error occured?
        std::vector<std::string> _messageList;
};
namespace ErrorMessage{
    template<typename  T>
    std::string outOfRange(char comparisonLower,T minValue,T value,T maxValue,char comparisonHigher){
        return " "+std::to_string(value)+" Is out of range. Value must be in Range: "+
                comparisonLower+std::to_string(minValue)+":"+std::to_string(maxValue)+comparisonHigher;
    }

    extern std::string listIsEmpty(std::string varName);
    extern std::string updateNetFirst();
}
#endif // ERROR_H

