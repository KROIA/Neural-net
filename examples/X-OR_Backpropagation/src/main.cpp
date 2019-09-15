#include <QCoreApplication>
#include <backpropnet.h>
#include <iostream>
#include <windows.h>

using namespace std;

vector<vector<float>    >trainingsSet;  //Trainings input vector
vector<vector<float>    >outputSet;     //Trainings expected output vector


FILE *genomlogFile;
FILE *logfile;
void printNet(Net &net);
void setupTrainingSet();
void netFinished(BackpropNet &net);
void cmdXY(int x, int y);
void logGenom(vector<float> genom);
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    setupTrainingSet(); //Setting the trainingset for the X-OR problem
    printf("setup\n");
    unsigned int inputNeurons = trainingsSet[0].size(); //Makes the amount of inputs dependend of the training set
    unsigned int hiddenNeuronX= 1;
    unsigned int hiddenNeuronY= 4;
    unsigned int outputNeuron = outputSet[0].size();    //Makes the amount of outputs dependent of the training set
    bool bias = true;
    bool enableAverage = false;

    BackpropNet net(inputNeurons,hiddenNeuronX,hiddenNeuronY,outputNeuron,bias,enableAverage,Activation::Sigmoid); //Makes the Net object
    net.loadFromNetFile();
    net.mutationFactor(0.005);

    genomlogFile = fopen("genom.csv","r");
    if(!genomlogFile)
    {
        fclose(genomlogFile);
        genomlogFile = fopen("genom.csv","w");
        for(unsigned int a=0; a<net.genomsize(); a++)
        {
            fprintf(genomlogFile,"w%i;",a+1);
        }
        fprintf(genomlogFile,"\n");
        fclose(genomlogFile);
    }
    vector<float> genom;
    vector<float> output;
    printf("net done, press enter\n");
    getchar();
    unsigned int counter =0;
    unsigned int saveCounter = 0;
    unsigned int saves = 0;
    float averageError = 0;



    system("cls");
    unsigned long learningSteps = 0;
    while(true)
    {
        cmdXY(0,0);  // Sets the cursor pos of the console


        net.input(trainingsSet[counter]);       // Sets the input of the net with the trainingset [counter]
        output = net.output();                  // Calculates the output vector and returns it
        net.expected(outputSet[counter]);       // Tells the net the right results

        averageError += abs(net.netError());    //The net calculates the error of netprediction and expected output
                                                //Saving only the positive value of the error to stop the training later when the error is low enough
        net.learn();                            //Improve the net
        learningSteps++;                        //Adding one training cycle


        counter++;                              //counts to the next trainingset

        saveCounter++;

        if(counter >= trainingsSet.size())
        {
            counter = 0;
            averageError /= trainingsSet.size(); //takes the average error of the whole training set
            if(saveCounter > saves)
            {
                saveCounter = 0;
                saves+=100; //spam the console in the beginning and later no more
                printf("error: %.5f\n",averageError);   //Prints the error
                printf("steps: %i\n",learningSteps);    //Prints the learn cyles

                logfile = fopen("score.csv","a");           //Saves the error in the file: score.csv
                fprintf(logfile,"%.5f;\n",averageError);    //
                fclose(logfile);                            //

                net.saveToNetFile();                        //Save the genom

                logGenom(net.genom());                      //Saves all weights of the net in: genom.csv so you can track the weights over the time of improvement
            }
            if(averageError < 0.005 || learningSteps > 1000000)//Learn until the error is below 0.005 or learning cycles are more then 1000000
            {
                netFinished(net);
            }
            averageError = 0;

        }
    }
    return a.exec();
}
void printNet(Net &net)
{
    printf("=================================================================================\n");
    printf("Input neurons:\n");
    printf("------------------\n");
    for(unsigned int y=0; y<net.hiddenNeuronsY(); y++)
    {
            printf("\tneuron      y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",y);
        for(unsigned int i=0; i<net.hiddenNeuron(0,y)->inputs(); i++)
        {
            printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net.hiddenNeuron(0,y)->input(i),net.hiddenNeuron(0,y)->weight(i));
        }
            printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net.hiddenNeuron(0,y)->output());
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
            for(unsigned int b=0; b<net.hiddenNeuron(x,y)->inputs(); b++)
            {
                printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net.hiddenNeuron(x,y)->input(b),net.hiddenNeuron(x,y)->weight(b));
            }
                printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net.hiddenNeuron(x,y)->output());
                printf("---------------------------------------------------------------------------------\n");
        }
    }
    printf("=================================================================================\n");
    printf("Output neurons:\n");
    printf("------------------\n");
    for(unsigned int y=0; y<net.outputNeurons(); y++)
    {
        printf("\tneuron      y: %i \t|\tinput\t|\tweight\t|\toutput\t|\n",y);
        for(unsigned int b=0; b<net.outputNeuron(y)->inputs(); b++)
        {
            printf("\t                \t|\t %.2f \t|\t %.2f \t|\t      \t|\n",net.outputNeuron(y)->input(b),net.outputNeuron(y)->weight(b));
        }
            printf("\t                \t|\t      \t|\t      \t|\t %.2f \t|\n",net.outputNeuron(y)->output());
            printf("---------------------------------------------------------------------------------\n");
    }
    printf("=================================================================================\n");

}
void setupTrainingSet()
{
    //              INPUT VALUES             EXPECTED OUTPUT
    trainingsSet.push_back({0,0});   outputSet.push_back({0});
    trainingsSet.push_back({0,1});   outputSet.push_back({1});
    trainingsSet.push_back({1,0});   outputSet.push_back({1});
    trainingsSet.push_back({1,1});   outputSet.push_back({0});
}
void netFinished(BackpropNet &net)
{
    vector<float> genom;
    vector<float> output;

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
}
void cmdXY(int x,int y)
{
    HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {(short)x, (short)y};
    SetConsoleCursorPosition(hConsole_c,pos);
}
void logGenom(vector<float> genom)
{
    genomlogFile = fopen("genom.csv","a");
    for(unsigned int a=0; a<genom.size(); a++)
    {
        fprintf(genomlogFile,"%.5f;",genom[a]);
    }
    fprintf(genomlogFile,"\n");
    fclose(genomlogFile);
}
