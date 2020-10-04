#include <geneticnet.h>

#ifdef QT_APP
GeneticNet::GeneticNet(QObject *parent)
    : QObject(parent)
#else
GeneticNet::GeneticNet()
#endif
{
    this->init(GENETICNET_MIN_ANIMALS,NET_MIN_INPUTNEURONS,NET_MIN_HIDDENNEURONS_X,NET_MIN_HIDDENNEURONS_Y,NET_MIN_OUTPUTNEURONS,true,false,Activation::Sigmoid);
}
#ifdef QT_APP
GeneticNet::GeneticNet(unsigned int animals,
                       QObject *parent)
    : QObject(parent)
#else
GeneticNet::GeneticNet(unsigned int animals)
#endif
{
    this->init(animals,NET_MIN_INPUTNEURONS,NET_MIN_HIDDENNEURONS_X,NET_MIN_HIDDENNEURONS_Y,NET_MIN_OUTPUTNEURONS,true,false,Activation::Sigmoid);
}
#ifdef QT_APP
GeneticNet::GeneticNet(unsigned int animals,
                       unsigned int inputs,
                       unsigned int hiddenX,
                       unsigned int hiddenY,
                       unsigned int outputs,
                       QObject *parent)
    : QObject(parent)
#else
GeneticNet::GeneticNet(unsigned int animals,
                       unsigned int inputs,
                       unsigned int hiddenX,
                       unsigned int hiddenY,
                       unsigned int outputs)
#endif
{
    this->init(animals,inputs,hiddenX,hiddenY,outputs,true,false,Activation::Sigmoid);
}
#ifdef QT_APP
GeneticNet::GeneticNet(unsigned int animals,
                       unsigned int inputs,
                       unsigned int hiddenX,
                       unsigned int hiddenY,
                       unsigned int outputs,
                       bool enableBias,
                       bool enableAverage,
                       Activation func,
                       QObject *parent)
    : QObject(parent)
#else
GeneticNet::GeneticNet(unsigned int animals,
                       unsigned int inputs,
                       unsigned int hiddenX,
                       unsigned int hiddenY,
                       unsigned int outputs,
                       bool enableBias,
                       bool enableAverage,
                       Activation func)
