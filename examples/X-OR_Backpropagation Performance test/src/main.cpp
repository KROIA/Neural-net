// Autor        Alex Krieg
// Datum        28.09.2020
// Version      00.01.00

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


using namespace std;

static std::vector<std::vector<double>    >trainingsSet;  //Trainings input std::vector
static std::vector<std::vector<double>    >outputSet;     //Trainings expected output std::vector


static FILE *genomlogFile;
static FILE *logfile;

static std::chrono::high_resolution_clock::time_point t2;
static std::chrono::duration<double> time_span;
static std::chrono::high_resolution_clock::time_point t1;

#define XOR

void runNet(unsigned int netID,unsigned long long &sycles,double &time);
void printNet(Net *net);
void setupTrainingSet();
void netFinished(BackpropNet *net);
void cmdXY(int x, int y);
void logGenom(unsigned int net,std::vector<double> genom);
void generateTone();
class ErrorHandler;

static unsigned int hiddenNeuronX= 10;
static unsigned int hiddenNeuronY= 10;
static bool enableBias = true;
static bool enableAverage = false;

static std::vector<std::vector<std::vector<double> >   > _genomCompareList;
static std::vector<std::vector<double>  >_runtimeList;
static unsigned int reserveSize = 10;


//static ErrorHandler    *errorHandler;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#ifdef XOR
    setupTrainingSet(); //Setting the trainingset for the X-OR problem
#else
    generateTone();
#endif
    std::vector<double> genom;
    std::vector<double> output;
    printf("net done, press enter\n");
    getchar();

    FILE *benchmarkFile;


    for(unsigned int trainings = 0; trainings<100; trainings++)
    {
        double learnTime = 0;
        unsigned long long sycles = 0;
        runNet(trainings,sycles,learnTime);
        _runtimeList.push_back({learnTime,
                                BackpropNet::__dbg_time1,
                                BackpropNet::__dbg_time2,
                                BackpropNet::__dbg_time3,
                                BackpropNet::__dbg_time4,
                                BackpropNet::__dbg_time5,
                                BackpropNet::__dbg_time6,
                                BackpropNet::__dbg_time7,
                                BackpropNet::__dbg_time8,
                                BackpropNet::__dbg_time9,
                                BackpropNet::__dbg_time10});
        benchmarkFile = fopen("bench.csv","a");
        fprintf(benchmarkFile,"%i;%.16f;\n",sycles,learnTime);
        fclose(benchmarkFile);
    }

