#include "net.h"

#ifdef QT_APP
Net::Net(unsigned int ID,
         QObject *parent)
    : QObject (parent)
#else
Net::Net(unsigned int ID)
#endif
{
    this->set_ID(ID);
    init(2,1,2,1,true,false,Activation::Sigmoid);
}
#ifdef QT_APP
Net::Net(unsigned int ID,
         unsigned int inputs,
         unsigned int hiddenX,
         unsigned int hiddenY,
         unsigned int outputs,
         QObject *parent)
    : QObject (parent)
#else
Net::Net(unsigned int ID,
         unsigned int inputs,
         unsigned int hiddenX,
         unsigned int hiddenY,
         unsigned int outputs)
#endif
{
    this->set_ID(ID);
    init(inputs,hiddenX,hiddenY,outputs,true,false,Activation::Sigmoid);
}

#ifdef QT_APP
Net::Net(unsigned int ID,
         unsigned int inputs,
         unsigned int hiddenX,
         unsigned int hiddenY,
         unsigned int outputs,
         bool enableBias,
         bool enableAverage,
         Activation func,
         QObject *parent)
    : QObject (parent)
#else
Net::Net(unsigned int ID,
         unsigned int inputs,
         unsigned int hiddenX,
         unsigned int hiddenY,
         unsigned int outputs,
         bool enableBias,
         bool enableAverage,
         Activation func)
#endif
{
    this->set_ID(ID);
    init(inputs,hiddenX,hiddenY,outputs,enableBias,enableAverage,func);
}

Net::~Net()
{
    for(unsigned int a=0; a<_allNeuronList.size(); a++)
    {
        try {
            delete _allNeuronList[a];
        } catch (std::exception &e) {
            //error_general("~Net()",e.what());
            CONSOLE<< "~Net() error: " << e.what();
        }
    }
    /*for(int a=_allNeuronList.size()-1; a>=0; a--)
    {
        try {
            delete _allNeuronList[a];
        } catch (std::exception &e) {
            //error_general("~Net()",e.what());
            CONSOLE<< "~Net() error: " << e.what();
        }
    }*/
    _inputSignalList.clear();
    _hiddenNeuronList.clear();
    _outputNeuronList.clear();
    _costumNeuronList.clear();
    _allNeuronList.clear();
    _ptr_genom.clear();
}
void                Net::set_ID(unsigned int ID)
{
    _ID = ID;
}
unsigned int        Net::get_ID()
{
    return _ID;
}

void                Net::set_inputNeurons(unsigned int inputs)
{
#if NET_MIN_INPUTNEURONS == 0
    if(inputs > NET_MAX_INPUTNEURONS)
#else
    if(inputs < NET_MIN_INPUTNEURONS || inputs > NET_MAX_INPUTNEURONS)
#endif
    {
        this->addError(Error("set_inputNeurons(unsigned int ["+std::to_string(inputs)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(NET_MIN_INPUTNEURONS),inputs,static_cast<unsigned int>(NET_MAX_INPUTNEURONS),']')));
        return;
        //error_general("inputNeurons(unsigned int ["+std::to_string(inputs)+"] )","Parameter 0 is out of range. Min: "+ std::to_string(NET_MIN_INPUTNEURONS)+" Max: "+std::to_string(NET_MAX_INPUTNEURONS));
    }
    if(inputs != _inputs)
    {
        #ifdef QT_APP
        emit accessLock(this);
        #endif
        _needsConfigurationUpdate   = true;
        _doRegenerateConnectionlist = true;
        #ifdef QT_APP
        emit netConfigurationUpdateNeeded(this);
        #endif
        _inputs                     = inputs;
    }
}
unsigned int        Net::get_inputNeurons()
{
    return _inputs;
}
void                Net::set_hiddenNeuronsX(unsigned int hiddenX)
{
#if NET_MIN_HIDDENNEURONS_X == 0
    if(hiddenX > NET_MAX_HIDDENNEURONS_X)
#else
    if(hiddenX < NET_MIN_HIDDENNEURONS_X || hiddenX > NET_MAX_HIDDENNEURONS_X)
#endif
    {
        this->addError(Error("set_hiddenNeuronsX(unsigned int ["+std::to_string(hiddenX)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(NET_MIN_HIDDENNEURONS_X),hiddenX,static_cast<unsigned int>(NET_MAX_HIDDENNEURONS_X),']')));
        return;
        //error_general("hiddenNeuronsX(unsigned int ["+std::to_string(hiddenX)+"] )","Parameter 0 is out of range. Min: "+ std::to_string(NET_MIN_HIDDENNEURONS_X)+" Max: "+std::to_string(NET_MAX_HIDDENNEURONS_X));
    }
    if(hiddenX != _hiddenX)
    {
        #ifdef QT_APP
        emit accessLock(this);
        #endif
        _needsConfigurationUpdate   = true;
        _doRegenerateConnectionlist = true;
        _hiddenX                    = hiddenX;
        #ifdef QT_APP
        emit netConfigurationUpdateNeeded(this);
        #endif
        //_hiddenNeurons              = _hiddenX * _hiddenY;
    }
}
unsigned int        Net::get_hiddenNeuronsX()
{
    return _hiddenX;
}
void                Net::set_hiddenNeuronsY(unsigned int hiddenY)
{
#if NET_MIN_HIDDENNEURONS_Y == 0
    if(hiddenY > NET_MAX_HIDDENNEURONS_Y)
#else
    if(hiddenY < NET_MIN_HIDDENNEURONS_Y || hiddenY > NET_MAX_HIDDENNEURONS_Y)
#endif
   {
        this->addError(Error("set_hiddenNeuronsY(unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(NET_MIN_HIDDENNEURONS_Y),hiddenY,static_cast<unsigned int>(NET_MAX_HIDDENNEURONS_Y),']')));
        return;
        //error_general("hiddenNeuronsY(unsigned int ["+std::to_string(hiddenY)+"] )","Parameter 0 is out of range. Min: " + std::to_string(NET_MIN_HIDDENNEURONS_Y)+" Max: "+std::to_string(NET_MAX_HIDDENNEURONS_Y));
    }
    if(hiddenY != _hiddenY)
    {
        #ifdef QT_APP
        emit accessLock(this);
        #endif
        _needsConfigurationUpdate   = true;
        _doRegenerateConnectionlist = true;
        _hiddenY                    = hiddenY;
        #ifdef QT_APP
        emit netConfigurationUpdateNeeded(this);
        #endif
        //_hiddenNeurons              = _hiddenX * _hiddenY;
    }
}
unsigned int        Net::get_hiddenNeuronsY()
{
    return _hiddenY;
}
unsigned int        Net::get_hiddenNeurons()
{
    return _hiddenNeurons;
}

void                Net::set_outputNeurons(unsigned int outputs)
{
#if NET_MIN_OUTPUTNEURONS == 0
    if(outputs > NET_MAX_OUTPUTNEURONS)
#else
    if(outputs < NET_MIN_OUTPUTNEURONS || outputs > NET_MAX_OUTPUTNEURONS)
#endif
    {
        this->addError(Error("set_outputNeurons(unsigned int ["+std::to_string(outputs)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(NET_MIN_OUTPUTNEURONS),outputs,static_cast<unsigned int>(NET_MAX_OUTPUTNEURONS),']')));
        return;
        //error_general("outputNeurons(unsigned int ["+std::to_string(outputs)+"] )","Parameter 0 is out of range. Min: "+std::to_string(NET_MIN_OUTPUTNEURONS)+" Max: "+std::to_string(NET_MAX_OUTPUTNEURONS));
    }
    if(outputs != _outputs)
    {
        #ifdef QT_APP
        emit accessLock(this);
        #endif
        _needsConfigurationUpdate   = true;
        _doRegenerateConnectionlist = true;
        _outputs                    = outputs;
        #ifdef QT_APP
        emit netConfigurationUpdateNeeded(this);
        #endif
        //_outputNeurons          = outputs;
    }
}
unsigned int        Net::get_outputNeurons()
{
    return _outputs;
}
/*void                Net::set_costumNeurons(unsigned int costum)
{
    _costumNeurons = costum;
}*/
unsigned int        Net::get_costumNeurons()
{
    return _costumNeurons;
}
unsigned int        Net::get_connections()
{
    return _connections;
}
unsigned int        Net::get_costumConnections()
{
    return _costumConnections;
}
/*void                Net::set_costumConnections(unsigned int connections)
{
    _costumConnections = connections;
}*/
/*void                Net::set_neurons(unsigned int neurons,unsigned int hiddenNeurons,unsigned int outputNeurons,unsigned int costumNeurons)
{
    _neurons        = neurons;
    _hiddenNeurons  = hiddenNeurons;
    _outputNeurons  = outputNeurons;
    _costumNeurons  = costumNeurons;
    if(_hiddenNeurons+_outputNeurons+_costumNeurons != _neurons)
    {
        this->addError(Error("set_neurons(unsigned int ["+std::to_string(neurons)+
                             "],unsigned int ["+std::to_string(hiddenNeurons)+
                             "],unsigned int ["+std::to_string(outputNeurons)+
                             "],unsigned int ["+std::to_string(costumNeurons)+"])",
                         std::string("Its not possible like so. Parameter 1 shuld be the sum of the others") ));
        return;
        //error_general("neurons(unsigned int ["+std::to_string(neurons)+"],unsigned int ["+std::to_string(hiddenNeurons)+"],unsigned int ["+std::to_string(outputNeurons)+"],unsigned int ["+std::to_string(costumNeurons)+"],","Its not possible like so. Parameter 1 shuld be the sum of the others");
    }
}*/
unsigned int        Net::get_neurons()
{
    return _neurons;
}
void                Net::set_bias(bool enableBias)
{
    if(enableBias != _bias)
    {
        #ifdef QT_APP
        emit accessLock(this);
        #endif
        _needsConfigurationUpdate   = true;
        _doRegenerateConnectionlist = true;
        _bias                       = enableBias;
        #ifdef QT_APP
        emit netConfigurationUpdateNeeded(this);
        #endif
    }
}
bool                Net::get_bias()
{
    return _bias;
}
void                Net::set_enableAverage(bool enableAverage)
{
    if(enableAverage != _enableAverage)
    {
        #ifdef QT_APP
        emit accessLock(this);
        #endif
        _needsConfigurationUpdate   = true;
        _enableAverage              = enableAverage;
        #ifdef QT_APP
        emit netConfigurationUpdateNeeded(this);
        #endif
    }
}
bool                Net::get_enableAverage()
{
    return _enableAverage;
}
void                Net::set_biasValue(double value)
{
    if(_biasValue != value)
    {
        _needsCalculationUpdate         = true;
        _biasValue                      = value;
        #ifdef QT_APP
        emit biasValueChanged(this);
        #endif
    }
}
double              Net::get_biasValue()
{
    return _biasValue;
}
void                Net::set_activationFunction(Activation func)
{
    if(func != _activationFunction)
    {
        #ifdef QT_APP
        emit accessLock(this);
        #endif
        _needsConfigurationUpdate   = true;
        _activationFunction         = func;
        #ifdef QT_APP
        emit netConfigurationUpdateNeeded(this);
        #endif
    }
}
Activation          Net::get_activationFunction()
{
    return _activationFunction;
}
bool                Net::hasHiddenLayer()
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("hasHiddenLayer()",
                       ErrorMessage::updateNetFirst()));
        return false;
    }
    return _hasHiddenLayer;
}

