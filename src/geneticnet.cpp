#include <geneticnet.h>


GeneticNet::GeneticNet()
{
    this->init(GENETICNET_MIN_ANIMALS,NET_MIN_INPUTNEURONS,NET_MIN_HIDDENNEURONS_X,NET_MIN_HIDDENNEURONS_Y,NET_MIN_OUTPUTNEURONS,true,false,Activation::Sigmoid);
}
GeneticNet::GeneticNet(unsigned int animals)
{
    this->init(animals,NET_MIN_INPUTNEURONS,NET_MIN_HIDDENNEURONS_X,NET_MIN_HIDDENNEURONS_Y,NET_MIN_OUTPUTNEURONS,true,false,Activation::Sigmoid);
}
GeneticNet::GeneticNet(unsigned int animals,
                       unsigned int inputs,
                       unsigned int hiddenX,
                       unsigned int hiddenY,
                       unsigned int outputs)
{
    this->init(animals,inputs,hiddenX,hiddenY,outputs,true,false,Activation::Sigmoid);
}
GeneticNet::GeneticNet(unsigned int animals,
                       unsigned int inputs,
                       unsigned int hiddenX,
                       unsigned int hiddenY,
                       unsigned int outputs,
                       bool enableBias,
                       bool enableAverage,
                       Activation func)
{
    this->init(animals,inputs,hiddenX,hiddenY,outputs,enableBias,enableAverage,func);
}
GeneticNet::~GeneticNet()
{
    for(unsigned int b=_netList.size(); b>0; b--)
    {
        try {
            delete _netList[b-1];
        } catch (std::exception &e) {
            qDebug() << "error: "<<e.what();
        }
    }
    _scoreList.clear();
#ifdef __enableGeneticNetThread
    pthread_mutex_lock(&_threadLock);
    _threadExit = true;
    pthread_cond_broadcast( &_thread_condition_var );
    pthread_mutex_unlock(&_threadLock);
    for(unsigned int a=_threadList.size(); a>0; a--)
    {
        pthread_join(_threadList[a], NULL);
    }
#endif
}
void                    GeneticNet::set(unsigned int animals,
                            unsigned int inputs,
                            unsigned int hiddenX,
                            unsigned int hiddenY,
                            unsigned int outputs,
                            bool enableBias,
                            bool enableAverage,
                            Activation func)
{
    this->animals(animals);
    this->inputNeurons(inputs);
    this->hiddenNeuronsX(hiddenX);
    this->hiddenNeuronsY(hiddenY);
    this->outputNeurons(outputs);
    this->bias(enableBias);
    this->enableAverage(enableAverage);
    this->activationFunction(func);
    this->clearConnectionList();
    this->updateNetConfiguration();
#ifdef __DEBUG_GENETICNET
    qDebug() << "Geneticnet constructor Setup done";
#endif
}

void                    GeneticNet::netFileName(std::string filename)
{
    _saveNet.filename(filename);
}
std::string             GeneticNet::netFileName()
{
    return _saveNet.filename();
}
void                    GeneticNet::netFileEnding(std::string fileEnding)
{
    _saveNet.fileEnding(fileEnding);
}
std::string             GeneticNet::netFileEnding()
{
    return _saveNet.fileEnding();
}
void                    GeneticNet::loadFromNetFile()
{
#ifdef __DEBUG_GENETICNET
    qDebug() << "loadFromNetFile() " << _saveNet.filename().c_str()<<"."<<_saveNet.fileEnding().c_str();
#endif
    try {
        _saveNet.loadFile();
    } catch (std::runtime_error &e) {
        qDebug() << "Warning: "<< e.what();
        return;
    }catch(...)
    {
        qDebug() << "ERROR: _saveNet.loadfile()";
    }
#ifdef __DEBUG_GENETICNET
    qDebug() << "read done -> applaying...";
#endif
    try {
        this->set(_saveNet.animals(),_saveNet.inputNeurons(),_saveNet.hiddenNeuronsX(),_saveNet.hiddenNeuronsY(),_saveNet.outputNeurons(),
                  _saveNet.bias(),_saveNet.enableAverage(),_saveNet.activationFunction());
        this->biasValue(_saveNet.biasValue());
        this->costumNeurons(_saveNet.costumConnections());
        this->connectionList(_saveNet.connectionList());
        this->costumConnections(_saveNet.costumConnections());
        this->neurons(_saveNet.neurons(),_saveNet.hiddenNeurons(),_saveNet.outputNeurons(),_saveNet.costumNeurons());
 //       this->genom(_saveNet.genom());
        //qDebug() << "only backloop test";

    } catch (std::runtime_error &e) {
        error_general("loadFromNetFile(std::string ["+_saveNet.filename()+"] , std::string ["+_saveNet.fileEnding()+"] )",
                      "unable to apply the settings. Maybe the file is damaged.",e);
    }
    this->updateNetConfiguration();
#ifdef __DEBUG_GENETICNET
    qDebug() << "applaying done";
#endif
}
void                    GeneticNet::loadFromNetFile(std::string filename)
{
    this->netFileName(filename);
    this->loadFromNetFile();
}
void                    GeneticNet::loadFromNetFile(std::string filename,std::string fileEnding)
{
    this->netFileName(filename);
    this->netFileEnding(fileEnding);
    this->loadFromNetFile();
}
void                    GeneticNet::saveToNetFile()
{
#ifdef __DEBUG_GENETICNET
    qDebug() << "saveToNetFile() " << _saveNet.filename().c_str()<<"."<<_saveNet.fileEnding().c_str();
#endif
    try {
        _saveNet.set(this->inputNeurons(),this->hiddenNeuronsX(),this->hiddenNeuronsY(),this->outputNeurons(),
                     this->bias(),this->enableAverage(),this->activationFunction(),this->biasValue());
        //_saveNet.setGenom(this->genom());
        for(unsigned int net = 0; net<_netList.size(); net++)
        {
            _saveNet.neuronsOfNet(_netList[net]->ID(),_netList[net]->allNeurons());
        }
        _saveNet.saveFile();
    } catch (std::runtime_error &e) {
        error_general("saveToNetFile()",e);
    }
#ifdef __DEBUG_GENETICNET
    qDebug() << "save done";
#endif
}
void                    GeneticNet::saveToNetFile(std::string filename)
{
    this->netFileName(filename);
    this->saveToNetFile();
}
void                    GeneticNet::saveToNetFile(std::string filename,std::string fileEnding)
{
    this->netFileName(filename);
    this->netFileEnding(fileEnding);
    this->saveToNetFile();
}


