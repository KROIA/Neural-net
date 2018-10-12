#include "backpropnet.h"

BackpropNet::BackpropNet()
    :   Net()
{
    _netError = 0;
}
BackpropNet::BackpropNet(unsigned int inputs,
            unsigned int hiddenX,
            unsigned int hiddenY,
            unsigned int outputs)
    :   Net(inputs,hiddenX,hiddenY,outputs)
{
    _netError = 0;
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
}


std::vector<float>  BackpropNet::netError()
{

}
float               BackpropNet::netError(unsigned int output)
{

}
void                BackpropNet::mutationFactor(float mutationFactor)
{
    if(mutationFactor <= 0)
    {
        throw std::runtime_error("BackpropNet::mutationFactor(float ["+std::to_string(mutationFactor)+"] ) ERROR: parameter 0 is out of range. MutationFactor has to be greather than 0");
    }
    _mutationFactor = mutationFactor;
}
float               BackpropNet::mutationFactor()
{
    return _mutationFactor;
}
void BackpropNet::expected(std::vector<float> expected)
{
    if(expected.size() != this->outputNeurons())
    {
        throw std::runtime_error("BackpropNet::expected(std::vector<float>) ERROR: parameter 0 has the wrong size: "+std::to_string(expected.size())+" std::vector.size() should be: "+std::to_string(this->outputNeurons()));
    }
    _expected = expected;
}
void                BackpropNet::learn(std::vector<float> expected)
{
    this->expected(expected);
    this->calc_netError();

}

void                BackpropNet::calc_netError()
{
    _outputError = std::vector<float> (this->outputNeurons(),0);
    _netError    = 0;

    for(unsigned int y=0; y<this->outputNeurons(); y++)
    {
        _outputError[y] = _expected[y] - this->output(y);
    }
}
