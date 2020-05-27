#include "net.h"

Net::Net(unsigned int ID,
         QObject *parent)
    : QObject (parent)
{
    this->set_ID(ID);
    init(2,1,2,1,true,false,Activation::Sigmoid);
}

Net::Net(unsigned int ID,
         unsigned int inputs,
         unsigned int hiddenX,
         unsigned int hiddenY,
         unsigned int outputs,
         QObject *parent)
    : QObject (parent)
{
    this->set_ID(ID);
    init(inputs,hiddenX,hiddenY,outputs,true,false,Activation::Sigmoid);
}

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
            qDebug() << "~Net() error: " << e.what();
        }
    }
    /*for(int a=_allNeuronList.size()-1; a>=0; a--)
    {
        try {
            delete _allNeuronList[a];
        } catch (std::exception &e) {
            //error_general("~Net()",e.what());
            qDebug() << "~Net() error: " << e.what();
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
    if(inputs < NET_MIN_INPUTNEURONS || inputs > NET_MAX_INPUTNEURONS)
    {
        this->addError(Error("set_inputNeurons(unsigned int ["+QString::number(inputs)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(NET_MIN_INPUTNEURONS),inputs,static_cast<unsigned int>(NET_MAX_INPUTNEURONS),']')));
        return;
        //error_general("inputNeurons(unsigned int ["+QString::number(inputs)+"] )","Parameter 0 is out of range. Min: "+ QString::number(NET_MIN_INPUTNEURONS)+" Max: "+QString::number(NET_MAX_INPUTNEURONS));
    }
    if(inputs != _inputs)
    {
        emit accessLock();
        _needsConfigurationUpdate   = true;
        emit netConfigurationUpdateNeeded();
        _inputs                     = inputs;
    }
}
unsigned int        Net::get_inputNeurons()
{
    return _inputs;
}
void                Net::set_hiddenNeuronsX(unsigned int hiddenX)
{
    if(hiddenX < NET_MIN_HIDDENNEURONS_X || hiddenX > NET_MAX_HIDDENNEURONS_X)
    {
        this->addError(Error("set_hiddenNeuronsX(unsigned int ["+QString::number(hiddenX)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(NET_MIN_HIDDENNEURONS_X),hiddenX,static_cast<unsigned int>(NET_MAX_HIDDENNEURONS_X),']')));
        return;
        //error_general("hiddenNeuronsX(unsigned int ["+QString::number(hiddenX)+"] )","Parameter 0 is out of range. Min: "+ QString::number(NET_MIN_HIDDENNEURONS_X)+" Max: "+QString::number(NET_MAX_HIDDENNEURONS_X));
    }
    if(hiddenX != _hiddenX)
    {
        emit accessLock();
        _needsConfigurationUpdate   = true;
        _hiddenX                    = hiddenX;
        emit netConfigurationUpdateNeeded();
        //_hiddenNeurons              = _hiddenX * _hiddenY;
    }
}
unsigned int        Net::get_hiddenNeuronsX()
{
    return _hiddenX;
}
void                Net::set_hiddenNeuronsY(unsigned int hiddenY)
{
    if(hiddenY < NET_MIN_HIDDENNEURONS_Y || hiddenY > NET_MAX_HIDDENNEURONS_Y)
    {
        this->addError(Error("set_hiddenNeuronsY(unsigned int ["+QString::number(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(NET_MIN_HIDDENNEURONS_Y),hiddenY,static_cast<unsigned int>(NET_MAX_HIDDENNEURONS_Y),']')));
        return;
        //error_general("hiddenNeuronsY(unsigned int ["+QString::number(hiddenY)+"] )","Parameter 0 is out of range. Min: " + QString::number(NET_MIN_HIDDENNEURONS_Y)+" Max: "+QString::number(NET_MAX_HIDDENNEURONS_Y));
    }
    if(hiddenY != _hiddenY)
    {
        emit accessLock();
        _needsConfigurationUpdate   = true;
        _hiddenY                    = hiddenY;
        emit netConfigurationUpdateNeeded();
        //_hiddenNeurons              = _hiddenX * _hiddenY;
    }
}
unsigned int        Net::get_hiddenNeuronsY()
{
    return _hiddenY;
}
void                Net::set_outputNeurons(unsigned int outputs)
{
    if(outputs < NET_MIN_OUTPUTNEURONS || outputs > NET_MAX_OUTPUTNEURONS)
    {
        this->addError(Error("set_outputNeurons(unsigned int ["+QString::number(outputs)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(NET_MIN_OUTPUTNEURONS),outputs,static_cast<unsigned int>(NET_MAX_OUTPUTNEURONS),']')));
        return;
        //error_general("outputNeurons(unsigned int ["+QString::number(outputs)+"] )","Parameter 0 is out of range. Min: "+QString::number(NET_MIN_OUTPUTNEURONS)+" Max: "+QString::number(NET_MAX_OUTPUTNEURONS));
    }
    if(outputs != _outputs)
    {
        emit accessLock();
        _needsConfigurationUpdate   = true;
        _outputs                    = outputs;
        emit netConfigurationUpdateNeeded();
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
        this->addError(Error("set_neurons(unsigned int ["+QString::number(neurons)+
                             "],unsigned int ["+QString::number(hiddenNeurons)+
                             "],unsigned int ["+QString::number(outputNeurons)+
                             "],unsigned int ["+QString::number(costumNeurons)+"])",
                         QString("Its not possible like so. Parameter 1 shuld be the sum of the others") ));
        return;
        //error_general("neurons(unsigned int ["+QString::number(neurons)+"],unsigned int ["+QString::number(hiddenNeurons)+"],unsigned int ["+QString::number(outputNeurons)+"],unsigned int ["+QString::number(costumNeurons)+"],","Its not possible like so. Parameter 1 shuld be the sum of the others");
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
        emit accessLock();
        _needsConfigurationUpdate   = true;
        _bias                       = enableBias;
        emit netConfigurationUpdateNeeded();
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
        emit accessLock();
        _needsConfigurationUpdate   = true;
        _enableAverage              = enableAverage;
        emit netConfigurationUpdateNeeded();
    }
}
bool                Net::get_enableAverage()
{
    return _enableAverage;
}
void                Net::set_biasValue(double value)
{
    _needsCalculationUpdate         = true;
    _biasValue                      = value;
}
double              Net::get_biasValue()
{
    return _biasValue;
}
void                Net::set_activationFunction(Activation func)
{
    if(func != _activationFunction)
    {
        emit accessLock();
        _needsConfigurationUpdate   = true;
        _activationFunction         = func;
        emit netConfigurationUpdateNeeded();
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
                       "parameter 0 has the wrong array size: "+QString::number(genom.size()),
                       " array size should by "+QString::number(_ptr_genom.size())}));
        return;
        //error_general("genom(std::vector<double>)","parameter 0 has the wrong array size: "+QString::number(genom.size())+" array size should by "+QString::number(_ptr_genom.size()));
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
        this->addError(Error("set_input(unsigned int ["+QString::number(input)+"] , double ["+QString::number(signal)+"] )",
                       ErrorMessage::updateNetFirst()));
        return;
        //error_general("input(unsigned int ["+QString::number(input)+"] , double ["+QString::number(signal)+"]) ) ",ErrorMessage::updateNetFirst());
    }
    if(_inputs == 0)
    {
        this->addError(Error("set_input(unsigned int ["+QString::number(input)+"] , double ["+QString::number(signal)+"] )",
                       QString("There are no inputs!")));
        return;
    } else if(input > _inputs-1)
    {
        this->addError(Error("set_input(unsigned int ["+QString::number(input)+"] , double ["+QString::number(signal)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),input,static_cast<unsigned int>(_inputs-1),']')));
        return;
        //error_general("input(unsigned int ["+QString::number(input)+"] , double ["+QString::number(signal)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }

    _needsCalculationUpdate = true;
    _inputSignalList[input] = signal;
}
double              Net::get_input(unsigned int input)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_input(unsigned int ["+QString::number(input)+"] )",
                       ErrorMessage::updateNetFirst()));
        return 0;
       // error_general("input(unsigned int ["+QString::number(input)+"] ) ",ErrorMessage::updateNetFirst());
    }
    if(input > _inputs-1)
    {
        this->addError(Error("get_input(unsigned int ["+QString::number(input)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),input,static_cast<unsigned int>(_inputs-1),']')));
        return 0;
        //error_general("input(unsigned int ["+QString::number(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    return _inputSignalList[input];
}
void                Net::set_input(std::vector<double> inputList)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_input(unsigned int ["+QString::number(input)+"] )",
                       ErrorMessage::updateNetFirst()));
        return;
        //error_general("input(std::vector<double>) ",ErrorMessage::updateNetFirst());
    }
    if(static_cast<unsigned int>(inputList.size()) != _inputs)
    {
        this->addError(Error("set_input(std::vector<double> inputList)",{
                       "parameter 0 has the wrong array size: "+QString::number(inputList.size()),
                       " array size should by "+QString::number(_inputs)}));
        return;
        //error_general("input(std::vector<double>) ","parameter 0 , size of the array is wrong: ["+QString::number(inputList.size()) + "] correct size is: ["+QString::number(_inputs)+"]");
    }
    _needsCalculationUpdate = true;
    for(unsigned int a=0; a<_inputs; a++)
    {
        _inputSignalList[a] = inputList[a];
    }
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

double              Net::get_hidden(unsigned int hiddenX, unsigned int hiddenY)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_hidden(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",
                       ErrorMessage::updateNetFirst()));
        return 0;
        //error_general("hidden(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_hidden(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",
                       QString("The network has no hidden layer")));
        return 0;
        //error_general("hidden(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )","the network has no hidden layer");
    }
    if(hiddenX > _hiddenX-1)
    {
        this->addError(Error("get_hidden(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenX,static_cast<unsigned int>(_hiddenX-1),']')));
        return 0;
        //error_general("hidden(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
    }
    if(hiddenY > _hiddenY-1)
    {
        this->addError(Error("get_hidden(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenY,static_cast<unsigned int>(_hiddenY-1),']')));
        return 0;
        //error_general("hidden(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",error_paramOutOfRange((unsigned int)1,hiddenY,(unsigned int)0,_hiddenY-1));
    }

    unsigned int ID = hiddenX * _hiddenY + hiddenY;
    return _hiddenNeuronList[ID]->get_output();
}
std::vector<double>       Net::get_hiddenX(unsigned int hiddenX)// |    Alle in einer Spalte
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_hiddenX(unsigned int ["+QString::number(hiddenX)+"] )",
                       ErrorMessage::updateNetFirst()));
        return std::vector<double>();
        //error_general("hiddenX(unsigned int ["+QString::number(hiddenX)+"] )",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_hiddenX(unsigned int ["+QString::number(hiddenX)+"] )",
                       QString("The network has no hidden layer")));
        return std::vector<double>();
        //error_general("hiddenX(unsigned int ["+QString::number(hiddenX)+"] )","the network has no hidden layer");
    }
    if(hiddenX > _hiddenX-1)
    {
        this->addError(Error("get_hiddenX(unsigned int ["+QString::number(hiddenX)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenX,static_cast<unsigned int>(_hiddenX-1),']')));
        return std::vector<double>();
        //error_general("hiddenX(unsigned int ["+QString::number(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
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
        this->addError(Error("get_hiddenY(unsigned int ["+QString::number(hiddenY)+"] )",
                       ErrorMessage::updateNetFirst()));
        return std::vector<double>();
        //error_general("hiddenY(unsigned int ["+QString::number(hiddenY)+"] )",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_hiddenY(unsigned int ["+QString::number(hiddenY)+"] )",
                       QString("The network has no hidden layer")));
        return std::vector<double>();
        //error_general("hiddenY(unsigned int ["+QString::number(hiddenY)+"] )","the network has no hidden layer");
    }
    if(hiddenY > _hiddenY-1)
    {
        this->addError(Error("get_hiddenY(unsigned int ["+QString::number(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenY,static_cast<unsigned int>(_hiddenY-1),']')));
        return std::vector<double>();
        //error_general("hiddenY(unsigned int ["+QString::number(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,hiddenY,(unsigned int)0,_hiddenY-1));
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
        this->addError(Error("get_ptr_neuron_viaID(unsigned int ["+QString::number(ID)+"] )",
                       ErrorMessage::updateNetFirst()));
        return nullptr;
        //error_general("neuron_viaID(unsigned int ["+QString::number(ID)+"])",ErrorMessage::updateNetFirst());
    }
    if(_allNeuronList.size() == 0)
    {
        this->addError(Error("get_ptr_neuron_viaID(unsigned int ["+QString::number(ID)+"] )",
                       ErrorMessage::listIsEmpty("_allNeuronList")));
        return nullptr;
    }else if(ID >= static_cast<unsigned int>(_allNeuronList.size()))
    {
        this->addError(Error("get_ptr_neuron_viaID(unsigned int ["+QString::number(ID)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),ID,static_cast<unsigned int>(static_cast<unsigned int>(_allNeuronList.size())-1),']')));
        return nullptr;
        //error_general("neuron_viaID(unsigned int ["+QString::number(ID)+"] )",error_paramOutOfRange((unsigned int)0,ID,(unsigned int)0,_allNeuronList.size()));
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
        //error_general("neuron_viaID(unsigned int ["+QString::number(ID)+"] )",error_paramOutOfRange((unsigned int)0,ID,(unsigned int)0,_allNeuronList.size()));
    }/*
    if(ID.ID >= _allNeuronList.size())
    {
        error_general("neuron_viaID(NeuronID [.ID="+QString::number(ID.ID)+",.TYPE="+QString::number(ID.TYPE)+"] )",error_paramOutOfRange((unsigned int)0,ID.ID,(unsigned int)0,_allNeuronList.size()));
    }*/

    return _allNeuronList[ID.ID];
}
Neuron             *Net::get_ptr_hiddenNeuron(unsigned int hiddenX, unsigned int hiddenY)
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_ptr_hiddenNeuron(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",
                       ErrorMessage::updateNetFirst()));
        return nullptr;
        //error_general("hiddenNeuron(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"])",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_ptr_hiddenNeuron(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",
                       QString("The network has no hidden layer")));
        return nullptr;
        //error_general("hiddenNeuron(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )","the network has no hidden layer");
    }
    if(hiddenX > _hiddenX-1)
    {
        this->addError(Error("get_ptr_hiddenNeuron(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenX,static_cast<unsigned int>(_hiddenX-1),']')));
        return nullptr;
        //error_general("hidden(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
    }
    if(hiddenY > _hiddenY-1)
    {
        this->addError(Error("get_ptr_hiddenNeuron(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenY,static_cast<unsigned int>(_hiddenY-1),']')));
        return nullptr;
        //error_general("hidden(unsigned int ["+QString::number(hiddenX)+"] , unsigned int ["+QString::number(hiddenY)+"] )",error_paramOutOfRange((unsigned int)1,hiddenY,(unsigned int)0,_hiddenY-1));
    }

    unsigned int ID = hiddenX * _hiddenY + hiddenY;
    return _hiddenNeuronList[ID];
}
std::vector<Neuron*>      Net::get_hiddenNeuronX_ptr(unsigned int hiddenX) // |    Alle in einer Spalte
{
    if(_needsConfigurationUpdate)
    {
        this->addError(Error("get_hiddenNeuronX_ptr(unsigned int ["+QString::number(hiddenX)+"] )",
                       ErrorMessage::updateNetFirst()));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronX(unsigned int ["+QString::number(hiddenX)+"] ) ",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_hiddenNeuronX_ptr(unsigned int ["+QString::number(hiddenX)+"] )",
                       QString("The network has no hidden layer")));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronX(unsigned int ["+QString::number(hiddenX)+"] )","the network has no hidden layer");
    }
    if(hiddenX >= _hiddenX)
    {
        this->addError(Error("get_hiddenNeuronX_ptr(unsigned int ["+QString::number(hiddenX)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenX,static_cast<unsigned int>(_hiddenX-1),']')));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronX(unsigned int ["+QString::number(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
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
        this->addError(Error("get_hiddenNeuronY_ptr(unsigned int ["+QString::number(hiddenY)+"] )",
                       ErrorMessage::updateNetFirst()));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronX(unsigned int ["+QString::number(hiddenX)+"] ) ",ErrorMessage::updateNetFirst());
    }
    if(!_hasHiddenLayer)
    {
        this->addError(Error("get_hiddenNeuronY_ptr(unsigned int ["+QString::number(hiddenY)+"] )",
                       QString("The network has no hidden layer")));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronY(unsigned int ["+QString::number(hiddenY)+"] )","the network has no hidden layer");
    }
    if(hiddenY >= _hiddenY)
    {
        this->addError(Error("get_hiddenNeuronY_ptr(unsigned int ["+QString::number(hiddenY)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),hiddenY,static_cast<unsigned int>(_hiddenY-1),']')));
        return std::vector<Neuron*>();
        //error_general("hiddenNeuronX(unsigned int ["+QString::number(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
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
                       QString("The network has no hidden layer")));
        return std::vector<std::vector<Neuron*> >();
        //error_general("hiddenNeuron()","the network has no hidden layer");
    }
    if(_hiddenX * _hiddenY != static_cast<unsigned int>(_hiddenNeuronList.size()))
    {
        this->addError(Error("get_hiddenNeuron_ptr()",
                       QString("something went wrong: _hiddenX * _hiddenY != _hiddenNeuronList.size()")));
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
                       QString("There are no costum neurons")));
        return nullptr;
        //error_general("costumNeuron(NeuronID ["+Neuron::IDString(ID)+"] )"," There are no costum neurons");
    }
    if(ID.TYPE != costum)
    {
        this->addError(Error("get_ptr_costumNeuron("+Neuron::toIDString(ID)+")",
                       QString("This is not a costum neuron")));
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
        this->addError(Error("get_ptr_outputNeuron(unsigned int ["+QString::number(output)+"] )",
                       ErrorMessage::updateNetFirst()));
        return nullptr;
        //error_general("outputNeuron(unsigned int ["+QString::number(output)+"]) ",ErrorMessage::updateNetFirst());
    }
    if(output >= _outputs)
    {
        this->addError(Error("get_ptr_outputNeuron(unsigned int ["+QString::number(output)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),output,_outputs-1,']')));
        return nullptr;
        //error_general("outputNeuron(unsigned int ["+QString::number(output)+"] )",error_paramOutOfRange((unsigned int)0,output,(unsigned int)0,_outputs-1));
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
        this->addError(Error("get_output(unsigned int ["+QString::number(output)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),output,static_cast<unsigned int>(_outputs-1),']')));
        return 0;
        //error_general("output(unsigned int ["+QString::number(output)+"] )",error_paramOutOfRange((unsigned int)0,output,(unsigned int)0,_outputs-1));
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
   /* while(!allUpdated)
    {
        for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
        {
            bool readyToCalcCostumNeuron = true;
            if(!_allNeuronList[neuron]->isUpdated())
            {
                for(unsigned int input=0; input<_allNeuronList[neuron]->inputs(); input++)
                {
                    if((_allNeuronList[neuron]->inputID(input).TYPE == NeuronType::costum ||
                        _allNeuronList[neuron]->inputID(input).TYPE == NeuronType::hidden ||
                        _allNeuronList[neuron]->inputID(input).TYPE == NeuronType::output) &&
                        (!_allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->isUpdated() &&
                         _allNeuronList[neuron]->inputConnectionDirection(_allNeuronList[neuron]->inputID(input)) == true))
                    {
                        readyToCalcCostumNeuron = false;
                    }
                }
                if(readyToCalcCostumNeuron)
                {
#if defined(_DEBUG_NET_RUN)
                    __DEBUG_NET(this,"run()","calculates neuron ID: "+QString::number(neuron));
#endif
                    _allNeuronList[neuron]->run();
                }
            }
        }
        allUpdated = true;
        for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
        {
            if(!_allNeuronList[neuron]->isUpdated())
            {
                allUpdated = false;
            }
        }
    }*/

