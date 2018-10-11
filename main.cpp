#include <QCoreApplication>
#include <neuron.h>
#include <net.h>
#include <QDebug>
#include <iostream>

using namespace std;
void printNet(Net net);
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    unsigned int inputNeurons = 2;
    unsigned int hiddenNeuronX= 2;
    unsigned int hiddenNeuronY= 2;
    unsigned int outputNeuron = 2;
    bool bias = false;
    bool enableAverage = false;

    Net net(inputNeurons,hiddenNeuronX,hiddenNeuronY,outputNeuron,bias,enableAverage,Activation::Linear);

    vector<float> genom;
    vector<float> input;
    vector<float> output;
    printf("net done, press enter\n");
    getchar();
    int counter =0;
    while(true)
    {
        printf("Getting genom\n ");
        genom = net.genom();
        printf("Genom: ");
        for(unsigned int a=0; a<genom.size(); a++)
        {
            printf("%.3f | ",genom[a]);
            //genom[a] = 1;
        }
        printf("\nSet genom\n");
        net.genom(genom);
        printf("Inputs: ");
        input = vector<float>(net.inputNeurons(),0);
        for(unsigned int a=0; a<input.size(); a++)
        {
            input[a] = 1/*(float)(rand() % 2000)/1000 -1*/;
            printf("%.3f | ",input[a]);
        }
        printf("\nSet inputs\n");
        net.input(input);
        printf("getting the output\n");
        output = net.output();
        printf("Output: ");
        for(unsigned int a=0; a<output.size(); a++)
        {
            printf("%.3f | ",output[a]);
        }
        printf("\ndone\n\n");
        printNet(net);
        getchar();
        counter++;
        switch(counter)
        {
            case 1:
            {
                net.bias(true);
                break;
            }
            case 2:
            {
                net.enableAverage(true);
                break;
            }
            case 3:
            {
                net.biasValue(0.5);
                break;
            }
            case 4:
            {
                net.inputNeurons(2);
                net.hiddenNeuronsX(3);
                net.hiddenNeuronsY(3);
                net.outputNeurons(1);
                break;
            }
            case 5:
            {
                net.activationFunction(Activation::Sigmoid);
                break;
            }
            case 6:
            {

                break;
            }
        }
    }
    return a.exec();
}
void printNet(Net net)
{
    printf("Input neurons:\n");
    for(unsigned int y=0; y<net.hiddenNeuronsY(); y++)
    {
        printf("\tneuron y: %i input\tweight\toutput\n",y);
        for(unsigned int i=0; i<net.hiddenNeuron(0,y).inputs(); i++)
        {
            printf("\t\t\ti %.2f\tw %.2f\n",net.hiddenNeuron(0,y).input(i),net.hiddenNeuron(0,y).weight(i));
        }
            printf("\t\t\t      \t      \tr %.2f\n",net.hiddenNeuron(0,y).output());
    }
    printf("Hidden neurons:\n");
    for(unsigned int x=1; x<net.hiddenNeuronsX(); x++)
    {
        for(unsigned int y=0; y<net.hiddenNeuronsY(); y++)
        {
            printf("\tneuron x: %i  y: %i input\tweight\toutput\n",x,y);
            for(unsigned int b=0; b<net.hiddenNeuron(x,y).inputs(); b++)
            {
                printf("\t\t\ti %.2f\tw %.2f\n",net.hiddenNeuron(x,y).input(b),net.hiddenNeuron(x,y).weight(b));
            }
                printf("\t\t\t      \t      \tr %.2f\n",net.hiddenNeuron(x,y).output());
        }
    }
    printf("Output neurons:\n");
    for(unsigned int y=0; y<net.outputNeurons(); y++)
    {
        printf("\tneuron y: %i input\tweight\toutput\n",y);
        for(unsigned int b=0; b<net.outputNeuron(y).inputs(); b++)
        {
            printf("\t\t\ti %.2f\tw %.2f\n",net.outputNeuron(y).input(b),net.outputNeuron(y).weight(b));
        }
            printf("\t\t\t      \t      \tr %.2f\n",net.outputNeuron(y).output());
    }

}