void                Net::set_randomGenom()
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("set_randomGenom()",
                       ErrorMessage::updateNetFirst()));
        return;
    }
    for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
    {
        _allNeuronList[neuron]->set_randWeight();
    }
}
void                Net::set_genom(std::vector<double> genom)
{
    if(_needsConfigurationUpdate)
    {
        //error_general("genom(std::vector<double>) ",ErrorMessage::updateNetFirst());
        this->addError(Error("set_genom(std::vector<double> genom)",
                       ErrorMessage::updateNetFirst()));
        return;
    }
    if(static_cast<unsigned int>(genom.size()) != _connections)
    {
        this->addError(Error("set_genom(std::vector<double> genom)",{
                       "parameter 0 has the wrong array size: "+std::to_string(genom.size()),
                       " array size should by "+std::to_string(_ptr_genom.size())}));
        return;
        //error_general("genom(std::vector<double>)","parameter 0 has the wrong array size: "+std::to_string(genom.size())+" array size should by "+std::to_string(_ptr_genom.size()));
    }
    unsigned int genIndex = 0;
    for(unsigned int ID=0; ID<_allNeuronList.size(); ID++)
    {
        for(unsigned int weight=0; weight<_allNeuronList[ID]->get_inputs(); weight++)
        {
            _allNeuronList[ID]->set_weight(weight,genom[genIndex]);
            genIndex++;
        }
    }
    #ifdef QT_APP
    emit weightValuesChanged(this);
    #endif
}
std::vector<double>       Net::get_genom()
{
    std::vector<double>  tmpGen(_connections,0);
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_genom()",
                       ErrorMessage::updateNetFirst()));
        return tmpGen;
    }
    //tmpGen.reserve(_connections);
    unsigned int genIndex = 0;
    for(unsigned int ID=0; ID<_allNeuronList.size(); ID++)
    {
        for(unsigned int weight=0; weight<_allNeuronList[ID]->get_inputs(); weight++)
        {
            tmpGen[genIndex] = _allNeuronList[ID]->get_weight(weight);
            genIndex++;
        }
    }
    return tmpGen;
}
std::vector<double*>     *Net::get_ptr_genom()
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_ptr_genom()",
                       ErrorMessage::updateNetFirst()));
        return nullptr;
    }
    return &_ptr_genom;
}
unsigned int        Net::get_genomsize()
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_genomsize()",
                       ErrorMessage::updateNetFirst()));
        return 0;
    }
    return static_cast<unsigned int>(_ptr_genom.size());
}

void                Net::set_input(unsigned int input, double signal)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("set_input(unsigned int ["+std::to_string(input)+"] , double ["+std::to_string(signal)+"] )",
                       ErrorMessage::updateNetFirst()));
        return;
        //error_general("input(unsigned int ["+std::to_string(input)+"] , double ["+std::to_string(signal)+"]) ) ",ErrorMessage::updateNetFirst());
    }
    if(_inputs == 0)
    {
        this->addError(Error("set_input(unsigned int ["+std::to_string(input)+"] , double ["+std::to_string(signal)+"] )",
                       std::string("There are no inputs!")));
        return;
    } else if(input > _inputs-1)
    {
        this->addError(Error("set_input(unsigned int ["+std::to_string(input)+"] , double ["+std::to_string(signal)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),input,static_cast<unsigned int>(_inputs-1),']')));
        return;
        //error_general("input(unsigned int ["+std::to_string(input)+"] , double ["+std::to_string(signal)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }

    _needsCalculationUpdate = true;
    _inputSignalList[input] = signal;
    //emit inputsChanged(this);
}
double              Net::get_input(unsigned int input)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_input(unsigned int ["+std::to_string(input)+"] )",
                       ErrorMessage::updateNetFirst()));
        return 0;
       // error_general("input(unsigned int ["+std::to_string(input)+"] ) ",ErrorMessage::updateNetFirst());
    }
    if(input > _inputs-1)
    {
        this->addError(Error("get_input(unsigned int ["+std::to_string(input)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),input,static_cast<unsigned int>(_inputs-1),']')));
        return 0;
        //error_general("input(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    return _inputSignalList[input];
}
void                Net::set_input(std::vector<double> inputList)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_input(unsigned int ["+std::to_string(input)+"] )",
                       ErrorMessage::updateNetFirst()));
        return;
        //error_general("input(std::vector<double>) ",ErrorMessage::updateNetFirst());
    }
    if(static_cast<unsigned int>(inputList.size()) != _inputs)
    {
        this->addError(Error("set_input(std::vector<double> inputList)",{
                       "parameter 0 has the wrong array size: "+std::to_string(inputList.size()),
                       " array size should by "+std::to_string(_inputs)}));
        return;
        //error_general("input(std::vector<double>) ","parameter 0 , size of the array is wrong: ["+std::to_string(inputList.size()) + "] correct size is: ["+std::to_string(_inputs)+"]");
    }
    _needsCalculationUpdate = true;
    _inputSignalList = inputList;
    //emit inputsChanged(this);
}
std::vector<double>       Net::get_input()
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_input()",
                       ErrorMessage::updateNetFirst()));
        return std::vector<double>();
        //error_general("input(std::vector<double>) ",ErrorMessage::updateNetFirst());
    }
    return _inputSignalList;
}