#if defined(_DEBUG_NET_TIMING)
    __debug_timer1_end          = std::chrono::high_resolution_clock::now();
    __debug_time_span           = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer1_end - __debug_timer1_start);
    __debug_run_time            = __debug_time_span.count()*1000;
    __debug_average_run_time    = __debug_average_run_time * 0.6 + 0.4 * __debug_run_time;
#endif
#if defined(_DEBUG_NET_RUN)
#if defined(_DEBUG_NET_TIMING)
    __DEBUG_NET(this,"run()","all updated, finish. Elapsed time: "+QString::number(__debug_run_time)+" ms");
#else
    __DEBUG_NET(this,"run()","all updated, finish");
#endif
#endif
    _needsCalculationUpdate = false;
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
                           QString(e.what())));
            return;
            //error_general("updateNetConfiguration()",e.what());
        }
    }
    _outputNeurons  = _outputs;
    _hiddenNeurons  = _hiddenX * _hiddenY;

    _neurons        = _hiddenNeurons + _outputNeurons + _costumNeurons;

    _connectionList.clear();
    prepareConnectionList();



    _costumConnections  = _costumConnectionList.size();
    _connections        = _connectionList.size() + _costumConnectionList.size();

    _inputSignalList.clear();
    _hiddenNeuronList.clear();
    _outputNeuronList.clear();
    _costumNeuronList.clear();
    _allNeuronList.clear();
    _ptr_genom.clear();

