#include "neuron.h"

float Neuron::activation_Linear(float netInput)
{
    return netInput;
}
float Neuron::activation_ReLu(float netInput)
{
    if(netInput < 0)
    {
        return 0;
    }
    return netInput;
}
float Neuron::activation_Binary(float netInput)
{
    if(netInput < 0)
        return 0;
    else
        return 1;
}
float Neuron::activation_Gaussian(float netInput)
{
    return (float)exp(-(pow((double)netInput,2)));
}
float Neuron::activation_Sigmoid(float netInput)
{
    return ((1.0/(1.0+(float)exp(-(double)netInput)))-0.5)*2;
}
