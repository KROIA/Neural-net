#include <QCoreApplication>
#include <enviroment.h>
#include <iostream>
#include <windows.h>

using namespace std;

void printNet(Net &net);
void cmdXY(int x, int y);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Enviroment enviroment;

    system("cls");
    while(true)
    {
        //cmdXY(0,0);  // Sets the cursor pos of the console
        enviroment.tick();
        enviroment.draw();
       // getchar();

    }
    return a.exec();
}
void printNet(GeneticNet &net)
{
    unsigned int animal = 0;
    printf("=================================================================================\n");
    printf("Input neurons:\n");
    printf("------------------\n");
    for(unsigned int y=0; y<net.hiddenNeuronsY(); y++)
    {
            printf("\tneuron      y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",y);
        for(unsigned int i=0; i<net.hiddenNeuron(animal,0,y)->inputs(); i++)
        {
            printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net.hiddenNeuron(animal,0,y)->input(i),net.hiddenNeuron(animal,0,y)->weight(i));
        }
            printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net.hiddenNeuron(animal,0,y)->output());
            printf("---------------------------------------------------------------------------------\n");
    }
    printf("=================================================================================\n");
    printf("Hidden neurons:\n");
    printf("------------------\n");
    for(unsigned int x=1; x<net.hiddenNeuronsX(); x++)
    {
        for(unsigned int y=0; y<net.hiddenNeuronsY(); y++)
        {
                printf("\tneuron x: %i y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",x,y);
            for(unsigned int b=0; b<net.hiddenNeuron(animal,x,y)->inputs(); b++)
            {
                printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net.hiddenNeuron(animal,x,y)->input(b),net.hiddenNeuron(animal,x,y)->weight(b));
            }
                printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net.hiddenNeuron(animal,x,y)->output());
                printf("---------------------------------------------------------------------------------\n");
        }
    }
    printf("=================================================================================\n");
    printf("Output neurons:\n");
    printf("------------------\n");
    for(unsigned int y=0; y<net.outputNeurons(); y++)
    {
        printf("\tneuron      y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",y);
        for(unsigned int b=0; b<net.outputNeuron(animal,y)->inputs(); b++)
        {
            printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net.outputNeuron(animal,y)->input(b),net.outputNeuron(animal,y)->weight(b));
        }
            printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net.outputNeuron(animal,y)->output());
            printf("---------------------------------------------------------------------------------\n");
    }
    printf("=================================================================================\n");

}
void cmdXY(int x,int y)
{
    HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {(short)x, (short)y};
    SetConsoleCursorPosition(hConsole_c,pos);
}