void                    GeneticNet::init(unsigned int animals,
                                         unsigned int inputs,
                                         unsigned int hiddenX,
                                         unsigned int hiddenY,
                                         unsigned int outputs,
                                         bool enableBias,
                                         bool enableAverage,
                                         Activation func)
{
    _debugCount = 0;
    _randEngine = std::default_random_engine(rand()%100);
    _animals = 0;
    _currentAnimal = 0;
    _netList = std::vector<Net * >();
    _scoreList = std::vector<float>();
    this->mutationChangeWeight((float)0.01);
    this->mutationFactor((float)0.01);
    this->netFileName("netFile");
    this->netFileEnding("gnet");
    this->set(animals,inputs,hiddenX,hiddenY,outputs,enableBias,enableAverage,func);
}

void                    GeneticNet::animals(unsigned int animals)
{
    if(animals < GENETICNET_MIN_ANIMALS || animals > GENETICNET_MAX_ANIMALS)
    {
        error_general("animals(unsigned int ["+std::to_string(animals)+"] )","Parameter 0 is out of range. Min: "+ std::to_string(GENETICNET_MIN_ANIMALS)+" Max: "+std::to_string(GENETICNET_MAX_ANIMALS));
    }
    if(animals == _animals)
    {
        return;
    }
    if(animals > _animals)
    {
       _threadExit = true;
       _threadPause = false;
       _threadDelayMicros = 1000000;
       if(_threadList.size() != 0)
       {
           for(unsigned int a=0; a<_threadList.size(); a++)
           {
               pthread_join(_threadList[a], NULL);
           }
       }
       if(_threadList_setupNet.size() != 0)
       {
           for(unsigned int a=0; a<_threadList_setupNet.size(); a++)
           {
               pthread_join(_threadList_setupNet[a], NULL);
           }
       }
       _netList.reserve(animals);
       _threadList.reserve(animals);
       _threadData.reserve(animals);
       _threadList_setupNet.reserve(animals);
       _threadData_setupNet.reserve(animals);
       for(unsigned int a=_animals; a<animals; a++)
        {
            _netList.push_back(new Net(a));
            _netList[_netList.size()-1]->ID(_netList.size()-1);
            _scoreList.push_back(0);
            _threadList.push_back(pthread_t());
            _threadList_setupNet.push_back(pthread_t());

            _threadData.push_back(thread_data_geneticNet());
            _threadData[a].thread_id = a;
            _threadData[a].net = _netList[a];
            _threadData[a].exit = &_threadExit;
            _threadData[a].pause = &_threadPause;
            _threadData[a].lock = &_threadLock;
            _threadData[a].condition_var = &_thread_condition_var;
            _threadData[a].isPaused = false;
            _threadData[a].delayMicros = &_threadDelayMicros;

            _threadData_setupNet.push_back(thread_data_geneticNet());
            _threadData_setupNet[a].thread_id = a;
            _threadData_setupNet[a].net = _netList[a];
            _threadData_setupNet[a].exit = &_threadExit;
            _threadData_setupNet[a].pause = &_threadPause;
            _threadData_setupNet[a].lock = &_threadLock_setupNet;
            _threadData_setupNet[a].condition_var = &_thread_condition_var_setupNet;
            _threadData_setupNet[a].isPaused = false;
            _threadData_setupNet[a].delayMicros = &_threadDelayMicros;
        }

    }
    else
    {
        for(unsigned int a=0; a<_animals - animals; a++)
        {
            delete _netList[_netList.size()-1];
            _netList.erase(_netList.end()-1);
            _scoreList.erase(_scoreList.end()-1);
            _threadList.erase(_threadList.end()-1);
            _threadData.erase(_threadData.end()-1);
            _threadData_setupNet.erase(_threadData_setupNet.end()-1);
            _threadList_setupNet.erase(_threadList_setupNet.end()-1);
        }
    }
    _animals = animals;
    _update  = true;

#ifdef __enableGeneticNetThread
    _threadExit = false;
    _threadPause = true;


    int rc;
    for(unsigned int a=0; a<_animals; a++)
    {
        rc = pthread_create(&_threadList[a], NULL, runThread, (void *)&_threadData[a]);
        if (rc)
        {
            qDebug() << "Error:unable to create thread," << rc << endl;
        }
    }
#endif
}
unsigned int            GeneticNet::animals()
{
    return _animals;
}
void                    GeneticNet::inputNeurons(unsigned int inputs)
{
    if(inputs != _netList[0]->inputNeurons())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->inputNeurons(inputs);
            } catch (std::runtime_error &e) {
                error_general("inputNeurons(unsigned int ["+std::to_string(inputs)+"] )","_netList["+std::to_string(a)+"].inputNeurons("+std::to_string(inputs)+")",e);
            }
        }
        _update = true;
    }
}
unsigned int            GeneticNet::inputNeurons()
{
    return _netList[0]->inputNeurons();
}
void                    GeneticNet::hiddenNeuronsX(unsigned int hiddenX)
{
    if(hiddenX != _netList[0]->hiddenNeuronsX())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->hiddenNeuronsX(hiddenX);
            } catch (std::runtime_error &e) {
                error_general("hiddenNeuronsX(unsigned int ["+std::to_string(hiddenX)+"] )","_netList["+std::to_string(a)+"].hiddenNeuronsX("+std::to_string(hiddenX)+")",e);
            }
        }
        _update = true;
    }
}
unsigned int            GeneticNet::hiddenNeuronsX()
{
    return _netList[0]->hiddenNeuronsX();
}
void                    GeneticNet::hiddenNeuronsY(unsigned int hiddenY)
{
    if(hiddenY != _netList[0]->hiddenNeuronsY())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->hiddenNeuronsY(hiddenY);
            } catch (std::runtime_error &e) {
                error_general("hiddenNeuronsY(unsigned int ["+std::to_string(hiddenY)+"] )","_netList["+std::to_string(a)+"].hiddenNeuronsY("+std::to_string(hiddenY)+")",e);
            }
        }
        _update = true;
    }
}
unsigned int            GeneticNet::hiddenNeuronsY()
{
    return _netList[0]->hiddenNeuronsY();
}
void                    GeneticNet::outputNeurons(unsigned int outputs)
{
    if(outputs != _netList[0]->outputNeurons())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->outputNeurons(outputs);
            } catch (std::runtime_error &e) {
                error_general("outputNeurons(unsigned int ["+std::to_string(outputs)+"] )","_netList["+std::to_string(a)+"].outputNeurons("+std::to_string(outputs)+")",e);
            }
        }
        _update = true;
    }
}
unsigned int            GeneticNet::outputNeurons()
{
    return _netList[0]->outputNeurons();
}
void                    GeneticNet::costumNeurons(unsigned int costum)
{
    if(costum != _netList[0]->costumNeurons())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->costumNeurons(costum);
            } catch (std::runtime_error &e) {
                error_general("costumNeurons(unsigned int ["+std::to_string(costum)+"] )","_netList["+std::to_string(a)+"].outputNeurons("+std::to_string(costum)+")",e);
            }
        }
    }
}
unsigned int            GeneticNet::costumNeurons()
{
    return _netList[0]->costumNeurons();
}
void                    GeneticNet::costumConnections(unsigned int connections)
{
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            _netList[a]->costumConnections(connections);
        } catch (std::runtime_error &e) {
            error_general("costumConnections(unsigned int ["+std::to_string(connections)+"],","animal: "+std::to_string(a),e);
        }
    }
}
void                    GeneticNet::neurons(unsigned int neurons,unsigned int hiddenNeurons,unsigned int outputNeurons,unsigned int costumNeurons)
{
   for(unsigned int a=0; a<_animals; a++)
   {
       try {
           _netList[a]->neurons(neurons,hiddenNeurons,outputNeurons,costumNeurons);
       } catch (std::runtime_error &e) {
           error_general("neurons(unsigned int ["+std::to_string(neurons)+"],unsigned int ["+std::to_string(hiddenNeurons)+"],unsigned int ["+std::to_string(outputNeurons)+"],unsigned int ["+std::to_string(costumNeurons)+"],","animal: "+std::to_string(a),e);
       }
   }
}

