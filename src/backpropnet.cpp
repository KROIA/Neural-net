#include "backpropnet.h"

BackpropNet::BackpropNet()
    :   Net()
{
    _netError = 0;
    _update = true;
    try {
        mutationFactor(0.1);
    } catch (std::runtime_error &e) {
        error_general("BackpropNet()",e);
    }

}
BackpropNet::BackpropNet(unsigned int inputs,
                         unsigned int hiddenX,
                         unsigned int hiddenY,
                         unsigned int outputs)
    :   Net(inputs,hiddenX,hiddenY,outputs)
{
    _netError = 0;
    _update = true;
    try {
        mutationFactor(0.1);
    } catch (std::runtime_error &e) {
        error_general("BackpropNet(unsigned int ["+std::to_string(inputs)+
                              "] , unsigned int ["+std::to_string(hiddenX)+
                              "] , unsigned int ["+std::to_string(hiddenY)+
                              "] , unsigned int ["+std::to_string(outputs)+"])",e);
    }
}
BackpropNet::BackpropNet(unsigned int inputs,
                         unsigned int hiddenX,
                         unsigned int hiddenY,
                         unsigned int outputs,
                         bool enableBias,
                         bool enableAverage,
                         Activation func)
    :   Net(inputs,hiddenX,hiddenY,outputs,enableBias,enableAverage,func)
{
    _netError = 0;
    _update = true;
    try {
        mutationFactor(0.1);
    } catch (std::runtime_error &e) {
        error_general("BackpropNet(unsigned int ["+std::to_string(inputs)+
                              "] , unsigned int ["+std::to_string(hiddenX)+
                              "] , unsigned int ["+std::to_string(hiddenY)+
                              "] , unsigned int ["+std::to_string(outputs)+
                              "] , bool ["+std::to_string(enableBias)+
                              "] , bool ["+std::to_string(enableAverage)+
                              "] , Activation ["+std::to_string(func)+"])",e);
    }
    if(this->activationFunction() == Activation::Binary)
    {
        error_general("BackpropNet(unsigned int ["+std::to_string(inputs)+
                      "] , unsigned int ["+std::to_string(hiddenX)+
                      "] , unsigned int ["+std::to_string(hiddenY)+
                      "] , unsigned int ["+std::to_string(outputs)+
                      "] , bool ["+std::to_string(enableBias)+
                      "] , bool ["+std::to_string(enableAverage)+
                      "] , Activation ["+std::to_string(func)+"])","you cant use this activation function: Binary for this learn algorithm");
    }
}