std::vector<double>     Net::get_hidden()
{
    std::vector<double> valueList;
    valueList.reserve(_hiddenNeurons);
    for(unsigned int ID=0; ID<_hiddenNeurons; ID++)
    {
        valueList.push_back(_hiddenNeuronList[ID]->get_output());
    }
    return valueList;
}
double              Net::get_hidden(unsigned int hiddenX, unsigned int hiddenY)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::updateNetFirst()));
        return 0;
        //error_general("hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",
                       std::string("The network has no hidden layer")));
        return 0;
        //error_general("hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )","the network has no hidden layer");
    }
    if(hiddenX > _hiddenX-1)
    {
        this->addError(Error("get_hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenX,static_cast<unsigned int>(_hiddenX-1),']')));
        return 0;
        //error_general("hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
    }
    if(hiddenY > _hiddenY-1)
    {
        this->addError(Error("get_hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenY,static_cast<unsigned int>(_hiddenY-1),']')));
        return 0;
        //error_general("hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)1,hiddenY,(unsigned int)0,_hiddenY-1));
    }

    unsigned int ID = hiddenX * _hiddenY + hiddenY;
    return _hiddenNeuronList[ID]->get_output();
}
std::vector<double>       Net::get_hiddenX(unsigned int hiddenX)// |    Alle in einer Spalte
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_hiddenX(unsigned int ["+std::to_string(hiddenX)+"] )",
                       ErrorMessage::updateNetFirst()));
        return std::vector<double>();
        //error_general("hiddenX(unsigned int ["+std::to_string(hiddenX)+"] )",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_hiddenX(unsigned int ["+std::to_string(hiddenX)+"] )",
                       std::string("The network has no hidden layer")));
        return std::vector<double>();
        //error_general("hiddenX(unsigned int ["+std::to_string(hiddenX)+"] )","the network has no hidden layer");
    }
    if(hiddenX > _hiddenX-1)
    {
        this->addError(Error("get_hiddenX(unsigned int ["+std::to_string(hiddenX)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenX,static_cast<unsigned int>(_hiddenX-1),']')));
        return std::vector<double>();
        //error_general("hiddenX(unsigned int ["+std::to_string(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
    }

    std::vector<double> ret(_hiddenY);
    unsigned int ID = hiddenX * _hiddenY;
    for(unsigned int y=0; y<_hiddenY; y++)
    {
        ret[y] = _hiddenNeuronList[ID]->get_output();
        ID++;
    }
    return ret;
}
std::vector<double>       Net::get_hiddenY(unsigned int hiddenY)// --   Alle in einer Reihe
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_hiddenY(unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::updateNetFirst()));
        return std::vector<double>();
        //error_general("hiddenY(unsigned int ["+std::to_string(hiddenY)+"] )",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_hiddenY(unsigned int ["+std::to_string(hiddenY)+"] )",
                       std::string("The network has no hidden layer")));
        return std::vector<double>();
        //error_general("hiddenY(unsigned int ["+std::to_string(hiddenY)+"] )","the network has no hidden layer");
    }
    if(hiddenY > _hiddenY-1)
    {
        this->addError(Error("get_hiddenY(unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenY,static_cast<unsigned int>(_hiddenY-1),']')));
        return std::vector<double>();
        //error_general("hiddenY(unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,hiddenY,(unsigned int)0,_hiddenY-1));
    }

    std::vector<double> ret(_hiddenX);
    unsigned int ID = hiddenY;
    for(unsigned int x=0; x<_hiddenX; x++)
    {
        ret[x] = _hiddenNeuronList[ID]->get_output();
        ID+=_hiddenY;
    }
    return ret;
}

Neuron             *Net::get_ptr_neuron_viaID(unsigned int ID)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_ptr_neuron_viaID(unsigned int ["+std::to_string(ID)+"] )",
                       ErrorMessage::updateNetFirst()));
        return nullptr;
        //error_general("neuron_viaID(unsigned int ["+std::to_string(ID)+"])",ErrorMessage::updateNetFirst());
    }
    if(_allNeuronList.size() == 0)
    {
        this->addError(Error("get_ptr_neuron_viaID(unsigned int ["+std::to_string(ID)+"] )",
                       ErrorMessage::listIsEmpty("_allNeuronList")));
        return nullptr;
    }else if(ID >= static_cast<unsigned int>(_allNeuronList.size()))
    {
        this->addError(Error("get_ptr_neuron_viaID(unsigned int ["+std::to_string(ID)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),ID,static_cast<unsigned int>(static_cast<unsigned int>(_allNeuronList.size())-1),']')));
        return nullptr;
        //error_general("neuron_viaID(unsigned int ["+std::to_string(ID)+"] )",error_paramOutOfRange((unsigned int)0,ID,(unsigned int)0,_allNeuronList.size()));
    }

    return _allNeuronList[ID];
}
Neuron             *Net::get_ptr_neuron_viaID(NeuronID ID)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_ptr_neuron_viaID("+Neuron::toIDString(ID)+")",
                       ErrorMessage::updateNetFirst()));
        return nullptr;
       // error_general("neuron_viaID(NeuronID ["+Neuron::IDString(ID)+"] )",ErrorMessage::updateNetFirst());
    }
    if(_allNeuronList.size() == 0)
    {
        this->addError(Error("get_ptr_neuron_viaID("+Neuron::toIDString(ID)+")",
                       ErrorMessage::listIsEmpty("_allNeuronList")));
        return nullptr;
    }else if(ID.ID >= static_cast<unsigned int>(_allNeuronList.size()))
    {
        this->addError(Error("get_ptr_neuron_viaID("+Neuron::toIDString(ID)+")",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),ID.ID,static_cast<unsigned int>(static_cast<unsigned int>(_allNeuronList.size())-1),']')));
        return nullptr;
        //error_general("neuron_viaID(unsigned int ["+std::to_string(ID)+"] )",error_paramOutOfRange((unsigned int)0,ID,(unsigned int)0,_allNeuronList.size()));
    }/*
    if(ID.ID >= _allNeuronList.size())
    {
        error_general("neuron_viaID(NeuronID [.ID="+std::to_string(ID.ID)+",.TYPE="+std::to_string(ID.TYPE)+"] )",error_paramOutOfRange((unsigned int)0,ID.ID,(unsigned int)0,_allNeuronList.size()));
    }*/

    return _allNeuronList[ID.ID];
}
Neuron             *Net::get_ptr_hiddenNeuron(unsigned int hiddenX, unsigned int hiddenY)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_ptr_hiddenNeuron(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::updateNetFirst()));
        return nullptr;
        //error_general("hiddenNeuron(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"])",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_ptr_hiddenNeuron(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",
                       std::string("The network has no hidden layer")));
        return nullptr;
        //error_general("hiddenNeuron(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )","the network has no hidden layer");
    }
    if(hiddenX > _hiddenX-1)
    {
        this->addError(Error("get_ptr_hiddenNeuron(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenX,static_cast<unsigned int>(_hiddenX-1),']')));
        return nullptr;
        //error_general("hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
    }
    if(hiddenY > _hiddenY-1)
    {
        this->addError(Error("get_ptr_hiddenNeuron(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenY,static_cast<unsigned int>(_hiddenY-1),']')));
        return nullptr;
        //error_general("hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)1,hiddenY,(unsigned int)0,_hiddenY-1));
    }

    unsigned int ID = hiddenX * _hiddenY + hiddenY;
    return _hiddenNeuronList[ID];
}
std::vector<Neuron*>      Net::get_hiddenNeuronX_ptr(unsigned int hiddenX) // |    Alle in einer Spalte
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_hiddenNeuronX_ptr(unsigned int ["+std::to_string(hiddenX)+"] )",
                       ErrorMessage::updateNetFirst()));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronX(unsigned int ["+std::to_string(hiddenX)+"] ) ",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_hiddenNeuronX_ptr(unsigned int ["+std::to_string(hiddenX)+"] )",
                       std::string("The network has no hidden layer")));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronX(unsigned int ["+std::to_string(hiddenX)+"] )","the network has no hidden layer");
    }
    if(hiddenX >= _hiddenX)
    {
        this->addError(Error("get_hiddenNeuronX_ptr(unsigned int ["+std::to_string(hiddenX)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenX,static_cast<unsigned int>(_hiddenX-1),']')));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronX(unsigned int ["+std::to_string(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
    }

    std::vector<Neuron*> ret(_hiddenY);
    unsigned int ID = hiddenX * _hiddenY;
    for(unsigned int y=0; y<_hiddenY; y++)
    {
        ret[y] = _hiddenNeuronList[ID];
        ID++;
    }

    return ret;
}
std::vector<Neuron*>      Net::get_hiddenNeuronY_ptr(unsigned int hiddenY)// --   Alle in einer Reihe
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_hiddenNeuronY_ptr(unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::updateNetFirst()));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronX(unsigned int ["+std::to_string(hiddenX)+"] ) ",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_hiddenNeuronY_ptr(unsigned int ["+std::to_string(hiddenY)+"] )",
                       std::string("The network has no hidden layer")));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronY(unsigned int ["+std::to_string(hiddenY)+"] )","the network has no hidden layer");
    }
    if(hiddenY >= _hiddenY)
    {
        this->addError(Error("get_hiddenNeuronY_ptr(unsigned int ["+std::to_string(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenY,static_cast<unsigned int>(_hiddenY-1),']')));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronX(unsigned int ["+std::to_string(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
    }

    std::vector<Neuron*> ret(_hiddenX);
    unsigned int ID = hiddenY;
    for(unsigned int y=0; y<_hiddenX; y++)
    {
        ret[y] = _hiddenNeuronList[ID];
        ID+=_hiddenX;
    }
    return ret;
}
std::vector<std::vector<Neuron*> > Net::get_hiddenNeuron_ptr()
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_hiddenNeuron_ptr()",
                       ErrorMessage::updateNetFirst()));
        return std::vector<std::vector<Neuron*> >();
        //error_general("hiddenNeuron() ",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_hiddenNeuron_ptr()",
                       std::string("The network has no hidden layer")));
        return std::vector<std::vector<Neuron*> >();
        //error_general("hiddenNeuron()","the network has no hidden layer");
    }
    if(_hiddenX * _hiddenY != static_cast<unsigned int>(_hiddenNeuronList.size()))
    {
        this->addError(Error("get_hiddenNeuron_ptr()",
                       std::string("something went wrong: _hiddenX * _hiddenY != _hiddenNeuronList.size()")));
        return std::vector<std::vector<Neuron*> >();
        //error_general("hiddenNeuron() ","something went wrong: _hiddenX * _hiddenY != _hiddenNeuronList.size()");
    }

    //std::vector<std::vector<Neuron*>    > ret(_hiddenX,std::vector<Neuron*>(_hiddenY));
    std::vector<std::vector<Neuron*>    > ret(_hiddenX,std::vector<Neuron*>(_hiddenY));
    //ret.reserve(_hiddenX);

    unsigned int ID = 0;
    for(unsigned int x=0; x<_hiddenX; x++)
    {
        for(unsigned int y=0; y<_hiddenX; y++)
        {
            ret[x][y] = _hiddenNeuronList[ID];
            ID++;
        }
    }
    return ret;
}
Neuron              *Net::get_ptr_costumNeuron(NeuronID ID)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_ptr_costumNeuron("+Neuron::toIDString(ID)+")",
                       ErrorMessage::updateNetFirst()));
        return nullptr;
        //error_general("costumNeuron(NeuronID ["+Neuron::IDString(ID)+"] ) ",ErrorMessage::updateNetFirst());
    }
    if(_costumNeuronList.size() == 0)
    {
        this->addError(Error("get_ptr_costumNeuron("+Neuron::toIDString(ID)+")",
                       std::string("There are no costum neurons")));
        return nullptr;
        //error_general("costumNeuron(NeuronID ["+Neuron::IDString(ID)+"] )"," There are no costum neurons");
    }
    if(ID.TYPE != costum)
    {
        this->addError(Error("get_ptr_costumNeuron("+Neuron::toIDString(ID)+")",
                       std::string("This is not a costum neuron")));
        return nullptr;
    }
    if(ID.ID < static_cast<unsigned int>(_allNeuronList.size()-_costumNeuronList.size()))
    {
        this->addError(Error("get_ptr_costumNeuron("+Neuron::toIDString(ID)+")",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),ID.ID,static_cast<unsigned int>(_allNeuronList.size()-_costumNeuronList.size()),']')));
        return nullptr;
        //error_general("costumNeuron(NeuronID ["+Neuron::IDString(ID)+"] )",error_paramOutOfRange((unsigned int)0,_costumNeuronList[0]->ID().ID,(unsigned int)0,_costumNeuronList[_costumNeuronList.size()-1]->ID().ID));
    }

    return _costumNeuronList[ID.ID];
}
std::vector<Neuron*>      *Net::get_ptr_costumNeuron()
{
    return &_costumNeuronList;
}
Neuron              *Net::get_ptr_outputNeuron(unsigned int output)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_ptr_outputNeuron(unsigned int ["+std::to_string(output)+"] )",
                       ErrorMessage::updateNetFirst()));
        return nullptr;
        //error_general("outputNeuron(unsigned int ["+std::to_string(output)+"]) ",ErrorMessage::updateNetFirst());
    }
    if(output >= _outputs)
    {
        this->addError(Error("get_ptr_outputNeuron(unsigned int ["+std::to_string(output)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),output,_outputs-1,']')));
        return nullptr;
        //error_general("outputNeuron(unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,output,(unsigned int)0,_outputs-1));
    }

    return _outputNeuronList[output];
}
std::vector<Neuron*>      *Net::get_ptr_outputNeuron()
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_ptr_outputNeuron()",
                       ErrorMessage::updateNetFirst()));
        return nullptr;
        //error_general("outputNeuron() ",ErrorMessage::updateNetFirst());
    }
    return &_outputNeuronList;
}
std::vector<Neuron*>      *Net::get_ptr_allNeurons()
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_ptr_allNeurons()",
                       ErrorMessage::updateNetFirst()));
        return nullptr;
        //error_general("allNeurons() ",ErrorMessage::updateNetFirst());
    }
    return &_allNeuronList;
}


