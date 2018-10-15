#include "neuron.h"


Neuron::Neuron()
{
    try {
        Neuron((unsigned int)NEURON_MIN_INPUTS);
    } catch (std::runtime_error *e) {
        error_general("Neuron()",e);
    }
}
Neuron::Neuron(unsigned int inputs)
{
    try {
        Neuron(inputs,Sigmoid);
    } catch (std::runtime_error *e) {
        error_general("Neuron(unsigned int ["+std::to_string(inputs),e);
    }
}
Neuron::Neuron(unsigned int inputs, Activation activationFunction)
{
    try {
        Neuron(inputs,activationFunction,false);
    } catch (std::runtime_error *e) {
        error_general("Neuron(unsigned int ["+std::to_string(inputs)+
                         "] , Avtication ["+std::to_string(activationFunction)+"]",e);
    }
}
Neuron::Neuron(unsigned int inputs, Activation activationFunction, bool enableAverage)
{
    try {
        this->inputs(inputs);
        this->activationFunction(activationFunction);
        this->enableAverage(enableAverage);
    } catch (std::runtime_error *e) {
        error_general("Neuron(unsigned int ["+std::to_string(inputs)+"] , Activation ["+std::to_string(activationFunction)+"] , bool ["+std::to_string(enableAverage)+"])",e);
    }
    randWeight();
    _update = true;
}

void Neuron::inputs(unsigned int inputs)
{
    if(inputs < NEURON_MIN_INPUTS || inputs > NEURON_MAX_INPUTS)
    {
        error_general("inputs(unsigned int ["+std::to_string(inputs)+"] )",error_paramOutOfRange(0,inputs,(unsigned int)NEURON_MIN_INPUTS,(unsigned int)NEURON_MAX_INPUTS));
    }
    if(inputs != _inputs)
    {
        _inputs     = inputs;
        _weightList = std::vector<float> (_inputs,0);
        _inputList  = std::vector<float> (_inputs,0);
        randWeight();
        _update     = true;
    }
}
unsigned int Neuron::inputs()
{
    return _inputs;
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

void Neuron::randWeight()
{
    time_t timer;
    time(&timer);
    ti = localtime(&timer);
    _randEngine = std::default_random_engine(rand()%100 + ti->tm_hour+ti->tm_min+ti->tm_sec);
    for(unsigned int a=0; a<_inputs; a++)
    {
         this->weight(a,(float)(_randEngine()%2000)/1000 - (float)1 );
    }
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

void Neuron::input(unsigned int pos, float input)
{
    if(pos >= _inputs)
    {
        error_general("input(unsigned int ["+std::to_string(pos)+"] , float ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,pos,(unsigned int)0,_inputs-1));
    }
    _inputList[pos] = input;
    _update         = true;
}
void Neuron::input(std::vector<float> inputList)
{
    if(inputList.size() != _inputs)
    {
        error_general("input(std::vector<float>)","parameter 0 has the wrong size: "+std::to_string(inputList.size())+" Correct size is "+std::to_string(_inputs));
    }
    _inputList  = inputList;
    _update     = true;
}

float Neuron::input(unsigned int pos)
{
    if(pos >= _inputs)
    {
        error_general("weight(unsigned int ["+std::to_string(pos)+"] )",error_paramOutOfRange((unsigned int)0,pos,(unsigned int)0,_inputs-1));
    }
    return _inputList[pos];
}
std::vector<float> Neuron::input()
{
    return _inputList;
}

float Neuron::netInput()
{
    try {
        run();
    } catch (std::runtime_error *e) {
        error_general("netInput()",e);
    }
    return _netInput;
}
float Neuron::output()
{
    try {
        run();
    } catch (std::runtime_error *e) {
        error_general("output()",e);
    }
    return _output;
}
void Neuron::run()
{
    if(_update == true)
    {
        calc_netInput();
        try {
            calc_output();
        } catch (std::runtime_error *e) {
            error_general("run()",e);
        }
        _update = false;
    }
}

void Neuron::calc_netInput()
{
    _netInput = 0;
    for(unsigned int a=0; a<_inputs; a++)
    {
        _netInput += _inputList[a] * _weightList[a];
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
    return " parameter "+std::to_string(paramPos)+" is out of range: "+value+"\tminimum is: "+min+"\tmaximum is: "+max;
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
void        Neuron::error_general(std::string function, std::runtime_error *e)
{
    error_general(function,"",e);
}
void        Neuron::error_general(std::string function, std::string cause, std::runtime_error *e)
{
    std::string error = "ERROR: Neuron::" + function + "\t" + cause;
    if(e != nullptr)
    {
        error += "\n --> "+std::string(e->what());
    }
    throw std::runtime_error(error + "\n");
}
