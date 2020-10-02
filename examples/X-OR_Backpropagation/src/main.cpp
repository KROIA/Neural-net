// Autor        Alex Krieg
// Datum        02.10.2020
// Version      00.01.01

#include <QCoreApplication>
#include <backpropnet.h>
#include <iostream>
#include <windows.h>

#include <QString>
#include <QDebug>
#include<stdio.h>
#include<time.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

//#define LOG_MUCH_DETAILED


using namespace std;

static std::vector<std::vector<double>    >trainingsSet;  //Trainings input std::vector
static std::vector<std::vector<double>    >outputSet;     //Trainings expected output std::vector

#ifdef LOG_MUCH_DETAILED
    static std::vector<std::vector<double>    >logGenomList;
    static std::vector<double    >             logErrorList;
    static std::vector<unsigned int>           logIterationList;
    const unsigned int                         logReserveSize = 10000;
#endif

static FILE *genomlogFile;
static FILE *logfile;

static std::chrono::high_resolution_clock::time_point t2;
static std::chrono::duration<double> time_span;
static std::chrono::high_resolution_clock::time_point t1;

void printNet(Net *net);
void setupTrainingSet();
void netFinished(BackpropNet *net);
void cmdXY(unsigned int x,unsigned int y);
#ifdef LOG_MUCH_DETAILED
    void logGenom();
    void logError();
#endif
void logGenom(std::vector<double> genom);

void logError(const unsigned int &steps, const double &error);
class ErrorHandler;

static std::vector<std::vector<std::vector<double> >   > _genomCompareList;
static std::vector<std::vector<double>  >_runtimeList;


//static ErrorHandler    *errorHandler;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setupTrainingSet(); //Setting the trainingset for the X-OR problem

    unsigned int netID              = 0;
    unsigned int inputNeurons       = trainingsSet[0].size(); //Makes the amount of inputs dependend of the training set
    unsigned int hiddenNeuronX      = 1;
    unsigned int hiddenNeuronY      = 2;
    unsigned int outputNeuron       = outputSet[0].size();    //Makes the amount of outputs dependent of the training set
    bool enableBias                 = true;
    bool enableAverage              = false;
    Activation activation           = Activation::Gaussian;

    //Makes the Net object
    BackpropNet *net = new BackpropNet(netID,
                                       inputNeurons,
                                       hiddenNeuronX,
                                       hiddenNeuronY,
                                       outputNeuron,
                                       enableBias,
                                       enableAverage,
                                       activation);

    //net->loadFromNetFile();
    net->set_mutationFactor(0.1);
    net->updateNetConfiguration();
    net->saveToNetFile();


    #ifdef LOG_MUCH_DETAILED
        logGenomList.reserve(logReserveSize);
        logErrorList.reserve(logReserveSize);
        logIterationList.reserve(logReserveSize);
    #endif


    std::vector<double> genom;
    std::vector<double> output;
    printf("net done, press enter\n");
    unsigned int counter =0;
    unsigned int saveCounter = 0;
    unsigned int saves = 10;
    double averageError = 0;



    system("cls");
    unsigned long learningSteps = 0;
    t1 = std::chrono::high_resolution_clock::now();
    double averageCalcTime = 0;

    logfile = fopen("score.csv","w");           // save Excel header
    fprintf(logfile,"Iteration;error;\n");      //
    fclose(logfile);                            //
    genomlogFile = fopen("genom.csv","w");
    fclose(genomlogFile);

    while(true)
    {

        if(net->get_errorAmount() != 0)
        {
            ErrorList errors = net->get_errorList();
            for(size_t error=0; error<errors.size(); error++)
            {
                std::cout << "Error: "<<errors[error].toString();
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

        #ifdef LOG_MUCH_DETAILED
            logGenomList.push_back(net->get_genom());
            logErrorList.push_back(abs(net->get_netError()));
            logIterationList.push_back(learningSteps);
        #endif

        if(isnan(averageError))
        {
            qDebug() << "nan";
        }

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
                saves+=30; //spam the console in the beginning and later no more
                printf("error: %.5f\n",averageError);   //Prints the error
                printf("steps: %lu\n",learningSteps);    //Prints the learn cyles



                net->saveToNetFile();                        //Save the genom

                #ifdef LOG_MUCH_DETAILED
                    logError();
                    logGenom();
                #else
                    logError(learningSteps,averageError);
                    logGenom(net->get_genom());                      //Saves all weights of the net in: genom.csv so you can track the weights over the time of improvement
                #endif
            }
            if(averageError < 0.0005 || learningSteps > 1000000)//Learn until the error is below 0.005 or learning cycles are more then 1000000
            {
                t2 = std::chrono::high_resolution_clock::now();
                time_span = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
                cmdXY(0,0);  // Sets the cursor pos of the console
                saveCounter = 0;
                saves+=100; //spam the console in the beginning and later no more
                printf("error: %.5f\n",averageError);   //Prints the error
                printf("steps: %lu\n",learningSteps);    //Prints the learn cyles


                net->saveToNetFile();                        //Save the genom
                #ifdef LOG_MUCH_DETAILED
                    logError();
                    logGenom();
                #else
                    logError(learningSteps,averageError);
                    logGenom(net->get_genom());                      //Saves all weights of the net in: genom.csv so you can track the weights over the time of improvement
                #endif
                qDebug() << "Learning time: "<< time_span.count() << "sec.\t Average net calculation Time: "<<averageCalcTime<<"sec.";
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
        printf("%s\n",net->get_ptr_neuron_viaID(b)->toString().c_str());
    }
}
void setupTrainingSet()
{
    //              INPUT VALUES             EXPECTED OUTPUT
    trainingsSet.push_back({0,0});   outputSet.push_back({0});
    trainingsSet.push_back({0,1});   outputSet.push_back({1});
    trainingsSet.push_back({1,0});   outputSet.push_back({1});
    trainingsSet.push_back({1,1});   outputSet.push_back({0});
}
void netFinished(BackpropNet *net)
{
    std::vector<double*> *genom;
    std::vector<double> output;

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
            printf("\n           \t=====");
            printf("\n\n\n=================================================================================\n");
            printNet(net);
            printf("\n=================================================================================\n");
            getchar();
        }
    }
}
void cmdXY(unsigned int x,unsigned int y)
{
    HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {static_cast<short>(x), static_cast<short>(y)};
    SetConsoleCursorPosition(hConsole_c,pos);
}
#ifdef LOG_MUCH_DETAILED
void logGenom()
{
    genomlogFile = fopen("genom.csv","a");
    for(size_t i=0; i<logGenomList.size(); i++)
    {
        for(unsigned int a=0; a<logGenomList[i].size(); a++)
        {
            fprintf(genomlogFile,"%.5f;",logGenomList[i][a]);
        }
        fprintf(genomlogFile,"\n");
    }

    fclose(genomlogFile);
    logGenomList.clear();
    logGenomList.reserve(logReserveSize);
}
void logError()
{
    logfile = fopen("score.csv","a");
    for(size_t i=0; i<logErrorList.size(); i++)
    {
        fprintf(logfile,"%u;%.5f;\n",logIterationList[i], logErrorList[i]);
    }

    fclose(logfile);
    logErrorList.clear();
    logErrorList.reserve(logReserveSize);
    logIterationList.clear();
    logIterationList.reserve(logReserveSize);
}
#endif
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

void logError(const unsigned int &steps, const double &error)
{
    logfile = fopen("score.csv","a");           //Saves the error in the file: score.csv
    fprintf(logfile,"%i;%.5f;\n",steps,error);  //
    fclose(logfile);                            //
}
