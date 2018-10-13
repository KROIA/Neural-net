#include "backpropnet.h"

BackpropNet::BackpropNet()
    :   Net()
{
    _netError = 0;
    _update = true;
    mutationFactor(0.1);
}
BackpropNet::BackpropNet(unsigned int inputs,
            unsigned int hiddenX,
            unsigned int hiddenY,
            unsigned int outputs)
    :   Net(inputs,hiddenX,hiddenY,outputs)
{
    _netError = 0;
    _update = true;
    mutationFactor(0.1);
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
    mutationFactor(0.1);
}

float               BackpropNet::netError()
{
    calc_netError();
    return _netError;
}
std::vector<float>  BackpropNet::outputError()
{
    calc_netError();
    return _outputError;
}
float               BackpropNet::outputError(unsigned int output)
{
    if(output >= this->outputNeurons())
    {
        throw std::runtime_error("BackpropNet::outputError(unsigned int ["+std::to_string(output)+"] ) ERROR: parameter 0 is out of range: "+std::to_string(output)+" maximum is: "+std::to_string(this->outputNeurons()-1)+"\n");
    }
    calc_netError();
    return _outputError[output];
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
void                BackpropNet::expected(std::vector<float> expected)
{
    if(expected.size() != this->outputNeurons())
    {
        throw std::runtime_error("BackpropNet::expected(std::vector<float>) ERROR: parameter 0 has the wrong size: "+std::to_string(expected.size())+" std::vector.size() should be: "+std::to_string(this->outputNeurons()));
    }
    _update   = true;
    _expected = expected;
    this->calc_netError();
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
        for(int x=(signed int)this->hiddenNeuronsX()-1; x>=0; x--)
        {
            #ifdef DEBUG
            printf("x: %i\n",(unsigned int)x);
#endif
            for(unsigned int y=0; y<this->hiddenNeuronsY(); y++)
            {
                float weightError = 0;
                if(x == this->hiddenNeuronsX()-1)
                {
                 //   printf("aaaaaaaaa\n");
                    for(unsigned int y2=0; y2<this->outputNeurons(); y2++)
                    {
                        weightError += output_error[y2] * this->outputNeuron(y2)->weight(y);
                    }
                }
                else
                {
                //    printf("bbbbbbbbbb\n");
                    for(unsigned int y2=0; y2<this->hiddenNeuronsY(); y2++)
                    {
                        weightError += hidden_error[x+1][y2] * this->hiddenNeuron(x+1,y2)->weight(y);
                    }
                }
                hidden_error[x][y] = derivative(this->hiddenNeuron(x,y)->netInput()) * weightError;
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
        throw std::runtime_error("backpropNet::calc_netError() ERROR: no expected output values defined. first set them by BackpropNet::expected(std::vector<float>)\n");
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
            throw std::runtime_error("BackpropNet::derivative(float ["+std::to_string(netinput)+"] ) ERROR: you cant use this activation function: Binary for this learn algorithm\n");
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