double              Net::get_output(unsigned int output)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_output()",
                       ErrorMessage::updateNetFirst()));
        return 0;
        //error_general("allNeurons() ",ErrorMessage::updateNetFirst());
    }
    if(output >= _outputs)
    {
        this->addError(Error("get_output(unsigned int ["+std::to_string(output)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),output,static_cast<unsigned int>(_outputs-1),']')));
        return 0;
        //error_general("output(unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,output,(unsigned int)0,_outputs-1));
    }
    return _outputNeuronList[output]->get_output();
}
std::vector<double>       Net::get_output()
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_output()",
                       ErrorMessage::updateNetFirst()));
        return std::vector<double>();
        //error_general("allNeurons() ",ErrorMessage::updateNetFirst());
    }
    std::vector<double> ret(_outputs);
    for(unsigned int y=0; y<_outputs; y++)
    {
        ret[y] = _outputNeuronList[y]->get_output();
    }
    return ret;
}

void                Net::run()
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("run()",
                       ErrorMessage::updateNetFirst()));
        return;
        //error_general("allNeurons() ",ErrorMessage::updateNetFirst());
    }
#if defined(_DEBUG_NET_RUN)
    __DEBUG_NET(this,"run()","begin, sets all neuron's to needsUpdate.");
#endif
#if defined(_DEBUG_NET_TIMING)
    __debug_timer1_start = std::chrono::high_resolution_clock::now();
#endif
    for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
    {
        _allNeuronList[neuron]->needsUpdate();
    }
 //   bool allUpdated = false;
#if defined(_DEBUG_NET_RUN)
    __DEBUG_NET(this,"run()","while neurons not updated");
#endif
    for(unsigned int neuron=0; neuron<_calculationOrderList.size(); neuron++)
    {
#if defined(_DEBUG_NET_RUN)
        __DEBUG_NET(this,"run()","calculates neuron ID: "+Neuron::toIDString(_calculationOrderList[neuron]));
#endif
        _allNeuronList[_calculationOrderList[neuron].ID]->run();
    }

#if defined(_DEBUG_NET_TIMING)
    __debug_timer1_end          = std::chrono::high_resolution_clock::now();
    __debug_time_span           = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer1_end - __debug_timer1_start);
    __debug_run_time            = __debug_time_span.count()*1000;
    __debug_average_run_time    = __debug_average_run_time * 0.6 + 0.4 * __debug_run_time;
#endif
#if defined(_DEBUG_NET_RUN)
#if defined(_DEBUG_NET_TIMING)
    __DEBUG_NET(this,"run()","all updated, finish. Elapsed time: "+std::to_string(__debug_run_time)+" ms");
#else
    __DEBUG_NET(this,"run()","all updated, finish");
#endif
#endif
    _needsCalculationUpdate = false;
    //emit hiddenOutputsChanged(this);
    //emit outputsChanged(this);
    #ifdef QT_APP
    emit runDone(this);
    #endif
}

