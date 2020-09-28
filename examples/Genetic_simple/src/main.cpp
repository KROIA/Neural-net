// Autor        Alex Krieg
// Datum        28.09.2020
// Version      00.01.00

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
    for(unsigned int y=0; y<net.get_hiddenNeuronsY(); y++)
    {
            printf("\tneuron      y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",y);
        for(unsigned int i=0; i<net.get_ptr_hiddenNeuron(animal,0,y)->get_inputs(); i++)
        {
            printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net.get_ptr_hiddenNeuron(animal,0,y)->get_input(i),net.get_ptr_hiddenNeuron(animal,0,y)->get_weight(i));
        }
            printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net.get_ptr_hiddenNeuron(animal,0,y)->get_output());
            printf("---------------------------------------------------------------------------------\n");
    }
    printf("=================================================================================\n");
    printf("Hidden neurons:\n");
    printf("------------------\n");
    for(unsigned int x=1; x<net.get_hiddenNeuronsX(); x++)
    {
        for(unsigned int y=0; y<net.get_hiddenNeuronsY(); y++)
        {
                printf("\tneuron x: %i y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",x,y);
            for(unsigned int b=0; b<net.get_ptr_hiddenNeuron(animal,x,y)->get_inputs(); b++)
            {
                printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net.get_ptr_hiddenNeuron(animal,x,y)->get_input(b),net.get_ptr_hiddenNeuron(animal,x,y)->get_weight(b));
            }
                printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net.get_ptr_hiddenNeuron(animal,x,y)->get_output());
                printf("---------------------------------------------------------------------------------\n");
        }
    }
    printf("=================================================================================\n");
    printf("Output neurons:\n");
    printf("------------------\n");
    for(unsigned int y=0; y<net.get_outputNeurons(); y++)
    {
        printf("\tneuron      y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",y);
        for(unsigned int b=0; b<net.get_ptr_outputNeuron(animal,y)->get_inputs(); b++)
        {
            printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net.get_ptr_outputNeuron(animal,y)->get_input(b),net.get_ptr_outputNeuron(animal,y)->get_weight(b));
        }
            printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net.get_ptr_outputNeuron(animal,y)->get_output());
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