float               BackpropNet::netError()
{
    try {
        calc_netError();
    } catch (std::runtime_error &e) {
        error_general("netError()",e);
    }
    return _netError;
}
std::vector<float>  BackpropNet::outputError()
{
    try {
        calc_netError();
    } catch (std::runtime_error &e) {
        error_general("outputError()",e);
    }
    return _outputError;
}
float               BackpropNet::outputError(unsigned int output)
{
    if(output >= this->outputNeurons())
    {
        error_general("outputError(unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,output,(unsigned int)0,this->outputNeurons()-1));
    }
    try {
        calc_netError();
    } catch (std::runtime_error &e) {
        error_general("outputError(unsigned int ["+std::to_string(output)+"] )",e);
    }
    return _outputError[output];
}
void                BackpropNet::mutationFactor(float mutationFactor)
{
    if(mutationFactor <= 0)
    {
        error_general("mutationFactor(float ["+std::to_string(mutationFactor)+"] )","parameter 0 is out of range. MutationFactor has to be greather than 0");
    }
    _mutationFactor = mutationFactor;
}
float               BackpropNet::mutationFactor()
{
    return _mutationFactor;
}
void                BackpropNet::expected(std::vector<float> expected)
{
    if(expected.size() != this->outputNeurons())
    {
        error_general("expected(std::vector<float>)","parameter 0 has the wrong size: "+std::to_string(expected.size())+" std::vector.size() should be: "+std::to_string(this->outputNeurons()));
    }
    _update   = true;
    _expected = expected;
    try {
        calc_netError();
    } catch (std::runtime_error &e) {
        error_general("expected(std::vector<float>)",e);
    }
}

void                BackpropNet::learn()
{    
    std::vector<float>  output_error(this->outputNeurons(),0);
    std::vector<std::vector<float>  > hidden_error(this->hiddenNeuronsX(),std::vector<float>(this->hiddenNeuronsY(),0));
    for(unsigned int y=0; y<this->outputNeurons(); y++)
    {
        output_error[y] = derivative(this->outputNeuron(y)->netInput()) * _outputError[y];
    }
    if(!this->noHiddenLayer())
    {
        for(unsigned int x=this->hiddenNeuronsX(); x>0; x--)
        {
            for(unsigned int y=0; y<this->hiddenNeuronsY(); y++)
            {
                float weightError = 0;
                if(x == this->hiddenNeuronsX())
                {
                    for(unsigned int y2=0; y2<this->outputNeurons(); y2++)
                    {
                        weightError += output_error[y2] * this->outputNeuron(y2)->weight(y);
                    }
                }
                else
                {
                    for(unsigned int y2=0; y2<this->hiddenNeuronsY(); y2++)
                    {
                        weightError += hidden_error[x][y2] * this->hiddenNeuron(x,y2)->weight(y);
                    }
                }
                hidden_error[x-1][y] = derivative(this->hiddenNeuron(x-1,y)->netInput()) * weightError;
            }
        }
    }
    //----------INPUT CONNECTIONS
    if(!this->noHiddenLayer())
    {
        for(unsigned int y=0; y<this->inputNeurons() + (unsigned int)this->bias(); y++)
        {
            for(unsigned int y2=0; y2<this->hiddenNeuronsY(); y2++)
            {
                float output = 0;
                if(y == this->inputNeurons() + (unsigned int)this->bias() -1)
                {
                    output = this->biasValue();
                }
                else
                {
                    output = this->input(y);
                }
                this->hiddenNeuron(0,y2)->weight(y,this->hiddenNeuron(0,y2)->weight(y) + (hidden_error[0][y2] * _mutationFactor * output));
            }
        }
    }
    //----------HIDDEN CONNECTIONS
    if(!this->noHiddenLayer())
    {
        for(unsigned int x=1; x<this->hiddenNeuronsX(); x++)
        {
            for(unsigned int y=0; y<this->hiddenNeuronsY() + (unsigned int)this->bias(); y++)
            {
                for(unsigned int y2=0; y2<this->hiddenNeuronsY(); y2++)
                {
                    float output = 0;
                    if(y == this->hiddenNeuronsY() + (unsigned int)this->bias() -1)
                    {
                        output = this->biasValue();
                    }
                    else
                    {
                        output = this->hiddenNeuron(x-1,y)->output();
                    }
                    this->hiddenNeuron(x,y2)->weight(y,this->hiddenNeuron(x,y2)->weight(y) + (hidden_error[x][y2] * _mutationFactor * output));
                }
            }
        }
    }
    //----------OUTPUT CONNECTIONS
    if(this->noHiddenLayer())
    {
        for(unsigned int y=0; y<this->inputNeurons() + (unsigned int)this->bias(); y++)
        {
            for(unsigned int y2=0; y2<this->outputNeurons(); y2++)
            {
                float output = 0;
                if(y == this->inputNeurons() + (unsigned int)this->bias() -1)
                {
                    output = this->biasValue();
                }
                else
                {
                    output = this->input(y);
                }
                this->outputNeuron(y2)->weight(y,this->outputNeuron(y2)->weight(y) + (output_error[y2] * _mutationFactor * output));
            }
        }
    }
    else
    {
        for(unsigned int y=0; y<this->hiddenNeuronsY() + (unsigned int)this->bias(); y++)
        {
            for(unsigned int y2=0; y2<this->outputNeurons(); y2++)
            {
                float output = 0;
                if(y == this->hiddenNeuronsY() + (unsigned int)this->bias() -1)
                {
                    output = this->biasValue();
                }
                else
                {
                    output = this->hiddenNeuron(this->hiddenNeuronsX()-1,y2)->output();
                }
                this->outputNeuron(y2)->weight(y,this->outputNeuron(y2)->weight(y) + (output_error[y2] * _mutationFactor * output));
            }
        }
    }
}
void                BackpropNet::learn(std::vector<float> expected)
{
    this->expected(expected);
    this->learn();
}

void                BackpropNet::calc_netError()
{
    if(_expected.size() == 0)
    {
        error_general("calc_netError()","no expected output values defined. first set them by BackpropNet::expected(std::vector<float>)");
    }
    _update = true;
    if(_update)
    {
        _outputError = std::vector<float> (this->outputNeurons(),0);
        _netError    = 0;

        for(unsigned int y=0; y<this->outputNeurons(); y++)
        {
            _outputError[y] = _expected[y] - this->output(y);
            _netError      += _outputError[y];
        }
        _update = false;
    }
}
float               BackpropNet::derivative(float netinput)
{
    float derivative = 0;
    switch(this->activationFunction())
    {
        case Activation::Binary:
        {
            error_general("derivative(float ["+std::to_string(netinput)+"] )","you cant use this activation function: Binary for this learn algorithm");
            break;
        }
        case Activation::Gaussian:
        {
            derivative = Neuron::deriv_activation_Gaussian(netinput);
            break;
        }
        case Activation::Linear:
        {
            derivative = Neuron::deriv_activation_Linear(netinput);
            break;
        }
        case Activation::ReLu:
        {
            derivative = Neuron::deriv_activation_ReLu(netinput);
            break;
        }
        case Activation::Sigmoid:
        {
            derivative = Neuron::deriv_activation_Sigmoid(netinput);
            break;
        }
    }
    return derivative;
}


std::string BackpropNet::error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max)
{
    return " parameter "+std::to_string(paramPos)+" is out of range: "+value+"     minimum is: "+min+"     maximum is: "+max;
}
std::string BackpropNet::error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string BackpropNet::error_paramOutOfRange(unsigned int paramPos,int value,int min, int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string BackpropNet::error_paramOutOfRange(unsigned int paramPos,float value,float min, float max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
void        BackpropNet::error_general(std::string function, std::string cause)
{
    throw std::runtime_error("ERROR: BackpropNet::" + function + "     " + cause);
}
void        BackpropNet::error_general(std::string function, std::runtime_error &e)
{
    error_general(function,"",e);
}
void        BackpropNet::error_general(std::string function, std::string cause, std::runtime_error &e)
{
    std::string error = "ERROR: BackpropNet::" + function + "     " + cause;
    error += "      --> "+std::string(e.what());
    throw std::runtime_error(error);
}