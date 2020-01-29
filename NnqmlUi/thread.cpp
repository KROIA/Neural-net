#include "thread.h"
#include <QtCore>
netThread::netThread(QObject *parent):
    QThread(parent)
{
    _hiddenNeuronX= 1;
    _hiddenNeuronY= 4;

     //Makes the Net object
    maxError(float(0.005));
    maxSteps(1000000);
    _bias = true;
    enableAverage = false;
    m_activFunc=4;
    qDebug()<<"bis da chunt";
    setupNet();
    net->loadFromNetFile();
    net->updateNetConfiguration();
    net->saveToNetFile();
    qDebug()<<"net done, press enter\n";
}

void netThread::run(){
    _learningSteps=0;
    while(finished==false){
    //qDebug()<<daten.trainingInput.daten(_counter)[0]<<""<<daten.trainingInput.daten(_counter)[1];
    net->input(daten.trainingInput.daten(_counter));//counter]);       // Sets the input of the net with the trainingset [counter]
    net->run();
    outputVector = net->output();                  // Calculates the output vector and returns it
    net->expected(daten.trainingOutput.daten(_counter));//counter]);
    _averageError += abs(net->netError());    //The net calculates the error of netprediction and expected output                                   //Saving only the positive value of the error to stop the training later when the error is low enough
    net->learn();                            //Improve the net
    _learningSteps++;
    //qDebug()<<_counter<<": "<<daten.trainingInput.daten(_counter);//Adding one training cycle
    _counter++;                              //counts to the next trainingset
    saveCounter++;
    if(_counter >= daten.trainingInput.daten().size())
    {
        _counter = 0;
        _averageError /= daten.trainingInput.daten().size(); //takes the average error of the whole training set
        if(saveCounter > saves)
        {
            saveCounter = 0;

            saves+=100; //spam the console in the beginning and later no more
            //printf("error: %.5f\n",double(_averageError));   //Prints the error
            //printf("steps: %i\n",_learningSteps);    //Prints the learn cyles
            logfile = fopen("score.csv","a");           //Saves the error in the file: score.csv
            fprintf(logfile,"%.5f;\n",double(_averageError));    //
            fclose(logfile);                            //
            net->saveToNetFile();//Save the genom
            logGenom(net->genom());//Saves all weights of the net in: genom.csv so you can track the weights over the time of improvement
        }

    }
    if(_averageError < _maxError || _learningSteps > _maxSteps)//Learn until the error is below 0.005 or learning cycles are more then 1000000
    {
        finished=true;
        net->saveToNetFile();
        break;
    }
    QMutex mutex;
    mutex.lock();
    if(this->stop) break;
    mutex.unlock();
    }
}
void netThread::logGenom(vector<float> genom)
{
    genomlogFile = fopen("genom.csv","a");
    for(unsigned int a=0; a<genom.size(); a++)
    {
        fprintf(genomlogFile,"%.5f;",genom[a]);
    }
    fprintf(genomlogFile,"\n");
    fclose(genomlogFile);
}
unsigned int netThread::inputNeurons(){
    return _inputNeurons;
}
unsigned int netThread::hiddenNeuronX(){
    return _hiddenNeuronX;
}
unsigned int netThread::hiddenNeuronY(){
    return _hiddenNeuronY;
}
unsigned int netThread::outputNeuron(){
    return _outputNeuron;
}
float netThread::averageError(){
    return _averageError;
}
unsigned long netThread::learningSteps(){
    return _learningSteps;
}
bool netThread::bias(){
    return _bias;
}
void netThread::activFunc(unsigned int i){
    m_activFunc=i;
}
void netThread::bias(bool i){
    _bias=i;
}
void netThread::setupNet(){
    _counter =0;
    saveCounter = 0;
    saves = 0;
    _averageError = 0;
    system("cls");
    _learningSteps = 0;
    _errorChart.clear();
    finished=false;
    _inputNeurons = unsigned(daten.trainingInput.daten(_counter).size());
    _outputNeuron = unsigned(daten.trainingOutput.daten(_counter).size());
    qDebug()<<_hiddenNeuronX<<"\t"<<_hiddenNeuronY<<"\t"<<_maxError<<"\t"<<_maxSteps<<"\t";
    qDebug()<<"activation Funktion="<<m_activFunc;
    /*Activation a;
    switch (m_activFunc) {
        case 0: a=Linear;
        break;
        case 1: a=ReLu;
        break;
        case 2: a=Binary;
        break;
        case 3: a=Gaussian;
        break;
        case 4: a=Sigmoid;
        break;
        default: a=Linear;
        break;
        }*/
    net= new BackpropNet(1);
    net->mutationFactor(float(0.0005));
    net->set(_inputNeurons,_hiddenNeuronX,
             _hiddenNeuronY,_outputNeuron,
             _bias,enableAverage,Sigmoid);
    net->updateNetConfiguration();
}
void netThread::reset(){
    net= new BackpropNet(1);
    _learningSteps=0;
    net->mutationFactor(float(0.0005));
    net->set(_inputNeurons,_hiddenNeuronX,
             _hiddenNeuronY,_outputNeuron,
             _bias,enableAverage,Sigmoid); 
    net->updateNetConfiguration();
    creatStandartCon();
    net->saveToNetFile();
}
vector<qreal> netThread::errorChart(){
    return _errorChart;
}
void netThread::inputNeurons(unsigned int i){
    _inputNeurons=i;
}
void netThread::hiddenNeuronX(unsigned int i){
    _hiddenNeuronX=i;
}
void netThread::hiddenNeuronY(unsigned int i){
    _hiddenNeuronY=i;
}
void netThread::outputNeuron(unsigned int i){
    _outputNeuron=i;
}
float netThread::maxError(){
    return _maxError;
}
unsigned long netThread::maxSteps(){
    return _maxSteps;
}
void netThread::maxError(float i){
    _maxError=i;
}
void netThread::maxSteps(unsigned long i){
    _maxSteps=i;
}

