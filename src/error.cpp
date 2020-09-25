#include <error.h>


Error::Error(const Error *err)
{
    if(err != nullptr){
        this->_namespace    = err->_namespace;
        this->_messageList  = err->_messageList;
    }else{
        this->_namespace    =   "";
        this->_messageList  =   std::vector<std::string>();
    }
}
Error::Error(const Error &err)
{
    this->_namespace    = err._namespace;
    this->_messageList  = err._messageList;
}
Error::Error(const std::string __namespace,const std::vector<std::string> __messageList)
{
    this->_namespace    =   __namespace;
    this->_messageList  =   __messageList;
}
Error::Error(const std::string __namespace,const std::string __message)
{
    this->_namespace    =   __namespace;
    this->_messageList  = std::vector<std::string>();
    this->_messageList.push_back(__message);
}
Error::Error(const std::string __message)
{
    this->_namespace    =   __message;
    this->_messageList  =   std::vector<std::string>();
}
Error::Error(const std::vector<std::string> __messageList)
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

void Error::setNamespace(const std::string &space)
{
    this->_namespace    =   space;
}
std::string Error::getNamespace() const
{
    return this->_namespace;
}

void Error::addMessage(const std::string &message)
{
    this->_messageList.push_back(message);
}
void Error::addMessageList(const std::vector<std::string> &messageList)
{
    this->_messageList.insert(this->_messageList.end(),messageList.begin(),messageList.end());
}
std::vector<std::string> Error::getMessageList() const
{
    return _messageList;
}

std::string Error::toString() const
{
    std::string message = "ERROR: "+_namespace+":\n";
    for(unsigned int line=0; line <_messageList.size(); line++)
    {
        message+=_messageList[line]+"\n";
    }
    return message;
}
std::vector<std::string> Error::toStringList() const
{
    std::vector<std::string> messageList;
    messageList.reserve(_messageList.size()+1);
    messageList.push_back("ERROR: "+_namespace+":\n");
    messageList.insert(messageList.end(),_messageList.begin(),_messageList.end());
    return messageList;
}
void Error::print(Error &e)
{
    CONSOLE << "Error: "<<e.getNamespace();
    for(unsigned int a=0; a<e.getMessageList().size(); a++)
    {
        CONSOLE << "   "<<e.getMessageList()[a];
    }
    CONSOLE << "\n";
}
void Error::print()
{
    print(*this);
}

std::string ErrorMessage::listIsEmpty(std::string varName)
{
        return varName+" is empty";
}
std::string ErrorMessage::updateNetFirst()
{
    return "Update required: call updateNetConfiguration() first!";
}