void                    GeneticNet::bias(bool enableBias)
{
    if(enableBias != _netList[0]->bias())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->bias(enableBias);
            } catch (std::runtime_error &e) {
                error_general("bias(unsigned int ["+std::to_string(enableBias)+"] )","_netList["+std::to_string(a)+"].bias("+std::to_string(enableBias)+")",e);
            }
        }
        _update = true;
    }
}
bool                    GeneticNet::bias()
{
    return _netList[0]->bias();
}
void                    GeneticNet::enableAverage(bool enableAverage)
{
    if(enableAverage != _netList[0]->enableAverage())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->enableAverage(enableAverage);
            } catch (std::runtime_error &e) {
                error_general("enableAverage(unsigned int ["+std::to_string(enableAverage)+"] )","_netList["+std::to_string(a)+"].enableAverage("+std::to_string(enableAverage)+")",e);
            }
        }
        _update = true;
    }
}
bool                    GeneticNet::enableAverage()
{
    return _netList[0]->enableAverage();
}
void                    GeneticNet::biasValue(float value)
{
    if(value != _netList[0]->biasValue())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->biasValue(value);
            } catch (std::runtime_error &e) {
                error_general("biasValue(unsigned int ["+std::to_string(value)+"] )","_netList["+std::to_string(a)+"].biasValue("+std::to_string(value)+")",e);
            }
        }
        _update = true;
    }
}
float                   GeneticNet::biasValue()
{
    return _netList[0]->biasValue();
}
void                    GeneticNet::activationFunction(Activation func)
{
    if(func != _netList[0]->activationFunction())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->activationFunction(func);
            } catch (std::runtime_error &e) {
                error_general("activationFunction(unsigned int ["+std::to_string(func)+"] )","_netList["+std::to_string(a)+"].activationFunction("+std::to_string(func)+")",e);
            }
        }
        _update = true;
    }
}
Activation              GeneticNet::activationFunction()
{
    return _netList[0]->activationFunction();
}
bool                    GeneticNet::noHiddenLayer()
{
    return _netList[0]->noHiddenLayer();
}

