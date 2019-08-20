#include "net.h"


Net::Net()
{
    init(2,1,2,1,true,false,Activation::Sigmoid);
}

Net::Net(unsigned int inputs,
         unsigned int hiddenX,
         unsigned int hiddenY,
         unsigned int outputs)
{
    init(inputs,hiddenX,hiddenY,outputs,true,false,Activation::Sigmoid);
}

Net::Net(unsigned int inputs,
         unsigned int hiddenX,
         unsigned int hiddenY,
         unsigned int outputs,
         bool enableBias,
         bool enableAverage,
         Activation func)
{
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
    _outputs = NET_MIN_OUTPUTNEURONS;
    _bias = true;
    _enableAverage = false;
    _ptr_biasValue = new float(0);
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
    for(unsigned int a=_hiddenNeuronList.size(); a>0; a--)
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
   /* for(unsigned int input = 0; input < _inputs; input++)
    {
        try {
            delete _ptr_inputSignalList[input];
        } catch (std::exception &e) {
            error_general("~Net()",e.what());
        }
    }*/
    _hiddenNeuronList.clear();
    _outputNeuronList.clear();
    _inputSignalList.clear();
   // _ptr_inputSignalList.clear();
    //_hiddenSignalList.clear();
    _genom.clear();

    delete _ptr_biasValue;
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
    }
}
unsigned int        Net::outputNeurons()
{
    return _outputs;
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
    *_ptr_biasValue     = _biasValue;
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
    unsigned int genomsize	= 0;
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
    setGenomToNeuron();
}
void                Net::genom(std::vector<float> genom)
{
    if(genom.size() != _genom.size())
    {
        error_general("genom(std::vector<float>)","parameter 0 has the wrong array size: "+std::to_string(genom.size())+" array size should by "+std::to_string(_genom.size()));
    }
    _genom = genom;
    setGenomToNeuron();
}
std::vector<float>  Net::genom()
{
    try {
        getGenomFromNeuron();
    } catch (std::runtime_error &e) {
        error_general("genom()",e);
    }
    return _genom;
}
unsigned int        Net::genomsize()
{
    try {
        getGenomFromNeuron();
    } catch (std::runtime_error &e) {
        error_general("genomsize()",e);
    }
    return _genom.size();
}

void                Net::input(unsigned int input, float signal)
{
    if(input > _inputs-1)
    {
        error_general("input(unsigned int ["+std::to_string(input)+"], float ["+std::to_string(signal)+"])",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    _update = true;
    _inputSignalList[input] = signal;
   /* if(_noHiddenLayer)
    {
        for(unsigned int y=0; y<_outputNeuronList.size(); y++)
        {
            try {
              //  *_ptr_inputSignalList[input] = signal;
                _outputNeuronList[y]->input(input,signal);
            } catch (std::runtime_error &e) {
                error_general("input(unsigned int ["+std::to_string(input)+"] , float ["+std::to_string(signal)+"] )","",e);
            }
        }
    }
    else
    {
        for(unsigned int y=0; y<_hiddenNeuronList[0].size(); y++)
        {

            try {
            //    *_ptr_inputSignalList[input] = signal;
                _hiddenNeuronList[0][y]->input(input,signal);
            } catch (std::runtime_error &e) {
                error_general("input(unsigned int ["+std::to_string(input)+"] , float ["+std::to_string(signal)+"] )","",e);
            }
        }
    }*/
}
float               Net::input(unsigned int input)
{
    if(input > _inputs-1)
    {
        error_general("input(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
   /* float inp = 0;
    if(_noHiddenLayer)
    {
        try {
            inp = _outputNeuronList[0]->input(input);
        } catch (std::runtime_error &e) {
            error_general("input(unsigned int ["+std::to_string(input)+"] )","output neuron: 0 input: "+std::to_string(input),e);
        }
    }
    else
    {
        try {
            inp = _hiddenNeuronList[0][0]->input(input);
        } catch (std::runtime_error &e) {
            error_general("input(unsigned int ["+std::to_string(input)+"] )","hidden neuron X: 0 hidden neuron Y: 0",e);
        }
    }
    return inp;*/
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
        //*_ptr_inputSignalList[a] = inputList[a];
        _inputSignalList[a] = inputList[a];
    }

}
std::vector<float>  Net::input()
{
    //bastel
   // std::vector<float>  retVal(_inputs,0);
    for(unsigned int a=0; a<_inputs; a++)
    {
        //retVal[a] = *_ptr_inputSignalList[a];
    }
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
    run();
    return _hiddenNeuronList[hiddenX][hiddenY]->output();
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
    run();
    std::vector<float> ret(_hiddenY,0);
    for(unsigned int y=0; y<_hiddenY; y++)
    {
        ret[y] = _hiddenNeuronList[hiddenX][y]->output();
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
    run();
    std::vector<float> ret(_hiddenX,0);
    for(unsigned int x=0; x<_hiddenX; x++)
    {
        ret[x] = _hiddenNeuronList[x][hiddenY]->output();
    }
    return ret;
}

Neuron              *Net::hiddenNeuron(unsigned int hiddenX, unsigned int hiddenY)
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
    run();
    return _hiddenNeuronList[hiddenX][hiddenY];
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
    run();
    return _hiddenNeuronList[hiddenX];
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
    run();
    std::vector<Neuron*> ret;
    for(unsigned int x=0; x<_hiddenX; x++)
    {
        ret.push_back(_hiddenNeuronList[x][hiddenY]);
    }
    return ret;
}
std::vector<std::vector<Neuron*> > *Net::hiddenNeuron()
{
    if(_noHiddenLayer)
    {
        error_general("hiddenNeuron()","the network has no hidden layer");
    }
    run();
    return &_hiddenNeuronList;
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


float               Net::output(unsigned int output)
{
    if(output > _outputs)
    {
        error_general("output(unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,output,(unsigned int)0,_outputs-1));
    }
    run();
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
    if(_update)
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
    }
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
    for(unsigned int a=_hiddenNeuronList.size(); a>0; a--)
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
    }
    //unsigned int abc = _ptr_inputSignalList.size();
    /*for(unsigned int a=0; a<abc; a++)
    {
        if(_ptr_inputSignalList[a] != nullptr)
            delete _ptr_inputSignalList[a];     // causes Problem:  HEAP[snake.exe]:
                                                //                  Invalid address specified to RtlFreeHeap( 188E0000, 1A0511A0 )
    }*/
    /*_ptr_inputSignalList.clear();
    for(unsigned int a=0; a<_inputs; a++)
    {
        _ptr_inputSignalList.push_back(new float(0));
    }*/
    _inputSignalList.clear();
    _inputSignalList.reserve(_inputs);
    for(unsigned int input=0; input<_inputs; input++)
    {
        _inputSignalList.push_back(0);
    }

    //- Connect the inputs
    if(_noHiddenLayer)
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
    }
    //- connecting done

    _updateNetConfiguration = false;
    try {
        randomGenom();
    } catch (std::runtime_error &e) {
        error_general("updateNetConfiguration()",e);
    }
}

void                Net::setGenomToNeuron()
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
}
void                Net::getGenomFromNeuron()
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
