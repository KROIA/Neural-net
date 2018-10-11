#include "neuron.h"


Neuron::Neuron()
{
    Neuron(1);
}
Neuron::Neuron(unsigned int inputs)
{
    Neuron(inputs,Sigmoid);
}
Neuron::Neuron(unsigned int inputs, Activation activationFunction)
{
    Neuron(inputs,activationFunction,false);
}
Neuron::Neuron(unsigned int inputs, Activation activationFunction, bool enableAverage)
{
    this->inputs(inputs);
    this->activationFunction(activationFunction);
    this->enableAverage(enableAverage);

    //this->weight(std::vector<float>	(_inputs,(float)(_randEngine()%1000)/1000));
    randWeight();
    _update = true;
}

void Neuron::inputs(unsigned int inputs)
{
    //printf("Neuron::inputs(%i)\n",inputs);
    if(inputs != 0)
    {
        if(inputs != _inputs)
        {
            _inputs     = inputs;
            _weightList = std::vector<float> (_inputs,0);
            _inputList  = std::vector<float> (_inputs,0);
            randWeight();
            _update     = true;
        }
    }
    else
    {
        throw std::runtime_error("Neuron::inputs("+std::to_string(inputs)+") parameter 0 is out of range! command ignored. Minimum is 1");
    }
}
unsigned int Neuron::inputs()
{
    return _inputs;
}

void Neuron::activationFunction(Activation activationFunction)
{
    if(activationFunction < neuron_activationFunctionAmount)
    {
        _activationFunction = activationFunction;
        _update             = true;
    }
    else
    {
        throw std::runtime_error("Neuron::activationFunction("+std::to_string(activationFunction)+") parameter 0 is out of range! command ignored. Maximum is "+std::to_string(neuron_activationFunctionAmount));
    }
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
    ////printf("Time: %ih %imin %is\n",ti->tm_hour,ti->tm_min,ti->tm_sec);
    _randEngine = std::default_random_engine(rand()%100 + ti->tm_hour+ti->tm_min+ti->tm_sec);
    for(unsigned int a=0; a<_inputs; a++)
    {
         this->weight(a,(float)(_randEngine()%2000)/1000 - (float)1 );
    }
}
void Neuron::weight(unsigned int pos, float weight)
{
    if(pos < _inputs)
    {
        _weightList[pos] = weight;
    }
    else
    {
        throw std::runtime_error("Neuron::weight("+std::to_string(pos)+","+std::to_string(weight)+") parameter 0 is out of range! command ignored. Maximum is "+std::to_string(_inputs-1));
    }
}
void Neuron::weight(std::vector<float>  weightList)
{
    if(weightList.size() == _inputs)
    {
        _weightList = weightList;
        _update     = true;
    }
    else
    {
        throw std::runtime_error("Neuron::weight(std::vector<float>  weightList) parameter 0 has the wrong size! command ignored. Correct size is "+std::to_string(_inputs));
    }
}

float Neuron::weight(unsigned int pos)
{
    if(pos < _inputs)
    {
        return _weightList[pos];
    }
    else
    {
        throw std::runtime_error("Neuron::weight("+std::to_string(pos)+") parameter 0 is out of range! command ignored. Maximum is "+std::to_string(_inputs-1));
    }
}
std::vector<float> Neuron::weight()
{
    return _weightList;
}

void Neuron::input(unsigned int pos, float input)
{
    if(pos < _inputs)
    {
        _inputList[pos] = input;
        _update         = true;
    }
    else
    {
        throw std::runtime_error("Neuron::input("+std::to_string(pos)+","+std::to_string(input)+") parameter 0 is out of range! command ignored. Maximum is "+std::to_string(_inputs-1));
    }
}
void Neuron::input(std::vector<float> inputList)
{
    if(inputList.size() == _inputs)
    {
        _inputList  = inputList;
        _update     = true;
    }
    else
    {
        throw std::runtime_error("Neuron::input(std::vector<float>  weightList) parameter 0 has the wrong size! command ignored. Correct size is "+std::to_string(_inputs));
    }
}

float Neuron::input(unsigned int pos)
{
    if(pos < _inputs)
    {
        return _inputList[pos];
    }
    else
    {
        throw std::runtime_error("Neuron::weight("+std::to_string(pos)+") parameter 0 is out of range! command ignored. Maximum is "+std::to_string(_inputs-1));
    }
}
std::vector<float> Neuron::input()
{
    return _inputList;
}

float Neuron::netInput()
{
    if(_update == true)
    {
        calc_netInput();
        calc_output();
        _update = false;
    }
    return _netInput;
}
float Neuron::output()
{
    run();
    return _output;
}
void Neuron::run()
{
    if(_update == true)
    {
        calc_netInput();
        calc_output();
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
            throw std::runtime_error("Neuron::calc_output() unknown activation function \""+std::to_string(_activationFunction)+"\"");
        }
    }
}
