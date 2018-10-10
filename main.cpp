#include <QCoreApplication>
#include <neuron.h>
#include <net.h>
#include <Config/Config/config.h>
#include <QDebug>
#include <iostream>

using namespace std;
void printNet(Net net);
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
  /*  Config config("configFile.txt");
    config.deleteMultipleParameter(true);

    try{
        config.read();
    }
    catch(const std::runtime_error& e){
        std::cout<< "ERROR: "<< e.what();
    }
    std::vector<std::string>    paramName = config.parameterList();
    std::vector<std::string>    paramValue = config.parameterValue();
    std::vector<std::string>    head = config.fileHead();
    std::cout << "HEAD\n";
    for(int a=0; a<paramName.size(); a++)
    {
        std::cout <<"head: |"<<head[a]<< "\n";
    }
    std::cout << "paramList\n";
    for(int a=0; a<paramName.size(); a++)
    {
        std::cout <<"paramName: "<<paramName[a] << "\tparamValue: "<<paramValue[a] << "\n";
    }

    config.parameter("param1","value1");
    config.parameter("param2","value2");
    config.parameter("param3","value3");
    config.save();*/
    vector<Neuron   >vec(3,Neuron(1,Activation::Linear,false));
   // vec = vector<Neuron   >(3,Neuron(1,Activation::Sigmoid,false));
    Neuron n;
    printf("neuronDone\n");

    Net net(3,2,2,3,true,false,Activation::Linear);
    printf("net done\n");
    getchar();
    vector<float> genom;
    vector<float> input(net.inputNeurons(),0);
    vector<float> output;
    getchar();
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
    }
    return a.exec();
}
void printNet(Net net)
{
    printf("Input neurons:\n");
    for(unsigned int a=0; a<net.hiddenNeuronsY(); a++)
    {
        printf("\tneuron y: %i input\tweight\toutput\n",a);
        for(unsigned int b=0; b<net.hiddenNeuron(0,a).inputs(); b++)
        {
            printf("\t\t\ti %.3f\tw %.3f\n",net.hiddenNeuron(0,a).input(b),net.hiddenNeuron(0,a).weight(b));
        }
            printf("\t\t\t      \t      \tr %3f\n",net.hiddenNeuron(0,a).output());
    }
    printf("Hidden neurons:\n");
    for(unsigned int x=1; x<net.hiddenNeuronsX(); x++)
    {
        for(unsigned int y=0; y<net.hiddenNeuronsY(); y++)
        {
            printf("\tneuron x: %i  y: %i input\tweight\toutput\n",x,y);
            for(unsigned int b=0; b<net.hiddenNeuron(x,y).inputs(); b++)
            {
                printf("\t\t\ti %.3f\tw %.3f\n",net.hiddenNeuron(x,y).input(b),net.hiddenNeuron(x,y).weight(b));
            }
                printf("\t\t\t      \t      \tr %3f\n",net.hiddenNeuron(x,y).output());
        }
    }
    printf("Output neurons:\n");
    for(unsigned int y=0; y<net.outputNeurons(); y++)
    {
        printf("\tneuron y: %i input\tweight\toutput\n",y);
        for(unsigned int b=0; b<net.outputNeuron(y).inputs(); b++)
        {
            printf("\t\t\ti %.3f\tw %.3f\n",net.outputNeuron(y).input(b),net.outputNeuron(y).weight(b));
        }
            printf("\t\t\t      \t      \tr %3f\n",net.outputNeuron(y).output());
    }

}
