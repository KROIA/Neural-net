#include "net.h"


Net::Net(unsigned int ID)
{
    this->ID(ID);
    init(2,1,2,1,true,false,Activation::Sigmoid);
}

Net::Net(unsigned int ID,
         unsigned int inputs,
         unsigned int hiddenX,
         unsigned int hiddenY,
         unsigned int outputs)
{
    this->ID(ID);
    init(inputs,hiddenX,hiddenY,outputs,true,false,Activation::Sigmoid);
}

Net::Net(unsigned int ID,
         unsigned int inputs,
         unsigned int hiddenX,
         unsigned int hiddenY,
         unsigned int outputs,
         bool enableBias,
         bool enableAverage,
         Activation func)
{
    this->ID(ID);
    init(inputs,hiddenX,hiddenY,outputs,enableBias,enableAverage,func);
}
void Net::init(unsigned int inputs,
          unsigned int hiddenX,
          unsigned int hiddenY,
          unsigned int outputs,
          bool enableBias,
          bool enableAverage,
          Activation func)
{
    _randEngine = std::default_random_engine(rand()%100 /*+ ti->tm_hour+ti->tm_min+ti->tm_sec*/);
    _updateNetConfiguration = true;
    _activationFunction = Activation::Sigmoid;
    _inputs = NET_MIN_INPUTNEURONS;
    _hiddenX = NET_MIN_HIDDENNEURONS_X;
    _hiddenY = NET_MIN_HIDDENNEURONS_Y;
    _hiddenNeurons = _hiddenX*_hiddenY;
    _outputs = NET_MIN_OUTPUTNEURONS;
    _outputNeurons = _outputs;
    _costumNeurons = 0;
    _connections = 0;
    costumConnections(0);
    _bias = true;
    _enableAverage = false;
   // this->ID(rand() % 30000);
    try {
        this->outputNeurons(outputs);
        this->hiddenNeuronsY(hiddenY);
        this->hiddenNeuronsX(hiddenX);
        this->inputNeurons(inputs);
        this->bias(enableBias);
        this->enableAverage(enableAverage);
        activationFunction(func);
    } catch (std::runtime_error &e) {
        error_general("Net(unsigned int ["+std::to_string(inputs)+
                      "] , unsigned int ["+std::to_string(hiddenX)+
                      "] , unsigned int ["+std::to_string(hiddenY)+
                      "] , unsigned int ["+std::to_string(outputs)+
                      "] , bool ["+std::to_string(enableBias)+
                      "] , bool ["+std::to_string(enableAverage)+
                      "] , Activation ["+std::to_string(func)+"])",e);
    }
    biasValue(1.0);
    updateNetConfiguration();

}
Net::~Net()
{
    for(unsigned int a=_allNeuronList.size(); a>0; a--)
    {
        try {
            delete _allNeuronList[a-1];
        } catch (std::exception &e) {
            error_general("~Net()",e.what());
        }
    }
    _inputSignalList.clear();
    _hiddenNeuronList.clear();
    _outputNeuronList.clear();
    _costumNeuronList.clear();
    _allNeuronList.clear();
    _ptr_genom.clear();
}
void                Net::ID(unsigned int ID)
{
    _ID = ID;
}
unsigned int        Net::ID()
{
    return _ID;
}

