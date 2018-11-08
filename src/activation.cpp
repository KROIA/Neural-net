#include "neuron.h"

float Neuron::activation_Linear(float netInput)
{
    return netInput;
}
float Neuron::deriv_activation_Linear(float netInput)
{
    return 1;
}

float Neuron::activation_ReLu(float netInput)
{
    if(netInput <= 0)
    {
        return 0;
    }
    return netInput;
}
float Neuron::deriv_activation_ReLu(float netInput)
{
    if(netInput < 0)
    {
        return 0;
    }
    return 1;
}

float Neuron::activation_Binary(float netInput)
{
    if(netInput < 0)
        return 0;
    else
        return 1;
}
/*float Neuron::deriv_activation_Binary(float netInput)
{
    //Not possible
}*/


float Neuron::activation_Gaussian(float netInput)
{
    return 2*(float)exp(-(pow((double)netInput,2)) * 4)-1;
}
float Neuron::deriv_activation_Gaussian(float netInput)
{
    return netInput * (1+activation_Gaussian(netInput))/-0.43;
}


float Neuron::activation_Sigmoid(float netInput)
{
    return atan((double)netInput * 5)* 3.141592653589/5;
}
float Neuron::deriv_activation_Sigmoid(float netInput)
{
    return (float)1 - (activation_Sigmoid(netInput) * activation_Sigmoid(netInput));
}