bool                Net::needsUpdate()
{
    return _needsConfigurationUpdate;
}
void                Net::updateNetConfiguration()
{
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","begin");
#endif
#if defined(_DEBUG_NET_TIMING)
    __debug_timer2_start  = std::chrono::high_resolution_clock::now();
#endif
    if(!_needsConfigurationUpdate)
    {
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
        __DEBUG_NET(this,"updateNetConfiguration()","already up to date");
#endif
        return;
    }
    #ifdef QT_APP
    emit netConfigurationUpdateStarted(this);
    #endif
    _needsCalculationUpdate = true;
    if(_hiddenX == 0 || _hiddenY == 0)
    {
        _hasHiddenLayer = false;
    }
    else
    {
        _hasHiddenLayer = true;
    }
    for(unsigned long long a=_allNeuronList.size(); a>0; a--)
    {
        try {
            delete _allNeuronList[a-1];
        } catch (std::exception &e) {
            this->addError(Error("updateNetConfiguration()",
                           std::string(e.what())));
            return;
            //error_general("updateNetConfiguration()",e.what());
        }
    }


    if(_doRegenerateConnectionlist || _connectionList.size() == 0)
    {
        _connectionList.clear();
        prepareConnectionList();
        _doRegenerateConnectionlist = false;
    }

    _outputNeurons  = 0;
    _hiddenNeurons  = 0;
    _neurons        = 0;

    std::vector<Connection>   __allConnectionList = _connectionList;
    for(unsigned int pos=0; pos<_costumConnectionList.size(); pos++)
        __allConnectionList.push_back(_costumConnectionList[pos]);

    std::vector<unsigned int> __inputIDs;
    std::vector<unsigned int> __hiddenIDs;
    std::vector<unsigned int> __outputIDs;
    std::vector<unsigned int> __costumIDs;


    for(unsigned int connection=0; connection<__allConnectionList.size(); connection++)
    {
        switch(__allConnectionList[connection].source_ID.TYPE)
        {
            case NeuronType::none:
            {
                break;
            }
            case NeuronType::bias:
            {
                break;
            }
            case NeuronType::input:
            {
                __inputIDs.push_back(__allConnectionList[connection].source_ID.ID);
                break;
            }
            case NeuronType::hidden:
            {
                __hiddenIDs.push_back(__allConnectionList[connection].source_ID.ID);
                break;
            }
            case NeuronType::output:
            {
                __outputIDs.push_back(__allConnectionList[connection].source_ID.ID);
                break;
            }
            case NeuronType::costum:
            {
                __costumIDs.push_back(__allConnectionList[connection].source_ID.ID);
                break;
            }
        }
        switch(__allConnectionList[connection].destination_ID.TYPE)
        {
            case NeuronType::none:
            {
                break;
            }
            case NeuronType::bias:
            {
                break;
            }
            case NeuronType::input:
            {
                __inputIDs.push_back(__allConnectionList[connection].destination_ID.ID);
                break;
            }
            case NeuronType::hidden:
            {
                __hiddenIDs.push_back(__allConnectionList[connection].destination_ID.ID);
                break;
            }
            case NeuronType::output:
            {
                __outputIDs.push_back(__allConnectionList[connection].destination_ID.ID);
                break;
            }
            case NeuronType::costum:
            {
                __costumIDs.push_back(__allConnectionList[connection].destination_ID.ID);
                break;
            }
        }
    }
    // __inputIDs kann mehrfach die gleiche ID enthalten.
    // __filtered_inputIDs enthällt jede ID nur noch ein mal.
    std::vector<unsigned int> __filtered_inputIDs;
    std::vector<unsigned int> __filtered_hiddenIDs;
    std::vector<unsigned int> __filtered_outputIDs;
    std::vector<unsigned int> __filtered_costumIDs;

    for(unsigned int pos=0; pos<__inputIDs.size(); pos++)
    {
        bool alreadyExists = false;
        for(unsigned int new_pos=0; new_pos<__filtered_inputIDs.size(); new_pos++)
        {
            if(__filtered_inputIDs[new_pos] == __inputIDs[pos])
            {
                alreadyExists = true;
            }
        }
        if(!alreadyExists)
            __filtered_inputIDs.push_back(__inputIDs[pos]);
    }
    for(unsigned int pos=0; pos<__hiddenIDs.size(); pos++)
    {
        bool alreadyExists = false;
        for(unsigned int new_pos=0; new_pos<__filtered_hiddenIDs.size(); new_pos++)
        {
            if(__filtered_hiddenIDs[new_pos] == __hiddenIDs[pos])
            {
                alreadyExists = true;
            }
        }
        if(!alreadyExists)
            __filtered_hiddenIDs.push_back(__hiddenIDs[pos]);
    }
    for(unsigned int pos=0; pos<__outputIDs.size(); pos++)
    {
        bool alreadyExists = false;
        for(unsigned int new_pos=0; new_pos<__filtered_outputIDs.size(); new_pos++)
        {
            if(__filtered_outputIDs[new_pos] == __outputIDs[pos])
            {
                alreadyExists = true;
            }
        }
        if(!alreadyExists)
            __filtered_outputIDs.push_back(__outputIDs[pos]);
    }
    for(unsigned int pos=0; pos<__costumIDs.size(); pos++)
    {
        bool alreadyExists = false;
        for(unsigned int new_pos=0; new_pos<__filtered_costumIDs.size(); new_pos++)
        {
            if(__filtered_costumIDs[new_pos] == __costumIDs[pos])
            {
                alreadyExists = true;
            }
        }
        if(!alreadyExists)
            __filtered_costumIDs.push_back(__costumIDs[pos]);
    }

    _outputNeurons  = __filtered_outputIDs.size();
    _hiddenNeurons  = __filtered_hiddenIDs.size();
    _costumNeurons  = __filtered_costumIDs.size();
    _neurons        = _hiddenNeurons + _outputNeurons + _costumNeurons;





    _costumConnections  = _costumConnectionList.size();
    _connections        = _connectionList.size() + _costumConnectionList.size();

    _inputSignalList.clear();
    _hiddenNeuronList.clear();
    _outputNeuronList.clear();
    _costumNeuronList.clear();
    _allNeuronList.clear();
    _ptr_genom.clear();

#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","inputs:         "+std::to_string(_inputs));
    __DEBUG_NET(this,"updateNetConfiguration()","hiddenX:        "+std::to_string(_hiddenX));
    __DEBUG_NET(this,"updateNetConfiguration()","hiddenY:        "+std::to_string(_hiddenY));
    __DEBUG_NET(this,"updateNetConfiguration()","outputs:        "+std::to_string(_outputs));
    __DEBUG_NET(this,"updateNetConfiguration()","costum:         "+std::to_string(_costumNeurons));
    __DEBUG_NET(this,"updateNetConfiguration()","connections:    "+std::to_string(_connections));
    __DEBUG_NET(this,"updateNetConfiguration()","reserving memory");
#endif

    _inputSignalList = std::vector<double>(_inputs,0);
    _hiddenNeuronList = std::vector<Neuron*>(_hiddenNeurons);
    _outputNeuronList = std::vector<Neuron*>(_outputs);
    _costumNeuronList = std::vector<Neuron*>(_costumNeurons);
    _allNeuronList = std::vector<Neuron*>(_neurons);


#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","setup _inputSignalList");
#endif
    /*for(unsigned int input=0; input<_inputs; input++)
    {
        _inputSignalList.push_back(0);
    }*/
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","setup _allNeuronList");
#endif
#if defined(_DEBUG_NET_TIMING)
    __debug_timer1_start = std::chrono::high_resolution_clock::now();
    double secCounter    = 0;
    const double debugInterval = 0.2; //sec
#endif
    for(unsigned int neuronID=0; neuronID<_neurons; neuronID++)
    {

        #ifdef QT_APP
        Neuron * tmp_neuron = new Neuron(this);
        #else
        Neuron * tmp_neuron = new Neuron();
        #endif

        tmp_neuron->set_activationFunction(_activationFunction);
        tmp_neuron->set_enableAverage(_enableAverage);
        tmp_neuron->set_ID(neuronID);
        #ifdef QT_APP
        QObject::connect(tmp_neuron,&Neuron::errorOccured,this,&Net::onNeuronError);
        #endif
        _allNeuronList[neuronID] = tmp_neuron;
#if defined(_DEBUG_NET_TIMING)
        __debug_timer1_end  = std::chrono::high_resolution_clock::now();
        __debug_time_span   = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer1_end - __debug_timer1_start);
        if(__debug_time_span.count() >= debugInterval)
        {
            secCounter+=debugInterval;
            __debug_timer1_start = __debug_timer1_end;
            __DEBUG_NET(this,"updateNetConfiguration()","Creating Neurons... "+std::to_string(neuronID)+" of: "+std::to_string(_neurons)+" generated. Time elapsed: "+std::to_string(secCounter)+" sec");
        }
#endif
    }
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","copy to _hiddenNeuronList");
#endif
    for(unsigned int ID=0; ID<_hiddenNeurons; ID++)    // Save the first neurons (the hidden neurons) also in the _hiddenNeuronList
    {
        _hiddenNeuronList[ID] = _allNeuronList[ID];
        _hiddenNeuronList[ID]->set_TYPE(NeuronType::hidden);
    }
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","copy to _outputNeuronList");
#endif
    for(unsigned int ID=_hiddenNeurons; ID<_hiddenNeurons+_outputNeurons; ID++)    // Save the rest (the output neurons) also in the _outputNeuronList
    {
        _outputNeuronList[ID-_hiddenNeurons] = _allNeuronList[ID];
        _outputNeuronList[ID-_hiddenNeurons]->set_TYPE(NeuronType::output);
    }
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","copy to _costumNeuronList");
#endif
    for(unsigned int ID=_hiddenNeurons+_outputNeurons; ID<_neurons; ID++)    // Save the rest (the output neurons) also in the _outputNeuronList
    {
        _costumNeuronList[ID-_costumNeurons-_outputNeurons] = _allNeuronList[ID];
        _costumNeuronList[ID-_costumNeurons-_outputNeurons]->set_TYPE(NeuronType::costum);
    }


#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","connect "+std::to_string(_connections)+" connections");
#endif
#if defined(_DEBUG_NET_TIMING)
    __debug_timer1_start = std::chrono::high_resolution_clock::now();
    secCounter    = 0;
#endif
    //- Connect the inputs
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    for(unsigned int connection=0; connection<_connections; connection++)
    {
        __DEBUG_NET(this,"updateNetConfiguration()","connection["+std::to_string(connection)+"] "+Neuron::toConnectionString(__allConnectionList[connection]));
    }
#endif
    for(unsigned int connection=0; connection<_connections; connection++)
    {
        /*if(connection < _connectionList.size())
            set_ptr_intern_connectNeuron(&_connectionList[connection]);
        else
            set_ptr_intern_connectNeuron(&_costumConnectionList[connection-_connectionList.size()]);*/
        set_ptr_intern_connectNeuron(&__allConnectionList[connection]);
#if defined(_DEBUG_NET_TIMING)
        __debug_timer1_end  = std::chrono::high_resolution_clock::now();
        __debug_time_span   = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer1_end - __debug_timer1_start);
        if(__debug_time_span.count() >= debugInterval)
        {
            secCounter+=debugInterval;
            __debug_timer1_start = std::chrono::high_resolution_clock::now();
            __DEBUG_NET(this,"updateNetConfiguration()","connecting... "+std::to_string(connection)+" of: "+std::to_string(_connections)+" connection. Time elapsed: "+std::to_string(secCounter)+" sec");
        }
#endif
    }
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","connect finish");
#endif
    update_ptr_genomList();
    prepareCalculationOrderList();
    _needsConfigurationUpdate = false;
#if defined(_DEBUG_NET_TIMING)
    __debug_timer2_end  = std::chrono::high_resolution_clock::now();
    __debug_time_span   = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer2_end - __debug_timer2_start);
    __DEBUG_NET(this,"updateNetConfiguration()","end. Elapsed time: "+std::to_string(__debug_time_span.count())+" sec");
#endif
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","end");
#endif
#ifdef QT_APP
    emit netConfigurationUpdated(this);
    emit weightValuesChanged(this);
    emit accessUnlock(this);