#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","inputs:         "+QString::number(_inputs));
    __DEBUG_NET(this,"updateNetConfiguration()","hiddenX:        "+QString::number(_hiddenX));
    __DEBUG_NET(this,"updateNetConfiguration()","hiddenY:        "+QString::number(_hiddenY));
    __DEBUG_NET(this,"updateNetConfiguration()","outputs:        "+QString::number(_outputs));
    __DEBUG_NET(this,"updateNetConfiguration()","costum:         "+QString::number(_costumNeurons));
    __DEBUG_NET(this,"updateNetConfiguration()","connections:    "+QString::number(_connections));
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
        Neuron * tmp_neuron = new Neuron(this);
        tmp_neuron->set_activationFunction(_activationFunction);
        tmp_neuron->set_enableAverage(_enableAverage);
        tmp_neuron->set_ID(neuronID);
        QObject::connect(tmp_neuron,&Neuron::errorOccured,this,&Net::onNeuronError);
        _allNeuronList[neuronID] = tmp_neuron;
#if defined(_DEBUG_NET_TIMING)
        __debug_timer1_end  = std::chrono::high_resolution_clock::now();
        __debug_time_span   = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer1_end - __debug_timer1_start);
        if(__debug_time_span.count() >= debugInterval)
        {
            secCounter+=debugInterval;
            __debug_timer1_start = __debug_timer1_end;
            __DEBUG_NET(this,"updateNetConfiguration()","Creating Neurons... "+QString::number(neuronID)+" of: "+QString::number(_neurons)+" generated. Time elapsed: "+QString::number(secCounter)+" sec");
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
        _costumNeuronList[ID-_costumNeurons] = _allNeuronList[ID];
        _costumNeuronList[ID-_costumNeurons]->set_TYPE(NeuronType::costum);
    }