void                Net::inputNeurons(unsigned int inputs)
{
    if(inputs < NET_MIN_INPUTNEURONS || inputs > NET_MAX_INPUTNEURONS)
    {
        error_general("inputNeurons(unsigned int ["+std::to_string(inputs)+"] )","Parameter 0 is out of range. Min: "+ std::to_string(NET_MIN_INPUTNEURONS)+" Max: "+std::to_string(NET_MAX_INPUTNEURONS));
    }
    if(inputs != _inputs)
    {
        _update                 = true;
        _updateNetConfiguration = true;
        _inputs                 = inputs;
    }
}
unsigned int        Net::inputNeurons()
{
    return _inputs;
}
void                Net::hiddenNeuronsX(unsigned int hiddenX)
{
    if(hiddenX < NET_MIN_HIDDENNEURONS_X || hiddenX > NET_MAX_HIDDENNEURONS_X)
    {
        error_general("hiddenNeuronsX(unsigned int ["+std::to_string(hiddenX)+"] )","Parameter 0 is out of range. Min: "+ std::to_string(NET_MIN_HIDDENNEURONS_X)+" Max: "+std::to_string(NET_MAX_HIDDENNEURONS_X));
    }
    if(hiddenX != _hiddenX)
    {
        _update                 = true;
        _updateNetConfiguration = true;
        _hiddenX                = hiddenX;
        _hiddenNeurons          = _hiddenX * _hiddenY;
    }
}
unsigned int        Net::hiddenNeuronsX()
{
    return _hiddenX;
}
void                Net::hiddenNeuronsY(unsigned int hiddenY)
{
    if(hiddenY < NET_MIN_HIDDENNEURONS_Y || hiddenY > NET_MAX_HIDDENNEURONS_Y)
    {
        error_general("hiddenNeuronsY(unsigned int ["+std::to_string(hiddenY)+"] )","Parameter 0 is out of range. Min: " + std::to_string(NET_MIN_HIDDENNEURONS_Y)+" Max: "+std::to_string(NET_MAX_HIDDENNEURONS_Y));
    }
    if(hiddenY != _hiddenY)
    {
        _update                 = true;
        _updateNetConfiguration = true;
        _hiddenY                = hiddenY;
        _hiddenNeurons          = _hiddenX * _hiddenY;
    }
}
unsigned int        Net::hiddenNeuronsY()
{
    return _hiddenY;
}
void                Net::outputNeurons(unsigned int outputs)
{
    if(outputs < NET_MIN_OUTPUTNEURONS || outputs > NET_MAX_OUTPUTNEURONS)
    {
        error_general("outputNeurons(unsigned int ["+std::to_string(outputs)+"] )","Parameter 0 is out of range. Min: "+std::to_string(NET_MIN_OUTPUTNEURONS)+" Max: "+std::to_string(NET_MAX_OUTPUTNEURONS));
    }
    if(outputs != _outputs)
    {
        _update                 = true;
        _updateNetConfiguration = true;
        _outputs                = outputs;
        _outputNeurons          = outputs;
    }
}
unsigned int        Net::outputNeurons()
{
    return _outputs;
}
void                Net::costumNeurons(unsigned int costum)
{
    _costumNeurons = costum;
}
unsigned int        Net::costumNeurons()
{
    return _costumNeurons;
}
unsigned int        Net::connections()
{
    return _connections;
}
unsigned int        Net::costumConnections()
{
    return _costumConnections;
}
void                Net::costumConnections(unsigned int connections)
{
    _costumConnections = connections;
}
void                Net::neurons(unsigned int neurons,unsigned int hiddenNeurons,unsigned int outputNeurons,unsigned int costumNeurons)
{
    _neurons        = neurons;
    _hiddenNeurons  = hiddenNeurons;
    _outputNeurons  = outputNeurons;
    _costumNeurons  = costumNeurons;
    if(_hiddenNeurons+_outputNeurons+_costumNeurons != _neurons)
    {
        error_general("neurons(unsigned int ["+std::to_string(neurons)+"],unsigned int ["+std::to_string(hiddenNeurons)+"],unsigned int ["+std::to_string(outputNeurons)+"],unsigned int ["+std::to_string(costumNeurons)+"],","Its not possible like so. Parameter 1 shuld be the sum of the others");
    }
}
void                Net::bias(bool enableBias)
{
    if(enableBias != _bias)
    {
        _update                 = true;
        _updateNetConfiguration = true;
        _bias                   = enableBias;
    }
}
bool                Net::bias()
{
    return _bias;
}
void                Net::enableAverage(bool enableAverage)
{
    if(enableAverage != _enableAverage)
    {
        _update                 = true;
        _updateNetConfiguration = true;
        _enableAverage          = enableAverage;
    }
}
bool                Net::enableAverage()
{
    return _enableAverage;
}
void                Net::biasValue(float value)
{
    _update             = true;
    _biasValue          = value;
}
float               Net::biasValue()
{
    return _biasValue;
}
void                Net::activationFunction(Activation func)
{
    _activationFunction = func;
    _update             = true;
}
Activation          Net::activationFunction()
{
    return _activationFunction;
}
bool                Net::noHiddenLayer()
{
    if(_updateNetConfiguration)
    {
        error_general("noHiddenLayer() ",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    return _noHiddenLayer;
}

void                Net::randomGenom()
{
    for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
    {
        _allNeuronList[neuron]->randWeight();
    }
}
void                Net::genom(std::vector<float> genom)
{
    if(_updateNetConfiguration)
    {
        error_general("genom(std::vector<float>) ",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    if(genom.size() != _connections)
    {
        error_general("genom(std::vector<float>)","parameter 0 has the wrong array size: "+std::to_string(genom.size())+" array size should by "+std::to_string(_ptr_genom.size()));
    }
    unsigned int genIndex = 0;
    for(unsigned int ID=0; ID<_allNeuronList.size(); ID++)
    {
        for(unsigned int weight=0; weight<_allNeuronList[ID]->inputs(); weight++)
        {
            _allNeuronList[ID]->weight(weight,genom[genIndex]);
            genIndex++;
        }
    }
}
std::vector<float>  Net::genom()
{
    std::vector<float>  tmpGen;
    tmpGen.reserve(_connections);
    for(unsigned int ID=0; ID<_allNeuronList.size(); ID++)
    {
        for(unsigned int weight=0; weight<_allNeuronList[ID]->inputs(); weight++)
        {
            tmpGen.push_back(_allNeuronList[ID]->weight(weight));
        }
    }
    return tmpGen;
}
std::vector<float*>  *Net::ptr_genom()
{
    return &_ptr_genom;
}
unsigned int        Net::genomsize()
{
    return _ptr_genom.size();
}

void                Net::input(unsigned int input, float signal)
{
    if(input > _inputs-1)
    {
        error_general("input(unsigned int ["+std::to_string(input)+"] , float ["+std::to_string(signal)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    if(_updateNetConfiguration)
    {
        error_general("input(unsigned int ["+std::to_string(input)+"] , float ["+std::to_string(signal)+"]) ) ",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    _update                 = true;
    _inputSignalList[input] = signal;
}
float               Net::input(unsigned int input)
{
    if(input > _inputs-1)
    {
        error_general("input(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    if(_updateNetConfiguration)
    {
        error_general("input(unsigned int ["+std::to_string(input)+"] ) ",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    return _inputSignalList[input];
}
void                Net::input(std::vector<float> inputList)
{
    if(inputList.size() != _inputs)
    {
        error_general("input(std::vector<float>) ","parameter 0 , size of the array is wrong: ["+std::to_string(inputList.size()) + "] correct size is: ["+std::to_string(_inputs)+"]");
    }
    if(_updateNetConfiguration)
    {
        error_general("input(std::vector<float>) ",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    _update = true;
    for(unsigned int a=0; a<_inputs; a++)
    {
        _inputSignalList[a] = inputList[a];
    }
}
std::vector<float>  Net::input()
{
    return _inputSignalList;
}

float               Net::hidden(unsigned int hiddenX, unsigned int hiddenY)
{
    if(_noHiddenLayer)
    {
        error_general("hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )","the network has no hidden layer");
    }
    if(hiddenX > _hiddenX-1)
    {
        error_general("hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
    }
    if(hiddenY > _hiddenY-1)
    {
         error_general("hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)1,hiddenY,(unsigned int)0,_hiddenY-1));
    }
    if(_updateNetConfiguration)
    {
        error_general("hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    unsigned int ID = hiddenX * _hiddenY + hiddenY;
    return _hiddenNeuronList[ID]->output();
}
std::vector<float>  Net::hiddenX(unsigned int hiddenX)// |    Alle in einer Spalte
{
    if(_noHiddenLayer)
    {
        error_general("hiddenX(unsigned int ["+std::to_string(hiddenX)+"] )","the network has no hidden layer");
    }
    if(hiddenX > _hiddenX-1)
    {
        error_general("hiddenX(unsigned int ["+std::to_string(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
    }
    if(_updateNetConfiguration)
    {
        error_general("hiddenX(unsigned int ["+std::to_string(hiddenX)+"] )",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    std::vector<float> ret(_hiddenY,0);
    unsigned int ID = hiddenX * _hiddenY;
    for(unsigned int y=0; y<_hiddenY; y++)
    {
        ret[y] = _hiddenNeuronList[ID]->output();
        ID++;
    }
    return ret;
}
std::vector<float>  Net::hiddenY(unsigned int hiddenY)// --   Alle in einer Reihe
{
    if(_noHiddenLayer)
    {
        error_general("hiddenY(unsigned int ["+std::to_string(hiddenY)+"] )","the network has no hidden layer");
    }
    if(hiddenY > _hiddenY-1)
    {
        error_general("hiddenY(unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,hiddenY,(unsigned int)0,_hiddenY-1));
    }
    if(_updateNetConfiguration)
    {
        error_general("hiddenY(unsigned int ["+std::to_string(hiddenY)+"] )",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    std::vector<float> ret(_hiddenX,0);
    unsigned int ID = hiddenY;
    for(unsigned int x=0; x<_hiddenX; x++)
    {
        ret[x] = _hiddenNeuronList[ID]->output();
        ID+=_hiddenY;
    }
    return ret;
}

Neuron             *Net::neuron_viaID(unsigned int ID)
{
    if(ID >= _allNeuronList.size())
    {
        error_general("neuron_viaID(unsigned int ["+std::to_string(ID)+"] )",error_paramOutOfRange((unsigned int)0,ID,(unsigned int)0,_allNeuronList.size()));
    }
    if(_updateNetConfiguration)
    {
        error_general("neuron_viaID(unsigned int ["+std::to_string(ID)+"])",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    return _allNeuronList[ID];
}
Neuron             *Net::neuron_viaID(NeuronID ID)
{
    if(ID.ID >= _allNeuronList.size())
    {
        error_general("neuron_viaID(NeuronID [.ID="+std::to_string(ID.ID)+",.TYPE="+std::to_string(ID.TYPE)+"] )",error_paramOutOfRange((unsigned int)0,ID.ID,(unsigned int)0,_allNeuronList.size()));
    }
    if(_updateNetConfiguration)
    {
        error_general("neuron_viaID(NeuronID ["+Neuron::neuronIDString(ID)+"] )",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    return _allNeuronList[ID.ID];
}
Neuron             *Net::hiddenNeuron(unsigned int hiddenX, unsigned int hiddenY)
{
    if(_noHiddenLayer)
    {
        error_general("hiddenNeuron(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )","the network has no hidden layer");
    }
    if(hiddenX > _hiddenX-1)
    {
        error_general("hiddenNeuron(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
    }
    if(hiddenY > _hiddenY-1)
    {
        error_general("hiddenNeuron(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)1,hiddenY,(unsigned int)0,_hiddenY-1));
    }
    if(_updateNetConfiguration)
    {
        error_general("hiddenNeuron(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"])",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    unsigned int ID = hiddenX * _hiddenY + hiddenY;
    return _hiddenNeuronList[ID];
}
std::vector<Neuron*> Net::hiddenNeuronX(unsigned int hiddenX) // |    Alle in einer Spalte
{
    if(_noHiddenLayer)
    {
        error_general("hiddenNeuronX(unsigned int ["+std::to_string(hiddenX)+"] )","the network has no hidden layer");
    }
    if(hiddenX > _hiddenX-1)
    {
        error_general("hiddenNeuronX(unsigned int ["+std::to_string(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,hiddenX,(unsigned int)0,_hiddenX-1));
    }
    std::vector<Neuron*> ret(_hiddenY,0);
    unsigned int ID = hiddenX * _hiddenY;
    for(unsigned int y=0; y<_hiddenY; y++)
    {
        ret[y] = _hiddenNeuronList[ID];
        ID++;
    }
    if(_updateNetConfiguration)
    {
        error_general("hiddenNeuronX(unsigned int ["+std::to_string(hiddenX)+"] ) ",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    return ret;
}
std::vector<Neuron*> Net::hiddenNeuronY(unsigned int hiddenY)// --   Alle in einer Reihe
{
    if(_noHiddenLayer)
    {
        error_general("hiddenNeuronY(unsigned int ["+std::to_string(hiddenY)+"] )","the network has no hidden layer");
    }
    if(hiddenY > _hiddenY-1)
    {
        error_general("hiddenNeuronY(unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,hiddenY,(unsigned int)0,_hiddenY-1));
    }
    if(_updateNetConfiguration)
    {
        error_general("hiddenNeuron(unsigned int ["+std::to_string(hiddenY)+"] ) ",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    std::vector<Neuron*> ret(_hiddenX,0);
    unsigned int ID = hiddenY;
    for(unsigned int y=0; y<_hiddenX; y++)
    {
        ret[y] = _hiddenNeuronList[ID];
        ID+=_hiddenX;
    }
    return ret;
}
Neuron               *Net::costumNeuron(NeuronID ID)
{
    if(_costumNeuronList.size() == 0)
    {
        error_general("costumNeuron(NeuronID ["+Neuron::neuronIDString(ID)+"] )"," There are no costum neurons");
    }
    if(ID.ID > _allNeuronList.size() || ID.ID < _allNeuronList.size()-_costumNeuronList.size())
    {
        error_general("costumNeuron(NeuronID ["+Neuron::neuronIDString(ID)+"] )",error_paramOutOfRange((unsigned int)0,_costumNeuronList[0]->ID().ID,(unsigned int)0,_costumNeuronList[_costumNeuronList.size()-1]->ID().ID));
    }
    if(_updateNetConfiguration)
    {
        error_general("costumNeuron(NeuronID ["+Neuron::neuronIDString(ID)+"] ) ",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    return _costumNeuronList[ID.ID];
}
std::vector<Neuron*> *Net::costumNeuron()
{
    return &_costumNeuronList;
}
std::vector<std::vector<Neuron*> > Net::hiddenNeuron()
{
    if(_noHiddenLayer)
    {
        error_general("hiddenNeuron()","the network has no hidden layer");
    }
    if(_updateNetConfiguration)
    {
        error_general("hiddenNeuron() ",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    std::vector<std::vector<Neuron*>    > ret(_hiddenX,std::vector<Neuron*>(_hiddenY));
    if(_hiddenX * _hiddenY != _hiddenNeuronList.size())
    {
        error_general("hiddenNeuron() ","something went wrong: _hiddenX * _hiddenY != _hiddenNeuronList.size()");
    }
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
Neuron              *Net::outputNeuron(unsigned int output)
{
    if(output > _outputs-1)
    {
        error_general("outputNeuron(unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,output,(unsigned int)0,_outputs-1));
    }
    if(_updateNetConfiguration)
    {
        error_general("outputNeuron(unsigned int ["+std::to_string(output)+"]) ",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    return _outputNeuronList[output];
}
std::vector<Neuron*> *Net::outputNeuron()
{
    if(_updateNetConfiguration)
    {
        error_general("outputNeuron() ",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    return &_outputNeuronList;
}
std::vector<Neuron*> *Net::allNeurons()
{
    if(_updateNetConfiguration)
    {
        error_general("allNeurons() ",__NET_ERROR_MESSAGE_UPDATE_FIRDST);
    }
    return &_allNeuronList;
}


float               Net::output(unsigned int output)
{
    if(output > _outputs)
    {
        error_general("output(unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,output,(unsigned int)0,_outputs-1));
    }
    return _outputNeuronList[output]->output();
}
std::vector<float>  Net::output()
{
    std::vector<float> ret(_outputs,0);
    for(unsigned int y=0; y<_outputs; y++)
    {
        ret[y] = _outputNeuronList[y]->output();
    }
    return ret;
}

void                Net::run()
{
#if defined(_DEBUG_NET_RUN)
    __DEBUG_NET(this,"run()","begin, sets all neuron's to needsUpdate.");
#endif
#if defined(_DEBUG_NET_TIMING)
    __debug_timer1 = std::chrono::high_resolution_clock::now();
#endif
    for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
    {
        _allNeuronList[neuron]->needsUpdate();
    }
    bool allUpdated = false;

#if defined(_DEBUG_NET_RUN)
    __DEBUG_NET(this,"run()","while neurons not updated");
#endif
    for(unsigned int neuron=0; neuron<_calculationOrderList.size(); neuron++)
    {
#if defined(_DEBUG_NET_RUN)
        __DEBUG_NET(this,"run()","calculates neuron ID: "+Neuron::neuronIDString(_calculationOrderList[neuron]));
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
                    __DEBUG_NET(this,"run()","calculates neuron ID: "+std::to_string(neuron));
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
    __debug_timer2 = std::chrono::high_resolution_clock::now();
    __debug_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(__debug_timer2 - __debug_timer1);
    __debug_run_time = __debug_time_span.count()*1000;
#endif
#if defined(_DEBUG_NET_RUN)
#if defined(_DEBUG_NET_TIMING)
    __DEBUG_NET(this,"run()","all updated, finish. Elapsed time: "+std::to_string(__debug_run_time)+" ms");
#else
    __DEBUG_NET(this,"run()","all updated, finish");
#endif
#endif
    _update = false;
}
void                Net::updateNetConfiguration()
{
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","begin");
#endif
    if(!_updateNetConfiguration)
    {
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
        __DEBUG_NET(this,"updateNetConfiguration()","already up to date");
#endif
        return;
    }
    _update = true;
    if(_hiddenX == 0 || _hiddenY == 0)
    {
        _noHiddenLayer = true;
    }
    else
    {
        _noHiddenLayer = false;
    }
    for(unsigned int a=_allNeuronList.size(); a>0; a--)
    {
        try {
            delete _allNeuronList[a-1];
        } catch (std::exception &e) {
            error_general("updateNetConfiguration()",e.what());
        }
    }
    unsigned int hiddenNeurons  = _hiddenX * _hiddenY;
    bool costumConnectionList = true;
    if(_connectionList.size() == 0)
    {
        costumConnectionList = false;
        _neurons = hiddenNeurons + _outputs;
        prepareConnectionList();
    }



    _connections = _connectionList.size();

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

    _inputSignalList.reserve(_inputs);
    _hiddenNeuronList.reserve(_hiddenNeurons);
    _outputNeuronList.reserve(_outputs);



#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","setup _inputSignalList");
#endif
    for(unsigned int input=0; input<_inputs; input++)
    {
        _inputSignalList.push_back(0);
    }
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","setup _allNeuronList");
#endif
    for(unsigned int neuronID=0; neuronID<_neurons; neuronID++)
    {
        Neuron * tmp_neuron = new Neuron();
        tmp_neuron->activationFunction(_activationFunction);
        tmp_neuron->enableAverage(_enableAverage);
        tmp_neuron->ID(neuronID);
        _allNeuronList.push_back(tmp_neuron);
    }
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","copy to _hiddenNeuronList");
#endif
    for(unsigned int ID=0; ID<_hiddenNeurons; ID++)    // Save the first neurons (the hidden neurons) also in the _hiddenNeuronList
    {
        _hiddenNeuronList.push_back(_allNeuronList[ID]);
        _hiddenNeuronList[_hiddenNeuronList.size()-1]->TYPE(NeuronType::hidden);
    }
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","copy to _outputNeuronList");
#endif
    for(unsigned int ID=_hiddenNeurons; ID<_hiddenNeurons+_outputNeurons; ID++)    // Save the rest (the output neurons) also in the _outputNeuronList
    {
        _outputNeuronList.push_back(_allNeuronList[ID]);
        _outputNeuronList[_outputNeuronList.size()-1]->TYPE(NeuronType::output);
    }
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","copy to _costumNeuronList");
#endif
    for(unsigned int ID=_hiddenNeurons+_outputNeurons; ID<_neurons; ID++)    // Save the rest (the output neurons) also in the _outputNeuronList
    {
        _costumNeuronList.push_back(_allNeuronList[ID]);
        _costumNeuronList[_costumNeuronList.size()-1]->TYPE(NeuronType::costum);
    }


#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","connect "+std::to_string(_connections)+" connections");
#if defined(_DEBUG_NET_TIMING)
    __debug_timer1 = std::chrono::high_resolution_clock::now();
    float secCounter = 0;
#endif
#endif
    //- Connect the inputs

    for(unsigned int connection=0; connection<_connections; connection++)
    {
        connectNeuron(&_connectionList[connection]);
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
#if defined(_DEBUG_NET_TIMING)
        __debug_timer2      = std::chrono::high_resolution_clock::now();
        __debug_time_span   = std::chrono::duration_cast<std::chrono::duration<double>>(__debug_timer2 - __debug_timer1);
        if(__debug_time_span.count() >= 100)
        {
            secCounter++;
            __debug_timer1 = std::chrono::high_resolution_clock::now();
            __DEBUG_NET(this,"updateNetConfiguration()","connecting... "+std::to_string(connection)+" of: "+std::to_string(_connections)+" connection. Time elapsed: "+std::to_string(secCounter*10)+" sec");
        }
#endif
#endif
    }
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","connect finish");
#endif
    update_ptr_genomList();
    _updateNetConfiguration = false;
    prepareCalculationOrderList();
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"updateNetConfiguration()","finish");
#endif
}
bool                Net::connectNeuronViaID(unsigned int fromNeuron,unsigned int toNeuron,ConnectionDirection direction)
{
    unsigned int maxNeuronID = _allNeuronList.size();
#if defined(_DEBUG_NET_CONNECT)
    __DEBUG_NET(this,"connectNeuronViaID(unsigned int ["+std::to_string(fromNeuron)+"],unsigned int ["+std::to_string(toNeuron)+"],ConnectionDirection ["+Neuron::directionSring(direction)+"])","begin");
    __DEBUG_NET(this,"connectNeuronViaID(...)","connecting Neuron: "+std::to_string(fromNeuron)+" to: "+std::to_string(toNeuron));
    __DEBUG_NET(this,"connectNeuronViaID(...)","maxNeuronID: "+std::to_string(maxNeuronID));
#endif
    if(fromNeuron > maxNeuronID)
    {
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuronViaID(...)","Problem: "+error_paramOutOfRange(0,std::to_string(fromNeuron),std::to_string(0),std::to_string(maxNeuronID)));
#endif
        return 0;
    }
    if(toNeuron > maxNeuronID)
    {
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuronViaID(...)","Problem: "+error_paramOutOfRange(1,std::to_string(toNeuron),std::to_string(0),std::to_string(maxNeuronID)));
#endif
        return 0;
    }
    try
    {

        if(_allNeuronList[toNeuron]->connectInput(_allNeuronList[fromNeuron],direction))
        {
            _connections++;
            _costumConnections++;
            update_ptr_genomList();
            prepareCalculationOrderList();
        }
ronIndexX << " Y" << fromNeuronIndexY << " is connected to outputNeuron:  X" <<toNeuronIndexX << " Y"<<toNeuronIndexY;

    } catch (std::runtime_error &e) {
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuronViaID(...)","Problem: "+std::string(e.what()));
#endif
        return 0;
    }
#if defined(_DEBUG_NET_CONNECT)
    __DEBUG_NET(this,"connectNeuronViaID(unsigned int ["+std::to_string(fromNeuron)+"],unsigned int ["+std::to_string(toNeuron)+"],ConnectionDirection ["+Neuron::directionSring(direction)+"])","end");
#endif
    return 1;
}
bool                Net::connectNeuron(Connection *connection)
{
    bool ret = intern_connectNeuron(connection);
    prepareCalculationOrderList();
    return ret;
}

bool                Net::intern_connectNeuron(Connection *connection)

{
#if defined(_DEBUG_NET_CONNECT)
    __DEBUG_NET(this,"connectNeuron(Connection ["+Neuron::connectionString(*connection)+"])","begin");
#endif
    if(connection->destination_ID.ID > _allNeuronList.size())
    {
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuron(...)","destination neuron ID: "+std::to_string(connection->destination_ID.ID)+" not found");
#endif
        return 0;
    }
    if((connection->source_ID.ID > _allNeuronList.size()) && (connection->source_ID.TYPE != NeuronType::input))
    {
#if defined(_DEBUG_NET_CONNECT)
        __DEBUG_NET(this,"connectNeuron(...)","source neuron ID: "+std::to_string(connection->source_ID.ID)+" not found");
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
    _allNeuronList[connection->destination_ID.ID]->weight(connection->source_ID,connection->weight);
#if defined(_DEBUG_NET_CONNECT)
    __DEBUG_NET(this,"connectNeuron(Connection ["+Neuron::connectionString(*connection)+"])","end");
#endif
    return 1;
}
bool                Net::connectNeuron(std::vector<Connection> *connections)
{
    bool ret = 1;
    for(unsigned int a=0; a<connections->size(); a++)
    {
        if(!intern_connectNeuron(&(*connections)[a]))
            ret = 0;
    }
    prepareConnectionList();
    update_ptr_genomList();
    return ret;
}
void                Net::connectionList(std::vector<Connection> *connections)
{
    _updateNetConfiguration = true;
    _connectionList = *connections;
}
std::vector<Connection> *Net::connectionList()
{
    return &_connectionList;
}
void                Net::clearConnectionList()
{
    _connectionList.clear();
}
NeuronID            Net::addNeuron()
{
    return addNeuron(new Neuron());
}
NeuronID            Net::addNeuron(Neuron *neuron)
{
    if(neuron == nullptr)
    {
        error_general("addNeuron(Neuron [ptr])","ptr == nullptr");
    }
    _costumNeuronList.push_back(neuron);
    _costumNeuronList[_costumNeuronList.size()-1]->ID(_allNeuronList.size());
    _costumNeuronList[_costumNeuronList.size()-1]->TYPE(NeuronType::costum);
    _allNeuronList.push_back(_costumNeuronList[_costumNeuronList.size()-1]);
    _neurons++;
    return _costumNeuronList[_costumNeuronList.size()-1]->ID();
}
NeuronID            Net::addNeuron(Connection connection)
{
    NeuronID ID = addNeuron();
    connection.destination_ID = _costumNeuronList[_costumNeuronList.size()-1]->ID();
    connectNeuron(&connection);
    update_ptr_genomList();
    return ID;
}
NeuronID            Net::addNeuron(std::vector<Connection> inputConnections)
{
    NeuronID ID = addNeuron();
    for(unsigned int a=0; a<inputConnections.size(); a++)
    {
        inputConnections[a].destination_ID = _costumNeuronList[_costumNeuronList.size()-1]->ID();
        connectNeuron(&inputConnections[a]);
    }
    update_ptr_genomList();
    return ID;
}
void                Net::prepareConnectionList()
{
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"prepareConnectionList()","begin");
#endif
#if defined(_DEBUG_NET_TIMING)
    __debug_timer1      = std::chrono::high_resolution_clock::now();
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
                    _connectionList[_connectionList.size()-1].netID = this->ID();
                    _connectionList[_connectionList.size()-1].weight = Neuron::calcRandWeight(_randEngine);
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
                    _connectionList[_connectionList.size()-1].netID = this->ID();
                    _connectionList[_connectionList.size()-1].weight = Neuron::calcRandWeight(_randEngine);
                    _connectionList[_connectionList.size()-1].source_ID.ID = ID-_hiddenY+hiddenNeuronY2;
                    _connectionList[_connectionList.size()-1].source_ID.TYPE = NeuronType::hidden;
                    _connectionList[_connectionList.size()-1].direction = ConnectionDirection::forward;
                }
            }
            if(_bias)
            {
                _connectionList.push_back(Connection());
                _connectionList[_connectionList.size()-1].destination_ID.ID = ID;
                _connectionList[_connectionList.size()-1].destination_ID.TYPE = NeuronType::hidden;
                _connectionList[_connectionList.size()-1].netID = this->ID();
                _connectionList[_connectionList.size()-1].weight = Neuron::calcRandWeight(_randEngine);
                _connectionList[_connectionList.size()-1].source_ID.ID = 0;
                _connectionList[_connectionList.size()-1].source_ID.TYPE = NeuronType::bias;
                _connectionList[_connectionList.size()-1].direction = ConnectionDirection::forward;
            }
            ID++;
        }
    }
    //ID = 0;
    if(_noHiddenLayer)
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
                _connectionList[_connectionList.size()-1].netID = this->ID();
                _connectionList[_connectionList.size()-1].weight = Neuron::calcRandWeight(_randEngine);
                _connectionList[_connectionList.size()-1].direction = ConnectionDirection::forward;
            }
            if(_bias)
            {
                _connectionList.push_back(Connection());
                _connectionList[_connectionList.size()-1].destination_ID.ID = ID;
                _connectionList[_connectionList.size()-1].destination_ID.TYPE = NeuronType::output;
                _connectionList[_connectionList.size()-1].netID = this->ID();
                _connectionList[_connectionList.size()-1].weight = Neuron::calcRandWeight(_randEngine);
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
                _connectionList[_connectionList.size()-1].netID = this->ID();
                _connectionList[_connectionList.size()-1].weight = Neuron::calcRandWeight(_randEngine);
                _connectionList[_connectionList.size()-1].direction = ConnectionDirection::forward;
            }
            if(_bias)
            {
                _connectionList.push_back(Connection());
                _connectionList[_connectionList.size()-1].destination_ID.ID = ID;
                _connectionList[_connectionList.size()-1].destination_ID.TYPE = NeuronType::output;
                _connectionList[_connectionList.size()-1].netID = this->ID();
                _connectionList[_connectionList.size()-1].weight = Neuron::calcRandWeight(_randEngine);
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
    __debug_timer2      = std::chrono::high_resolution_clock::now();
    __debug_time_span   = std::chrono::duration_cast<std::chrono::duration<double>>(__debug_timer2 - __debug_timer1);
    __DEBUG_NET(this,"prepareConnectionList()","end. Elapsed time: "+std::to_string(__debug_time_span.count()*1000)+" ms");
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
    std::vector<bool> isUpdated(_allNeuronList.size(),false);
    bool allUpdated = false;
    while(!allUpdated)
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
                        (!isUpdated[_calculationOrderList.size()-1] &&
                         _allNeuronList[neuron]->inputConnectionDirection(_allNeuronList[neuron]->inputID(input)) == true))
                    {
                        readyToCalcCostumNeuron = false;
                    }
                }
                if(readyToCalcCostumNeuron)
                {
                    _calculationOrderList.push_back(_allNeuronList[neuron]->ID());
#if defined(_DEBUG_NET_CONNECT)
                    __DEBUG_NET(this,"prepareCalculationOrderList()","next in order: "+Neuron::neuronIDString(_allNeuronList[neuron]->ID()));
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
void                Net::update_ptr_genomList()
{
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"update_ptr_genomList()","begin");
#endif
    _ptr_genom.clear();
    _ptr_genom.reserve(_connections);
    for(unsigned int ID=0; ID<_neurons; ID++)
    {
        for(unsigned int weight=0; weight<_allNeuronList[ID]->inputs(); weight++)
        {
            _ptr_genom.push_back(_allNeuronList[ID]->ptr_weight(weight));
        }
    }
#if defined(_DEBUG_NET_UPDATE_NET_CONFIGURATION)
    __DEBUG_NET(this,"update_ptr_genomList()","end");
#endif
}
#if defined(_DEBUG_NEURON_TIMING)
double Neuron::debug_runtime()
{
    return __debug_run_time;
}
#endif
//----------ERROR

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
std::string Net::error_paramOutOfRange(unsigned int paramPos,float value,float min, float max)
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
}