#endif
{
    this->init(animals,inputs,hiddenX,hiddenY,outputs,enableBias,enableAverage,func);
}
GeneticNet::~GeneticNet()
{
    for(unsigned long long b=_netList.size(); b>0; b--)
    {
        try {
            delete _netList[b-1];
        } catch (std::exception &e) {
            CONSOLE << "error: "<<e.what();
        }
    }
    _scoreList.clear();
#ifdef __enableGeneticNetThread
    pthread_mutex_lock(&_threadLock);
    _threadExit = true;
    pthread_cond_broadcast( &_thread_condition_var );
    pthread_mutex_unlock(&_threadLock);
    for(unsigned long long a=_threadList.size(); a>0; a--)
    {
        pthread_join(_threadList[a],nullptr);
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
    this->set_animals(animals);
    this->set_inputNeurons(inputs);
    this->set_hiddenNeuronsX(hiddenX);
    this->set_hiddenNeuronsY(hiddenY);
    this->set_outputNeurons(outputs);
    this->set_bias(enableBias);
    this->set_enableAverage(enableAverage);
    this->set_activationFunction(func);
#ifdef __DEBUG_GENETICNET
    CONSOLE<< "Geneticnet constructor Setup done";
#endif
}

void                    GeneticNet::set_netFileName(std::string filename)
{
    _saveNet.set_filename(filename);
}
std::string             GeneticNet::get_netFileName()
{
    return _saveNet.get_filename();
}
void                    GeneticNet::set_netFileEnding(std::string fileEnding)
{
    _saveNet.set_fileEnding(fileEnding);
}
std::string             GeneticNet::get_netFileEnding()
{
    return _saveNet.get_fileEnding();
}
void                    GeneticNet::loadFromNetFile()
{
#ifdef __DEBUG_GENETICNET
    CONSOLE<< "loadFromNetFile() " << _saveNet.get_filename().c_str()<<"."<<_saveNet.get_fileEnding().c_str();
#endif
    bool loadSucceed = false;
    try {
        loadSucceed = _saveNet.loadFile();
    } catch (std::runtime_error &e) {
        CONSOLE<< "Warning: "<< e.what();
        return;
    }catch(...)
    {
        CONSOLE<< "ERROR: _saveNet.loadfile()";
    }

    if(loadSucceed)
    {
		#ifdef __DEBUG_GENETICNET
		CONSOLE<< "read done -> applaying...";
		#endif
        try {
            this->set(_saveNet.get_animals(),
                      _saveNet.get_inputNeurons(),
                      _saveNet.get_hiddenNeuronsX(),
                      _saveNet.get_hiddenNeuronsY(),
                      _saveNet.get_outputNeurons(),
                      _saveNet.get_bias(),
                      _saveNet.get_enableAverage(),
                      _saveNet.get_activationFunction());
            this->set_biasValue(_saveNet.get_biasValue());
            //this->set_costumNeurons(_saveNet.get_costumConnections());
            this->set_connectionList(*_saveNet.get_ptr_connectionList());
            //this->set_costumConnections(_saveNet.get_costumConnections());
            /*this->set_neurons(_saveNet.get_neurons(),
                          _saveNet.get_hiddenNeurons(),
                          _saveNet.get_outputNeurons(),
                          _saveNet.get_costumNeurons());*/
     //       this->genom(_saveNet.genom());
            //CONSOLE<< "only backloop test";
            //this->updateNetConfiguration();

        } catch (std::runtime_error &e) {
            /*error_general("loadFromNetFile(std::string ["+_saveNet.filename()+"] , std::string ["+_saveNet.fileEnding()+"] )",
                          "unable to apply the settings. Maybe the file is damaged.",e);*/
            addError(Error("loadFromNetFile(std::string ["+_saveNet.get_filename()+"] , std::string ["+_saveNet.get_fileEnding()+"] )",
                          std::vector<std::string>{"unable to apply the settings. Maybe the file is damaged.",
                           e.what()}));
        }
#ifdef __DEBUG_GENETICNET
        CONSOLE << "applaying done";
#endif
    }
#ifdef __DEBUG_GENETICNET
    else
    {
        CONSOLE << (_saveNet.get_filename()+"."+_saveNet.get_fileEnding() +" dosesn't exist").c_str();
    }
#endif



}
void                    GeneticNet::loadFromNetFile(std::string filename)
{
    this->set_netFileName(filename);
    this->loadFromNetFile();
}
void                    GeneticNet::loadFromNetFile(std::string filename,std::string fileEnding)
{
    this->set_netFileName(filename);
    this->set_netFileEnding(fileEnding);
    this->loadFromNetFile();
}
void                    GeneticNet::saveToNetFile()
{
#ifdef __DEBUG_GENETICNET
    CONSOLE<< "saveToNetFile() " << _saveNet.get_filename().c_str()<<"."<<_saveNet.get_fileEnding().c_str();
#endif
    try {
        _saveNet.set(this->get_inputNeurons(),
                     this->get_hiddenNeuronsX(),
                     this->get_hiddenNeuronsY(),
                     this->get_outputNeurons(),
                     this->get_bias(),
                     this->get_enableAverage(),
                     this->get_activationFunction(),
                     this->get_biasValue());
        //_saveNet.setGenom(this->genom());
        for(unsigned int net = 0; net<_netList.size(); net++)
        {
            _saveNet.set_ptr_neuronsOfNet(_netList[net]->get_ID(),_netList[net]->get_ptr_allNeurons());
        }
        _saveNet.saveFile();
    } catch (std::runtime_error &e) {
        addError(Error("saveToNetFile()",std::string(e.what())));
        //error_general("saveToNetFile()",e);
    }
#ifdef __DEBUG_GENETICNET
    CONSOLE<< "save done";
#endif
}
void                    GeneticNet::saveToNetFile(std::string filename)
{
    this->set_netFileName(filename);
    this->saveToNetFile();
}
void                    GeneticNet::saveToNetFile(std::string filename,std::string fileEnding)
{
    this->set_netFileName(filename);
    this->set_netFileEnding(fileEnding);
    this->saveToNetFile();
}



void                    GeneticNet::set_animals(unsigned int animals)
{
    if(animals < GENETICNET_MIN_ANIMALS || animals > GENETICNET_MAX_ANIMALS)
    {
        addError(Error("set_animals(unsigned int ["+std::to_string(animals)+"] )",
                 ErrorMessage::outOfRange('[',static_cast<unsigned int>(GENETICNET_MIN_ANIMALS),animals,static_cast<unsigned int>(GENETICNET_MAX_ANIMALS),']')));
        return;
        //error_general("animals(unsigned int ["+std::to_string(animals)+"] )","Parameter 0 is out of range. Min: "+ std::to_string(GENETICNET_MIN_ANIMALS)+" Max: "+std::to_string(GENETICNET_MAX_ANIMALS));
    }
    if(animals == _animals)
    {
        return;
    }
    if(animals > _animals)
    {
       _threadExit = true;
       _threadPause = false;
   //    _threadDelayMicros = 1000000;
       if(_threadList.size() != 0)
       {
           for(unsigned int a=0; a<_threadList.size(); a++)
           {
               pthread_join(_threadList[a], nullptr);
           }
       }
       if(_threadList_setupNet.size() != 0)
       {
           for(unsigned int a=0; a<_threadList_setupNet.size(); a++)
           {
               pthread_join(_threadList_setupNet[a], nullptr);
           }
       }
       _netList.reserve(animals);
       _threadList.reserve(animals);
       _threadData.reserve(animals);
       _threadList_setupNet.reserve(animals);
       _threadData_setupNet.reserve(animals);
       for(unsigned int a=_animals; a<animals; a++)
        {
           #ifdef QT_APP
            _netList.push_back(new Net(a,this));
            this->connectSignalsFromNet(_netList[_netList.size()-1]);
            #else
            _netList.push_back(new Net(a));
            #endif
            _scoreList.push_back(0);
            _threadList.push_back(pthread_t());
            _threadList_setupNet.push_back(pthread_t());

            _threadData.push_back(thread_data_geneticNet());
            _threadData[a].thread_id = static_cast<int>(a);
            _threadData[a].net = _netList[a];
            _threadData[a].exit = &_threadExit;
            _threadData[a].pause = &_threadPause;
            _threadData[a].lock = &_threadLock;
            _threadData[a].condition_var = &_thread_condition_var;
            _threadData[a].isPaused = false;
     //       _threadData[a].delayMicros = &_threadDelayMicros;

            _threadData_setupNet.push_back(thread_data_geneticNet());
            _threadData_setupNet[a].thread_id = static_cast<int>(a);
            _threadData_setupNet[a].net = _netList[a];
            _threadData_setupNet[a].exit = &_threadExit;
            _threadData_setupNet[a].pause = &_threadPause;
            _threadData_setupNet[a].lock = &_threadLock_setupNet;
            _threadData_setupNet[a].condition_var = &_thread_condition_var_setupNet;
            _threadData_setupNet[a].isPaused = false;
            //_threadData_setupNet[a].delayMicros = &_threadDelayMicros;
        }

    }
    else
    {
        for(unsigned int a=0; a<_animals - animals; a++)
        {
#ifdef QT_APP
            QObject::disconnect(_netList[_netList.size()-1],&Net::errorOccured,this,&GeneticNet::onNetError);
#endif
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
    _needsCalculationUpdate  = true;

#ifdef __enableGeneticNetThread
    _threadExit = false;
    _threadPause = true;


    int rc;
    for(unsigned int a=0; a<_animals; a++)
    {
        rc = pthread_create(&_threadList[a], nullptr, runThread, static_cast<void *>(&_threadData[a]));
        if (rc)
        {
            addError(Error("set_animals(unsigned int ["+std::to_string(animals)+"] )",
                           std::string("Unable to create thread. ID: "+std::to_string(a) + " rc: "+std::to_string(rc))));
            CONSOLE<< "Error: unable to create thread," << rc << "\n";
        }
    }
#endif
}
unsigned int            GeneticNet::get_animals()
{
    return _animals;
}
void                    GeneticNet::set_inputNeurons(unsigned int inputs)
{
    if(inputs != _netList[0]->get_inputNeurons())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->set_inputNeurons(inputs);
        }
        _needsCalculationUpdate = true;
    }
}
unsigned int            GeneticNet::get_inputNeurons()
{
    return _netList[0]->get_inputNeurons();
}
void                    GeneticNet::set_hiddenNeuronsX(unsigned int hiddenX)
{
    if(hiddenX != _netList[0]->get_hiddenNeuronsX())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->set_hiddenNeuronsX(hiddenX);
        }
        _needsCalculationUpdate = true;
    }
}
unsigned int            GeneticNet::get_hiddenNeuronsX()
{
    return _netList[0]->get_hiddenNeuronsX();
}
void                    GeneticNet::set_hiddenNeuronsY(unsigned int hiddenY)
{
    if(hiddenY != _netList[0]->get_hiddenNeuronsY())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->set_hiddenNeuronsY(hiddenY);
        }
        _needsCalculationUpdate = true;
    }
}
unsigned int            GeneticNet::get_hiddenNeuronsY()
{
    return _netList[0]->get_hiddenNeuronsY();
}
void                    GeneticNet::set_outputNeurons(unsigned int outputs)
{
    if(outputs != _netList[0]->get_outputNeurons())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->set_outputNeurons(outputs);
        }
        _needsCalculationUpdate = true;
    }
}
unsigned int            GeneticNet::get_outputNeurons()
{
    return _netList[0]->get_outputNeurons();
}
/*void                    GeneticNet::set_costumNeurons(unsigned int costum)
{
    if(costum != _netList[0]->get_costumNeurons())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->set_costumNeurons(costum);
        }
    }
}*/
unsigned int            GeneticNet::get_costumNeurons()
{
    return _netList[0]->get_costumNeurons();
}
/*void                    GeneticNet::set_costumConnections(unsigned int connections)
{
    for(unsigned int a=0; a<_animals; a++)
    {
        _netList[a]->set_costumConnections(connections);
    }
}
void                    GeneticNet::set_neurons(unsigned int neurons,unsigned int hiddenNeurons,unsigned int outputNeurons,unsigned int costumNeurons)
{
   for(unsigned int a=0; a<_animals; a++)
   {
       _netList[a]->set_neurons(neurons,hiddenNeurons,outputNeurons,costumNeurons);
   }
}
*/
void                    GeneticNet::set_bias(bool enableBias)
{
    if(enableBias != _netList[0]->get_bias())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->set_bias(enableBias);
        }
        _needsCalculationUpdate = true;
    }
}
bool                    GeneticNet::get_bias()
{
    return _netList[0]->get_bias();
}
void                    GeneticNet::set_enableAverage(bool enableAverage)
{
    if(enableAverage != _netList[0]->get_enableAverage())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->set_enableAverage(enableAverage);
        }
        _needsCalculationUpdate = true;
    }
}
bool                    GeneticNet::get_enableAverage()
{
    return _netList[0]->get_enableAverage();
}
void                    GeneticNet::set_biasValue(double value)
{
    if(value - _netList[0]->get_biasValue() < 0.0001)// has the value changed?
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->set_biasValue(value);
        }
        _needsCalculationUpdate = true;
    }
}
double                   GeneticNet::get_biasValue()
{
    return _netList[0]->get_biasValue();
}
void                    GeneticNet::set_activationFunction(Activation func)
{
    if(func != _netList[0]->get_activationFunction())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            _netList[a]->set_activationFunction(func);
        }
        _needsCalculationUpdate = true;
    }
}
Activation              GeneticNet::get_activationFunction()
{
    return _netList[0]->get_activationFunction();
}
bool                    GeneticNet::hasHiddenLayer()
{
    return _netList[0]->hasHiddenLayer();
}