#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","connect "+QString::number(_connections)+" connections");
#endif
#if defined(_DEBUG_NET_TIMING)
    __debug_timer1_start = std::chrono::high_resolution_clock::now();
    secCounter    = 0;
#endif
    //- Connect the inputs
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    for(unsigned int connection=0; connection<_connections; connection++)
    {
        __DEBUG_NET(this,"updateNetConfiguration()","connection["+QString::number(connection)+"] "+Neuron::toConnectionString(_connectionList[connection]));
    }
#endif
    for(unsigned int connection=0; connection<_connections; connection++)
    {
        if(connection < _connectionList.size())
            set_ptr_intern_connectNeuron(&_connectionList[connection]);
        else
            set_ptr_intern_connectNeuron(&_costumConnectionList[connection]);
#if defined(_DEBUG_NET_TIMING)
        __debug_timer1_end  = std::chrono::high_resolution_clock::now();
        __debug_time_span   = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer1_end - __debug_timer1_start);
        if(__debug_time_span.count() >= debugInterval)
        {
            secCounter+=debugInterval;
            __debug_timer1_start = std::chrono::high_resolution_clock::now();
            __DEBUG_NET(this,"updateNetConfiguration()","connecting... "+QString::number(connection)+" of: "+QString::number(_connections)+" connection. Time elapsed: "+QString::number(secCounter)+" sec");
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
    __DEBUG_NET(this,"prepareConnectionList()","end. Elapsed time: "+QString::number(__debug_time_span.count())+" sec");
#endif
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","finish");
#endif
    emit netConfigurationUpdated();
    emit accessUnlock();
}
void                Net::addConnection(NeuronID fromNeuron,NeuronID toNeuron,ConnectionDirection direction)
{
    Connection con;
    con.netID = this->get_ID();
    con.source_ID = fromNeuron;
    con.destination_ID = toNeuron;
    con.direction = direction;
    con.weight = Neuron::get_calcRandWeight(this->_randEngine);
    this->addConnection(con);
    //unsigned int maxNeuronID = _allNeuronList.size();
#if defined(_DEBUG_NET_CONNECT)
    __DEBUG_NET(this,"connectNeuronViaID(unsigned int ["+QString::number(fromNeuron)+"],unsigned int ["+QString::number(toNeuron)+"],ConnectionDirection ["+Neuron::directionString(direction)+"])","begin");
    __DEBUG_NET(this,"connectNeuronViaID(...)","connecting Neuron: "+QString::number(fromNeuron)+" to: "+QString::number(toNeuron));
    __DEBUG_NET(this,"connectNeuronViaID(...)","maxNeuronID: "+QString::number(maxNeuronID));
#endif
  /*  if(fromNeuron >= maxNeuronID)
    {
        this->addError(Error("connectNeuronViaID(unsigned int ["+QString::number(fromNeuron)+"] ,"+
                             "unsigned int ["+QString::number(toNeuron)+"] ,"+
                             "ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),fromNeuron,static_cast<unsigned int>(maxNeuronID-1),']')));
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuronViaID(...)","Problem: "+error_paramOutOfRange(0,QString::number(fromNeuron),QString::number(0),QString::number(maxNeuronID)));
#endif
        return 0;
    }
    if(toNeuron > maxNeuronID)
    {
        this->addError(Error("connectNeuronViaID(unsigned int ["+QString::number(fromNeuron)+"] ,"+
                             "unsigned int ["+QString::number(toNeuron)+"] ,"+
                             "ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),toNeuron,static_cast<unsigned int>(maxNeuronID-1),']')));
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuronViaID(...)","Problem: "+error_paramOutOfRange(1,QString::number(toNeuron),QString::number(0),QString::number(maxNeuronID)));
#endif
        return 0;
    }*/
    /*if(_allNeuronList[toNeuron]->connectInput(_allNeuronList[fromNeuron],direction))
    {
        _costumConnections++;
    }*/
    /*} catch (std::runtime_error &e) {
        this->addError(Error("connectNeuronViaID(unsigned int ["+QString::number(fromNeuron)+"] ,"+
                             "unsigned int ["+QString::number(toNeuron)+"] ,"+
                             "ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                             QString(e.what())));
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuronViaID(...)","Problem: "+std::string(e.what()));
#endif
        return 0;
    }*/
#if defined(_DEBUG_NET_CONNECT)
    __DEBUG_NET(this,"connectNeuronViaID(unsigned int ["+QString::number(fromNeuron)+"],unsigned int ["+QString::number(toNeuron)+"],ConnectionDirection ["+Neuron::directionString(direction)+"])","end");
#endif
}
void                Net::addConnection(Connection connection)
{
    //bool ret = set_ptr_intern_connectNeuron(connection);
    //prepareCalculationOrderList();
    emit accessLock();
    _costumConnectionList.push_back(connection);
    _needsConfigurationUpdate = true;
    emit netConfigurationUpdateNeeded();

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
    emit accessLock();
    _connectionList = connections;
    emit netConfigurationUpdateNeeded();
    _needsConfigurationUpdate = true;
}
std::vector<Connection>  Net::get_connectionList()
{
    return _connectionList;
}


