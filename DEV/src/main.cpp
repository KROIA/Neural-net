#include <QApplication>
#include <iostream>
#include <windows.h>
#include <geneticnet.h>
#include <savenet.h>
#include <netvisu.h>
#include <QtQuickWidgets/QQuickWidget>

using namespace std;



FILE *genomlogFile;
FILE *logfile;
void printNet(Net* net);
//void netFinished(BackpropNet net);
void cmdXY(int x, int y);
void logGenom(vector<double> genom,string name);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    printf("setup Net()\n");

    unsigned int animals = 100;
    unsigned int inputNeurons = 4;
    unsigned int hiddenNeuronX= 1;
    unsigned int hiddenNeuronY= 4;
    unsigned int outputNeuron = 1;
    bool bias = true;
    bool enableAverage = false;


    GeneticNet net(animals,inputNeurons,hiddenNeuronX,hiddenNeuronY,outputNeuron,bias,enableAverage,Activation::Sigmoid); //Makes the Net object
    net.set_netFileName("netFile");
    net.loadFromNetFile();


    //GeneticNet net(saveNet.animals(),saveNet.inputNeurons(),saveNet.hiddenNeuronsX(),saveNet.hiddenNeuronsY(),saveNet.outputNeurons(),saveNet.bias(),saveNet.enableAverage(),saveNet.activationFunction()); //Makes the Net object
    qDebug() << "inputs: "<<net.get_inputNeurons();
    qDebug() << "hiddenX: "<<net.get_hiddenNeuronsX();
    qDebug() << "hiddenY: "<<net.get_hiddenNeuronsY();
    qDebug() << "outputs: "<<net.get_outputNeurons();
    qDebug() << "animals: "<<net.get_animals();
    net.set_mutationFactor(0.5);
    net.set_mutationChangeWeight(0.01);

    net.updateNetConfiguration();

    QQuickWidget widget;
    NetVisu visu(net.get_netList_ptr());
    visu.loadNetInUi(&widget);
    visu.showWindow();

   // saveNet.set(net.inputNeurons(),net.hiddenNeuronsX(),net.hiddenNeuronsY(),net.outputNeurons(),
   //             net.bias(),net.enableAverage(),net.activationFunction(),net.biasValue());
   //saveNet.addGenom(net.genom());
   //saveNet.saveFile();
   // saveNet.loadFile();
   // saveNet.saveFile("newFile");


    genomlogFile = fopen("genom.csv","w");
    for(unsigned int a=0; a<net.get_genomsize(); a++)
    {
        fprintf(genomlogFile,"w%i;",a+1);
    }
    fprintf(genomlogFile,"\n");
    fclose(genomlogFile);