void netThread::creatStandartCon(){
    vector<Connection> con;

    for (unsigned int i=0;i<_inputNeurons*_hiddenNeuronY;++i) {
        Connection tempCon;
        NeuronID neuronSourceID;
        neuronSourceID.ID=i/_hiddenNeuronY;
        neuronSourceID.TYPE= NeuronType::input;
        NeuronID destinationID = net->hiddenNeuron(0,i%_hiddenNeuronY)->ID();
        tempCon.netID=i;
        tempCon.source_ID=neuronSourceID;
        tempCon.destination_ID=destinationID;
        con.push_back(tempCon);
    }
    if(_hiddenNeuronX>1){
        for (unsigned int i=0;i<_hiddenNeuronX-1;++i) {
            for (unsigned int j=0;j<_hiddenNeuronY;++j) {
                NeuronID neuronSourceID= net->hiddenNeuron(i,j)->ID();
                Connection tempCon;
                qDebug()<<"j"<<j;
                for(unsigned int o=0;j<_hiddenNeuronY;++j){
                    NeuronID destinationID = net->hiddenNeuron(i+1,o)->ID();
                    tempCon.netID=o+j*_hiddenNeuronY+i*_hiddenNeuronY+_inputNeurons*_hiddenNeuronY;
                    tempCon.source_ID=neuronSourceID;
                    tempCon.destination_ID=destinationID;
                    con.push_back(tempCon);
                    qDebug()<<"o"<<o;
                }
            }
        }
    }

    for (unsigned int i=0;i<_hiddenNeuronY*_outputNeuron;++i) {
        NeuronID destinationID = net->hiddenNeuron(_hiddenNeuronX-1,i%_hiddenNeuronY)->ID();
        Connection tempCon;
        NeuronID neuronSourceID= net->outputNeuron(i/_hiddenNeuronY)->ID();
        tempCon.netID=i+_inputNeurons*_hiddenNeuronY+(_hiddenNeuronX-1)*_hiddenNeuronY;
        tempCon.source_ID=neuronSourceID;
        tempCon.destination_ID=destinationID;
        con.push_back(tempCon);
    }
    for (unsigned int i=0;i<_hiddenNeuronX;++i) {
        for (unsigned int j=0;j<_hiddenNeuronY;++j) {
            NeuronID destinationID = net->hiddenNeuron(i,j)->ID();
            Connection tempCon;
            NeuronID neuronSourceID;
            neuronSourceID.TYPE= NeuronType::bias;
            tempCon.netID=_inputNeurons*_hiddenNeuronY+(_hiddenNeuronX-1)*_hiddenNeuronY+i*hiddenNeuronY()+j;
            tempCon.source_ID=neuronSourceID;
            tempCon.destination_ID=destinationID;
            con.push_back(tempCon);
        }
    }
    for (unsigned int i=0;i<_outputNeuron;++i) {
            NeuronID destinationID = net->outputNeuron(i)->ID();
            Connection tempCon;
            NeuronID neuronSourceID;
            neuronSourceID.TYPE= NeuronType::bias;
            tempCon.netID=_inputNeurons*_hiddenNeuronY+(_hiddenNeuronX-1)*_hiddenNeuronY+_hiddenNeuronX*_hiddenNeuronY+i;
            tempCon.source_ID=neuronSourceID;
            tempCon.destination_ID=destinationID;
            con.push_back(tempCon);
        }
    net->connectionList(con);
    }


