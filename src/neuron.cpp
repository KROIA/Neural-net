#include "neuron.h"


Neuron::Neuron()
{
    init(1,Activation::Sigmoid,false);
}
Neuron::Neuron(unsigned int inputs)
{
    init(inputs,Activation::Sigmoid,false);
}
Neuron::Neuron(unsigned int inputs, Activation activationFunction)
{
    init(inputs,activationFunction,false);
}
Neuron::Neuron(unsigned int inputs, Activation activationFunction, bool enableAverage)
{
    init(inputs,activationFunction,enableAverage);
}
void Neuron::init(unsigned int inputs, Activation activationFunction, bool enableAverage)
{
    _inputs = 0;
    _netInput = 0;
    _output = 0;
    _update = true;
    _randEngine = std::default_random_engine(rand()%100 /*+ ti->tm_hour+ti->tm_min+ti->tm_sec*/);
    this->ID(NeuronID{.ID = unsigned(rand()) % 30000,.TYPE = NeuronType::none});
    try {
        this->inputs(inputs);
        this->activationFunction(activationFunction);
        this->enableAverage(enableAverage);
    } catch (std::runtime_error &e) {
        error_general("init(unsigned int ["+std::to_string(inputs)+"] , Activation ["+std::to_string(activationFunction)+"] , bool ["+std::to_string(enableAverage)+"])",e);
    }

}
Neuron::~Neuron()
{
   _weightList.clear();
   _ptr_inputList.clear();
   _inputConnectionList.clear();
   _inputConnectionID_list.clear();
}
void Neuron::ID(NeuronID ID)
{
    _ID = ID;
}
void Neuron::ID(unsigned int ID)
{
    _ID.ID = ID;
}
void Neuron::TYPE(NeuronType TYPE)
{
    _ID.TYPE = TYPE;
}
NeuronID Neuron::ID()
{
    return _ID;
}


void Neuron::inputs(unsigned int inputs)
{
    if(inputs < NEURON_MIN_INPUTS || inputs > NEURON_MAX_INPUTS)
    {
        error_general("inputs(unsigned int ["+std::to_string(inputs)+"] )",error_paramOutOfRange(0,inputs,(unsigned int)NEURON_MIN_INPUTS,(unsigned int)NEURON_MAX_INPUTS));
    }
    if(inputs != _inputs)
    {
        //_weightList = std::vector<float> (inputs,0);
        //_inputList  = std::vector<float> (inputs,0);
        if(inputs > _inputs)
        {
            try{
                _weightList.reserve(inputs);
                _inputConnectionList.reserve(inputs);
                _ptr_inputList.reserve(inputs);
                _inputConnectionID_list.reserve(inputs);
            }
            catch (std::exception &e) {
                error_general("inputs(unsigned int ["+std::to_string(inputs)+"])",e.what());
            }
        }
        while(inputs > _inputs)
        {
            try {
                _ptr_inputList.push_back(nullptr);
                _inputConnectionList.push_back(false);
                _weightList.push_back(0);
                _inputConnectionID_list.push_back(NeuronID{.ID = NEURON_ID_INVALID,.TYPE = NeuronType::none});
                _inputs++;
                randWeight(_inputs-1);
            } catch (std::exception &e) {
                error_general("inputs(unsigned int ["+std::to_string(inputs)+"])",e.what());
            }catch (...) {
                error_general("inputs(unsigned int ["+std::to_string(inputs)+"])","unnkown");
            }
        }
        while(inputs < _inputs)
        {
            try{
                deleteInput(_inputs-1);
            } catch (std::exception &e) {
                error_general("inputs(unsigned int ["+std::to_string(inputs)+"])",e.what());
            }catch (...) {
                error_general("inputs(unsigned int ["+std::to_string(inputs)+"])","unnkown");
            }
        }
        _update     = true;
    }
}
unsigned int Neuron::inputs()
{
    return _inputs;
}
void Neuron::deleteInput(unsigned int input)
{
    if(input >= _inputs)
    {
        error_general("deleteInput(unsigned int ["+std::to_string(input)+"])",error_paramOutOfRange(0,std::to_string(input),"0",std::to_string(_inputs-1)));
    }

    try{

        //delete _ptr_inputList[input];
        _ptr_inputList.erase(_ptr_inputList.begin()+input);
        _inputConnectionList.erase(_inputConnectionList.begin()+input);
        _weightList.erase(_weightList.begin()+input);
        _inputConnectionID_list.erase(_inputConnectionID_list.begin()+input);
        _inputs--;
    } catch (std::exception &e) {
        error_general("deleteInput(unsigned int ["+std::to_string(input)+"])",e.what());
    }
}