NeuronID            Net::addNeuron()
{
    return addNeuron(new Neuron());
}
NeuronID            Net::addNeuron(Neuron *neuron)
{
    if(neuron == nullptr)
    {
        addError(Error("addNeuron(Neuron *neuron)",QString("neuron == nullptr")));
        return NeuronID();
    }
    _costumNeuronList.push_back(neuron);
    _costumNeuronList[_costumNeuronList.size()-1]->set_ID(_allNeuronList.size());
    _costumNeuronList[_costumNeuronList.size()-1]->set_TYPE(NeuronType::costum);
    _allNeuronList.push_back(_costumNeuronList[_costumNeuronList.size()-1]);
    _costumNeurons++;
    return _costumNeuronList[_costumNeuronList.size()-1]->get_ID();
}

QString             Net::toString()
{
    QString separator = "\n";
    QString buffer = "Net: ["+QString::number(this->get_ID())+"]"+separator;
    buffer += "Neurons: "+separator;
    buffer += "\tInputs:           "+QString::number(this->get_inputNeurons())+separator;
    buffer += "\tHiddenlayers:     "+QString::number(this->get_hiddenNeuronsX())+separator;
    buffer += "\tNeuronsPerLayer:  "+QString::number(this->get_hiddenNeuronsY())+separator;
    buffer += "\tCostumneurons:    "+QString::number(this->get_costumNeurons())+separator;
    buffer += "\tOutputneurons:    "+QString::number(this->get_outputNeurons())+separator;
    buffer += "--------------------------------------------------------"+separator;
    buffer += "\tTotal:            "+QString::number(this->get_neurons())+separator+separator;

    buffer += "Connections:"+separator;
    buffer += "\tNormal:           "+QString::number(this->get_connections()-this->get_costumConnections())+separator;
    buffer += "\tCostum:           "+QString::number(this->get_costumConnections())+separator;
    buffer += "--------------------------------------------------------"+separator;
    buffer += "\tTotal:            "+QString::number(this->get_connections())+separator+separator;

    buffer += "Activation: "+Neuron::toActivationString(this->get_activationFunction())+separator;
    buffer += "Bias:       ";
    if(this->get_bias())
        buffer+= "true\tvalue: "+QString::number(this->get_biasValue())+separator;
    else
        buffer+="false"+separator;

#ifdef _DEBUG_NET_TIMING
    buffer += "Calculation time:   "+QString::number(__debug_average_run_time)+"ms"+separator;
#endif

    return buffer;
}
QStringList         Net::toStringList()
{
    QStringList list;
    QString separator = "\n";
    QString buff = this->toString();
    while(buff.indexOf(separator) != -1)
    {
        list.push_back(buff.mid(0,buff.indexOf(separator)));
        buff=buff.mid(buff.indexOf(separator)+1,buff.size());
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
        return Error("get_error(unsigned int ["+QString::number(index)+"] )",
                     ErrorMessage::listIsEmpty("_errorList"));
    }
    if(index >= static_cast<unsigned int>(allErrors))
    {
        return Error("get_error(unsigned int ["+QString::number(index)+"] )",
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
    list.append(_errorList);
    for(unsigned int a=0; a<this->_allNeuronList.size(); a++)
    {
        list.append(_allNeuronList[a]->get_errorList());
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

void                Net::onNeuronError(NeuronID ID,Error &e)
{
    e.setNamespace(Neuron::toIDString(ID)+" :: "+e.getNamespace());
    emit errorOccured(this->get_ID(),e);
}
void Net::init(unsigned int inputs,
          unsigned int hiddenX,
          unsigned int hiddenY,
          unsigned int outputs,
          bool enableBias,
          bool enableAverage,
          Activation func)
{
    emit accessLock();
    time_t now                  = time(nullptr);
    struct tm *currentTime      = localtime(&now);
    this->set_seed(unsigned(currentTime->tm_min+currentTime->tm_sec+currentTime->tm_sec+clock()));

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
       /* error_general("Net(unsigned int ["+QString::number(inputs)+
                      "] , unsigned int ["+QString::number(hiddenX)+
                      "] , unsigned int ["+QString::number(hiddenY)+
                      "] , unsigned int ["+QString::number(outputs)+
                      "] , bool ["+QString::number(enableBias)+
                      "] , bool ["+QString::number(enableAverage)+
                      "] , Activation ["+QString::number(func)+"])",e);*/
        this->addError(Error("init()",{"unsigned int ["+QString::number(inputs)+"]",
                           " , unsigned int ["+QString::number(hiddenX)+"]",
                           " , unsigned int ["+QString::number(hiddenY)+"]",
                           " , unsigned int ["+QString::number(outputs)+"]",
                           " , bool ["+QString::number(enableBias)+"]",
                           " , bool ["+QString::number(enableAverage)+"]",
                           " , Activation ["+QString::number(func)+"])",
                           "error: "+QString(e.what())}));
        return;
    }
    this->set_biasValue(1.0);
   // this->updateNetConfiguration();
    emit netConfigurationUpdateNeeded();
    _needsConfigurationUpdate = true;
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
    __DEBUG_NET(this,"connectNeuron(Connection ["+Neuron::connectionString(*connection)+"])","begin");
#endif
    if(connection->destination_ID.ID >= _allNeuronList.size())
    {
        this->addError(Error("set_ptr_intern_connectNeuron(Connection *ptr)",{
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),connection->destination_ID.ID,static_cast<unsigned int>(_allNeuronList.size()-1),']'),
                       Neuron::toConnectionString(*connection)}));
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuron(...)","destination neuron ID: "+QString::number(connection->destination_ID.ID)+" not found");
#endif
        return 0;
    }
    if((connection->source_ID.ID >= _allNeuronList.size()) && (connection->source_ID.TYPE != NeuronType::input))
    {
        this->addError(Error("set_ptr_intern_connectNeuron(Connection *ptr)",{
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),connection->source_ID.ID,static_cast<unsigned int>(_allNeuronList.size()-1),']'),
                       Neuron::toConnectionString(*connection)}));
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuron(...)","source neuron ID: "+QString::number(connection->source_ID.ID)+" not found");
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
            __DEBUG_NET(this,"connectNeuron(...)","unknown source Neuron TYPE: "+Neuron::typeString(connection->source_ID.TYPE));
#endif
            return 0;
        }
    }
    _allNeuronList[connection->destination_ID.ID]->set_weight(connection->source_ID,connection->weight);