#endif
}
void                Net::addConnection(NeuronID fromNeuron,NeuronID toNeuron,ConnectionDirection direction)
{
#if defined(_DEBUG_NET_CONNECT)
    __DEBUG_NET(this,"addConnection(NeuronID,NeuronID,ConnectionDirection)","begin");
    __DEBUG_NET(this,"addConnection(NeuronID,NeuronID,ConnectionDirection)","from:        "+Neuron::toIDString(fromNeuron));
    __DEBUG_NET(this,"addConnection(NeuronID,NeuronID,ConnectionDirection)","to:          "+Neuron::toIDString(toNeuron));
    __DEBUG_NET(this,"addConnection(NeuronID,NeuronID,ConnectionDirection)","direction:   "+Neuron::toDirectionString(direction));
#endif
    Connection con;
    con.netID = this->get_ID();
    con.source_ID = fromNeuron;
    con.destination_ID = toNeuron;
    con.direction = direction;
    con.weight = Neuron::get_calcRandWeight(this->_randEngine);
    this->addConnection(con);
    //unsigned int maxNeuronID = _allNeuronList.size();
//#if defined(_DEBUG_NET_CONNECT)

    //__DEBUG_NET(this,"connectNeuronViaID(...)","connecting Neuron: "+std::to_string(fromNeuron)+" to: "+std::to_string(toNeuron));
   // __DEBUG_NET(this,"connectNeuronViaID(...)","maxNeuronID: "+std::to_string(maxNeuronID));
//#endif
  /*  if(fromNeuron >= maxNeuronID)
    {
        this->addError(Error("connectNeuronViaID(unsigned int ["+std::to_string(fromNeuron)+"] ,"+
                             "unsigned int ["+std::to_string(toNeuron)+"] ,"+
                             "ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),fromNeuron,static_cast<unsigned int>(maxNeuronID-1),']')));
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuronViaID(...)","Problem: "+error_paramOutOfRange(0,std::to_string(fromNeuron),std::to_string(0),std::to_string(maxNeuronID)));
#endif
        return 0;
    }
    if(toNeuron > maxNeuronID)
    {
        this->addError(Error("connectNeuronViaID(unsigned int ["+std::to_string(fromNeuron)+"] ,"+
                             "unsigned int ["+std::to_string(toNeuron)+"] ,"+
                             "ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),toNeuron,static_cast<unsigned int>(maxNeuronID-1),']')));
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuronViaID(...)","Problem: "+error_paramOutOfRange(1,std::to_string(toNeuron),std::to_string(0),std::to_string(maxNeuronID)));
#endif
        return 0;
    }*/
    /*if(_allNeuronList[toNeuron]->connectInput(_allNeuronList[fromNeuron],direction))
    {
        _costumConnections++;
    }*/
    /*} catch (std::runtime_error &e) {
        this->addError(Error("connectNeuronViaID(unsigned int ["+std::to_string(fromNeuron)+"] ,"+
                             "unsigned int ["+std::to_string(toNeuron)+"] ,"+
                             "ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                             std::string(e.what())));
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuronViaID(...)","Problem: "+std::string(e.what()));
#endif
        return 0;
    }*/
#if defined(_DEBUG_NET_CONNECT)
    __DEBUG_NET(this,"addConnection(NeuronID,NeuronID,ConnectionDirection)","end");
#endif
}
void                Net::addConnection(Connection connection)
{
    //bool ret = set_ptr_intern_connectNeuron(connection);
    //prepareCalculationOrderList();
    #ifdef QT_APP
    emit accessLock(this);
    #endif
    _costumConnectionList.push_back(connection);
    _needsConfigurationUpdate = true;
    #ifdef QT_APP
    emit netConfigurationUpdateNeeded(this);
    #endif

}

void                Net::addConnection(std::vector<Connection> connections)
{
    for(unsigned int a=0; a<connections.size(); a++)
    {
        addConnection(connections[a]);
    }
}
void                Net::set_connectionList(std::vector<Connection> connections)
{
    #ifdef QT_APP
    emit accessLock(this);
    #endif
    _connectionList = connections;
    #ifdef QT_APP
    emit netConfigurationUpdateNeeded(this);
    #endif
    _needsConfigurationUpdate = true;
}
std::vector<Connection>  Net::get_connectionList()
{
    return _connectionList;
}
std::vector<Connection>  Net::get_costumConnectionList()
{
    return _costumConnectionList;
}

NeuronID            Net::addNeuron()
{
    return addNeuron(new Neuron());
}
NeuronID            Net::addNeuron(Neuron *neuron)
{
    if(neuron == nullptr)
    {
        addError(Error("addNeuron(Neuron *neuron)",std::string("neuron == nullptr")));
        return NeuronID();
    }
    _costumNeuronList.push_back(neuron);
    _costumNeuronList[_costumNeuronList.size()-1]->set_ID(_allNeuronList.size());
    _costumNeuronList[_costumNeuronList.size()-1]->set_TYPE(NeuronType::costum);
    _allNeuronList.push_back(_costumNeuronList[_costumNeuronList.size()-1]);
    _costumNeurons++;
    return _costumNeuronList[_costumNeuronList.size()-1]->get_ID();
}

std::string             Net::toString()
{
    std::string separator = "\n";
    std::string buffer = "Net: ["+std::to_string(this->get_ID())+"]"+separator;
    buffer += "Neurons: "+separator;
    buffer += "\tInputs:           "+std::to_string(this->get_inputNeurons())+separator;
    buffer += "\tHiddenlayers:     "+std::to_string(this->get_hiddenNeuronsX())+separator;
    buffer += "\tNeuronsPerLayer:  "+std::to_string(this->get_hiddenNeuronsY())+separator;
    buffer += "\tCostumneurons:    "+std::to_string(this->get_costumNeurons())+separator;
    buffer += "\tOutputneurons:    "+std::to_string(this->get_outputNeurons())+separator;
    buffer += "--------------------------------------------------------"+separator;
    buffer += "\tTotal:            "+std::to_string(this->get_neurons())+separator+separator;

    buffer += "Connections:"+separator;
    buffer += "\tNormal:           "+std::to_string(this->get_connections()-this->get_costumConnections())+separator;
    buffer += "\tCostum:           "+std::to_string(this->get_costumConnections())+separator;
    buffer += "--------------------------------------------------------"+separator;
    buffer += "\tTotal:            "+std::to_string(this->get_connections())+separator+separator;

    buffer += "Activation: "+Neuron::toActivationString(this->get_activationFunction())+separator;
    buffer += "Bias:       ";
    if(this->get_bias())
        buffer+= "true\tvalue: "+std::to_string(this->get_biasValue())+separator;
    else
        buffer+="false"+separator;

#ifdef _DEBUG_NET_TIMING
    buffer += "Calculation time:   "+std::to_string(__debug_average_run_time)+"ms"+separator;
#endif

    return buffer;
}
std::vector<std::string>         Net::toStringList()
{
    std::vector<std::string> list;
    std::string separator = "\n";
    std::string buff = this->toString();
    while(buff.find(separator) != std::string::npos)
    {
        list.push_back(buff.substr(0,buff.find(separator)));
        buff=buff.substr(buff.find(separator)+1,buff.size());
    }

    return list;
}
void                Net::set_seed(unsigned int seed)
{
    _randEngine             = std::default_random_engine(seed);
}
void                Net::clearErrors()
{
    _errorList.clear();
    for(unsigned int a=0; a<_allNeuronList.size(); a++)
    {
        _allNeuronList[a]->clearErrors();
    }
}
Error               Net::get_lastError() const
{
    if(static_cast<unsigned int>(_errorList.size()) == 0)
    {
        return Error("get_lastError()",ErrorMessage::listIsEmpty("_errorList"));
    }
    return _errorList[_errorList.size()-1];
}
Error               Net::get_error(unsigned int index)
{
    unsigned int neuronErrors = 0;
    for(unsigned int a=0; a<_allNeuronList.size(); a++)
    {
        neuronErrors += _allNeuronList[a]->get_errorAmount();
    }
    int allErrors = _errorList.size()+static_cast<int>(neuronErrors);
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
ErrorList           Net::get_errorList() const
{
    unsigned int neuronErrors = 0;
    for(unsigned int a=0; a<_allNeuronList.size(); a++)
    {
        neuronErrors += _allNeuronList[a]->get_errorAmount();
    }
    int allErrors = _errorList.size()+static_cast<int>(neuronErrors);
    ErrorList list;
    list.reserve(allErrors);
    list.insert(list.end(),_errorList.begin(),_errorList.end());
    for(unsigned int a=0; a<this->_allNeuronList.size(); a++)
    {
        ErrorList toInsert = _allNeuronList[a]->get_errorList();
        list.insert(list.end(),toInsert.begin(),toInsert.end());
    }

    return list;
}
unsigned int        Net::get_errorAmount() const
{
    unsigned int neuronErrors = 0;
    for(unsigned int a=0; a<_allNeuronList.size(); a++)
    {
        neuronErrors += _allNeuronList[a]->get_errorAmount();
    }
    return static_cast<unsigned int>(_errorList.size())+neuronErrors;
}
#ifdef QT_APP
void                Net::onNeuronError(NeuronID ID,Error &e)
{
    e.setNamespace(Neuron::toIDString(ID)+" :: "+e.getNamespace());
    emit errorOccured(this->get_ID(),e);
}
#endif
void Net::init(unsigned int inputs,
          unsigned int hiddenX,
          unsigned int hiddenY,
          unsigned int outputs,
          bool enableBias,
          bool enableAverage,
          Activation func)
{
    #ifdef QT_APP
    emit accessLock(this);
    #endif
    time_t now                  = time(nullptr);
    struct tm *currentTime      = localtime(&now);
    this->set_seed(unsigned(currentTime->tm_min+currentTime->tm_sec+currentTime->tm_sec+clock())+rand()%100);

    _activationFunction         = Activation::Sigmoid;
    _inputs                     = NET_MIN_INPUTNEURONS;
    _hiddenX                    = NET_MIN_HIDDENNEURONS_X;
    _hiddenY                    = NET_MIN_HIDDENNEURONS_Y;
    _hiddenNeurons              = _hiddenX*_hiddenY;
    _outputs                    = NET_MIN_OUTPUTNEURONS;
    _outputNeurons              = _outputs;
    _costumNeurons              = 0;
    _connections                = 0;
    _costumConnections          = 0;
   // set_costumConnections(0);
    _bias                       = true;
    _enableAverage              = false;
   // this->ID(rand() % 30000);
    try {
        this->set_outputNeurons(outputs);
        this->set_hiddenNeuronsY(hiddenY);
        this->set_hiddenNeuronsX(hiddenX);
        this->set_inputNeurons(inputs);
        this->set_bias(enableBias);
        this->set_enableAverage(enableAverage);
        this->set_activationFunction(func);
    } catch (std::runtime_error &e) {
       /* error_general("Net(unsigned int ["+std::to_string(inputs)+
                      "] , unsigned int ["+std::to_string(hiddenX)+
                      "] , unsigned int ["+std::to_string(hiddenY)+
                      "] , unsigned int ["+std::to_string(outputs)+
                      "] , bool ["+std::to_string(enableBias)+
                      "] , bool ["+std::to_string(enableAverage)+
                      "] , Activation ["+std::to_string(func)+"])",e);*/
        this->addError(Error("init()",{"unsigned int ["+std::to_string(inputs)+"]",
                           " , unsigned int ["+std::to_string(hiddenX)+"]",
                           " , unsigned int ["+std::to_string(hiddenY)+"]",
                           " , unsigned int ["+std::to_string(outputs)+"]",
                           " , bool ["+std::to_string(enableBias)+"]",
                           " , bool ["+std::to_string(enableAverage)+"]",
                           " , Activation ["+std::to_string(func)+"])",
                           "error: "+std::string(e.what())}));
        return;
    }
    this->set_biasValue(1.0);
   // this->updateNetConfiguration();
    #ifdef QT_APP
    emit netConfigurationUpdateNeeded(this);
    #endif
    _needsConfigurationUpdate = true;
    _doRegenerateConnectionlist = true;
    _needsCalculationUpdate = true;

#ifdef _DEBUG_NET_TIMING
    __debug_run_time = 0;
    __debug_average_run_time = 0;
#endif
}
void                Net::update_ptr_genomList()
{
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"update_ptr_genomList()","begin");
#endif
    _ptr_genom.clear();
    _ptr_genom = std::vector<double*>(_connections);
    unsigned int genomIndex = 0;
    for(unsigned int ID=0; ID<_neurons; ID++)
    {
        for(unsigned int weight=0; weight<_allNeuronList[ID]->get_inputs(); weight++)
        {
            _ptr_genom[genomIndex] = _allNeuronList[ID]->get_ptr_weight(weight);
            genomIndex++;
        }
    }
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"update_ptr_genomList()","end");
#endif
}