void Neuron::activationFunction(Activation activationFunction)
{
    if(activationFunction >= neuron_activationFunctionAmount)
    {
        error_general("activationFunction(Avtivation ["+std::to_string(activationFunction)+"] )",error_paramOutOfRange((unsigned int)0,(unsigned int)activationFunction,(unsigned int)0,(unsigned int)neuron_activationFunctionAmount));
    }
    _activationFunction = activationFunction;
    _update             = true;
}
Activation Neuron::activationFunction()
{
    return _activationFunction;
}

void Neuron::enableAverage(bool enableAverage)
{
    if(_enableAverage != enableAverage)
    {
        _enableAverage = enableAverage;
        _update        = true;
    }
}
bool Neuron::enableAverage()
{
    return _enableAverage;
}
float Neuron::calcRandWeight(std::default_random_engine &randEngine)
{
    return (float)(randEngine() %2000)/1000 - (float)1;
}
void Neuron::randWeight()
{
    for(unsigned int a=0; a<_inputs; a++)
    {
        randWeight(a);
    }
}
void Neuron::randWeight(unsigned int input)
{
    this->weight(input,calcRandWeight(_randEngine));
}
void Neuron::weight(unsigned int pos, float weight)
{
    if(pos >= _inputs)
    {
        error_general("weight(unsigned int ["+std::to_string(pos)+"] , float ["+std::to_string(weight)+"] )",error_paramOutOfRange((unsigned int)0,pos,(unsigned int)0,_inputs-1));
    }
    _weightList[pos] = weight;
}
void Neuron::weight(std::vector<float>  weightList)
{
    if(weightList.size() != _inputs)
    {
        error_general("weight(std::vector<float>)","parameter 0 has the wrong size: "+std::to_string(weightList.size())+" Correct size is "+std::to_string(_inputs));
    }
    _weightList = weightList;
    _update     = true;
}
void Neuron::weight(NeuronID ID,float weight)
{
    for(unsigned int a=0; a<_inputConnectionID_list.size(); a++)
    {
        if(_inputConnectionID_list[a].ID == ID.ID && _inputConnectionID_list[a].TYPE == ID.TYPE)
        {
            this->weight(a,weight);
            return;
        }
    }
    error_general("weight(NeuronID [.ID="+std::to_string(ID.ID)+",.TYPE="+Neuron::typeString(ID.TYPE)+"],float ["+std::to_string(weight)+"])","No input with such an ID");
}

float Neuron::weight(unsigned int pos)
{
    if(pos >= _inputs)
    {
        error_general("weight(unsigned int ["+std::to_string(pos)+"] )",error_paramOutOfRange((unsigned int)0,pos,(unsigned int)0,_inputs-1));
    }
    return _weightList[pos];
}
std::vector<float> Neuron::weight()
{
    return _weightList;
}