void                    GeneticNet::randomGenom()
{
    _update = true;
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            _netList[a]->randomGenom();
        } catch (std::runtime_error &e) {
            error_general("randomGenom()","_netList["+std::to_string(a)+"]->randomGenom()",e);
        }
    }
}
void                    GeneticNet::genom(std::vector<std::vector<float>    > genomList)
{
    if(genomList.size() != _animals)
    {
        error_general("genom(std::vector<std::vector<float> >)","std::vector<std::vector<float> >.size() == "+std::to_string(genomList.size())+" "+error_paramOutOfRange(0,genomList.size(),_animals,_animals));
    }
    _update = true;
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            _netList[a]->genom(genomList[a]);
        } catch (std::runtime_error &e) {
            error_general("genom(std::vector<std::vector<float> >)","_netList["+std::to_string(a)+"]->genom(genomList["+std::to_string(a)+"])",e);
        }
    }
}
void                    GeneticNet::genom(unsigned int animal, std::vector<float> genom)
{
    if(animal > _animals-1)
    {
        error_general("genom(unsigned int ["+std::to_string(animal)+"] , std::vector<float> )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    if(genom.size() != genomsize())
    {
        error_general("genom(unsigned int ["+std::to_string(animal)+"] , std::vector<float> )",error_paramOutOfRange((unsigned int)1,genom.size(),genomsize(),genomsize()));
    }
    _update = true;
    try {
        _netList[animal]->genom(genom);
    } catch (std::runtime_error &e) {
        error_general("genom(unsigned int ["+std::to_string(animal)+"] , std::vector<float> )","_netList["+std::to_string(animal)+"].genom(std::vector<float>)",e);
    }
}
std::vector<float>      GeneticNet::genom(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("genom(unsigned int ["+std::to_string(animal)+"])",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal]->genom();
}
std::vector<std::vector<float>  >GeneticNet::genom()
{
    std::vector<std::vector<float>  > gen(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        gen[a] = _netList[a]->genom();
    }
    return gen;
}
unsigned int            GeneticNet::genomsize()
{
    return _netList[0]->genomsize();
}
void                    GeneticNet::genomFromNetFile()
{
    this->genom(_saveNet.genom());
}

void                    GeneticNet::input(unsigned int animal, unsigned int input, float signal)
{
    if(animal > _animals-1)
    {
        error_general("input(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(input)+"] , float ["+std::to_string(signal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    _update = true;
    try {
        _netList[animal]->input(input,signal);
    } catch (std::runtime_error &e) {
        error_general("input(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(input)+"] , float ["+std::to_string(signal)+"] )","_netList["+std::to_string(animal)+"]->input("+std::to_string(input)+","+std::to_string(signal)+")",e);
    }
}
float                   GeneticNet::input(unsigned int animal, unsigned int input)
{
    if(animal > _animals-1)
    {
        error_general("input(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    try {
        return _netList[animal]->input(input);
    } catch (std::runtime_error &e) {
        error_general("input(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(input)+"] )","return _netList["+std::to_string(animal)+"]->input("+std::to_string(input)+")",e);
    }
}
void                    GeneticNet::input(unsigned int animal, std::vector<float> inputList)
{
    if(animal > _animals-1)
    {
        error_general("input(unsigned int ["+std::to_string(animal)+"] , std::vector<float> )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    _update = true;
    try {
        _netList[animal]->input(inputList);
    } catch (std::runtime_error &e) {
        error_general("input(unsigned int ["+std::to_string(animal)+"] , std::vector<float> )","_netList["+std::to_string(animal)+"]->input(std::vector<float>)",e);
    }
}
std::vector<float>      GeneticNet::input(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("input(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    try {
        return _netList[animal]->input();
    } catch (std::runtime_error &e) {
        error_general("input(unsigned int ["+std::to_string(animal)+"] )","return _netList["+std::to_string(animal)+"]->input()",e);
    }
}
void                    GeneticNet::input(std::vector<std::vector<float>    > input)
{
    if(input.size() != _animals)
    {
        error_general("input(std::vector<std::vector<float> >)","std::vector<>.size() == "+std::to_string(input.size()) +" "+ error_paramOutOfRange((unsigned int)0,input.size(),_animals,_animals));
    }
    _update = true;
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            _netList[a]->input(input[a]);
        } catch (std::runtime_error &e) {
            error_general("input(std::vector<std::vector<float> >)","_netList["+std::to_string(a)+"]->input(std::vector<float>["+std::to_string(a)+"])",e);
        }
    }
}
std::vector<std::vector<float>  >GeneticNet::input()
{
    std::vector<std::vector<float>  > inp(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        inp[a] = _netList[a]->input();
    }
    return inp;
}

float                   GeneticNet::hidden(unsigned int animal, unsigned int hiddenX, unsigned int hiddenY)
{
    if(animal > _animals-1)
    {
        error_general("hidden(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    try {
        return _netList[animal]->hidden(hiddenX,hiddenY);
    } catch (std::runtime_error &e) {
        error_general("hidden(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )","return _netList["+std::to_string(animal)+"]->hidden("+std::to_string(hiddenX)+","+std::to_string(hiddenY)+")",e);
    }
}
std::vector<float>      GeneticNet::hidden(unsigned int hiddenX, unsigned int hiddenY)
{
    //this->run();
    std::vector<float> ret(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            ret[a] = _netList[a]->hidden(hiddenX,hiddenY);
        } catch (std::runtime_error &e) {
            error_general("hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )","ret["+std::to_string(a)+"] = _netList["+std::to_string(a)+"]->hidden("+std::to_string(hiddenX)+","+std::to_string(hiddenY)+")",e);
        }
    }
    return ret;
}
std::vector<float>      GeneticNet::hiddenX(unsigned int animal, unsigned int hiddenX) // |    Alle in einer Spalte
{
    if(animal > _animals-1)
    {
        error_general("hiddenX(unsigned int ["+std::to_string(animal)+"] , unsinged int ["+std::to_string(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    try {
        return _netList[animal]->hiddenX(hiddenX);
    } catch (std::runtime_error &e) {
        error_general("hiddenX(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] )","return _netList["+std::to_string(animal)+"]->hiddenX("+std::to_string(hiddenX)+")",e);
    }
}
std::vector<std::vector<float>  >GeneticNet::hiddenX(unsigned int hiddenX)
{
    //this->run();
    std::vector<std::vector<float>  > hidX(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            hidX[a] = _netList[a]->hiddenX(hiddenX);
        } catch (std::runtime_error &e) {
            error_general("hiddenX(unsigned int ["+std::to_string(hiddenX)+"] )","hidX["+std::to_string(a)+"] = _netList["+std::to_string(a)+"]->hiddenX("+std::to_string(hiddenX)+");",e);
        }
    }
    return hidX;
}
std::vector<float>      GeneticNet::hiddenY(unsigned int animal, unsigned int hiddenY)// --   Alle in einer Reihe
{
    if(animal > _animals-1)
    {
        error_general("hiddenY(unsigned int ["+std::to_string(animal)+"] , unsinged int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    try {
        return _netList[animal]->hiddenY(hiddenY);
    } catch (std::runtime_error &e) {
        error_general("hiddenY(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenY)+"] )","return _netList["+std::to_string(animal)+"]->hiddenY("+std::to_string(hiddenY)+")",e);
    }
}
std::vector<std::vector<float>  >GeneticNet::hiddenY(unsigned int hiddenY)
{
    //this->run();
    std::vector<std::vector<float>  > hidY(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            hidY[a] = _netList[a]->hiddenY(hiddenY);
        } catch (std::runtime_error &e) {
            error_general("hiddenY(unsigned int ["+std::to_string(hiddenY)+"] )","hidX["+std::to_string(a)+"] = _netList["+std::to_string(a)+"]->hiddenY("+std::to_string(hiddenY)+");",e);
        }
    }
    return hidY;
}

Neuron                 *GeneticNet::hiddenNeuron(unsigned int animal, unsigned int hiddenX, unsigned int hiddenY)
{
    if(animal > _animals-1)
    {
        error_general("hiddenNeuron(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] , unsinged int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    try {
        return _netList[animal]->hiddenNeuron(hiddenX,hiddenY);
    } catch (std::runtime_error &e) {
        error_general("hiddenNeuron(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] , unsinged int ["+std::to_string(hiddenY)+"] )","return _netList["+std::to_string(animal)+"]->hiddenNeuron("+std::to_string(hiddenX)+","+std::to_string(hiddenY)+")",e);
    }
}
std::vector<Neuron*>    GeneticNet::hiddenNeuronX(unsigned int animal, unsigned int hiddenX)// |    Alle in einer Spalte
{
    if(animal > _animals-1)
    {
        error_general("hiddenNeuronX(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    try {
        return _netList[animal]->hiddenNeuronX(hiddenX);
    } catch (std::runtime_error &e) {
        error_general("hiddenNeuronX(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] )","return _netList["+std::to_string(animal)+"]->hiddenNeuronX("+std::to_string(hiddenX)+");",e);
    }
}
std::vector<Neuron*>    GeneticNet::hiddenNeuronY(unsigned int animal, unsigned int hiddenY)// --   Alle in einer Reihe
{
    if(animal > _animals-1)
    {
        error_general("hiddenNeuronY(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    try {
        return _netList[animal]->hiddenNeuronY(hiddenY);
    } catch (std::runtime_error &e) {
        error_general("hiddenNeuronY(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenY)+"] )","return _netList["+std::to_string(animal)+"]->hiddenNeuronY("+std::to_string(hiddenY)+")",e);
    }
}
std::vector<std::vector<Neuron*> > GeneticNet::hiddenNeuron(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("hiddenNeuron(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    return _netList[animal]->hiddenNeuron();
}
Neuron                 *GeneticNet::outputNeuron(unsigned int animal, unsigned int output)
{
    if(animal > _animals-1)
    {
        error_general("outputNeuron(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    try {
        return _netList[animal]->outputNeuron(output);
    } catch (std::runtime_error &e) {
        error_general("outputNeuron(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(output)+"] )","return _netList["+std::to_string(animal)+"]->outputNeuron("+std::to_string(output)+")",e);
    }
}
std::vector<Neuron*>   *GeneticNet::outputNeuron(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("outputNeuron(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    try {
        return _netList[animal]->outputNeuron();
    } catch (std::runtime_error &e) {
        error_general("outputNeuron(unsigned int ["+std::to_string(animal)+"] , )","return _netList["+std::to_string(animal)+"]->outputNeuron()",e);
    }
}


float                   GeneticNet::output(unsigned int animal, unsigned int output)
{
    if(animal > _animals-1)
    {
        error_general("output(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    try {
        return _netList[animal]->output(output);
    } catch (std::runtime_error &e) {
        error_general("output(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(output)+"] )","return _netList["+std::to_string(animal)+"]->output("+std::to_string(output)+")",e);
    }
}
std::vector<float>      GeneticNet::output(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("output(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    try {
        return _netList[animal]->output();
    } catch (std::runtime_error &e) {
        error_general("output(unsigned int ["+std::to_string(animal)+"] , )","return _netList["+std::to_string(animal)+"]->output()",e);
    }
}
std::vector<std::vector<float>  >GeneticNet::output()
{
    //this->run();
    std::vector<std::vector<float>  > out(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            out[a] = _netList[a]->output();
        } catch (std::runtime_error &e) {
            error_general("output()","out["+std::to_string(a)+"] = _netList["+std::to_string(a)+"]->output()",e);
        }
    }
    return out;
}


void                    GeneticNet::run()
{
    if(_update)
    {
        double timeout = 1; //1 sec
       // qDebug() << "threads start";
        std::chrono::high_resolution_clock::time_point t2;
        std::chrono::duration<double> time_span;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

#ifdef __enableGeneticNetThread
        pthread_mutex_lock(&_threadLock);
        _threadDelayMicros = 1000000;
        _threadExit = false;
        _threadPause = false;
        for(unsigned int a=0; a<_animals; a++)
        {
            _threadData[a].isPaused = false;
        }
        pthread_cond_broadcast( &_thread_condition_var );
        pthread_mutex_unlock(&_threadLock);
        unsigned int pause = 0;
        unsigned int _pause;
        std::vector<bool> restartCheckList(_animals,false);

        do{
            pause = 0;
            for(unsigned int a=0; a<_animals; a++)
            {
                t2 = std::chrono::high_resolution_clock::now();
                time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
                if(time_span.count() >= timeout)
                {
                    qDebug() << "GeneticNet::run() thread timeout: "<<timeout;
                    qDebug() << "Threads running: "<<_animals-pause;
                    timeout+=timeout;
                    pause = 0;
                    for(unsigned int b=0; b<_animals; b++)
                    {
                        if(restartCheckList[b])
                        {
                            pause++;
                        }
                    }

                    FILE *p_file = fopen("threadTimeout.txt","a");
                    if(p_file)
                    {
                        fprintf(p_file,"running threads: %i\n",_animals-pause);
                        pthread_mutex_lock(&_threadLock);
                        for(unsigned int b=0; b<_animals; b++)
                        {
                            fprintf(p_file,"thread: %i\tdebugParam: %i\tpaused: %i\n",b,_threadData[b].debugParam,_threadData[b].isPaused);
                        }
                        pthread_mutex_unlock(&_threadLock);
                        fclose(p_file);
                    }
                }
                if(restartCheckList[a])
                {
                    pause++;
                    continue;
                }
                pthread_mutex_lock(&_threadLock);
                _pause = _threadData[a].debugParam;
                pthread_mutex_unlock(&_threadLock);
                if(_pause == 7)
                {
                   restartCheckList[a] = true;
                }
                //pthread_join(_threadList[a], NULL);
            }
        }while(pause != _animals);

        pthread_mutex_lock(&_threadLock);
        _threadPause = true;
        pthread_mutex_unlock(&_threadLock);
#else

        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->run();
        }
#endif

        t2 = std::chrono::high_resolution_clock::now();
        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        _timeInterval = 0.9*_timeInterval + 0.1*time_span.count();
#ifdef __DEBUG_TIMEINTERVAL
        if(_debugCount > 1000)
        {
            qDebug() << "n: "<<_timeInterval;
            _debugCount = 0;
        }
        _debugCount++;
#endif
        //qDebug() << "threads ended";
        _update = false;
    }
}
void                    GeneticNet::run(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("run(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    try {
        _netList[animal]->run();
    } catch (std::runtime_error &e) {
        error_general("run(unsigned int ["+std::to_string(animal)+"] )","_netList["+std::to_string(animal)+"]->run()",e);
    }
}


Net                    *GeneticNet::operator[](unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("operator[](unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal];
}

void                    GeneticNet::mutationFactor(float factor)
{
    if(factor <= __FLOATINPOINT_TOLERANCE)
    {
        error_general("mutationFactor(float ["+std::to_string(factor)+"] )","mutationFactor must be greater than 0");
    }
    _mutationFactor = factor;
}
void                    GeneticNet::mutationChangeWeight(float weight)
{
    if(weight <= __FLOATINPOINT_TOLERANCE)
    {
        error_general("mutationChangeWeight(float ["+std::to_string(weight)+"] )","mutationChangeWeight must be greater than 0");
    }
    _mutationChangeWeight = weight;
}

void                    GeneticNet::score(unsigned int animal, float score)
{
    if(animal > _animals-1)
    {
        error_general("score(unsigned int ["+std::to_string(animal)+"] , float ["+std::to_string(score)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    if(score <= 0)
    {
        score = 1;
    }
    _scoreList[animal] = score;
}
float                   GeneticNet::score(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("score(unsigned int ["+std::to_string(animal)+"])",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _scoreList[animal];
}
void                    GeneticNet::score(std::vector<float>   scoreList)
{
    if(scoreList.size() != _scoreList.size())
    {
        qDebug() << "scoreList.size() "<<scoreList.size();
        error_general("score(std::vector<float>)","std::vector<float>.size() == "+std::to_string(scoreList.size())+"  "+error_paramOutOfRange(0,scoreList.size(),_animals,_animals));
    }
    for(unsigned int a=0; a<_animals; a++)
    {
        score(a,scoreList[a]);
    }
}
std::vector<float>      GeneticNet::score()
{
    return _scoreList;
}

void                    GeneticNet::learn(std::vector<float>   scoreList)
{
    this->score(scoreList);
    this->learn();
}
void                    GeneticNet::learn()
{
    float gesScore      = 0;
    float scoreOffset   = 0;
    unsigned int selection1;
    unsigned int selection2;
    std::vector<std::vector<float>  >   newGenom(_animals,std::vector<float>(this->genomsize(),0));


    for(unsigned int a=0; a<_animals; a++)
    {
        if(_scoreList[a] < scoreOffset)
        {
            scoreOffset = _scoreList[a];
        }
    }
    scoreOffset         = -scoreOffset;
    for(unsigned int a=0; a<_animals; a++)
    {
        gesScore       += scoreOffset + _scoreList[a]+0.1;
        _scoreList[a]  += scoreOffset+0.1;
    }
    for(unsigned int a=0; a<_animals; a+=2)
    {
        learn_selectAnimal(gesScore,selection1,selection2);

        if(a+1 == _animals)
        {
            learn_crossover(selection1,selection2,newGenom[a],newGenom[a-1]);
            learn_mutate(newGenom[a-1]);
        }
        else
        {
            learn_crossover(selection1,selection2,newGenom[a],newGenom[a+1]);
            learn_mutate(newGenom[a+1]);
        }
        learn_mutate(newGenom[a]);
    }
    this->genom(newGenom);
}

void                    GeneticNet::updateNetConfiguration()
{
#ifdef __enableGeneticNetThread
       int rc;
       for(unsigned int a=0; a<_animals; a++)
       {
           rc = pthread_create(&_threadList_setupNet[a], NULL, runThread_setupNet, (void *)&_threadData_setupNet[a]);
           if (rc)
           {
               qDebug() << "Error:unable to create thread," << rc << endl;
           }
       }
       for(unsigned int a=0; a<_animals; a++)
       {
           pthread_join(_threadList_setupNet[a], NULL);
       }
       qDebug() << "setup nets done";
#endif
   /* try
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->updateNetConfiguration();
        }
    } catch (std::runtime_error &e) {
        error_general("updateNetConfiguration()",e);
    }*/
}
void                    GeneticNet::connectNeuronViaID(unsigned int fromNeuron,unsigned int toNeuron,ConnectionDirection direction)
{
    try
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->connectNeuronViaID(fromNeuron,toNeuron,direction);
        }
    } catch (std::runtime_error &e) {
        error_general("connectNeuronViaID(unsigned int ["+std::to_string(fromNeuron)+"],unsigned int ["+std::to_string(toNeuron)+"])",e);
    }
}
bool                    GeneticNet::connectNeuron(Connection *connection)
{
    bool ret = 0;
    try
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            if(_netList[a]->connectNeuron(connection)){
                ret = 1;
                _netList[a]->update_ptr_genomList();
            }
        }

    } catch (std::runtime_error &e) {
        error_general("connectNeuron(Connection ["+Neuron::connectionString(*connection)+"])",e);
    }
    return ret;
}
bool                    GeneticNet::connectNeuron(std::vector<Connection> *connections)
{
    bool ret = 0;
    try
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            if(_netList[a]->connectNeuron(connections)){
                ret = 1;
            }
        }
    } catch (std::runtime_error &e) {
        error_general("connectNeuron(std::vector<Connection> [connection list])",e);
    }
    return ret;
}
void                    GeneticNet::connectionList(std::vector<std::vector<Connection> >*connections)
{
    if(connections->size() != _animals)
    {
        error_general("connectionList(std::vector<std::vector<Connection> >connections)","size of connections: "+std::to_string(connections->size())+" ist not the same as _animals: "+std::to_string(_animals));
    }
    try
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->connectionList(&(*connections)[a]);
        }
    } catch (std::runtime_error &e) {
        error_general("connectionList(std::vector<std::vector<Connection> >connections)",e);
    }
}
std::vector<Connection> *GeneticNet::connectionList(unsigned int netID)
{
    for(unsigned int net=0; net<_netList.size(); net++)
    {
        if(_netList[net]->ID() == netID)
        {
            return _netList[netID]->connectionList();
        }
    }
    error_general("connectionList(unsigned int ["+std::to_string(netID)+"])","No net with such an ID");
}

std::vector<std::vector<Connection> *>GeneticNet::connectionList()
{
    std::vector<std::vector<Connection> * > list;
    for(unsigned int net=0; net<_netList.size(); net++)
    {
        list.push_back(_netList[net]->connectionList());
    }

    return list;
}
void                    GeneticNet::clearConnectionList()
{
    for(unsigned int a=0; a<_animals; a++)
    {
        _netList[a]->clearConnectionList();
    }
}

NeuronID                GeneticNet::addNeuron()
{
    NeuronID ID;
    try {
        for(unsigned int a=0; a<_netList.size(); a++)
        {
            ID = _netList[a]->addNeuron();
        }
    } catch (std::runtime_error &e) {
        error_general("addNeuron()",e);
    }
    return ID;
}
NeuronID                GeneticNet::addNeuron(Neuron *neuron)
{
    NeuronID ID;
    try {
        for(unsigned int a=0; a<_netList.size(); a++)
        {
            ID = _netList[a]->addNeuron(neuron);
        }
    } catch (std::runtime_error &e) {
        error_general("addNeuron(Neruon *neuron)",e);
    }
    return ID;
}
NeuronID                GeneticNet::addNeuron(Connection connection)
{
    NeuronID ID;
    try {
        for(unsigned int a=0; a<_netList.size(); a++)
        {
            ID = _netList[a]->addNeuron(connection);
        }
    } catch (std::runtime_error &e) {
        error_general("addNeuron(Connection ["+Neuron::connectionString(connection)+"])",e);
    }
    return ID;
}
NeuronID                GeneticNet::addNeuron(std::vector<Connection> inputConnections)
{
    NeuronID ID;
    try {
        for(unsigned int a=0; a<_netList.size(); a++)
        {
            ID = _netList[a]->addNeuron(inputConnections);
        }
    } catch (std::runtime_error &e) {
        error_general("addNeuron(std::vector<Connection> [connection list])",e);
    }
    return ID;
}

void                    GeneticNet::learn_selectAnimal(float gesScore,unsigned int &selection1,unsigned int &selection2)
{
    float random;
    float beginScore;

    random = (float)(_randEngine() % (unsigned int)(gesScore * 100))/100;
    beginScore = 0;
    for(unsigned int a=0; a<_animals; a++)
    {
        if(beginScore <= random && _scoreList[a] + beginScore > random)
        {
            selection1 = a;
            break;
        }
        else
        {
            beginScore += _scoreList[a];
        }
    }
    unsigned int counter = 0;

    do{
        random = (float)(_randEngine() % (unsigned int)((gesScore - _scoreList[selection1]) * 100))/100;
        beginScore = 0;
        for(unsigned int a=0; a<_animals; a++)
        {
            if(a == selection1)
            {
                continue;
            }
            if(beginScore <= random && _scoreList[a] + beginScore > random)
            {
                selection2 = a;
                break;
            }
            else
            {
                beginScore += _scoreList[a];
            }
            if(counter > 100)
            {
                qDebug() << "warning: learn_selectAnimal(...) counter > 100";
            }
        }
        counter++;

    }while(selection1 == selection2);
}
void                    GeneticNet::learn_crossover(unsigned int selection1,unsigned int selection2,std::vector<float> &newGen1,std::vector<float> &newGen2)
{
    //_netList[selection1]->ptr_genom(),_netList[selection2]->ptr_genom()
    unsigned int random =  1 + _randEngine() % (genomsize() - 2);

    for(unsigned int a=0; a<random; a++)
    {
        newGen1[a] = *(*_netList[selection2]->ptr_genom())[a];
        newGen2[a] = *(*_netList[selection1]->ptr_genom())[a];
    }
    for(unsigned int a=random; a<genomsize(); a++)
    {
        newGen1[a] = *(*_netList[selection1]->ptr_genom())[a];
        newGen2[a] = *(*_netList[selection2]->ptr_genom())[a];
    }
}
void                    GeneticNet::learn_mutate(std::vector<float> &genom)
{
    if(_mutationFactor <= __FLOATINPOINT_TOLERANCE || _mutationChangeWeight <= __FLOATINPOINT_TOLERANCE)
        return;
    for(unsigned int a=0; a<genomsize(); a++)
    {
        if(_randEngine() % (unsigned int)(1/_mutationFactor) == 0)
        {
            unsigned int random = _randEngine();
            float ran = (float)(random % 2000);
            ran = ran-1000;
            ran = ran/1000;
            genom[a] += _mutationChangeWeight * ran;
        }
    }
}
double                 GeneticNet::cycleTime()
{
    return _timeInterval;
}
void                   GeneticNet::update_ptr_genomList()
{
    for(unsigned int a=0; a<_animals; a++)
    {
        _netList[a]->update_ptr_genomList();
    }
}
void                   *GeneticNet::runThread(void *threadarg)
{
    //pthread_detach(pthread_self());
    struct thread_data_geneticNet *my_data;
    bool ret = false;
    bool pause = false;
    bool enableLoop = false;
    //bool lastPauseState = false;
    my_data = (struct thread_data_geneticNet *) threadarg;
    struct timespec time, timestart;
    time.tv_sec = 0;
    pthread_mutex_lock(my_data->lock);
    my_data->isPaused = false;
    time.tv_nsec = *my_data->delayMicros;
    pthread_mutex_unlock(my_data->lock);
    qDebug() << "thread start: "<<my_data->thread_id << " "<<my_data->net;
    pthread_mutex_lock(my_data->lock);
    my_data->debugParam = 1; //start
    pthread_mutex_unlock(my_data->lock);
#ifdef __DEBUG_TIMEINTERVAL_IN_THREAD
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    std::chrono::duration<double> time_span;
    double _timeInterval = 0;
    unsigned int _debugCount = 0;
#endif
    while(!ret)
    {
        if(enableLoop)
        {
            try {

                pthread_mutex_lock(my_data->lock);
                my_data->debugParam = 3; //run begin
                pthread_mutex_unlock(my_data->lock);
                my_data->net->run();
                //pause = true;
                pthread_mutex_lock(my_data->lock);
                my_data->debugParam = 4; //run end
                pthread_mutex_unlock(my_data->lock);

            }
            catch(std::runtime_error &e)
                {
                pthread_mutex_lock(my_data->lock);
                    my_data->debugParam = 20; //error 1
                    pthread_mutex_unlock(my_data->lock);
                    qDebug() << "ERROR  thread "<< my_data->thread_id << " : " << e.what();
                    FILE *file = fopen("error_n.txt","a");
                    if(file)
                    {
                        fprintf(file,"%s\n",(std::string("ERROR  net ")+std::to_string(my_data->thread_id)+e.what()).c_str());
                        fclose(file);
                    }
                }catch(...)
            {
                pthread_mutex_lock(my_data->lock);
                my_data->debugParam = 21; //error 2
                pthread_mutex_unlock(my_data->lock);
                qDebug() << "ERROR  thread "<< my_data->thread_id << " : ";
                FILE *file = fopen("error_n.txt","a");
                if(file)
                {
                    fprintf(file,"%s\n",(std::string("ERROR  net ")+std::to_string(my_data->thread_id)).c_str());
                    fclose(file);
                }
            }
            enableLoop = false;
            pthread_mutex_lock(my_data->lock);
            my_data->debugParam = 5; //finish
            pthread_mutex_unlock(my_data->lock);
        }else {
            //qDebug() << "thread: "<<my_data->thread_id << " sleeping";
            //usleep(1);

            //nanosleep(&time, NULL);
#ifdef __DEBUG_TIMEINTERVAL_IN_THREAD
            t2 = std::chrono::high_resolution_clock::now();
            time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
            _timeInterval = 0.9*_timeInterval + 0.1*time_span.count();
            if(_debugCount > 1000)
            {
                if(my_data->thread_id == 0)
                    qDebug() << "n_: "<<my_data->thread_id << " " <<_timeInterval;
                _debugCount = 0;
            }
            _debugCount++;
#endif
            pthread_mutex_lock(my_data->lock);
            my_data->debugParam = 6; //set isPaused to true
            my_data->isPaused = true;
            my_data->debugParam = 7; //isPaused true
            pthread_mutex_unlock(my_data->lock);



            pthread_mutex_lock(my_data->lock);
            pthread_cond_wait( my_data->condition_var, my_data->lock);
            ret = *my_data->exit;
            //pause = *my_data->pause;
            //pause = my_data->isPaused;
            //time.tv_nsec = *my_data->delayMicros;
            my_data->debugParam = 2; //enableLoop
            pthread_mutex_unlock(my_data->lock);
            enableLoop = true;


#ifdef __DEBUG_TIMEINTERVAL_IN_THREAD
            t1 = std::chrono::high_resolution_clock::now();
#endif
           // if(!pause)
           // {
           //     enableLoop = true;
                /*pthread_mutex_lock(my_data->lock);
                my_data->isPaused = false;
                pthread_mutex_unlock(my_data->lock);*/
           // }

        }
        /*if(pause != lastPauseState)
        {
            if(pause)
            {
                qDebug() << "thread: "<<my_data->thread_id << " pause ON";
            }else {
                qDebug() << "thread: "<<my_data->thread_id << " pause OFF";
            }
        }
        lastPauseState = pause;*/
    }
    qDebug() << "thread stop: "<<my_data->thread_id << " "<<my_data->net;
    pthread_exit(NULL);
}
void                   *GeneticNet::runThread_setupNet(void *threadarg)
{
    //pthread_detach(pthread_self());
    struct thread_data_geneticNet *my_data;
    my_data = (struct thread_data_geneticNet *) threadarg;

 //   my_data->net = new Net(my_data->thread_id);
    qDebug() << "threadSetup begin: "<<my_data->thread_id;
    //pthread_mutex_lock(my_data->lock);
    my_data->net->updateNetConfiguration();
    //pthread_mutex_unlock(my_data->lock);
    //my_data->net->updateNetConfiguration();
    qDebug() << "threadSetup done: "<<my_data->thread_id;
    pthread_exit(NULL);
}
//----------ERROR

std::string GeneticNet::error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max)
{
    return " parameter "+std::to_string(paramPos)+" is out of range: "+value+"     minimum is: "+min+"     maximum is: "+max;
}
std::string GeneticNet::error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string GeneticNet::error_paramOutOfRange(unsigned int paramPos,int value,int min, int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string GeneticNet::error_paramOutOfRange(unsigned int paramPos,float value,float min, float max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
void        GeneticNet::error_general(std::string function, std::string cause)
{
    throw std::runtime_error("ERROR: GeneticNet::" + function + "     " + cause);
}
void        GeneticNet::error_general(std::string function, std::runtime_error &e)
{
    error_general(function,"",e);
}
void        GeneticNet::error_general(std::string function, std::string cause, std::runtime_error &e)
{
    std::string error = "ERROR: GeneticNet::" + function + "     " + cause;
    error += "     --> "+std::string(e.what());
    throw std::runtime_error(error);
}