bool                Net::set_ptr_intern_connectNeuron(Connection *connection)
{
#if defined(_DEBUG_NET_CONNECT)
    __DEBUG_NET(this,"set_ptr_intern_connectNeuron(Connection ["+Neuron::toConnectionString(*connection)+"])","begin");
#endif
    if(connection->destination_ID.ID >= _allNeuronList.size())
    {
        this->addError(Error("set_ptr_intern_connectNeuron(Connection *ptr)",{
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),connection->destination_ID.ID,static_cast<unsigned int>(_allNeuronList.size()-1),']'),
                       Neuron::toConnectionString(*connection)}));
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"set_ptr_intern_connectNeuron(Connection *ptr)","destination neuron ID: "+std::to_string(connection->destination_ID.ID)+" not found");
#endif
        return 0;
    }
    if((connection->source_ID.ID >= _allNeuronList.size()) && (connection->source_ID.TYPE != NeuronType::input))
    {
        this->addError(Error("set_ptr_intern_connectNeuron(Connection *ptr)",{
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),connection->source_ID.ID,static_cast<unsigned int>(_allNeuronList.size()-1),']'),
                       Neuron::toConnectionString(*connection)}));
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"set_ptr_intern_connectNeuron(Connection *ptr)","source neuron ID: "+std::to_string(connection->source_ID.ID)+" not found");
#endif
        return 0;
    }
    switch(connection->source_ID.TYPE)
    {
        case NeuronType::input:
        {
            _allNeuronList[connection->destination_ID.ID]->connectInput(connection->source_ID,&_inputSignalList[connection->source_ID.ID],connection->direction);
            break;
        }
        case NeuronType::hidden:
        case NeuronType::output:
        case NeuronType::costum:
        {
            _allNeuronList[connection->destination_ID.ID]->connectInput(_allNeuronList[connection->source_ID.ID],connection->direction);
            break;
        }
        case NeuronType::bias:
        {
            _allNeuronList[connection->destination_ID.ID]->connectInput(connection->source_ID,&_biasValue,connection->direction);
            break;
        }
        default:
        {
#if defined(_DEBUG_NET_CONNECT)
            __DEBUG_NET(this,"set_ptr_intern_connectNeuron(Connection *ptr)","unknown source Neuron TYPE: "+Neuron::toTypeString(connection->source_ID.TYPE));
#endif
            return 0;
        }
    }
    _allNeuronList[connection->destination_ID.ID]->set_weight(connection->source_ID,connection->weight);
#if defined(_DEBUG_NET_CONNECT)
    __DEBUG_NET(this,"set_ptr_intern_connectNeuron(Connection ["+Neuron::toConnectionString(*connection)+"])","end");
#endif
    return 1;
}
void                Net::prepareConnectionList()
{
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"prepareConnectionList()","begin");
#endif
#if defined(_DEBUG_NET_TIMING)
    __debug_timer1_start      = std::chrono::high_resolution_clock::now();
#endif
    _connectionList.clear();
    //_connectionList.reserve(1000000);
    //qDebug() << "reserve finish";

    _outputNeurons  = _outputs;
    _hiddenNeurons  = _hiddenX * _hiddenY;
    _neurons        = _hiddenNeurons + _outputNeurons + _costumNeurons;

    unsigned int ID = 0;
    for(unsigned int hiddenNeuronX=0; hiddenNeuronX<_hiddenX; hiddenNeuronX++)
    {
        for(unsigned int hiddenNeuronY=0; hiddenNeuronY<_hiddenY; hiddenNeuronY++)
        {

            if(hiddenNeuronX == 0)
            {
                for(unsigned int input=0; input<_inputs; input++)
                {
                    _connectionList.push_back(Connection());
                    _connectionList[_connectionList.size()-1].destination_ID.ID = ID;
                    _connectionList[_connectionList.size()-1].destination_ID.TYPE = NeuronType::hidden;
                    _connectionList[_connectionList.size()-1].netID = this->get_ID();
                    _connectionList[_connectionList.size()-1].weight = Neuron::get_calcRandWeight(_randEngine);
                    _connectionList[_connectionList.size()-1].source_ID.ID = input;
                    _connectionList[_connectionList.size()-1].source_ID.TYPE = NeuronType::input;
                    _connectionList[_connectionList.size()-1].direction = ConnectionDirection::forward;
                }
            }else {
                for(unsigned int hiddenNeuronY2=0; hiddenNeuronY2<_hiddenY; hiddenNeuronY2++)
                {
                    _connectionList.push_back(Connection());
                    _connectionList[_connectionList.size()-1].destination_ID.ID = ID;
                    _connectionList[_connectionList.size()-1].destination_ID.TYPE = NeuronType::hidden;
                    _connectionList[_connectionList.size()-1].netID = this->get_ID();
                    _connectionList[_connectionList.size()-1].weight = Neuron::get_calcRandWeight(_randEngine);
                    _connectionList[_connectionList.size()-1].source_ID.ID = ID-(_hiddenY+hiddenNeuronY)+hiddenNeuronY2;
                    _connectionList[_connectionList.size()-1].source_ID.TYPE = NeuronType::hidden;
                    _connectionList[_connectionList.size()-1].direction = ConnectionDirection::forward;
                }
            }
            if(_bias)
            {
                _connectionList.push_back(Connection());
                _connectionList[_connectionList.size()-1].destination_ID.ID = ID;
                _connectionList[_connectionList.size()-1].destination_ID.TYPE = NeuronType::hidden;
                _connectionList[_connectionList.size()-1].netID = this->get_ID();
                _connectionList[_connectionList.size()-1].weight = Neuron::get_calcRandWeight(_randEngine);
                _connectionList[_connectionList.size()-1].source_ID.ID = 0;
                _connectionList[_connectionList.size()-1].source_ID.TYPE = NeuronType::bias;
                _connectionList[_connectionList.size()-1].direction = ConnectionDirection::forward;
            }
            ID++;
        }
    }
    //ID = 0;
    if(!_hasHiddenLayer)
    {
        for(unsigned int outputNeuron=0; outputNeuron<_outputs; outputNeuron++)
        {
            for(unsigned int input=0; input<_inputs; input++)
            {
                _connectionList.push_back(Connection());
                _connectionList[_connectionList.size()-1].destination_ID.ID = ID;
                _connectionList[_connectionList.size()-1].destination_ID.TYPE = NeuronType::output;
                _connectionList[_connectionList.size()-1].source_ID.ID = input;
                _connectionList[_connectionList.size()-1].source_ID.TYPE = NeuronType::input;
                _connectionList[_connectionList.size()-1].netID = this->get_ID();
                _connectionList[_connectionList.size()-1].weight = Neuron::get_calcRandWeight(_randEngine);
                _connectionList[_connectionList.size()-1].direction = ConnectionDirection::forward;
            }
            if(_bias)
            {
                _connectionList.push_back(Connection());
                _connectionList[_connectionList.size()-1].destination_ID.ID = ID;
                _connectionList[_connectionList.size()-1].destination_ID.TYPE = NeuronType::output;
                _connectionList[_connectionList.size()-1].netID = this->get_ID();
                _connectionList[_connectionList.size()-1].weight = Neuron::get_calcRandWeight(_randEngine);
                _connectionList[_connectionList.size()-1].source_ID.ID = 0;
                _connectionList[_connectionList.size()-1].source_ID.TYPE = NeuronType::bias;
                _connectionList[_connectionList.size()-1].direction = ConnectionDirection::forward;
            }
            ID++;
        }
    }
    else {
        for(unsigned int outputNeuron=0; outputNeuron<_outputs; outputNeuron++)
        {
            for(unsigned int hiddenNeuronY=0; hiddenNeuronY<_hiddenY; hiddenNeuronY++)
            {
                _connectionList.push_back(Connection());
                _connectionList[_connectionList.size()-1].destination_ID.ID = ID;
                _connectionList[_connectionList.size()-1].destination_ID.TYPE = NeuronType::output;
                _connectionList[_connectionList.size()-1].source_ID.ID = (_hiddenX-1)*_hiddenY+hiddenNeuronY;
                _connectionList[_connectionList.size()-1].source_ID.TYPE = NeuronType::hidden;
                _connectionList[_connectionList.size()-1].netID = this->get_ID();
                _connectionList[_connectionList.size()-1].weight = Neuron::get_calcRandWeight(_randEngine);
                _connectionList[_connectionList.size()-1].direction = ConnectionDirection::forward;
            }
            if(_bias)
            {
                _connectionList.push_back(Connection());
                _connectionList[_connectionList.size()-1].destination_ID.ID = ID;
                _connectionList[_connectionList.size()-1].destination_ID.TYPE = NeuronType::output;
                _connectionList[_connectionList.size()-1].netID = this->get_ID();
                _connectionList[_connectionList.size()-1].weight = Neuron::get_calcRandWeight(_randEngine);
                _connectionList[_connectionList.size()-1].source_ID.ID = 0;
                _connectionList[_connectionList.size()-1].source_ID.TYPE = NeuronType::bias;
                _connectionList[_connectionList.size()-1].direction = ConnectionDirection::forward;
            }
            ID++;
        }
    }
    ID = 0;
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
#if defined(_DEBUG_NET_TIMING)
    __debug_timer1_end  = std::chrono::high_resolution_clock::now();
    __debug_time_span   = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer1_end - __debug_timer1_start);
    __DEBUG_NET(this,"prepareConnectionList()","end. Elapsed time: "+std::to_string(__debug_time_span.count())+" sec");
