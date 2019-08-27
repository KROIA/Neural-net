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
/*    for(unsigned int a=_hiddenNeuronList.size(); a>0; a--)
    {
        for(unsigned int b=_hiddenNeuronList[a-1].size(); b>0; b--)
        {
            try {
                delete _hiddenNeuronList[a-1][b-1];
            } catch (std::exception &e) {
                error_general("~Net()",e.what());
            }
        }
    }
    for(unsigned int b=_outputNeuronList.size(); b>0; b--)
    {
        try {
            delete _outputNeuronList[b-1];
        } catch (std::exception &e) {
            error_general("~Net()",e.what());
        }
    }
    _hiddenNeuronList.clear();
    _outputNeuronList.clear();*/
    _inputSignalList.clear();
    _hiddenNeuronList.clear();
    _outputNeuronList.clear();
    _costumNeuronList.clear();
    _allNeuronList.clear();
    _ptr_genom.clear();
    //_genom.clear();
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
        _update = true;
        _updateNetConfiguration = true;
        _inputs = inputs;
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
        _update  = true;
        _updateNetConfiguration = true;
        _hiddenX = hiddenX;
        _hiddenNeurons = _hiddenX * _hiddenY;
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
        _update  = true;
        _updateNetConfiguration = true;
        _hiddenY = hiddenY;
        _hiddenNeurons = _hiddenX * _hiddenY;
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
        _update  = true;
        _updateNetConfiguration = true;
        _outputs = outputs;
        _outputNeurons = outputs;
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
    _neurons = neurons;
    _hiddenNeurons = hiddenNeurons;
    _outputNeurons = outputNeurons;
    _costumNeurons = costumNeurons;
    if(_hiddenNeurons+_outputNeurons+_costumNeurons != _neurons)
    {
        error_general("neurons(unsigned int ["+std::to_string(neurons)+"],unsigned int ["+std::to_string(hiddenNeurons)+"],unsigned int ["+std::to_string(outputNeurons)+"],unsigned int ["+std::to_string(costumNeurons)+"],","Its not possible like so. Parameter 1 shuld be the sum of the others");
    }
}
void                Net::bias(bool enableBias)
{
    if(enableBias != _bias)
    {
        _update = true;
        _updateNetConfiguration = true;
        _bias   = enableBias;
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
        _update         = true;
        _updateNetConfiguration = true;
        _enableAverage  = enableAverage;
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
    return _noHiddenLayer;
}

void                Net::randomGenom()
{
    /*unsigned int genomsize	= 0;
    if(_noHiddenLayer)
    {
        genomsize = (_inputs+(unsigned int)_bias) * _outputs;
    }
    else
    {
        genomsize = (_inputs+(unsigned int)_bias) * _hiddenY + (_hiddenY+(unsigned int)_bias) * _hiddenY * (_hiddenX-1) + (_hiddenY+(unsigned int)_bias) * _outputs;
    }
    if(genomsize == 0)
    {
        error_general("randomGenom()","genomsize can't be zero (0)");
    }
    _genom = std::vector<float>(genomsize,0);
    for(unsigned int a=0; a<genomsize; a++)
    {
        _genom[a] = (float)(_randEngine()%2000)/1000 - (float)1;
    }
    setGenomToNeuron();*/
    for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
    {
        _allNeuronList[neuron]->randWeight();
    }
}
void                Net::genom(std::vector<float> genom)
{
    if(genom.size() != _connections)
    {
        //error_general("genom(std::vector<float>)","parameter 0 has the wrong array size: "+std::to_string(genom.size())+" array size should by "+std::to_string(_genom.size()));
        qDebug() << "WARNING: "<< "genom(std::vector<float>) parameter 0 has the wrong array size: " << genom.size() << " array size should by "<<_connections;        //only to test the aditional connections

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
    /*try {
        getGenomFromNeuron();
    } catch (std::runtime_error &e) {
        error_general("genom()",e);
    }
    return _genom;*/
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
    /*try {
        getGenomFromNeuron();
    } catch (std::runtime_error &e) {
        error_general("genomsize()",e);
    }
    return _genom.size();*/
    return _ptr_genom.size();
}

void                Net::input(unsigned int input, float signal)
{
    if(input > _inputs-1)
    {
        error_general("input(unsigned int ["+std::to_string(input)+"], float ["+std::to_string(signal)+"])",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    _update = true;
    _inputSignalList[input] = signal;
}
float               Net::input(unsigned int input)
{
    if(input > _inputs-1)
    {
        error_general("input(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    return _inputSignalList[input];
}
void                Net::input(std::vector<float> inputList)
{
    if(inputList.size() != _inputs)
    {
        error_general("input(std::vector<float>)","parameter 0 , size of the array is wrong: ["+std::to_string(inputList.size()) + "] correct size is: ["+std::to_string(_inputs)+"]");
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
    //run();
    //return _hiddenNeuronList[hiddenX][hiddenY]->output();
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
    //run();
    std::vector<float> ret(_hiddenY,0);
    unsigned int ID = hiddenX * _hiddenY;
    for(unsigned int y=0; y<_hiddenY; y++)
    {
        //ret[y] = _hiddenNeuronList[hiddenX][y]->output();
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
    //run();
    std::vector<float> ret(_hiddenX,0);
    unsigned int ID = hiddenY;
    for(unsigned int x=0; x<_hiddenX; x++)
    {
        //ret[x] = _hiddenNeuronList[x][hiddenY]->output();
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
    return _allNeuronList[ID];
}
Neuron             *Net::neuron_viaID(NeuronID ID)
{
    if(ID.ID >= _allNeuronList.size())
    {
        error_general("neuron_viaID(NeuronID [.ID="+std::to_string(ID.ID)+",.TYPE="+std::to_string(ID.TYPE)+"] )",error_paramOutOfRange((unsigned int)0,ID.ID,(unsigned int)0,_allNeuronList.size()));
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
    //run();
    //return _hiddenNeuronList[hiddenX][hiddenY];
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
    //run();
    //return _hiddenNeuronList[hiddenX];
    std::vector<Neuron*> ret(_hiddenY,0);
    unsigned int ID = hiddenX * _hiddenY;
    for(unsigned int y=0; y<_hiddenY; y++)
    {
        ret[y] = _hiddenNeuronList[ID];
        ID++;
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
   // run();
   // std::vector<Neuron*> ret;
    std::vector<Neuron*> ret(_hiddenX,0);
    unsigned int ID = hiddenY;
    for(unsigned int y=0; y<_hiddenX; y++)
    {
        ret[y] = _hiddenNeuronList[ID];
        ID+=_hiddenX;
    }
    return ret;
}
std::vector<std::vector<Neuron*> > *Net::hiddenNeuron()
{
    if(_noHiddenLayer)
    {
        error_general("hiddenNeuron()","the network has no hidden layer");
    }
    //run();
    //return &_hiddenNeuronList;
}
Neuron              *Net::outputNeuron(unsigned int output)
{
    if(output > _outputs-1)
    {
        error_general("outputNeuron(unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,output,(unsigned int)0,_outputs-1));
    }
    run();
    return _outputNeuronList[output];
}
std::vector<Neuron*> *Net::outputNeuron()
{
    return &_outputNeuronList;
}
std::vector<Neuron*> *Net::allNeurons()
{
    return &_allNeuronList;
}


float               Net::output(unsigned int output)
{
    if(output > _outputs)
    {
        error_general("output(unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,output,(unsigned int)0,_outputs-1));
    }
    run();
    //return _outputNeuronList[output]->output();
    return _outputNeuronList[output]->output();
}
std::vector<float>  Net::output()
{
    run();
    std::vector<float> ret(_outputs,0);
    for(unsigned int y=0; y<_outputs; y++)
    {
        ret[y] = _outputNeuronList[y]->output();
    }
    return ret;
}

void                Net::run()
{
    for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
    {
        _allNeuronList[neuron]->needsUpdate();
    }
    bool allUpdated = false;
   // qDebug() << "----------------start run-------------------";
    while(!allUpdated)
    {
        for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
        {
            /*if(_allNeuronList[neuron]->ID().TYPE == NeuronType::costum)
            {*/
                bool readyToCalcCostumNeuron = true;
                if(!_allNeuronList[neuron]->isUpdated())
                {
                    for(unsigned int input=0; input<_allNeuronList[neuron]->inputs(); input++)
                    {
                       /* qDebug() << "_allNeuronList[neuron]->inputID(input).TYPE: "<<QString::fromStdString(Neuron::typeString(_allNeuronList[neuron]->inputID(input).TYPE));
                        qDebug() << "_allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->isUpdated(): "<<_allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->isUpdated();
                        qDebug() << "_allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->ID().TYPE: "<<QString::fromStdString(Neuron::typeString(_allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->ID().TYPE));
                        qDebug() << "_allNeuronList[neuron]->ID().TYPE: "<<QString::fromStdString(Neuron::typeString(_allNeuronList[neuron]->ID().TYPE));
                        qDebug() << "_allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->ID().ID: " << _allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->ID().ID;
                        qDebug() << "_allNeuronList[neuron]->ID().ID: "<<_allNeuronList[neuron]->ID().ID;
*/
                        /*if(_allNeuronList[neuron]->inputID(input).TYPE == NeuronType::costum ||
                           _allNeuronList[neuron]->inputID(input).TYPE == NeuronType::hidden ||
                           _allNeuronList[neuron]->inputID(input).TYPE == NeuronType::output)
                        {
                            qDebug() << "first true";
                        }
                        if(!_allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->isUpdated())
                        {
                            qDebug() << "second true";
                        }
                        if(!(_allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->ID().TYPE == _allNeuronList[neuron]->ID().TYPE &&    //if it is a looped back neuron
                           _allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->ID().ID   == _allNeuronList[neuron]->ID().ID))
                        {
                            qDebug() << "third true";
                        }*/
                        if((_allNeuronList[neuron]->inputID(input).TYPE == NeuronType::costum ||
                            _allNeuronList[neuron]->inputID(input).TYPE == NeuronType::hidden ||
                            _allNeuronList[neuron]->inputID(input).TYPE == NeuronType::output) &&
                            (!_allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->isUpdated() &&
                             _allNeuronList[neuron]->inputConnectionDirection(_allNeuronList[neuron]->inputID(input)) == true
                            /*!(_allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->ID().TYPE == _allNeuronList[neuron]->ID().TYPE &&    //if it is a looped back neuron
                              _allNeuronList[_allNeuronList[neuron]->inputID(input).ID]->ID().ID   == _allNeuronList[neuron]->ID().ID)*/))       //
                        {
                            readyToCalcCostumNeuron = false;
                            qDebug() << "don't run neuron: "<<neuron;
                           // allUpdated = false;
                        }
                    }
                    if(readyToCalcCostumNeuron)
                    {
                        if(neuron == 21)
                        {

                        }
                        _allNeuronList[neuron]->run();
                        //qDebug() << "run ID: "<<neuron << " Type: "<<QString::fromStdString(Neuron::typeString(_allNeuronList[neuron]->ID().TYPE));
                    }
                }
            /*}else
            {
                _allNeuronList[neuron]->run();
            }*/
        }
        allUpdated = true;
        for(unsigned int neuron=0; neuron<_allNeuronList.size(); neuron++)
        {
            if(!_allNeuronList[neuron]->isUpdated())
            {
                allUpdated = false;
            }
        }
    }
  //  qDebug() << "----------------end run-------------------";
    /*if(_update)
    {
        if(!_noHiddenLayer)
        {
            for(unsigned int x=0; x<_hiddenNeuronList.size()-1; x++)  //For every hidden layer in x ---> direction
            {
                for(unsigned int y=0; y<_hiddenNeuronList[x+1].size(); y++)
                {
                    for(unsigned int y_layerBefore=0; y_layerBefore<_hiddenNeuronList[x].size(); y_layerBefore++)
                    {
                        _hiddenNeuronList[x+1][y]->input(y_layerBefore,_hiddenNeuronList[x][y_layerBefore]->output());
                    }
                    if(_bias)
                    {
                        _hiddenNeuronList[x+1][y]->input(_hiddenY,_biasValue);//_hiddenY because pos begins by 0 for the value 1 and with the bias we have to calc the last element + 1
                    }
                }
            }
            for(unsigned int outY=0; outY<_outputs; outY++)
            {
                for(unsigned int hidY=0; hidY<_hiddenY; hidY++)
                {
                    _outputNeuronList[outY]->input(hidY,_hiddenNeuronList[_hiddenX-1][hidY]->output());
                }
                if(_bias)
                {
                    _outputNeuronList[outY]->input(_hiddenY,_biasValue);//_hiddenY because pos begins by 0 for the value 1 and with the bias we have to calc the last element + 1
                }
            }
        }
        for(unsigned int outY=0; outY<_outputs; outY++)
        {
            _outputNeuronList[outY]->run();
        }
        _update = false;
    }*/
}
void                Net::updateNetConfiguration()
{
    if(!_updateNetConfiguration)
        return;

    _update = true;
    if(_hiddenX == 0 || _hiddenY == 0)
    {
        _noHiddenLayer = true;
    }
    else
    {
        _noHiddenLayer = false;
    }
    /*for(unsigned int a=_hiddenNeuronList.size(); a>0; a--)
    {
        for(unsigned int b=_hiddenNeuronList[a-1].size(); b>0; b--)
        {
            delete _hiddenNeuronList[a-1][b-1];
        }
    }
    for(unsigned int b=_outputNeuronList.size(); b>0; b--)
    {
        delete _outputNeuronList[b-1];
    }
    _hiddenNeuronList.clear();
    _outputNeuronList.clear();
    _hiddenNeuronList.reserve(_hiddenX);
    _outputNeuronList.reserve(_outputs);
    for(unsigned int y=0; y<_outputs; y++)
    {
        _outputNeuronList.push_back(new Neuron());
        _outputNeuronList[y]->inputs(_hiddenY+(unsigned int)_bias);
        _outputNeuronList[y]->activationFunction(_activationFunction);
        _outputNeuronList[y]->enableAverage(_enableAverage);
    }
    for(unsigned int x=0; x<_hiddenX; x++)
    {
        _hiddenNeuronList.push_back(std::vector<Neuron*>());
        _hiddenNeuronList[_hiddenNeuronList.size()-1].reserve(_hiddenY);
        for(unsigned int y=0; y<_hiddenY; y++)
        {
            _hiddenNeuronList[x].push_back(new Neuron());
            _hiddenNeuronList[x][y]->inputs(_hiddenY+(unsigned int)_bias);
            _hiddenNeuronList[x][y]->activationFunction(_activationFunction);
            _hiddenNeuronList[x][y]->enableAverage(_enableAverage);
        }
    }
    if(_noHiddenLayer)
    {
        for(unsigned int y=0; y<_outputs; y++)
        {
            _outputNeuronList[y]->inputs(_inputs+(unsigned int)_bias);
        }
    }
    else
    {
        for(unsigned int y=0; y<_hiddenY; y++)
        {
            _hiddenNeuronList[0][y]->inputs(_inputs+(unsigned int)_bias);
        }
    }*/
    for(unsigned int a=_allNeuronList.size(); a>0; a--)
    {
        try {
            delete _allNeuronList[a-1];
        } catch (std::exception &e) {
            error_general("~Net()",e.what());
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


    //unsigned int neurons        = hiddenNeurons + _outputs;

    _connections = _connectionList.size();

    _inputSignalList.clear();
    _hiddenNeuronList.clear();
    _outputNeuronList.clear();
    _costumNeuronList.clear();
    _allNeuronList.clear();
    _ptr_genom.clear();

    _inputSignalList.reserve(_inputs);
    //_hiddenNeuronList.reserve(hiddenNeurons);
    _outputNeuronList.reserve(_outputs);


    for(unsigned int input=0; input<_inputs; input++)
    {
        _inputSignalList.push_back(0);
    }

    for(unsigned int neuronID=0; neuronID<_neurons; neuronID++)
    {
        Neuron * tmp_neuron = new Neuron();
        tmp_neuron->activationFunction(_activationFunction);
        tmp_neuron->enableAverage(_enableAverage);
        tmp_neuron->ID(neuronID);
        _allNeuronList.push_back(tmp_neuron);
    }

    for(unsigned int ID=0; ID<_hiddenNeurons; ID++)    // Save the first neurons (the hidden neurons) also in the _hiddenNeuronList
    {
        _hiddenNeuronList.push_back(_allNeuronList[ID]);
        _hiddenNeuronList[_hiddenNeuronList.size()-1]->TYPE(NeuronType::hidden);
    }
    for(unsigned int ID=_hiddenNeurons; ID<_hiddenNeurons+_outputNeurons; ID++)    // Save the rest (the output neurons) also in the _outputNeuronList
    {
        _outputNeuronList.push_back(_allNeuronList[ID]);
        _outputNeuronList[_outputNeuronList.size()-1]->TYPE(NeuronType::output);
    }
    for(unsigned int ID=_hiddenNeurons+_outputNeurons; ID<_neurons; ID++)    // Save the rest (the output neurons) also in the _outputNeuronList
    {
        _costumNeuronList.push_back(_allNeuronList[ID]);
        _costumNeuronList[_costumNeuronList.size()-1]->TYPE(NeuronType::costum);
    }




    //- Connect the inputs
    for(unsigned int connection=0; connection<_connections; connection++)
    {
        switch(_connectionList[connection].source_ID.TYPE)
        {
            case NeuronType::input:
            {
                _allNeuronList[_connectionList[connection].destination_ID.ID]->connectInput(_connectionList[connection].source_ID,&_inputSignalList[_connectionList[connection].source_ID.ID],_connectionList[connection].direction);
                break;
            }
            case NeuronType::hidden:
            case NeuronType::output:
            case NeuronType::costum:
            {
                _allNeuronList[_connectionList[connection].destination_ID.ID]->connectInput(_allNeuronList[_connectionList[connection].source_ID.ID],_connectionList[connection].direction);

                break;
            }
            case NeuronType::bias:
            {
                _allNeuronList[_connectionList[connection].destination_ID.ID]->connectInput(_connectionList[connection].source_ID,&_biasValue,_connectionList[connection].direction);
                break;
            }
        }
        _allNeuronList[_connectionList[connection].destination_ID.ID]->weight(_connectionList[connection].source_ID,_connectionList[connection].weight);
    }
    for(unsigned int ID=0; ID<_neurons; ID++)
    {
        for(unsigned int weight=0; weight<_allNeuronList[ID]->inputs(); weight++)
        {
            _ptr_genom.push_back(_allNeuronList[ID]->ptr_weight(weight));
        }
    }

    /*unsigned int ID = 0;
    for(unsigned int hiddenNeuronX=0; hiddenNeuronX<_hiddenX; hiddenNeuronX++)
    {
        for(unsigned int hiddenNeuronY=0; hiddenNeuronY<_hiddenY; hiddenNeuronY++)
        {
            if(hiddenNeuronX == 0)
            {
                for(unsigned int input=0; input<_inputs; input++)
                {
                    _hiddenNeuronList[ID]->connectInput(NeuronType::input,&_inputSignalList[input]);
                    _connections++;
                }
            }else {
                for(unsigned int hiddenNeuronY2=0; hiddenNeuronY2<_hiddenY; hiddenNeuronY2++)
                {
                    _hiddenNeuronList[ID]->connectInput(_hiddenNeuronList[ID-_hiddenY+hiddenNeuronY2]);
                    _connections++;
                }
            }
            ID++;
        }
    }
    ID = 0;
    if(_noHiddenLayer)
    {
        for(unsigned int outputNeuron=0; outputNeuron<_outputs; outputNeuron++)
        {
            for(unsigned int input=0; input<_inputs; input++)
            {
                _outputNeuronList[ID]->connectInput(NeuronType::input,&_inputSignalList[input]);
                _connections++;
            }
            ID++;
        }
    }
    else {
        for(unsigned int outputNeuron=0; outputNeuron<_outputs; outputNeuron++)
        {
            for(unsigned int hiddenNeuronY=0; hiddenNeuronY<_hiddenY; hiddenNeuronY++)
            {
                _outputNeuronList[ID]->connectInput(_hiddenNeuronList[hiddenNeuronY+((_hiddenX-1)*_hiddenY)]);
                _connections++;
            }
            ID++;
        }
    }
    if(_bias)
    {
        for(unsigned int ID=0; ID<neurons; ID++)
        {
            _allNeuronList[ID]->connectInput(NeuronType::bias,&_biasValue);
            _connections++;
        }
    }
    for(unsigned int ID=0; ID<neurons; ID++)
    {
        for(unsigned int weight=0; weight<_allNeuronList[ID]->inputs(); weight++)
        {
            _ptr_genom.push_back(_allNeuronList[ID]->ptr_weight(weight));
        }
    }*/

   /* if(_noHiddenLayer)
    {
        for(unsigned int outputNeuron=0; outputNeuron<_outputs; outputNeuron++)
        {
            for(unsigned int input=0; input<_inputs; input++)
            {
                _outputNeuronList[outputNeuron]->connectInput(input,&_inputSignalList[input]);
            }
            if(_bias)
            {
                _outputNeuronList[outputNeuron]->connectInput(_inputs,&_biasValue);
            }
        }
    }
    else
    {
        for(unsigned int hiddenNeuronY=0; hiddenNeuronY<_hiddenY; hiddenNeuronY++)
        {
            for(unsigned int input=0; input<_inputs; input++)
            {
                _hiddenNeuronList[0][hiddenNeuronY]->connectInput(input,&_inputSignalList[input]);
            }
            if(_bias)
            {
                _hiddenNeuronList[0][hiddenNeuronY]->connectInput(_inputs,&_biasValue);
            }
        }
        for(unsigned int hiddenNeuronX=1; hiddenNeuronX<_hiddenX; hiddenNeuronX++)
        {
            for(unsigned int hiddenNeuronY=0; hiddenNeuronY<_hiddenY; hiddenNeuronY++)
            {
                for(unsigned int input=0; input<_hiddenY; input++)
                {
                    _hiddenNeuronList[hiddenNeuronX][hiddenNeuronY]->connectInput(input,_hiddenNeuronList[hiddenNeuronX-1][input]->ptr_output());
                }
                if(_bias)
                {
                    _hiddenNeuronList[hiddenNeuronX][hiddenNeuronY]->connectInput(_inputs,&_biasValue);
                }
            }
        }
        for(unsigned int outputNeuron=0; outputNeuron<_outputs; outputNeuron++)
        {
            for(unsigned int input=0; input<_hiddenY; input++)
            {
                _outputNeuronList[outputNeuron]->connectInput(input,_hiddenNeuronList[_hiddenX-1][input]->ptr_output());
            }
            if(_bias)
            {
                _outputNeuronList[outputNeuron]->connectInput(_hiddenY,&_biasValue);
            }
        }
    }*/
    //- connecting done

    _updateNetConfiguration = false;
    if(!costumConnectionList)
    {
        _connectionList.clear();
    }/*else {
        try {
            randomGenom();
        } catch (std::runtime_error &e) {
            error_general("updateNetConfiguration()",e);
        }
    }*/

}
void                Net::connectNeuronViaID(unsigned int fromNeuron,unsigned int toNeuron,bool forward)
{
    unsigned int maxNeuronID = hiddenNeuronsX() * hiddenNeuronsY() + outputNeurons()  -1;

    qDebug() << "connecting Neuron: "<<fromNeuron << " to: "<<toNeuron;
    qDebug() << "maxNeuronID: "<<maxNeuronID;
    if(fromNeuron > maxNeuronID)
    {
        error_general("connectNeuronViaID(unsigned int ["+std::to_string(fromNeuron)+"],unsigned int ["+std::to_string(toNeuron)+"])",error_paramOutOfRange(0,std::to_string(fromNeuron),std::to_string(0),std::to_string(maxNeuronID)));
    }
    if(toNeuron > maxNeuronID)
    {
        error_general("connectNeuronViaID(unsigned int ["+std::to_string(fromNeuron)+"],unsigned int ["+std::to_string(toNeuron)+"])",error_paramOutOfRange(1,std::to_string(toNeuron),std::to_string(0),std::to_string(maxNeuronID)));
    }
    // dummy code
    try
    {
        if(_allNeuronList[toNeuron]->connectInput(_allNeuronList[fromNeuron],forward))
        {
            _connections++;
            _costumConnections++;
            _ptr_genom.clear();
            for(unsigned int ID=0; ID<_neurons; ID++)
            {
                for(unsigned int weight=0; weight<_allNeuronList[ID]->inputs(); weight++)
                {
                    _ptr_genom.push_back(_allNeuronList[ID]->ptr_weight(weight));
                }
            }
        }
        //_ptr_genom.push_back(_allNeuronList[toNeuron]->ptr_weight(_allNeuronList[fromNeuron]->ID()));

        /*if(fromNeuron >= hiddenNeuronsX() * hiddenNeuronsY())  //is fromNeuron a outputNeuron?
        {
            if(toNeuron >= hiddenNeuronsX() * hiddenNeuronsY()) //is toNeuron a outputNeuron?
            {
                unsigned int fromNeuronIndex  = fromNeuron - hiddenNeuronsX() * hiddenNeuronsY();
                unsigned int toNeuronIndex    = toNeuron   - hiddenNeuronsX() * hiddenNeuronsY();
           //     _outputNeuronList[toNeuronIndex]->connectInput(_outputNeuronList[fromNeuronIndex]->ptr_output());
                qDebug() << "outputNeuron: "<< fromNeuronIndex << " is connected to outputNeuron: " <<toNeuronIndex;
            }
            else //toNeuron is a hiddenNeuron
            {
                if(hiddenNeuronsX() == 0 || hiddenNeuronsY() == 0)
                {
                    error_general("connectNeuronViaID(unsigned int ["+std::to_string(fromNeuron)+"],unsigned int ["+std::to_string(toNeuron)+"])","Division by zero!");
                }
                unsigned int fromNeuronIndex  = fromNeuron - hiddenNeuronsX() * hiddenNeuronsY();
                unsigned int toNeuronIndexY   = toNeuron / hiddenNeuronsX();
                unsigned int toNeuronIndexX   = toNeuron % hiddenNeuronsX();
           //     _hiddenNeuronList[toNeuronIndexX][toNeuronIndexY]->connectInput(_outputNeuronList[fromNeuronIndex]->ptr_output());
                qDebug() << "outputNeuron: "<< fromNeuronIndex << " is connected to hiddenNeuron: X" <<toNeuronIndexX << " Y"<<toNeuronIndexY;
            }
        }
        else //fromNeuron is a hiddenNeuron
        {
            if(toNeuron >= inputNeurons() + hiddenNeuronsX() * hiddenNeuronsY()) //is toNeuron a outputNeuron?
            {
                if(hiddenNeuronsX() == 0 || hiddenNeuronsY() == 0)
                {
                    error_general("connectNeuronViaID(unsigned int ["+std::to_string(fromNeuron)+"],unsigned int ["+std::to_string(toNeuron)+"])","Division by zero!");
                }
                unsigned int fromNeuronIndexY = fromNeuron / hiddenNeuronsX();
                unsigned int fromNeuronIndexX = fromNeuron % hiddenNeuronsX();
                unsigned int toNeuronIndex    = toNeuron   - hiddenNeuronsX() * hiddenNeuronsY();
           //     _outputNeuronList[toNeuronIndex]->connectInput(_hiddenNeuronList[fromNeuronIndexX][fromNeuronIndexY]->ptr_output());
                qDebug() << "hiddenNeuron: X"<< fromNeuronIndexX << " Y" << fromNeuronIndexY << " is connected to outputNeuron: " <<toNeuronIndex;
            }
            else //toNeuron is a hiddenNeuron
            {
                unsigned int toNeuronIndexY   = toNeuron / hiddenNeuronsX();
                unsigned int toNeuronIndexX   = toNeuron % hiddenNeuronsX();
                unsigned int fromNeuronIndexY = fromNeuron / hiddenNeuronsX();
                unsigned int fromNeuronIndexX = fromNeuron % hiddenNeuronsX();
           //     _hiddenNeuronList[toNeuronIndexX][toNeuronIndexY]->connectInput(_hiddenNeuronList[fromNeuronIndexX][fromNeuronIndexY]->ptr_output());
                qDebug() << "hiddenNeuron: X"<< fromNeuronIndexX << " Y" << fromNeuronIndexY << " is connected to outputNeuron:  X" <<toNeuronIndexX << " Y"<<toNeuronIndexY;
            }
        }*/
    } catch (std::runtime_error &e) {
        error_general("connectNeuronViaID(unsigned int ["+std::to_string(fromNeuron)+"],unsigned int ["+std::to_string(toNeuron)+"])",e);
    }
    qDebug() << "finish connecting";
}
void                Net::connectionList(std::vector<Connection> connections)
{
    _connectionList = connections;
}
std::vector<Connection> *Net::connectionList()
{
    return &_connectionList;
}
/*void                Net::setGenomToNeuron()
{
    _update = true;
    unsigned int genomsize = 0;
    for(unsigned int x=0; x<_hiddenNeuronList.size(); x++)
    {
        for(unsigned int y=0; y<_hiddenNeuronList[x].size(); y++)
        {
            genomsize += _hiddenNeuronList[x][y]->inputs();
        }
    }
    for(unsigned int y=0; y<_outputNeuronList.size(); y++)
    {
        genomsize += _outputNeuronList[y]->inputs();
    }
    if(genomsize != _genom.size())
    {
        std::string error = "geonomsize is wrong\n";
                    error+= "genomsize is: "+std::to_string(_genom.size()) + " but the Net has only place for an amount of: "+ std::to_string((unsigned int)genomsize) + " weights.\n";
                    error+= "Check your net configuration:\n";
                    error+= "\tInput Neurons Y:\t"+std::to_string(_inputs)+"\n";
                    error+= "\tHiddenNeurons X:\t"+std::to_string(_hiddenX)+"\n";
                    error+= "\tHiddenNeurons Y:\t"+std::to_string(_hiddenY)+"\n";
                    error+= "\tOutputNeurons Y:\t"+std::to_string(_outputs);
                    error+= "\tBias"+ std::to_string(_bias);
        error_general("setGenomToNeuron()",error);
    }
    unsigned int weightPos = 0;
    if(!_noHiddenLayer)
    {
        for(unsigned int x=0; x<_hiddenNeuronList.size(); x++)
        {
            for(unsigned int y=0; y<_hiddenNeuronList[x].size(); y++)
            {
                for(unsigned int w=0; w<_hiddenNeuronList[x][y]->inputs(); w++)
                {
                    try {
                        _hiddenNeuronList[x][y]->weight(w,_genom[weightPos]);
                        weightPos++;
                    } catch (std::runtime_error &e) {
                        std::string error = "On setting the weight for the hidden neuron X: "+std::to_string(x)+" Y: "+std::to_string(y)+" weight: "+std::to_string(w) + " ";
                                    error +="with the value: "+std::to_string(_genom[weightPos]) + " from the genom on the arrayPos: "+std::to_string(weightPos);
                        error_general("setGenomToNeuron()",error,e);
                    }
                }
            }
        }
    }
    for(unsigned int y=0; y<_outputNeuronList.size(); y++)
    {
        for(unsigned int w=0; w<_outputNeuronList[y]->inputs(); w++)
        {
            try {
                _outputNeuronList[y]->weight(w,_genom[weightPos]);
                weightPos++;
            } catch (std::runtime_error &e) {
                std::string error = "On setting the weight for the output neuron Y: "+std::to_string(y)+" weight: "+std::to_string(w) + " ";
                            error +="with the value: "+std::to_string(_genom[weightPos]) + " from the genom on the arrayPos: "+std::to_string(weightPos);
                error_general("setGenomToNeuron()",error,e);
            }
        }
    }
}*/
/*void                Net::getGenomFromNeuron()
{
    _genom = std::vector<float>();
    if(!_noHiddenLayer)
    {
        for(unsigned int x=0; x<_hiddenNeuronList.size(); x++)
        {
            for(unsigned int y=0; y<_hiddenNeuronList[x].size(); y++)
            {
                for(unsigned int w=0; w<_hiddenNeuronList[x][y]->inputs(); w++)
                {
                    try {
                        _genom.push_back(_hiddenNeuronList[x][y]->weight(w));
                    } catch (std::runtime_error &e) {
                        std::string error = "On getting the weight for the hidden neuron X: "+std::to_string(x)+" Y: "+std::to_string(y)+" weight: "+std::to_string(w) + " ";
                                    error +="current genomsize: "+std::to_string(_genom.size());
                        error_general("getGenomFromNeuron()",error,e);
                    }
                }
            }
        }
    }
    for(unsigned int y=0; y<_outputNeuronList.size(); y++)
    {
        for(unsigned int w=0; w<_outputNeuronList[y]->inputs(); w++)
        {
            try {

                _genom.push_back(_outputNeuronList[y]->weight(w));

            } catch (std::runtime_error &e) {
                std::string error = "On getting the weight for the output neuron Y: "+std::to_string(y)+" weight: "+std::to_string(w) + " ";
                            error +="current genomsize: "+std::to_string(_genom.size());
                error_general("getGenomFromNeuron()",error,e);
            }
        }
    }
}*/
void        Net::prepareConnectionList()
{
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
                    _connectionList[_connectionList.size()-1].direction = true;
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
                    _connectionList[_connectionList.size()-1].direction = true;
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
                _connectionList[_connectionList.size()-1].direction = true;
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
                _connectionList[_connectionList.size()-1].direction = true;

               // _outputNeuronList[ID]->connectInput(NeuronType::input,&_inputSignalList[input]);
               // _connections++;
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
                _connectionList[_connectionList.size()-1].direction = true;
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
                _connectionList[_connectionList.size()-1].direction = true;



                //_outputNeuronList[ID]->connectInput(_hiddenNeuronList[hiddenNeuronY+((_hiddenX-1)*_hiddenY)]);
                //_connections++;
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
                _connectionList[_connectionList.size()-1].direction = true;
            }
            ID++;
        }
    }
    ID = 0;
}


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