/*    _genomCompareList = std::vector<std::vector<std::vector<double> >   >(2);
    _genomCompareList[0] = {{1,2,3,4,5},
                            {2,3,4,5,6},
                            {3,4,5,6,7},
                            {4,5,6,7,8}};
    _genomCompareList[1] = {{1,1,1,1,1},
                            {2,2,2,2,2},
                            {3,3,3,3,3},
                            {4,4,4,4,4}};

*/

    unsigned int minTimedGenomSize = _genomCompareList[0].size();
    for(unsigned int net = 1; net<_genomCompareList.size(); net++)
    {
        if(_genomCompareList[net].size() < minTimedGenomSize)
        {
            minTimedGenomSize = _genomCompareList[net].size();
        }
    }

    std::vector<double> diverenceGenom(_genomCompareList[0][0].size());
    double diference = 0;
    for(unsigned int timedGenom = 0; timedGenom<minTimedGenomSize; timedGenom++)
    {
        for(unsigned int net = 1; net<_genomCompareList.size(); net++)
        {
            for(unsigned int genomIndex = 0; genomIndex<_genomCompareList[net][timedGenom].size(); genomIndex++)
            {
                diverenceGenom[genomIndex] += abs(_genomCompareList[net][timedGenom][genomIndex] - _genomCompareList[net-1][timedGenom][genomIndex]);
                diference += diverenceGenom[genomIndex];
            }
        }

        //logGenom(100,diverenceGenom);
        diverenceGenom = std::vector<double>(_genomCompareList[0][0].size());


    }

    FILE *file = fopen("time.csv","w");
    for(unsigned int a=0; a<_runtimeList.size(); a++)
    {
        for(unsigned int b=0; b<_runtimeList[a].size(); b++)
        {
            fprintf(file,"%.16f;",_runtimeList[a][b]);
        }
        fprintf(file,"\n");
    }
    fclose(file);
    qDebug() << "diference: "<<diference;


    /*unsigned int counter =0;
    unsigned int saveCounter = 0;
    unsigned int saves = 1000;
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
       // logGenom(net->genom());
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
                saves+=100; //spam the console in the beginning and later no more
                printf("error: %.5f\n",averageError);   //Prints the error
                printf("steps: %i\n",learningSteps);    //Prints the learn cyles

                logfile = fopen("score.csv","a");           //Saves the error in the file: score.csv
                fprintf(logfile,"%.5f;\n",averageError);    //
                fclose(logfile);                            //



                net->saveToNetFile();                        //Save the genom

                logGenom(net->get_genom());                      //Saves all weights of the net in: genom.csv so you can track the weights over the time of improvement
               // system("cls");
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
    }*/
    return a.exec();
}
void runNet(unsigned int netID,unsigned long long &sycles,double &time)
{
    sycles  = 0;
    time    = 0;
    unsigned int inputNeurons = trainingsSet[0].size(); //Makes the amount of inputs dependend of the training set
    unsigned int outputNeuron = outputSet[0].size();    //Makes the amount of outputs dependent of the training set


    BackpropNet *net = new BackpropNet(0,inputNeurons,hiddenNeuronX,hiddenNeuronY,outputNeuron,enableBias,enableAverage,Activation::Gaussian); //Makes the Net object
    net->set_seed(0);
    net->set_mutationFactor(0.1);
    net->updateNetConfiguration();
    t1 = std::chrono::high_resolution_clock::now();
    double averageError  = 0;
    unsigned int counter = 0;
    unsigned int logInterval = 100;
    std::vector<double> output;
    bool doLearn = true;
    _genomCompareList.push_back(std::vector<std::vector<double> >());
    _genomCompareList[_genomCompareList.size()-1].reserve(10000);
   //system("cls");
    while(doLearn)
    {
        net->set_input(trainingsSet[counter]);       // Sets the input of the net with the trainingset [counter]
        net->run();
        output = net->get_output();                  // Calculates the output std::vector and returns it
        net->set_expected(outputSet[counter]);       // Tells the net the right results

        averageError += abs(net->get_netError());    //The net calculates the error of netprediction and expected output
                                                //Saving only the positive value of the error to stop the training later when the error is low enough
        net->learn();                            //Improve the net
        sycles++;                        //Adding one training cycle
        if(isnan(averageError))
        {
            qDebug() << "nan";
        }
        counter++;                              //counts to the next trainingset
        if(counter >= trainingsSet.size())
        {

            counter = 0;
            averageError /= trainingsSet.size(); //takes the average error of the whole training set
            if(averageError < 0.001 || sycles > 10000)//Learn until the error is below 0.005 or learning cycles are more then 1000000
            {
                doLearn = false;
            }
            if(counter/trainingsSet.size() % logInterval == 0)
            {
                _genomCompareList[_genomCompareList.size()-1].push_back(net->get_genom());
                //logGenom(netID,net->get_genom());
                //logGenom(netID,net->get_genom());
            }
            averageError = 0;
        }
    }
    t2 = std::chrono::high_resolution_clock::now();
    time_span = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
    system("cls");
    qDebug() << "Net:   "<<netID;
    qDebug() << "time:  "<< time_span.count() << " sec";
    qDebug() << "error: "<<averageError;   //Prints the error
    qDebug() << "steps: "<<sycles;    //Prints the learn cyles
    qDebug() << BackpropNet::__dbg_time1 << "\t"<< BackpropNet::__dbg_time2 << "\t"<< BackpropNet::__dbg_time3 << "\t"<< BackpropNet::__dbg_time4 << "\t"<< BackpropNet::__dbg_time5 << "\t";
    qDebug() << BackpropNet::__dbg_time6 << "\t"<< BackpropNet::__dbg_time7 << "\t"<< BackpropNet::__dbg_time8 << "\t"<< BackpropNet::__dbg_time9 << "\t"<< BackpropNet::__dbg_time10 << "\t";

    time = time_span.count();
    delete net;
}
void printNet(Net *net)
{
    for(unsigned int b=0; b<net->get_neurons(); b++)
    {
    std::vector<std::string>    list = net->get_ptr_neuron_viaID(b)->toStringList();
    for(size_t a=0; a<list.size(); a++)
    {
        printf(list[a].c_str());
    }
    printf("------------------------------------\n");
    }
   /* printf("=================================================================================\n");
    printf("Input neurons:\n");
    printf("------------------\n");
    for(unsigned int y=0; y<net->hiddenNeuronsY(); y++)
    {
            printf("\tneuron      y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",y);
        for(unsigned int i=0; i<net->hiddenNeuron(0,y)->inputs(); i++)
        {
            printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net->hiddenNeuron(0,y)->input(i),net->hiddenNeuron(0,y)->weight(i));
        }
            printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net->hiddenNeuron(0,y)->output());
            printf("---------------------------------------------------------------------------------\n");
    }
    printf("=================================================================================\n");
    printf("Hidden neurons:\n");
    printf("------------------\n");
    for(unsigned int x=1; x<net->hiddenNeuronsX(); x++)
    {
        for(unsigned int y=0; y<net->hiddenNeuronsY(); y++)
        {
                printf("\tneuron x: %i y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",x,y);
            for(unsigned int b=0; b<net->hiddenNeuron(x,y)->inputs(); b++)
            {
                printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net->hiddenNeuron(x,y)->input(b),net->hiddenNeuron(x,y)->weight(b));
            }
                printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net->hiddenNeuron(x,y)->output());
                printf("---------------------------------------------------------------------------------\n");
        }
    }
    printf("=================================================================================\n");
    printf("Output neurons:\n");
    printf("------------------\n");
    for(unsigned int y=0; y<net->outputNeurons(); y++)
    {
        printf("\tneuron      y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",y);
        for(unsigned int b=0; b<net->outputNeuron(y)->inputs(); b++)
        {
            printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net->outputNeuron(y)->input(b),net->outputNeuron(y)->weight(b));
        }
            printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net->output(y));
            printf("---------------------------------------------------------------------------------\n");
    }
    printf("=================================================================================\n");
*/
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

#ifndef XOR
    generateTone();
#endif

    genom = net->get_ptr_genom();
    system("cls");
    printf("Genom: \n | ");
    for(int a=0; a<(*genom).size(); a++)
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
    for(int counter=0; counter<trainingsSet.size(); counter++)
    {
        net->set_input(trainingsSet[counter]);
        net->run();
        output = net->get_output();
        fprintf(file,"%.8f;%.8f;%.8f;%.8f;\n",trainingsSet[counter][0],trainingsSet[counter][1],output[0],output[0]-outputSet[counter][0]);
    }
    fclose(file);
    while(true)
    {
        for(int counter=0; counter<trainingsSet.size(); counter++)
        {




            cmdXY(0,(*genom).size() / 5 +4);
            net->set_input(trainingsSet[counter]);
            net->run();
            output = net->get_output();

            printf("inputs are:\t");
            for(int a=0; a<trainingsSet[counter].size(); a++)
            {
                printf("%.3f | ",trainingsSet[counter][a]);
            }
            printf("\noutputs are:\t");
            for(int a=0; a<output.size(); a++)
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
void cmdXY(int x,int y)
{
    HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {static_cast<short>(x), static_cast<short>(y)};
    SetConsoleCursorPosition(hConsole_c,pos);
}
void logGenom(unsigned int net,std::vector<double> genom)
{
    genomlogFile = fopen((QString::number(net)+"genom.csv").toStdString().c_str(),"a");
    for(int a=0; a<genom.size(); a++)
    {
        fprintf(genomlogFile,"%.5f;",genom[a]);
    }
    fprintf(genomlogFile,"\n");
    fclose(genomlogFile);
}
/*class ErrorHandler : public QObject
{
    Q_OBJECT
    public:
        ErrorHandler(QObject *parent = nullptr):QObject(parent){}
        ~ErrorHandler(){}

    public slots:
        void errorNet(unsigned int netID, Error &e){
            qDebug() << "Error in Net ["<<netID<<"] : "<<e.toQString();
        }
        void errorNeuron(NeuronID ID,Error &e){
            qDebug() << "Error in Neuron ["<<Neuron::toIDString(ID)<<"] : "<<e.toQString();
        }
};*/