#else
    __DEBUG_NET(this,"prepareConnectionList()","end");
#endif
#endif
}
void                Net::prepareCalculationOrderList()
{
#if defined(_DEBUG_NET_CALCULATION_ORDER_LIST)
    __DEBUG_NET(this,"prepareCalculationOrderList()","begin");
#endif
    _calculationOrderList.clear();
    _calculationOrderList.reserve(_allNeuronList.size());



    // Jede Variable in "isUpdated" befindet sich im array auch am selben ort wie das Neuron die ID hat.
    // isUpdated[3] -> _allNeuronList[3]
    // isUpdated wird verwendet um bei der Reihenfolgensuche zu schauen ob das vorherige Neuron bereits berechnet wurde.
    std::vector<bool> isUpdated(_allNeuronList.size(),false);
    bool allUpdated = false;
    unsigned int counter = 0;
    while(!allUpdated)
    {
        // Durch alle Neuronen gehen und jede einzeln analysieren
        for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
        {
            counter++;
            // Dieses Neuron wurde bereits einmal berechnet
            if(isUpdated[_allNeuronList[neuron]->get_ID().ID])
                continue;

            // invertiert initialisiert. So kann bei der ersten unstimmigkeit auf false gesetzt werden.
            bool allInputsUpdated = true;

            // Durch jeden Input gehen und schauen:
            // 1) Von wo jede Verbindung kommt
            // 2) Ob die Verbindung vorwärts oder rückwärts geht. (Rückwärts = Rückkopplung)
            // 3) Ob die Neuronen, die sich am start der Verbindung befinden bereits berechnet wurden.
            for(unsigned int input=0; input<_allNeuronList[neuron]->get_inputs(); input++)
            {

                unsigned int inputID                = _allNeuronList[neuron]->get_inputID(input).ID;
                NeuronType   inputType              = _allNeuronList[neuron]->get_inputID(input).TYPE;
                ConnectionDirection inputDirection  = _allNeuronList[neuron]->get_inputConnectionDirection(_allNeuronList[neuron]->get_inputID(input));

                // 1)
                // Diese Typen sind immer auf dem aktuellen Stand und müssen nicht berücksichtigt werden.
                if(inputType == NeuronType::bias ||
                   inputType == NeuronType::input)
                    continue;

                // 2)
                // Rückwertsführende Verbindungen können noch gar nicht aktuell sein, müssen das auch nicht
                if(inputDirection == ConnectionDirection::backward)
                    continue;

                // 3)
                if(!isUpdated[inputID]) // Neuron an Quelle der Verbindung wurde noch nicht berechnet
                {
                    allInputsUpdated = false;
                    break; // Es muss nicht weiter gesucht werden, dieses Neuron kann noch nicht berechnet werden.
                }
            }
            if(!allInputsUpdated)
                continue;

            _calculationOrderList.push_back(_allNeuronList[neuron]->get_ID());
#if defined(_DEBUG_NET_CALCULATION_ORDER_LIST)
            __DEBUG_NET(this,"prepareCalculationOrderList()","couter: "+std::to_string(counter)+"\t_calculationOrderList[last] (TYPE: NeuronID) = "+Neuron::toIDString(_calculationOrderList[_calculationOrderList.size()-1]));
#endif
            isUpdated[_allNeuronList[neuron]->get_ID().ID] = true;
        }

        allUpdated = true;
        for(unsigned int updates = 0; updates < isUpdated.size(); updates++)
        {
            if(!isUpdated[updates])
                allUpdated = false;
        }
    }
#if defined(_DEBUG_NET_CALCULATION_ORDER_LIST)
    __DEBUG_NET(this,"prepareCalculationOrderList()","all updated. Neurons are calculated in this order:");
    for(unsigned int a=0; a<_calculationOrderList.size(); a++)
    {
        __DEBUG_NET(this,"prepareCalculationOrderList()","CalculationPos: "+std::to_string(a)+"\tNeuron: (ID): "+Neuron::toIDString(_calculationOrderList[a]));
    }
#endif


  /*

    _calculationOrderList.clear();
    _calculationOrderList.reserve(_allNeuronList.size());
    //std::vector<bool> isUpdated(_allNeuronList.size(),false);
    std::vector<bool> isUpdated;
    isUpdated.reserve(_allNeuronList.size());
    for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
    {
        isUpdated.push_back(false);
    }

    bool allUpdated = false;
    while(!allUpdated)
    {
        for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
        {
            bool readyToCalcCostumNeuron = true;
            if(!_allNeuronList[neuron]->isUpdated())
            {
                for(unsigned int input=0; input<_allNeuronList[neuron]->get_inputs(); input++)
                {
                    if((_allNeuronList[neuron]->get_inputID(input).TYPE == NeuronType::costum ||
                        _allNeuronList[neuron]->get_inputID(input).TYPE == NeuronType::hidden ||
                        _allNeuronList[neuron]->get_inputID(input).TYPE == NeuronType::output) &&
                        (!isUpdated[_calculationOrderList.size()-1] &&
                         _allNeuronList[neuron]->get_inputConnectionDirection(_allNeuronList[neuron]->get_inputID(input)) == true))
                    {
                        readyToCalcCostumNeuron = false;
                    }
                }
                if(readyToCalcCostumNeuron)
                {
                    _calculationOrderList.push_back(_allNeuronList[neuron]->get_ID());
#if defined(_DEBUG_NET_CONNECT)
                    __DEBUG_NET(this,"prepareCalculationOrderList()","next in order: "+Neuron::IDString(_allNeuronList[neuron]->ID()));
#endif
                    isUpdated[_calculationOrderList.size()-1] = true;
                }
            }
        }
        allUpdated = true;
        for(unsigned int neuron=0; neuron<isUpdated.size(); neuron++)
        {
            if(!isUpdated[neuron])
            {
                allUpdated = false;
            }
        }
    }
    */
#if defined(_DEBUG_NET_CALCULATION_ORDER_LIST)
    __DEBUG_NET(this,"prepareCalculationOrderList()","end");
#endif
}
void                Net::clearConnectionList()
{
    _connectionList.clear();
}
void                Net::addError(const Error &e)
{
    _errorList.push_back(e);
    _errorList[_errorList.size()-1].setNamespace("Net::"+_errorList[_errorList.size()-1].getNamespace());
    Error::print(_errorList[_errorList.size()-1]);
    #ifdef QT_APP
    emit errorOccured(this->get_ID(),_errorList[_errorList.size()-1]);
    #endif
}
bool                Net::isEqual(Net *toCompare)
{
    bool ret = true;
    ret &= this->get_inputNeurons()         == toCompare->get_inputNeurons();
    ret &= this->get_hiddenNeuronsX()       == toCompare->get_hiddenNeuronsX();
    ret &= this->get_hiddenNeuronsY()       == toCompare->get_hiddenNeuronsY();
    ret &= this->get_outputNeurons()        == toCompare->get_outputNeurons();
    ret &= this->get_costumNeurons()        == toCompare->get_costumNeurons();
    ret &= this->get_bias()                 == toCompare->get_bias();
    ret &= this->get_activationFunction()   == toCompare->get_activationFunction();
    ret &= this->get_connections()          == toCompare->get_connections();

    return ret;
}
#if defined(_DEBUG_NET_TIMING)
double Net::get_runtime()
{
    return __debug_run_time;
}
#endif
//----------ERROR
/*
std::string Net::error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max)
{
    return " parameter "+std::to_string(paramPos)+" is out of range: "+value+"     minimum is: "+min+"     maximum is: "+max;
}
std::string Net::error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string Net::error_paramOutOfRange(unsigned int paramPos,int value,int min, int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string Net::error_paramOutOfRange(unsigned int paramPos,double value,double min, double max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
void        Net::error_general(std::string function, std::string cause)
{
    throw std::runtime_error("ERROR: Net::" + function + "     " + cause);
}
void        Net::error_general(std::string function, std::runtime_error &e)
{
    error_general(function,"",e);
}
void        Net::error_general(std::string function, std::string cause, std::runtime_error &e)
{
    std::string error = "ERROR: Net::" + function + "     " + cause;
    error += "     --> "+std::string(e.what());
    throw std::runtime_error(error);
}*/