void                    GeneticNet::set_randomGenom()
{
    _needsCalculationUpdate = true;
    for(unsigned int a=0; a<_animals; a++)
    {
        _netList[a]->set_randomGenom();
    }
}
void                    GeneticNet::set_genom(std::vector<std::vector<double>    > genomList)
{
    if(unsigned(genomList.size()) != _animals)
    {
        addError(Error("set_genom(std::vector<std::vector<double>    > genomList )",
                       std::string("genomList.size() != _animals : ["+std::to_string(genomList.size())+"] != ["+std::to_string(_animals)+"]")));
        return;
        //error_general("genom(std::vector<std::vector<double> >)","std::vector<std::vector<double> >.size() == "+std::to_string(genomList.size())+" "+error_paramOutOfRange(0,genomList.size(),_animals,_animals));
    }
    if(unsigned(genomList[0].size()) != get_genomsize())
    {
        addError(Error("set_genom(std::vector<std::vector<double>    > genomList )",
                       std::string("genomList[0].size() != get_genomsize() : ["+std::to_string(genomList[0].size())+"] != ["+std::to_string(get_genomsize())+"]")));
        return;
        //error_general("genom(unsigned int ["+std::to_string(animal)+"] , std::vector<double> )",error_paramOutOfRange((unsigned int)1,genom.size(),genomsize(),genomsize()));
    }
    _needsCalculationUpdate = true;
    for(unsigned int a=0; a<_animals; a++)
    {
        _netList[a]->set_genom(genomList[a]);
    }
}
void                    GeneticNet::set_genom(unsigned int animal, std::vector<double> genom)
{
    if(animal > _animals-1)
    {
        addError(Error("set_genom(unsigned int ["+std::to_string(animal)+"] , std::vector<double> genom )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return;
        //error_general("genom(unsigned int ["+std::to_string(animal)+"] , std::vector<double> )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    if(unsigned(genom.size()) != get_genomsize())
    {
        addError(Error("set_genom(unsigned int ["+std::to_string(animal)+"] , std::vector<double> genom )",
                       std::string("genom.size() != get_genomsize() : ["+std::to_string(genom.size())+"] != ["+std::to_string(get_genomsize())+"]")));
        return;
        //error_general("genom(unsigned int ["+std::to_string(animal)+"] , std::vector<double> )",error_paramOutOfRange((unsigned int)1,genom.size(),genomsize(),genomsize()));
    }
    _needsCalculationUpdate = true;
    _netList[animal]->set_genom(genom);
}
std::vector<double>      GeneticNet::get_genom(unsigned int animal)
{
    if(animal > _animals-1)
    {
        addError(Error("get_genom(unsigned int ["+std::to_string(animal)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return std::vector<double>();
        //error_general("genom(unsigned int ["+std::to_string(animal)+"])",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal]->get_genom();
}
std::vector<std::vector<double>  >GeneticNet::get_genom()
{
    std::vector<std::vector<double>  > gen;
    gen.reserve(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        gen.push_back(_netList[a]->get_genom());
    }
    return gen;
}
unsigned int            GeneticNet::get_genomsize()
{
    return _netList[0]->get_genomsize();
}
void                    GeneticNet::set_genomFromNetFile()
{
    this->set_genom(_saveNet.get_genom());
}

void                    GeneticNet::set_input(unsigned int animal, unsigned int input, double signal)
{
    if(animal > _animals-1)
    {
        addError(Error("set_input(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(input)+"] , double ["+std::to_string(signal)+"])",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return;
        //error_general("input(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(input)+"] , double ["+std::to_string(signal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    _needsCalculationUpdate = true;
    _netList[animal]->set_input(input,signal);
}
double                   GeneticNet::get_input(unsigned int animal, unsigned int input)
{
    if(animal > _animals-1)
    {
        addError(Error("get_input(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(input)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return 0;
        //error_general("input(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal]->get_input(input);
}
void                    GeneticNet::set_input(unsigned int animal, std::vector<double> inputList)
{
    if(animal > _animals-1)
    {
        addError(Error("set_input(unsigned int ["+std::to_string(animal)+"] , std::vector<double> inputList )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return;
        //error_general("input(unsigned int ["+std::to_string(animal)+"] , std::vector<double> )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    _needsCalculationUpdate = true;
    _netList[animal]->set_input(inputList);
}
std::vector<double>      GeneticNet::get_input(unsigned int animal)
{
    if(animal > _animals-1)
    {
        addError(Error("get_input(unsigned int ["+std::to_string(animal)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return std::vector<double>();
        //error_general("input(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal]->get_input();
}
void                    GeneticNet::set_input(std::vector<std::vector<double>    > input)
{
    if(unsigned(input.size()) != _animals)
    {
        addError(Error("set_input( std::vector<std::vector<double>    > input )",
                       std::string("input.size() != _animals : ["+std::to_string(input.size())+"] != ["+std::to_string(_animals)+"]")));
        return;
        //error_general("input(std::vector<std::vector<double> >)","std::vector<>.size() == "+std::to_string(input.size()) +" "+ error_paramOutOfRange((unsigned int)0,input.size(),_animals,_animals));
    }
    _needsCalculationUpdate = true;
    for(unsigned int a=0; a<_animals; a++)
    {
        _netList[a]->set_input(input[a]);
    }
}
std::vector<std::vector<double>  >GeneticNet::get_input()
{
    std::vector<std::vector<double>  > inp;
    inp.reserve(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        inp.push_back(_netList[a]->get_input());
    }
    return inp;
}

double                   GeneticNet::get_hidden(unsigned int animal, unsigned int hiddenX, unsigned int hiddenY)
{
    if(animal > _animals-1)
    {
        addError(Error("get_hidden(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"])",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return 0;
        //error_general("hidden(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal]->get_hidden(hiddenX,hiddenY);
}
std::vector<double>      GeneticNet::get_hidden(unsigned int hiddenX, unsigned int hiddenY)
{
    std::vector<double> ret;
    ret.reserve(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        ret.push_back(_netList[a]->get_hidden(hiddenX,hiddenY));
    }
    return ret;
}
//---------------------------------------------
std::vector<double>      GeneticNet::get_hiddenX(unsigned int animal, unsigned int hiddenX) // |    Alle in einer Spalte
{
    if(animal > _animals-1)
    {
        addError(Error("get_hiddenX(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return std::vector<double>();
        //error_general("hiddenX(unsigned int ["+std::to_string(animal)+"] , unsinged int ["+std::to_string(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal]->get_hiddenX(hiddenX);
}
std::vector<std::vector<double>  >GeneticNet::get_hiddenX(unsigned int hiddenX)
{
    //this->run();
    if(!this->hasHiddenLayer())
    {
        addError(Error("get_hiddenX(unsigned int ["+std::to_string(hiddenX)+"] )",
                       std::string("The net doesen't have a hidden layer")));
        return std::vector<std::vector<double>  >();
    }
    std::vector<std::vector<double>  > hidX;
    hidX.reserve(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        hidX.push_back(_netList[a]->get_hiddenX(hiddenX));
    }
    return hidX;
}
std::vector<double>      GeneticNet::get_hiddenY(unsigned int animal, unsigned int hiddenY)// --   Alle in einer Reihe
{
    if(animal > _animals-1)
    {
        addError(Error("get_hiddenY(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return std::vector<double>();
        //error_general("hiddenY(unsigned int ["+std::to_string(animal)+"] , unsinged int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal]->get_hiddenY(hiddenY);
}
std::vector<std::vector<double>  >GeneticNet::get_hiddenY(unsigned int hiddenY)
{
    //this->run();
    if(!this->hasHiddenLayer())
    {
        addError(Error("get_hiddenY(unsigned int ["+std::to_string(hiddenY)+"] )",
                       std::string("The net doesen't have a hidden layer")));
        return std::vector<std::vector<double>  >();
    }
    std::vector<std::vector<double>  > hidY;
    hidY.reserve(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        hidY.push_back(_netList[a]->get_hiddenY(hiddenY));
    }
    return hidY;
}

Neuron                 *GeneticNet::get_ptr_hiddenNeuron(unsigned int animal, unsigned int hiddenX, unsigned int hiddenY)
{
    if(animal > _animals-1)
    {
        addError(Error("get_ptr_hiddenNeuron(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] ,unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return nullptr;
        //error_general("hiddenNeuron(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] , unsinged int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    return _netList[animal]->get_ptr_hiddenNeuron(hiddenX,hiddenY);
}
std::vector<Neuron*>    GeneticNet::get_hiddenNeuronX_ptr(unsigned int animal, unsigned int hiddenX)// |    Alle in einer Spalte
{
    if(animal > _animals-1)
    {
        addError(Error("get_hiddenNeuronX_ptr(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronX(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    return _netList[animal]->get_hiddenNeuronX_ptr(hiddenX);
}
std::vector<Neuron*>    GeneticNet::get_hiddenNeuronY_ptr(unsigned int animal, unsigned int hiddenY)// --   Alle in einer Reihe
{
    if(animal > _animals-1)
    {
        addError(Error("get_hiddenNeuronY_ptr(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return std::vector<Neuron*>();
       // error_general("get_hiddenNeuronY_ptr(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    return _netList[animal]->get_hiddenNeuronY_ptr(hiddenY);
}
std::vector<std::vector<Neuron*> > GeneticNet::get_hiddenNeuron_ptr(unsigned int animal)
{
    if(animal > _animals-1)
    {
        addError(Error("get_hiddenNeuron_ptr(unsigned int ["+std::to_string(animal)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return std::vector<std::vector<Neuron*> >();
        //error_general("get_hiddenNeuron_ptr(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    return _netList[animal]->get_hiddenNeuron_ptr();
}
Neuron                 *GeneticNet::get_ptr_outputNeuron(unsigned int animal, unsigned int output)
{
    if(animal > _animals-1)
    {
        addError(Error("get_ptr_outputNeuron(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(output)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return nullptr;
       // error_general("outputNeuron(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    //this->run();
    return _netList[animal]->get_ptr_outputNeuron(output);
}
std::vector<Neuron*>   *GeneticNet::get_ptr_outputNeuron(unsigned int animal)
{
    if(animal > _animals-1)
    {
        addError(Error("get_ptr_outputNeuron(unsigned int ["+std::to_string(animal)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return nullptr;
        //error_general("outputNeuron(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal]->get_ptr_outputNeuron();
}


double                   GeneticNet::get_output(unsigned int animal, unsigned int output)
{
    if(animal > _animals-1)
    {
        addError(Error("get_output(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(output)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return 0;
        //error_general("output(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal]->get_output(output);
}
std::vector<double>      GeneticNet::get_output(unsigned int animal)
{
    if(animal > _animals-1)
    {
        addError(Error("get_output(unsigned int ["+std::to_string(animal)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return std::vector<double>();
        //error_general("output(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal]->get_output();
}
std::vector<std::vector<double>  >GeneticNet::get_output()
{
    std::vector<std::vector<double>  > out;
    out.reserve(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        out.push_back(_netList[a]->get_output());
    }
    return out;
}


void                    GeneticNet::run()
{
    if(_needsCalculationUpdate)
    {
        double timeout = 1; //1 sec
       // CONSOLE<< "threads start";
        std::chrono::high_resolution_clock::time_point timeoutTimerEnd;
        std::chrono::duration<double> time_span;
        std::chrono::high_resolution_clock::time_point timeoutTimerStart = std::chrono::high_resolution_clock::now();
#ifdef _DEBUG_NET_TIMING
        __debug_timer1_start = std::chrono::high_resolution_clock::now();
#endif
#ifdef __enableGeneticNetThread
        pthread_mutex_lock(&_threadLock);
      //  _threadDelayMicros = 1000000;
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
        std::vector<bool> restartCheckList;
        restartCheckList.reserve(_animals);
        for(unsigned int a=0; a<_animals; a++)
        {
            restartCheckList.push_back(false);
        }

        do{
            pause = 0;
            for(unsigned int a=0; a<_animals; a++)
            {
                timeoutTimerEnd = std::chrono::high_resolution_clock::now();
                time_span = std::chrono::duration_cast<std::chrono::microseconds>(timeoutTimerEnd - timeoutTimerStart);
                if(time_span.count() >= timeout)
                {
                    timeoutTimerStart = timeoutTimerEnd;
                    CONSOLE<< "GeneticNet::run() thread timeout: "<<timeout;
                    CONSOLE<< "Threads running: "<<_animals-pause;
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

       /* t2 = std::chrono::high_resolution_clock::now();
        time_span = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
        _timeInterval = 0.9*_timeInterval + 0.1*time_span.count();*/
#ifdef _DEBUG_NET_TIMING
        __debug_timer1_end = std::chrono::high_resolution_clock::now();
        __debug_time_span = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer1_end - __debug_timer1_start);
        __debug_run_time = __debug_run_time*0.5 + 0.5*__debug_time_span.count();    //Filtered time
#endif
#ifdef __DEBUG_TIMEINTERVAL
        if(_debugCount > 1000)
        {
            CONSOLE<< "n: "<<_timeInterval;
            _debugCount = 0;
        }
        _debugCount++;
#endif
        //CONSOLE<< "threads ended";
        _needsCalculationUpdate = false;
    }
}
void                    GeneticNet::run(unsigned int animal)
{
    if(animal > _animals-1)
    {
        addError(Error("run(unsigned int ["+std::to_string(animal)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return;
        //error_general("run(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    _netList[animal]->run();
}


Net                    *GeneticNet::operator[](unsigned int animal)
{
    if(animal > _animals-1)
    {
        addError(Error("operator[](unsigned int ["+std::to_string(animal)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return nullptr;
        //error_general("operator[](unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal];
}

std::vector<Net*>       GeneticNet::get_netList_ptr()
{
    return _netList;
}
std::vector<Net*>   *GeneticNet::get_ptr_netList_ptr()
{
    return &_netList;
}

void                    GeneticNet::set_mutationFactor(double factor)
{
    if(factor <= __FLOATINGPOINT_TOLERANCE)
    {
        addError(Error("set_mutationFactor(double ["+std::to_string(factor)+"] )",
                       ErrorMessage::outOfRange(']',double(__FLOATINGPOINT_TOLERANCE),factor,double(NAN),'[')));
        //error_general("mutationFactor(double ["+std::to_string(factor)+"] )","mutationFactor must be greater than 0");
    }
    _mutationFactor = factor;
}
void                    GeneticNet::set_mutationChangeWeight(double weight)
{
    if(weight <= __FLOATINGPOINT_TOLERANCE)
    {
        addError(Error("set_mutationChangeWeight(double ["+std::to_string(weight)+"] )",
                       ErrorMessage::outOfRange(']',double(__FLOATINGPOINT_TOLERANCE),weight,double(NAN),'[')));
        //error_general("mutationChangeWeight(double ["+std::to_string(weight)+"] )","mutationChangeWeight must be greater than 0");
    }
    _mutationChangeWeight = weight;
}

void                    GeneticNet::set_score(unsigned int animal, double score)
{
    if(animal > _animals-1)
    {
        addError(Error("set_score(unsigned int ["+std::to_string(animal)+"] , double ["+std::to_string(score)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return;
        //error_general("score(unsigned int ["+std::to_string(animal)+"] , double ["+std::to_string(score)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    if(score < 1)
    {
        score = 1;
    }
    _scoreList[animal] = score;
}
double                   GeneticNet::get_score(unsigned int animal)
{
    if(animal > _animals-1)
    {
        addError(Error("get_score(unsigned int ["+std::to_string(animal)+"] )",
                       ErrorMessage::outOfRange('[',unsigned(0),animal,_animals-1,']')));
        return 0;
        //error_general("score(unsigned int ["+std::to_string(animal)+"])",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _scoreList[animal];
}
void                    GeneticNet::set_score(std::vector<double>   scoreList)
{
    if(scoreList.size() != _scoreList.size())
    {
        addError(Error("set_score(std::vector<double>   scoreList )",
                       std::string("scoreList.size() != _scoreList.size() : "+std::to_string(scoreList.size())+" != "+std::to_string(_scoreList.size()))));
        return;
        //CONSOLE<< "scoreList.size() "<<scoreList.size();
        //error_general("score(std::vector<double>)","std::vector<double>.size() == "+std::to_string(scoreList.size())+"  "+error_paramOutOfRange(0,scoreList.size(),_animals,_animals));
    }
    for(unsigned int a=0; a<_animals; a++)
    {
        set_score(a,scoreList[a]);
    }
}
std::vector<double>      GeneticNet::get_score()
{
    return _scoreList;
}

void                    GeneticNet::learn(std::vector<double>   scoreList)
{
    this->set_score(scoreList);
    this->learn();
}
void                    GeneticNet::learn()
{
    double gesScore      = 0;
    double scoreOffset   = 0;
    unsigned int selection1;
    unsigned int selection2;
    //std::vector<std::vector<double>  >   newGenom(_animals,std::vector<double>(this->get_genomsize(),0));
    std::vector<std::vector<double>  >   newGenom;
    newGenom.reserve(_animals);



    for(unsigned int a=0; a<_animals; a++)
    {
        newGenom.push_back(std::vector<double>());
        newGenom[newGenom.size()-1].reserve(this->get_genomsize());
        for(unsigned int genPos=0; genPos<this->get_genomsize(); genPos++)
        {
            newGenom[newGenom.size()-1].push_back(0);
        }
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
    this->set_genom(newGenom);
}

void                    GeneticNet::updateNetConfiguration()
{
//#ifdef __enableGeneticNetThread

       //int rc;
    bool isEqual = true;
    for(unsigned int net=1; net<_animals; net++)
    {
        isEqual &= _netList[net]->isEqual(_netList[net-1]);
    }
    if(!isEqual)
    {
        addError(Error("updateNetConfiguration()",
                       std::string("The nets aren't equal! Check "+_saveNet.get_filename()+"."+_saveNet.get_fileEnding())));
        return;
    }

#ifdef _DEBUG_NET_TIMING
    __debug_timer2_start = std::chrono::high_resolution_clock::now();
#endif
    for(unsigned int a=0; a<_animals; a++)
    {
        _netList[a]->updateNetConfiguration();
#ifdef _DEBUG_NET_TIMING
        __debug_timer2_end = std::chrono::high_resolution_clock::now();
        __debug_time_span = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer2_end - __debug_timer2_start);
        if(__debug_time_span.count() >= 1.0)//1sec
        {
            CONSOLE<< "GeneticNet::updateNetConfiguration()... Nets done: "<<a+1<<"\telapsed time: "<<__debug_time_span.count()<<"sec\tremaining:"<<__debug_time_span.count()*double(_animals)/double(a)-__debug_time_span.count()<<"sec";
        }
#endif
        /*rc = pthread_create(&_threadList_setupNet[a], nullptr, runThread_setupNet, static_cast<void *>(&_threadData_setupNet[a]));
        if (rc)
        {
            CONSOLE<< "Error:unable to create thread," << rc << endl;
        }*/
    }
/*    for(unsigned int a=0; a<_animals; a++)
    {
        pthread_join(_threadList_setupNet[a], nullptr);
    }*/
    CONSOLE<< "setup nets done";

//#endif
}
void                    GeneticNet::addConnection(NeuronID fromNeuron,NeuronID toNeuron,ConnectionDirection direction)
{
    for(unsigned int a=0; a<_animals; a++)
    {
        _netList[a]->addConnection(fromNeuron,toNeuron,direction);
    }
}
void                    GeneticNet::addConnection(Connection connection)
{
    for(unsigned int a=0; a<_animals; a++)
    {
        _netList[a]->addConnection(connection);
    }
}
void                    GeneticNet::addConnection(std::vector<Connection> connections)
{
    for(unsigned int a=0; a<_animals; a++)
    {
        _netList[a]->addConnection(connections);
    }
}
void                    GeneticNet::set_connectionList(std::vector<std::vector<Connection> >connections)
{
    if(unsigned(connections.size()) != _animals)
    {
        addError(Error("set_connectionList(std::vector<std::vector<Connection> >connections )",
                       std::string("connections->size() != _animals : "+std::to_string(connections.size())+" != "+std::to_string(_animals))));
        return;
        //error_general("connectionList(std::vector<std::vector<Connection> >connections)","size of connections: "+std::to_string(connections->size())+" ist not the same as _animals: "+std::to_string(_animals));
    }
    for(unsigned int a=0; a<_animals; a++)
    {
        _netList[a]->set_connectionList(connections[a]);
    }
}

NeuronID                GeneticNet::addNeuron()
{
    NeuronID ID;
    for(unsigned int a=0; a<_netList.size(); a++)
    {
        ID = _netList[a]->addNeuron();
    }
    return ID;
}
NeuronID                GeneticNet::addNeuron(Neuron *neuron)
{
    NeuronID ID;
    for(unsigned int a=0; a<_netList.size(); a++)
    {
        ID = _netList[a]->addNeuron(neuron);
    }
    return ID;
}
/*double                  GeneticNet::get_cycleTime()
{
    return _timeInterval;
}*/
std::string                 GeneticNet::toString()
{
    return _netList[0]->toString();
}
std::vector<std::string>             GeneticNet::toStringList()
{
    return _netList[0]->toStringList();
}

SaveNet                *GeneticNet::get_ptr_saveNet()
{
    return &_saveNet;
}

#if defined(_DEBUG_NET_TIMING)
double                  GeneticNet::get_runtime()
{
    return __debug_run_time;
}
#endif
void                    GeneticNet::clearErrors()
{
    _errorList.clear();
    for(unsigned int a=0; a<_netList.size(); a++)
    {
        _netList[a]->clearErrors();
    }
}
Error                   GeneticNet::get_lastError() const
{
    if(static_cast<unsigned int>(_errorList.size()) == 0)
    {
        return Error("get_lastError()",ErrorMessage::listIsEmpty("_errorList"));
    }
    return _errorList[_errorList.size()-1];
}
Error                   GeneticNet::get_error(unsigned int index)
{
    unsigned int allErrors = get_errorAmount();
    if(allErrors == 0)
    {
        return Error("get_error(unsigned int ["+std::to_string(index)+"] )",
                     ErrorMessage::listIsEmpty("_errorList"));
    }
    if(index >= static_cast<unsigned int>(allErrors))
    {
        return Error("get_error(unsigned int ["+std::to_string(index)+"] )",
                     ErrorMessage::outOfRange<unsigned int>('[',0,index,static_cast<unsigned int>(_errorList.size()-1),']'));
    }
    return this->get_errorList()[static_cast<int>(index)];
}
ErrorList               GeneticNet::get_errorList() const
{
    ErrorList list;
    list.reserve(signed(get_errorAmount()));
    list.insert(list.end(),_errorList.begin(),_errorList.end());
    for(unsigned int a=0; a<this->_netList.size(); a++)
    {
        ErrorList toInsert = _netList[a]->get_errorList();
        list.insert(list.end(),toInsert.begin(),toInsert.end());
    }

    return list;
}
unsigned int            GeneticNet::get_errorAmount() const
{
    unsigned int neuronErrors = 0;
    for(unsigned int a=0; a<_netList.size(); a++)
    {
        neuronErrors += _netList[a]->get_errorAmount();
    }
    return static_cast<unsigned int>(_errorList.size())+neuronErrors;
}
#ifdef QT_APP
void GeneticNet::onNetError(unsigned int netID,Error &e)
{
    e.setNamespace("GeneticNet : NET["+std::to_string(netID)+"]  ::  "+e.getNamespace());
    emit errorOccured(e);
}
void GeneticNet::onNetConfigurationUpdateNeeded(Net *net)
{
    emit netConfigurationUpdateNeeded(net);
}
void GeneticNet::onNetConfigurationUpdateStarted(Net *net)
{
    emit netConfigurationUpdateStarted(net);
}
void GeneticNet::onNetConfigurationUpdated(Net *net)
{
    emit netConfigurationUpdated(net);
}
void GeneticNet::onAccessLock(Net *net)
{
    emit accessLock(net);
}
void GeneticNet::onAccessUnlock(Net *net)
{
    emit accessUnlock(net);
}
void GeneticNet::onRunDone(Net *net)
{
    emit runDone(net);
}
void GeneticNet::onBiasValueChanged(Net *net)
{
    emit biasValueChanged(net);
}
void GeneticNet::onWeightValuesChanged(Net *net)
{
    emit weightValuesChanged(net);
}
#endif
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
    _scoreList = std::vector<double>();
    this->set_mutationChangeWeight(0.01);
    this->set_mutationFactor(0.01);
    this->set_netFileName("netFile");
    this->set_netFileEnding("gnet");
    this->set(animals,inputs,hiddenX,hiddenY,outputs,enableBias,enableAverage,func);
}
#ifdef QT_APP
void GeneticNet::connectSignalsFromNet(Net *net)
{
    QObject::connect(net,&Net::errorOccured,                    this,&GeneticNet::onNetError);
    QObject::connect(net,&Net::netConfigurationUpdateNeeded,    this,&GeneticNet::onNetConfigurationUpdateNeeded);
    QObject::connect(net,&Net::netConfigurationUpdateStarted,   this,&GeneticNet::onNetConfigurationUpdateStarted);
    QObject::connect(net,&Net::netConfigurationUpdated,         this,&GeneticNet::onNetConfigurationUpdated);
    QObject::connect(net,&Net::accessLock,                      this,&GeneticNet::onAccessLock);
    QObject::connect(net,&Net::accessUnlock,                    this,&GeneticNet::onAccessUnlock);
    QObject::connect(net,&Net::runDone,                         this,&GeneticNet::onRunDone);
    QObject::connect(net,&Net::biasValueChanged,                this,&GeneticNet::onBiasValueChanged);
    QObject::connect(net,&Net::weightValuesChanged,             this,&GeneticNet::onWeightValuesChanged);
}
#endif

void                    GeneticNet::learn_selectAnimal(double gesScore,unsigned int &selection1,unsigned int &selection2)
{
    double random;
    double beginScore;

    random = (_randEngine() % unsigned((gesScore * 100))/100);
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
        random = (_randEngine() % unsigned(((gesScore - _scoreList[selection1]) * 100))/100);
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
                CONSOLE<< "warning: learn_selectAnimal(...) counter > 100";
            }
        }
        counter++;

    }while(selection1 == selection2);
}
void                    GeneticNet::learn_crossover(unsigned int selection1,unsigned int selection2,std::vector<double> &newGen1,std::vector<double> &newGen2)
{
    //_netList[selection1]->ptr_genom(),_netList[selection2]->ptr_genom()
    unsigned int random =  1 + _randEngine() % (get_genomsize() - 2);

    for(unsigned int a=0; a<random; a++)
    {
        newGen1[a] = *(*_netList[selection2]->get_ptr_genom())[a];
        newGen2[a] = *(*_netList[selection1]->get_ptr_genom())[a];
    }
    for(unsigned int a=random; a<get_genomsize(); a++)
    {
        newGen1[a] = *(*_netList[selection1]->get_ptr_genom())[a];
        newGen2[a] = *(*_netList[selection2]->get_ptr_genom())[a];
    }
}
void                    GeneticNet::learn_mutate(std::vector<double> &genom)
{
    if(_mutationFactor <= __FLOATINGPOINT_TOLERANCE || _mutationChangeWeight <= __FLOATINGPOINT_TOLERANCE)
        return;
    for(unsigned int a=0; a<get_genomsize(); a++)
    {
        if(_randEngine() % unsigned(1/_mutationFactor) == 0)
        {
            unsigned int random = _randEngine();
            double ran = (random % 2000);
            ran = ran-1000;
            ran = ran/1000;
            genom[a] += _mutationChangeWeight * ran;
        }
    }
}

void                   *GeneticNet::runThread(void *threadarg)
{
    //pthread_detach(pthread_self());
    struct thread_data_geneticNet *my_data;
    bool ret = false;
    bool enableLoop = false;
    //bool lastPauseState = false;
    my_data = static_cast<struct thread_data_geneticNet *>(threadarg);
    pthread_mutex_lock(my_data->lock);
    my_data->isPaused = false;
    //time.tv_nsec = *my_data->delayMicros;
    pthread_mutex_unlock(my_data->lock);
 //   CONSOLE<< "thread start: "<<my_data->thread_id << " "<<my_data->net;
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
                    CONSOLE<< "ERROR  thread "<< my_data->thread_id << " : " << e.what();
                    FILE *file = fopen("error_n.txt","a");
                    if(file)
                    {
                        fprintf(file,"%s\n",("ERROR  net "+std::to_string(my_data->thread_id)+std::string(e.what())).c_str());
                        fclose(file);
                    }
                }catch(...)
            {
                pthread_mutex_lock(my_data->lock);
                my_data->debugParam = 21; //error 2
                pthread_mutex_unlock(my_data->lock);
                CONSOLE<< "ERROR  thread "<< my_data->thread_id << " : unknown";
                FILE *file = fopen("error_n.txt","a");
                if(file)
                {
                    fprintf(file,"%s\n",("ERROR  net "+std::to_string(my_data->thread_id)).c_str());
                    fclose(file);
                }
            }
            enableLoop = false;
            pthread_mutex_lock(my_data->lock);
            my_data->debugParam = 5; //finish
            pthread_mutex_unlock(my_data->lock);
        }else {
            //CONSOLE<< "thread: "<<my_data->thread_id << " sleeping";
            //usleep(1);

            //nanosleep(&time, NULL);
#ifdef __DEBUG_TIMEINTERVAL_IN_THREAD
            t2 = std::chrono::high_resolution_clock::now();
            time_span = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
            _timeInterval = 0.9*_timeInterval + 0.1*time_span.count();
            if(_debugCount > 1000)
            {
                if(my_data->thread_id == 0)
                    CONSOLE<< "n_: "<<my_data->thread_id << " " <<_timeInterval;
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
                CONSOLE<< "thread: "<<my_data->thread_id << " pause ON";
            }else {
                CONSOLE<< "thread: "<<my_data->thread_id << " pause OFF";
            }
        }
        lastPauseState = pause;*/
    }
  //  CONSOLE<< "thread stop: "<<my_data->thread_id << " "<<my_data->net;
    pthread_exit(nullptr);
}
void                   *GeneticNet::runThread_setupNet(void *threadarg)
{
    //pthread_detach(pthread_self());
    struct thread_data_geneticNet *my_data;
    my_data = static_cast<struct thread_data_geneticNet *>(threadarg);

 //   my_data->net = new Net(my_data->thread_id);
 //   CONSOLE<< "threadSetup begin: "<<my_data->thread_id;
    //pthread_mutex_lock(my_data->lock);
    my_data->net->updateNetConfiguration();
    //pthread_mutex_unlock(my_data->lock);
    //my_data->net->updateNetConfiguration();
 //   CONSOLE<< "threadSetup done: "<<my_data->thread_id;
    pthread_exit(nullptr);
}
//----------ERROR
void                    GeneticNet::addError(const Error &e)
{
    _errorList.push_back(e);
    _errorList[_errorList.size()-1].setNamespace("GeneticNet::"+_errorList[_errorList.size()-1].getNamespace());
    CONSOLE << "Error: "<<_errorList[_errorList.size()-1].toString().c_str();
#ifdef QT_APP
    emit errorOccured(_errorList[_errorList.size()-1]);
#endif
}
/*
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
std::string GeneticNet::error_paramOutOfRange(unsigned int paramPos,double value,double min, double max)
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
*/