/*void Neuron::input(unsigned int pos, float input)
{
    if(pos >= _inputs)
    {
        error_general("input(unsigned int ["+std::to_string(pos)+"] , float ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,pos,(unsigned int)0,_inputs-1));
    }
    //_inputList[pos] = input;
    _update         = true;
}*/
/*void Neuron::input(std::vector<float> inputList)
{
    if(inputList.size() != _inputs)
    {
        error_general("input(std::vector<float>)","parameter 0 has the wrong size: "+std::to_string(inputList.size())+" Correct size is "+std::to_string(_inputs));
    }
    //_inputList  = inputList;
    _update     = true;
}*/
float Neuron::input(unsigned int input)
{
    if(input >= _inputs)
    {
        error_general("weight(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    if(_ptr_inputList[input] == nullptr)
    {
        error_general("weight(unsigned int ["+std::to_string(input)+"] )","no input connected");
    }
    return *_ptr_inputList[input];
}
std::vector<float> Neuron::input()
{
    //return _inputList;
    //bastel
    std::vector<float> retVal(_inputs,0);
    for(unsigned int a=0; a<_inputs; a++)
    {
        if(_ptr_inputList[a] == nullptr)
        {
            continue;
        }
        retVal[a] = *_ptr_inputList[a];
    }
    return retVal;
}


/*void Neuron::connectInput(float *ptr_float)
{
    bool allFull = true;
    for(unsigned int a=0; a<_inputs; a++)
    {
        if(_inputConnectionList[a] == false)
        {
            allFull = false;
            connectInput(a,ptr_float);
            break;
        }
    }
    if(allFull)                         // If all Inputs are used, create a new one and connect to it.
    {
        inputs(_inputs+1);
        connectInput(_inputs-1,ptr_float);
    }
}*/
/*void Neuron::connectInput(NeuronID ID, float *ptr_float)
{
    connectInput(input,ID,ptr_float);
}*/
void Neuron::connectInput(NeuronID ID, float *ptr_float)
{
    bool allFull = true;
    for(unsigned int a=0; a<_inputs; a++)
    {
        if(_inputConnectionList[a] == false)
        {
            allFull = false;
            connectInput(a,ID,ptr_float);
            break;
        }else if(_inputConnectionID_list[a].ID == ID.ID && _inputConnectionID_list[a].TYPE == ID.TYPE)
        {
            qDebug() << "Connection: " << ID.ID<<" "<<QString::fromStdString(this->typeString(ID.TYPE))<<" to: "<<this->ID().ID<<" "<<QString::fromStdString(this->typeString(this->ID().TYPE))<<" already exists.";
            return;
        }
    }
    if(allFull)                         // If all Inputs are used, create a new one and connect to it.
    {
        inputs(_inputs+1);
        connectInput(_inputs-1,ID,ptr_float);
    }
}
void Neuron::connectInput(unsigned int input, NeuronID ID, float *ptr_float)
{
    if(input >= _inputs)
    {
        error_general("connectInput(unsigned int ["+std::to_string(input)+"],NeuronID ["+std::to_string(ID.ID)+","+Neuron::typeString(ID.TYPE)+"], float [ptr])",error_paramOutOfRange(0,std::to_string(input),"0",std::to_string(_inputs-1)));
    }
    if(ptr_float == nullptr)
    {
        error_general("connectInput(unsigned int ["+std::to_string(input)+"],NeuronID ["+std::to_string(ID.ID)+","+Neuron::typeString(ID.TYPE)+"], float [nullptr]","pointer to the Neuron doesent exist");
    }
    _inputConnectionList[input] = true;
    _inputConnectionID_list[input] = ID;
    _ptr_inputList[input] = ptr_float;
}
void Neuron::connectInput(Neuron *ptr_neuron)
{
    bool allFull = true;
    for(unsigned int a=0; a<_inputs; a++)
    {
        if(_inputConnectionList[a] == false)
        {
            allFull = false;
            connectInput(a,ptr_neuron);
            break;
        }else if(_inputConnectionID_list[a].ID == ptr_neuron->ID().ID && _inputConnectionID_list[a].TYPE == ptr_neuron->ID().TYPE)
        {
            qDebug() << "Connection: " << ptr_neuron->ID().ID<<" "<<QString::fromStdString(this->typeString(ptr_neuron->ID().TYPE))<<" to: "<<this->ID().ID<<" "<<QString::fromStdString(this->typeString(this->ID().TYPE))<<" already exists.";
            return;
        }
    }
    if(allFull)                         // If all Inputs are used, create a new one and connect to it.
    {
        inputs(_inputs+1);
        connectInput(_inputs-1,ptr_neuron);
    }
}
void Neuron::connectInput(unsigned int input,Neuron *ptr_neuron)
{
    if(input >= _inputs)
    {
        error_general("connectInput(unsigned int ["+std::to_string(input)+"],Neuron [ptr])",error_paramOutOfRange(0,std::to_string(input),"0",std::to_string(_inputs-1)));
    }
    if(ptr_neuron == nullptr)
    {
        error_general("connectInput(unsigned int ["+std::to_string(input)+"],Neuron [nullptr]","pointer to the Neuron doesent exist");
    }
    for(unsigned int connection=0; connection<_inputConnectionID_list.size(); connection++)
    {
        if(_inputConnectionID_list[connection].ID == ptr_neuron->ID().ID)
        {
            qDebug() << "Trying to connect a connection which already exists. ignore command";
            return;
        }
    }
    _inputConnectionList[input] = true;
    _inputConnectionID_list[input] = ptr_neuron->ID();
    _ptr_inputList[input] = ptr_neuron->ptr_output();
}
void Neuron::disconnect(unsigned int input)
{
    if(input >= _inputs)
    {
        error_general("disconnect(unsigned int ["+std::to_string(input)+"])",error_paramOutOfRange(0,std::to_string(input),"0",std::to_string(_inputs-1)));
    }
    deleteInput(input);
}



float Neuron::netInput()
{
    try {
        run();
    } catch (std::runtime_error &e) {
        error_general("netInput()",e);
    }
    return _netInput;
}
float Neuron::output()
{
    try {
        run();
    } catch (std::runtime_error &e) {
        error_general("output()",e);
    }
    return _output;
}
float *Neuron::ptr_output()
{
    return &_output;
}
NeuronID Neuron::inputID(unsigned int input)
{
    if(input >= _inputs)
    {
         error_general("inputID(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    return _inputConnectionID_list[input];
}
float *Neuron::ptr_weight(unsigned int input)
{
    if(input >= _inputs)
    {
         error_general("ptr_weight(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    return &_weightList[input];
}
std::vector<NeuronID> Neuron::inputID()
{
    return _inputConnectionID_list;
}

void Neuron::run()
{
    //if(_update == true)
    {
        calc_netInput();
        try {
            calc_output();
        } catch (std::runtime_error &e) {
            error_general("run()",e);
        }
        _update = false;
    }
}
const std::string Neuron::typeString(NeuronType TYPE)
{
    std::string str;
    switch(TYPE)
    {
        case NeuronType::none:
        {
            str = "none";
            break;
        }
        case NeuronType::input:
        {
            str = "input";
            break;
        }
        case NeuronType::hidden:
        {
            str = "hidden";
            break;
        }
        case NeuronType::output:
        {
            str = "output";
            break;
        }
        case NeuronType::bias:
        {
            str = "bias";
            break;
        }
        default:
        {
            str = "unknown";
            break;
        }
    }
    return str;
}
const std::string Neuron::activationString(Activation activationFunction)
{
    std::string str;
    switch(activationFunction)
    {
        case Activation::Linear:
        {
            str = "Linear";
            break;
        }
        case Activation::ReLu:
        {
            str = "ReLu";
            break;
        }
        case Activation::Binary:
        {
            str = "Binary";
            break;
        }
        case Activation::Gaussian:
        {
            str = "Gaussian";
            break;
        }
        case Activation::Sigmoid:
        {
            str = "Sigmoid";
            break;
        }
        default:
        {
            str = "unknown";
            break;
        }
    }
    return str;
}
void Neuron::calc_netInput()
{
    _netInput = 0;
    for(unsigned int a=0; a<_inputs; a++)
    {
        //_netInput += _inputList[a] * _weightList[a];
        if(_ptr_inputList[a] == nullptr)
        {
            continue;
        }
        _netInput += *_ptr_inputList[a] * _weightList[a];
    }
    if(_enableAverage == true)
    {
        _netInput /= _inputs;
    }
}
void Neuron::calc_output()
{
    switch(_activationFunction)
    {
        case Activation::Linear:
        {
            _output = this->activation_Linear(_netInput);
            break;
        }
        case Activation::ReLu:
        {
            _output = this->activation_ReLu(_netInput);
            break;
        }
        case Activation::Binary:
        {
            _output = this->activation_Binary(_netInput);
            break;
        }
        case Activation::Gaussian:
        {
            _output = this->activation_Gaussian(_netInput);
            break;
        }
        case Activation::Sigmoid:
        {
            _output = this->activation_Sigmoid(_netInput);
            break;
        }
        default:
        {
            error_general("calc_output()","unknown activation function ["+std::to_string(_activationFunction)+"]");
        }
    }
}

//----------ERROR

std::string Neuron::error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max)
{
    return " parameter "+std::to_string(paramPos)+" is out of range: "+value+"     minimum is: "+min+"     maximum is: "+max;
}
std::string Neuron::error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string Neuron::error_paramOutOfRange(unsigned int paramPos,int value,int min, int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string Neuron::error_paramOutOfRange(unsigned int paramPos,float value,float min, float max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
void        Neuron::error_general(std::string function, std::string cause)
{
    throw std::runtime_error("ERROR: Neuron::" + function + "     " + cause);
}
void        Neuron::error_general(std::string function, std::runtime_error &e)
{
    error_general(function,"",e);
}
void        Neuron::error_general(std::string function, std::string cause, std::runtime_error &e)
{
    std::string error = "ERROR: Neuron::" + function + "     " + cause;
    error += "     --> "+std::string(e.what());
    throw std::runtime_error(error);
}