//    logfile = fopen("score.csv","w");
//    fclose(logfile);

    vector<vector<double>    > inpuList(animals,vector<double>(inputNeurons,0));
    vector<vector<double>    > genom;
    vector<vector<double>    > output;
    printf("net done, press enter\n");
    getchar();
    unsigned int counter =0;
    unsigned int saveCounter = 0;
    unsigned int saves = 0;
    double averageScore = 0;
    vector<double> scoreList(animals,0);


    system("cls");
    unsigned long learningSteps = 0;
    logGenom(net.get_genom(0),"gen0.csv");
    logGenom(net.get_genom(1),"gen1.csv");

    while(true)
    {
        QCoreApplication::processEvents();
        cmdXY(0,0);  // Sets the cursor pos of the console

        for(unsigned int a=0; a<animals; a++)
        {
            scoreList[a] = 0;
        }
        for(unsigned int x=0; x<10; x++)
        {
            for(unsigned int a=0; a<animals; a++)
            {

                for(unsigned int b=0; b<inputNeurons; b++)
                {
                    inpuList[a][b] = (double)(rand()%100)/100;
                }
            }

            net.set_input(inpuList);       // Sets the input of the net with the trainingset [counter]
            net.run();
            output = net.get_output();                  // Calculates the output vector and returns it




            averageScore = 0;
            for(unsigned int a=0; a<animals; a++)
            {
                double tmp = 0;
                for(unsigned int b=0; b<inputNeurons; b++)
                {
                    tmp += inpuList[a][b];
                }
                tmp /= inputNeurons;
                scoreList[a] += 10*(2-abs(tmp - output[a][0]));
            }
        }
        for(unsigned int a=0; a<animals; a++)
        {
            scoreList[a] /= 10;
            averageScore+=scoreList[a];
        }
        averageScore /= animals;

        net.set_score(scoreList);
        net.learn();                            //Improve the net
        genom = net.get_genom();

        saveCounter++;
        learningSteps++;



       if(saveCounter > 5)
       {
           saveCounter = 0;
           printNet(net[0]);
           //saves+=10; //spam the console in the beginning and later no more
           printf("averScore: %.5f\n",averageScore);   //Prints the error
           printf("score[0]: %.5f\n",scoreList[0]);    //Prints the learn cyles
           printf("steps: %i\n",learningSteps);    //Prints the learn cyles


           logfile = fopen("score.csv","a");           //Saves the error in the file: score.csv
           fprintf(logfile,"%.5f;\n",averageScore);    //
           fclose(logfile);                            //


                                 //Saves all weights of the net in: genom.csv so you can track the weights over the time of improvement
           net.saveToNetFile();
           getchar();
       }


      /* for(unsigned int a=1; a<animals; a++)
       {
           for(unsigned int b=0; b<net.genomsize(); b++)
           {
            genom[0][b] += genom[a][b];
           }
       }
       for(unsigned int b=0; b<net.genomsize(); b++)
       {
           genom[0][b] /= animals;
       }
       logGenom(net.genom(0),"gen0.csv");
       logGenom(genom[0],"genAver.csv");
        */
   }
    return a.exec();
}
void printNet(Net *net)
{
    printf("=================================================================================\n");
    printf("Input neurons:\n");
    printf("------------------\n");
    for(unsigned int y=0; y<net->get_hiddenNeuronsY(); y++)
    {
            printf("\tneuron      y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",y);
        for(unsigned int i=0; i<net->get_ptr_hiddenNeuron(0,y)->get_inputs(); i++)
        {
            printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net->get_ptr_hiddenNeuron(0,y)->get_input(i),net->get_ptr_hiddenNeuron(0,y)->get_weight(i));
        }
            printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net->get_ptr_hiddenNeuron(0,y)->get_output());
            printf("---------------------------------------------------------------------------------\n");
    }
    printf("=================================================================================\n");
    printf("Hidden neurons:\n");
    printf("------------------\n");
    for(unsigned int x=1; x<net->get_hiddenNeuronsX(); x++)
    {
        for(unsigned int y=0; y<net->get_hiddenNeuronsY(); y++)
        {
                printf("\tneuron x: %i y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",x,y);
            for(unsigned int b=0; b<net->get_ptr_hiddenNeuron(x,y)->get_inputs(); b++)
            {
                printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net->get_ptr_hiddenNeuron(x,y)->get_input(b),net->get_ptr_hiddenNeuron(x,y)->get_weight(b));
            }
                printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net->get_ptr_hiddenNeuron(x,y)->get_output());
                printf("---------------------------------------------------------------------------------\n");
        }
    }
    printf("=================================================================================\n");
    printf("Output neurons:\n");
    printf("------------------\n");
    for(unsigned int y=0; y<net->get_outputNeurons(); y++)
    {
        printf("\tneuron      y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",y);
        for(unsigned int b=0; b<net->get_ptr_outputNeuron(y)->get_inputs(); b++)
        {
            printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net->get_ptr_outputNeuron(y)->get_input(b),net->get_ptr_outputNeuron(y)->get_weight(b));
        }
            printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net->get_ptr_outputNeuron(y)->get_output());
            printf("---------------------------------------------------------------------------------\n");
    }
    printf("=================================================================================\n");

}

/*void netFinished(GeneticNet net)
{
    vector<double> genom;
    vector<double> output;

    genom = net.genom();
    system("cls");
    printf("Genom: \n | ");
    for(unsigned int a=0; a<genom.size(); a++)
    {
        printf("%.3f\t|\t",genom[a]);
        if((a+1)%5 == 0)
        {
            printf("\n | ");
        }
    }
    printf("\n=================================================================================\n\n");

    while(true)
    {

        for(unsigned int counter=0; counter<trainingsSet.size(); counter++)
        {




            cmdXY(0,genom.size() / 5 +4);
            net.input(trainingsSet[counter]);
            output = net.output();

            printf("inputs are:\t");
            for(unsigned int a=0; a<trainingsSet[counter].size(); a++)
            {
                printf("%.3f | ",trainingsSet[counter][a]);
            }
            printf("\noutputs are:\t");
            for(unsigned int a=0; a<output.size(); a++)
            {
                printf("%.3f | ",output[a]);
            }
            printf("\n           \t=====");
            printf("\n\n\n=================================================================================\n");
            printNet(net);
            printf("\n=================================================================================\n");
            getchar();
        }
    }
}*/
void cmdXY(int x,int y)
{
    HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {(short)x, (short)y};
    SetConsoleCursorPosition(hConsole_c,pos);
}
void logGenom(vector<double> genom,string name)
{
    genomlogFile = fopen(name.c_str(),"a");
    for(unsigned int a=0; a<genom.size(); a++)
    {
        fprintf(genomlogFile,"%.5f;",genom[a]);
    }
    fprintf(genomlogFile,"\n");
    fclose(genomlogFile);
}
