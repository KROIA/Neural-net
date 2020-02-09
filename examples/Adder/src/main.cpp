#include <QCoreApplication>
#include <backpropnet.h>
#include <iostream>
#include <windows.h>
#include "pid.h"

#include <QString>
#include <QDebug>
#include<stdio.h>
#include<time.h>

using namespace std;

static std::vector<std::vector<double>    >trainingsSet;  //Trainings input std::vector
static std::vector<std::vector<double>    >outputSet;     //Trainings expected output std::vector


static FILE *genomlogFile;
static FILE *logfile;

static std::chrono::high_resolution_clock::time_point t2;
static std::chrono::duration<double> time_span;
static std::chrono::high_resolution_clock::time_point t1;


void printNet(Net *net);
void setupTrainingSet();
void netFinished(BackpropNet *net);
void cmdXY(int x, int y);
void logGenom(std::vector<double> genom);
void generateTone();
class ErrorHandler;



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);




    generateTone();// Generate the trainingsset

    printf("setup\n");
    unsigned int inputNeurons   = trainingsSet[0].size(); //Makes the amount of inputs dependend of the training set
    unsigned int hiddenNeuronX  = 0;
    unsigned int hiddenNeuronY  = 0;
    unsigned int outputNeuron   = outputSet[0].size();    //Makes the amount of outputs dependent of the training set
    bool bias                   = true;
    bool enableAverage          = false;

    BackpropNet *net = new BackpropNet(0,inputNeurons,hiddenNeuronX,hiddenNeuronY,outputNeuron,bias,enableAverage,Activation::Linear); //Makes the Net object

    net->loadFromNetFile();
    net->set_mutationFactor(0.1);
    net->updateNetConfiguration();
    net->saveToNetFile();


    genomlogFile = fopen("genom.csv","r");
    if(!genomlogFile)
    {
        fclose(genomlogFile);
        genomlogFile = fopen("genom.csv","w");
        for(unsigned int a=0; a<net->get_genomsize(); a++)
        {
            fprintf(genomlogFile,"w%i;",a+1);
        }
        fprintf(genomlogFile,"\n");
        fclose(genomlogFile);
    }
    std::vector<double> genom;
    std::vector<double> output;
    std::cout << net->toString().toStdString();
    printf("net done, press enter\n");
    getchar();
    unsigned int counter =0;
    unsigned int saveCounter = 0;
    unsigned int saves = 0;
    double averageError = 0;



    system("cls");
    unsigned long learningSteps = 0;
    clock_t startTime = clock();
    t1 = std::chrono::high_resolution_clock::now();
    double averageCalcTime = 0;
    while(true)
    {
        if(net->get_errorAmount() != 0)
        {
            ErrorList errors = net->get_errorList();
            for(int error=0; error<errors.size(); error++)
            {
                std::cout << "Error: "<<errors[error].toQString().toStdString();
            }
            net->clearErrors();
            getchar();
        }


        net->set_input(trainingsSet[counter]);       // Sets the input of the net with the trainingset [counter]
        net->run();
        averageCalcTime = averageCalcTime*0.8+0.2*net->get_runtime();
        output = net->get_output();                  // Calculates the output std::vector and returns it
        net->set_expected(outputSet[counter]);       // Tells the net the right results

        averageError += abs(net->get_netError());    //The net calculates the error of netprediction and expected output
                                                //Saving only the positive value of the error to stop the training later when the error is low enough
        net->learn();                            //Improve the net
        learningSteps++;                        //Adding one training cycle


        counter++;                              //counts to the next trainingset
        saveCounter++;

        if(counter >= trainingsSet.size())
        {
            counter = 0;
            averageError /= trainingsSet.size(); //takes the average error of the whole training set
            if(saveCounter > saves)
            {
                cmdXY(0,0);  // Sets the cursor pos of the console
                saveCounter = 0;
                saves+=100; //spam the console in the beginning and later no more
                printf("error: %.5f\n",averageError);   //Prints the error
                printf("steps: %i\n",learningSteps);    //Prints the learn cyles

                logfile = fopen("score.csv","a");           //Saves the error in the file: score.csv
                fprintf(logfile,"%.5f;\n",averageError);    //
                fclose(logfile);                            //



                net->saveToNetFile();                        //Save the genom

                logGenom(net->get_genom());                      //Saves all weights of the net in: genom.csv so you can track the weights over the time of improvement

            }
            if(averageError < 0.0005 || learningSteps > 1000000)//Learn until the error is below 0.005 or learning cycles are more then 1000000
            {
                clock_t endTime = clock();
                t2 = std::chrono::high_resolution_clock::now();
                time_span = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
                cmdXY(0,0);  // Sets the cursor pos of the console
                saveCounter = 0;
                //saves+=100; //spam the console in the beginning and later no more
                printf("error: %.5f\n",averageError);   //Prints the error
                printf("steps: %i\n",learningSteps);    //Prints the learn cyles

                logfile = fopen("score.csv","a");           //Saves the error in the file: score.csv
                fprintf(logfile,"%.5f;\n",averageError);    //
                fclose(logfile);                            //



                net->saveToNetFile();                        //Save the genom

                logGenom(net->get_genom());
                qDebug() << "time: "<< time_span.count() << "sec\t" << QString::number(double(endTime-startTime)/CLOCKS_PER_SEC) << "\t calcTime: "<<averageCalcTime;
                getchar();
                netFinished(net);
            }
            averageError = 0;

        }
    }
    return a.exec();
}
void printNet(Net *net)
{
    for(unsigned int b=0; b<net->get_neurons(); b++)
    {
    QStringList    list = net->get_ptr_neuron_viaID(b)->toStringList();
    for(int a=0; a<list.size(); a++)
    {
        printf(list[a].toStdString().c_str());
    }
    printf("------------------------------------\n");
    }
}
void netFinished(BackpropNet *net)
{
    std::vector<double*> *genom;
    std::vector<double> output;

    generateTone();

    genom = net->get_ptr_genom();
    system("cls");
    printf("Genom: \n | ");
    for(unsigned int a=0; a<(*genom).size(); a++)
    {
        printf("%.3f\t|\t",*(*genom)[a]);
        if((a+1)%5 == 0)
        {
            printf("\n | ");
        }
    }
    printf("\n=================================================================================\n\n");
    FILE *file;
    file = fopen("toneOut.csv","w");
    fprintf(file,"input1;input2;output;error;\n");
    for(unsigned int counter=0; counter<trainingsSet.size(); counter++)
    {
        net->set_input(trainingsSet[counter]);
        net->run();
        output = net->get_output();
        fprintf(file,"%.8f;%.8f;%.8f;%.8f;\n",trainingsSet[counter][0],trainingsSet[counter][1],output[0],output[0]-outputSet[counter][0]);
    }
    fclose(file);
    while(true)
    {
        for(unsigned int counter=0; counter<trainingsSet.size(); counter++)
        {
            cmdXY(0,(*genom).size() / 5 +4);
            net->set_input(trainingsSet[counter]);
            net->run();
            output = net->get_output();

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
            double sum = 0;
            for(unsigned int index=0; index<trainingsSet[counter].size(); index++)
            {
                sum += trainingsSet[counter][index];
            }

            printf("\nerror: %.8f",output[0] - sum);
            printf("\n           \t=====");
            printf("\n\n\n=================================================================================\n");
            printNet(net);
            printf("\n=================================================================================\n");
            getchar();
        }
    }
}
void cmdXY(int x,int y)
{
    HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {static_cast<short>(x), static_cast<short>(y)};
    SetConsoleCursorPosition(hConsole_c,pos);
}
void logGenom(std::vector<double> genom)
{
    genomlogFile = fopen("genom.csv","a");
    for(unsigned int a=0; a<genom.size(); a++)
    {
        fprintf(genomlogFile,"%.5f;",genom[a]);
    }
    fprintf(genomlogFile,"\n");
    fclose(genomlogFile);
}
void generateTone()
{
    PID pid(0.1,0.020,0.05);
    PID pid2(0.1,0.020,0.05);
    trainingsSet.clear();
    outputSet.clear();
    printf("generate tone\n");
    FILE *file;
    file = fopen("tone.csv","w");

    int amount = 300;
    int range = 100;
    double target = 1;
    double target2 = -1;
    double analogValue = 0;
    double pid2Value = 0;
    //double delayed = 1;
    pid.expected(target);
    bool _switch = false;
    for(int counter = 0; counter<amount; counter++)
    {
        if(_switch)
        target = static_cast<double>((rand()%range)-(range/2))/static_cast<double>(range);
        else
        target2 = static_cast<double>((rand()%range)-(range/2))/static_cast<double>(range);

        _switch = !_switch;
        pid2.expected(target2);
        pid.expected(target);
        for(int counter1 = 0; counter1<50; counter1++)
        {

            pid.input(analogValue);
            pid.update();
            analogValue += pid.output();

            pid2.input(pid2Value);
            pid2.update();
            pid2Value += pid2.output();
            trainingsSet.push_back({analogValue,pid2Value});
            outputSet.push_back({analogValue+pid2Value});
            fprintf(file,"%.8f;%.8f;%.8f;%.8f;%.8f\n",target,target2,analogValue,pid2Value,analogValue+pid2Value);
        }
        printf("target: %.8f\tvalue: %.8f error: %.8f \t out: %.8f \tp: %.8f \ti: %.8f \td: %.8f \n",target,analogValue,pid.error(),pid.output(),pid.P(),pid.I(),pid.D());
    }
    fclose(file);
    printf("generate tone done\n");
}
