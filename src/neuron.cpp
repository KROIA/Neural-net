#include "neuron.h"


Neuron::Neuron()
{
    init(NEURON_MIN_INPUTS,Activation::Sigmoid,false);
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
    _delayedOutput = 0;
    _update = true;
    _randEngine = std::default_random_engine(rand()%100);
    this->ID(NeuronID{.ID = unsigned(rand()) % 30000,.TYPE = NeuronType::none});
    try {
        this->inputs(inputs);
        this->activationFunction(activationFunction);
        this->enableAverage(enableAverage);
    } catch (std::runtime_error &e) {
        error_general("init(unsigned int ["+std::to_string(inputs)+"] , Activation ["+activationString(activationFunction)+"] , bool ["+std::to_string(enableAverage)+"])",e);
    }
}
Neuron::~Neuron()
{
   _weightList.clear();
   _ptr_inputList.clear();
   _inputConnectionList.clear();
   _inputConnectionID_list.clear();
   _inputConnectionDirection_List.clear();
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
        if(inputs > _inputs)
        {
            try{
                _weightList.reserve(inputs);
                _inputConnectionList.reserve(inputs);
                _ptr_inputList.reserve(inputs);
                _inputConnectionID_list.reserve(inputs);
                _inputConnectionDirection_List.reserve(inputs);
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
                _inputConnectionDirection_List.push_back(ConnectionDirection::forward);
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

int Neuron::deleteInput(unsigned int input)

{
    if(input >= _inputs)
    {
#ifdef _DEBUG_NEURON_DELETE_INPUT
        __DEBUG_NEURON(this,"deleteInput(unsigned int ["+std::to_string(input)+"])",error_paramOutOfRange((unsigned int)0,(unsigned int)input,(unsigned int)0,(unsigned int)_inputs-1));
#endif
        return 2;
    }
    try{
        _ptr_inputList.erase(_ptr_inputList.begin()+input);
        _inputConnectionList.erase(_inputConnectionList.begin()+input);
        _weightList.erase(_weightList.begin()+input);
        _inputConnectionID_list.erase(_inputConnectionID_list.begin()+input);
        _inputConnectionDirection_List.erase(_inputConnectionDirection_List.begin()+input);
        _inputs--;
        return true;
    } catch (std::exception &e) {
        error_general("deleteInput(unsigned int ["+std::to_string(input)+"])",e.what());
        return 0;
    }

    return 1;

}

void Neuron::activationFunction(Activation activationFunction)
{
    if(activationFunction >= neuron_activationFunctionAmount)
    {
        error_general("activationFunction(Avtivation ["+activationString(activationFunction)+"] )",error_paramOutOfRange((unsigned int)0,(unsigned int)activationFunction,(unsigned int)0,(unsigned int)neuron_activationFunctionAmount));
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

float Neuron::weight(unsigned int input)
{
    if(input >= _inputs)
    {
        error_general("weight(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    return _weightList[input];
}
std::vector<float> Neuron::weight()
{
    return _weightList;
}

void Neuron::input(unsigned int input, float value)
{
    if(input >= _inputs)
    {
        error_general("input(unsigned int ["+std::to_string(input)+"] , float ["+std::to_string(value)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    try {
        *_ptr_inputList[input] = value;
    } catch (std::runtime_error &e) {
        error_general("input(unsigned int ["+std::to_string(input)+"] , float ["+std::to_string(value)+"] )","input: "+std::to_string(input)+" of Neuron is not assigned to another Neuron or variable. "+e.what());
    }
    _update         = true;
}
void Neuron::input(std::vector<float> inputList)
{
    if(inputList.size() != _inputs)
    {
        error_general("input(std::vector<float>)","parameter 0 has the wrong size: "+std::to_string(inputList.size())+" Correct size is "+std::to_string(_inputs));
    }
    try{
        for(unsigned int a=0; a<_ptr_inputList.size(); a++)
        {
            input(a,inputList[a]);
        }
    }catch(std::runtime_error &e)
    {
         error_general("input(std::vector<float>)",e);
    }
    _update     = true;
}
float Neuron::input(unsigned int input)
{
    if(input >= _inputs)
    {
        error_general("weight(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    try {
        return *_ptr_inputList[input];
    }catch(std::runtime_error &e)
    {
        error_general("weight(unsigned int ["+std::to_string(input)+"] )","no input connected. ",e);
    }
}
std::vector<float> Neuron::input()
{
    std::vector<float> retVal;
    for(unsigned int a=0; a<_inputs; a++)
    {
        try {
            retVal.push_back(*_ptr_inputList[a]);
        } catch (std::runtime_error &e) {
            __DEBUG_NEURON(this,"std::vector<float> Neuron::input()",e.what());
        }
    }
    return retVal;
}

bool Neuron::connectInput(NeuronID ID, float *ptr_sourceNeuronOutput,ConnectionDirection direction)
{
    bool allFull = true;
    for(unsigned int a=0; a<_inputs; a++)
    {
        if(_inputConnectionList[a] == false)
        {
            allFull = false;
            return connectInput(a,ID,ptr_sourceNeuronOutput,direction);
        }else if(_inputConnectionID_list[a].ID == ID.ID && _inputConnectionID_list[a].TYPE == ID.TYPE)
        {
#ifdef _DEBUG_NEURON_CONNECT
            __DEBUG_NEURON(this,"connectInput(NeuronID ["+Neuron::neuronIDString(ID)+"] , float *ptr_sourceNeuronOutput , ConnectionDirection ["+Neuron::directionSring(direction)+"] )",
                           "Connection: " + std::to_string(ID.ID) +" "+this->typeString(ID.TYPE)+" to: "+std::to_string(this->ID().ID)+" "+this->typeString(this->ID().TYPE)+" already exists.");
#endif

            return false;
        }
    }
    if(allFull)                         // If all Inputs are used, create a new one and connect to it.
    {
        inputs(_inputs+1);

        return connectInput(_inputs-1,ID,ptr_sourceNeuronOutput,direction);
    }
    return false;
}
bool Neuron::connectInput(unsigned int input, NeuronID ID, float *ptr_sourceNeuronOutput,ConnectionDirection direction)

{
    if(input >= _inputs)
    {
        error_general("connectInput(unsigned int ["+std::to_string(input)+"],NeuronID ["+std::to_string(ID.ID)+","+Neuron::typeString(ID.TYPE)+"], float [ptr])",error_paramOutOfRange(0,std::to_string(input),"0",std::to_string(_inputs-1)));
    }
    if(ptr_sourceNeuronOutput == nullptr)
    {
        error_general("connectInput(unsigned int ["+std::to_string(input)+"],NeuronID ["+std::to_string(ID.ID)+","+Neuron::typeString(ID.TYPE)+"], float [nullptr]","pointer to the Neuron doesent exist");
    }
    _inputConnectionList[input] = true;
    _inputConnectionID_list[input] = ID;

    _inputConnectionDirection_List[input] = direction;
    _ptr_inputList[input] = ptr_sourceNeuronOutput;
    return true;
}
bool Neuron::connectInput(Neuron *ptr_neuron,ConnectionDirection direction)

{
    bool allFull = true;
    for(unsigned int a=0; a<_inputs; a++)
    {
        if(_inputConnectionList[a] == false)
        {
            allFull = false;

            return connectInput(a,ptr_neuron,direction);
        }else if(_inputConnectionID_list[a].ID == ptr_neuron->ID().ID && _inputConnectionID_list[a].TYPE == ptr_neuron->ID().TYPE)
        {
#ifdef _DEBUG_NEURON_CONNECT
            __DEBUG_NEURON(this,"connectInput(Neuron *ptr_neuron , ConnectionDirection ["+Neuron::directionSring(direction)+"] )",
                           "Connection: " + std::to_string(ptr_neuron->ID().ID)+" "+this->typeString(ptr_neuron->ID().TYPE)+" to: "+std::to_string(this->ID().ID)+" "+this->typeString(this->ID().TYPE)+" already exists.");
#endif

            return false;
        }
    }
    if(allFull)                         // If all Inputs are used, create a new one and connect to it.
    {
        inputs(_inputs+1);

        return connectInput(_inputs-1,ptr_neuron,direction);
    }
    return false;
}
bool Neuron::connectInput(unsigned int input,Neuron *ptr_neuron,ConnectionDirection direction)

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
        if(_inputConnectionID_list[connection].ID == ptr_neuron->ID().ID && _inputConnectionID_list[connection].TYPE == ptr_neuron->ID().TYPE)
        {

#ifdef _DEBUG_NEURON_CONNECT
            __DEBUG_NEURON(this,"connectInput(unsigned int ["+std::to_string(input)+"] , Neuron *ptr_neuron , ConnectionDirection ["+Neuron::directionSring(direction)+"] )",
                           "Trying to connect a connection which already exists. ignore command. Input: "+std::to_string(input)+" Neuron: "+std::to_string((unsigned int)ptr_neuron)+" direction: "+ Neuron::directionSring(direction));
#endif

            return false;
        }
    }
    _inputConnectionList[input] = true;
    _inputConnectionID_list[input] = ptr_neuron->ID();

    _inputConnectionDirection_List[input] = direction;
    if(direction == ConnectionDirection::forward)
    {
        _ptr_inputList[input] = ptr_neuron->ptr_output();
    }else{
        _ptr_inputList[input] = ptr_neuron->ptr_loopBackOutput();
    }

    return true;
}
bool Neuron::disconnect(unsigned int input)
{
    if(input >= _inputs)
    {
#ifdef _DEBUG_NEURON_CONNECT
        __DEBUG_NEURON(this,"disconnect(unsigned int ["+std::to_string(input)+"])",
                       error_paramOutOfRange(0,std::to_string(input),"0",std::to_string(_inputs-1)));
#endif
    }
    if(!_inputConnectionList[input])
        return false;
    _ptr_inputList[input] = nullptr;
    _inputConnectionList[input] = false;
    _inputConnectionID_list[input] = NeuronID{.ID = NEURON_ID_INVALID,.TYPE = NeuronType::none};
    _inputs--;
    return true;
}
bool Neuron::disconnect(NeuronID sourceNeuron)
{
    for(unsigned int input=0; input<_inputConnectionID_list.size(); input++)
    {
        if(_inputConnectionID_list[input].ID    == sourceNeuron.ID &&
           _inputConnectionID_list[input].TYPE  == sourceNeuron.TYPE)
        {
            return disconnect(input);
        }
    }

#ifdef _DEBUG_NEURON_CONNECT
    __DEBUG_NEURON(this,"disconnect(NeuronID ["+Neuron::neuronIDString(sourceNeuron)+"])",
                   "No neuron with such an ID is connected to this neuron");
#endif
    return false;
}
ConnectionDirection Neuron::inputConnectionDirection(NeuronID inputID)
{
    for(unsigned int input=0; input<_inputConnectionID_list.size(); input++)
    {
        if(_inputConnectionID_list[input].ID == inputID.ID && _inputConnectionID_list[input].TYPE == inputID.TYPE)
        {
            return _inputConnectionDirection_List[input];
        }
    }
    error_general("inputConnectionDirection(NeuronID [.ID="+std::to_string(inputID.ID)+",.TYPE="+Neuron::typeString(inputID.TYPE)+"])","No connection with such an ID");
}
std::vector<ConnectionDirection>   Neuron::inputConnectionDirection()
{
    return _inputConnectionDirection_List;

}



float Neuron::netInput()
{
    return _netInput;
}
float Neuron::output()
{
    return _output;
}
float *Neuron::ptr_output()
{
    return &_output;
}
float *Neuron::ptr_loopBackOutput()
{
    return &_delayedOutput;
}
NeuronID Neuron::inputID(unsigned int input)
{
    if(input >= _inputs)
    {
         error_general("inputID(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    return _inputConnectionID_list[input];
}
std::vector<NeuronID> Neuron::inputID()
{
    return _inputConnectionID_list;
}
float *Neuron::ptr_weight(unsigned int input)
{
    if(input >= _inputs)
    {
         error_general("ptr_weight(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    return &_weightList[input];
}
float *Neuron::ptr_weight(NeuronID  connectionID)

{
    for(unsigned int a=0; a<_inputConnectionID_list.size(); a++)
    {
        if(connectionID.ID == _inputConnectionID_list[a].ID &&
           connectionID.TYPE == _inputConnectionID_list[a].TYPE)
        {
            return ptr_weight(a);
        }
    }
    error_general("ptr_weight(NeuronID [.ID="+std::to_string(connectionID.ID)+",.TYPE="+Neuron::typeString(connectionID.TYPE)+"])","No connection with such an ID");
}


void Neuron::run()
{
    if(_update == true)
    {
#if defined(_DEBUG_NEURON_RUN)
        __DEBUG_NEURON(this,"run()","begin");
#endif
#if defined(_DEBUG_NEURON_TIMING)
        __debug_timer1 = std::chrono::high_resolution_clock::now();
#endif
        calc_netInput();
        try {
            calc_output();
        } catch (std::runtime_error &e) {
            error_general("run()",e);
        }
        _update = false;
#if defined(_DEBUG_NEURON_TIMING)
        __debug_timer2 = std::chrono::high_resolution_clock::now();
        __debug_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(__debug_timer2 - __debug_timer1);
        __debug_run_time = __debug_time_span.count()*1000;
#endif
#if defined(_DEBUG_NEURON_RUN)
#if defined(_DEBUG_NEURON_TIMING)
        __DEBUG_NEURON(this,"run()","end, time: "+std::to_string(__debug_run_time)+" ms");
#else
        __DEBUG_NEURON(this,"run()","end");
#endif
#endif
    }
}
const std::string Neuron::neuronIDString(NeuronID ID)
{
    return "NeuronID {\nID: "+std::to_string(ID.ID)+"\nTYPE: "+Neuron::typeString(ID.TYPE)+"}\n";
}
const std::string Neuron::typeString(NeuronType TYPE)
{
    std::string str = "["+std::to_string(TYPE)+"] ";
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
        case NeuronType::costum:
        {
            str = "costum";
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
    std::string str = "["+std::to_string(activationFunction)+"] ";
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

const std::string Neuron::connectionString(Connection connection)
{
    std::string message;
    message  = "netID:  "+std::to_string(connection.netID)+"\n"+
               "SourceNeuron {\n"+
               "ID:     "+std::to_string(connection.source_ID.ID)+"\n"+
               "TYPE:   "+Neuron::typeString(connection.source_ID.TYPE)+"}\n"+
               "DestinationNeuron {\n"+
               "ID:     "+std::to_string(connection.destination_ID.ID)+"\n"+
               "TYPE:   "+Neuron::typeString(connection.destination_ID.TYPE)+"}\n"+
               "weight: "+std::to_string(connection.weight)+"\n"+
               "direction: "+Neuron::directionSring(connection.direction);
    return message;
}
const std::string Neuron::directionSring(ConnectionDirection dir)
{
    if(dir == ConnectionDirection::forward)
        return "forward";
    return "backward";
}
void Neuron::needsUpdate()
{
    _delayedOutput = _output;

    _update = true;
}
bool Neuron::isUpdated()
{
    return !_update;
}
void Neuron::calc_netInput()
{
#if defined(_DEBUG_NEURON_RUN)
    __DEBUG_NEURON(this,"calc_netInput()","begin");
#endif
    _netInput = 0;
    for(unsigned int a=0; a<_inputs; a++)
    {
        if(_ptr_inputList[a] == nullptr)
        {
#if defined(_DEBUG_NEURON_RUN)
            __DEBUG_NEURON(this,"calc_netInput()","input: "+std::to_string(a)+" is not connected");
#endif
            continue;
        }
        _netInput += *_ptr_inputList[a] * _weightList[a];
    }
    if(_enableAverage == true)
    {
        _netInput /= _inputs;
    }
#if defined(_DEBUG_NEURON_RUN)
    __DEBUG_NEURON(this,"calc_netInput()","end, netInput: "+std::to_string(_netInput));
#endif
}
void Neuron::calc_output()
{
#if defined(_DEBUG_NEURON_RUN)
    __DEBUG_NEURON(this,"calc_output()","begin, activation: "+Neuron::activationString(_activationFunction));
#endif
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
#if defined(_DEBUG_NEURON_RUN)
             __DEBUG_NEURON(this,"calc_output()","unknown activation function ["+std::to_string(_activationFunction)+"]");
#endif
            error_general("calc_output()","unknown activation function ["+std::to_string(_activationFunction)+"]");
        }
    }
#if defined(_DEBUG_NEURON_RUN)
    __DEBUG_NEURON(this,"calc_output()","end, output: "+std::to_string(_output));
#endif
}
#if defined(_DEBUG_NEURON_TIMING)
double Neuron::debug_runtime()
{
    return __debug_run_time;
}
#endif
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
