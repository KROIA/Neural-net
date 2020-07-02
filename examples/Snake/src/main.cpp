#include "snake.h"
#include <QApplication>
std::string GetLastErrorStdStr();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("AIOpenCloedProject");
    a.setOrganizationDomain("https://github.com/KROIA/Neural-net");
    Snake w;
    //w.show();

    try{
        return a.exec();
    }
    catch(std::runtime_error &e)
    {
        qDebug() << "ERROR  main "<< e.what();
        FILE *file = fopen("error.txt","a");
        if(file)
        {
            fprintf(file,"%s\n",(std::string("ERROR  main ")+e.what()).c_str());
            fclose(file);
        }
    }catch(...)
    {
        qDebug() << "ERROR  main -> unnkown "<<QString::fromStdString(GetLastErrorStdStr());
        FILE *file = fopen("error.txt","a");
        if(file)
        {
            fprintf(file,"%s\n",(std::string("ERROR  main -> unnkown ")+GetLastErrorStdStr()).c_str());
            fclose(file);
        }
    }
}
std::string GetLastErrorStdStr()
{
  DWORD error = GetLastError();
  if (error)
  {
    LPVOID lpMsgBuf;
    DWORD bufLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );
    if (bufLen)
    {
      LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
      std::string result(lpMsgStr, lpMsgStr+bufLen);

      LocalFree(lpMsgBuf);

      return result;
    }
  }
  return std::string();
}