#if defined(_DEBUG_NET_CONNECT)
    __DEBUG_NET(this,"connectNeuron(Connection ["+Neuron::connectionString(*connection)+"])","end");
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
    __DEBUG_NET(this,"prepareConnectionList()","end. Elapsed time: "+QString::number(__debug_time_span.count())+" sec");
#else
    __DEBUG_NET(this,"prepareConnectionList()","end");
#endif
#endif
}
void                Net::prepareCalculationOrderList()
{
#if defined(_DEBUG_NET_CONNECT)
    __DEBUG_NET(this,"prepareCalculationOrderList()","begin");
#endif
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
#if defined(_DEBUG_NET_CONNECT)
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
    emit errorOccured(this->get_ID(),_errorList[_errorList.size()-1]);
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
    return " parameter "+QString::number(paramPos)+" is out of range: "+value+"     minimum is: "+min+"     maximum is: "+max;
}
std::string Net::error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max)
{
    return error_paramOutOfRange(paramPos,QString::number(value),QString::number(min),QString::number(max));
}
std::string Net::error_paramOutOfRange(unsigned int paramPos,int value,int min, int max)
{
    return error_paramOutOfRange(paramPos,QString::number(value),QString::number(min),QString::number(max));
}
std::string Net::error_paramOutOfRange(unsigned int paramPos,double value,double min, double max)
{
    return error_paramOutOfRange(paramPos,QString::number(value),QString::number(min),QString::number(max));
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
