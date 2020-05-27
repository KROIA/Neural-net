#include <error.h>


Error::Error(const Error *err)
{
    if(err != nullptr){
        this->_namespace    = err->_namespace;
        this->_messageList  = err->_messageList;
    }else{
        this->_namespace    =   "";
        this->_messageList  =   QStringList();
    }
}
Error::Error(const Error &err)
{
    this->_namespace    = err._namespace;
    this->_messageList  = err._messageList;
}
Error::Error(const QString __namespace,const QStringList __messageList)
{
    this->_namespace    =   __namespace;
    this->_messageList  =   __messageList;
}
Error::Error(const QString __namespace,const QString __message)
{
    this->_namespace    =   __namespace;
    this->_messageList  = QStringList();
    this->_messageList.push_back(__message);
}
Error::Error(const QString __message)
{
    this->_namespace    =   __message;
    this->_messageList  =   QStringList();
}
Error::Error(const QStringList __messageList)
{
    this->_namespace    =   "";
    this->_messageList  =   __messageList;
}


Error::~Error()
{
    _messageList.clear();
}
Error &Error::operator=(const Error &e)
{
    this->_namespace    =   e._namespace;
    this->_messageList  =   e._messageList;
    return *this;
}

void Error::setNamespace(const QString &space)
{
    this->_namespace    =   space;
}
QString Error::getNamespace() const
{
    return this->_namespace;
}

void Error::addMessage(const QString &message)
{
    this->_messageList.push_back(message);
}
void Error::addMessageList(const QStringList &messageList)
{
    this->_messageList.append(messageList);
}
QStringList Error::getMessageList() const
{
    return _messageList;
}

QString Error::toQString() const
{
    QString message = "ERROR: "+_namespace+":\n";
    for(int line=0; line <_messageList.size(); line++)
    {
        message+=_messageList[line]+"\n";
    }
    return message;
}
QStringList Error::toQStringList() const
{
    QStringList messageList;
    messageList.reserve(_messageList.size()+1);
    messageList.push_back("ERROR: "+_namespace+":\n");
    messageList.append(_messageList);
    return messageList;
}
void Error::print(Error &e)
{
    std::cout << "Error: "<<e.getNamespace().toStdString();
    for(int a=0; a<e.getMessageList().size(); a++)
    {
        std::cout << "   "<<e.getMessageList()[a].toStdString();
    }
}
void Error::print()
{
    print(*this);
}

QString ErrorMessage::listIsEmpty(QString varName)
{
        return varName+" is empty";
}
QString ErrorMessage::updateNetFirst()
{
    return "Update required: call updateNetConfiguration() first!";
}
