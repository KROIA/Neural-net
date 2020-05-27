#include "neuron.h"

double Neuron::activation_Linear(double netInput)
{
    return netInput;
}
double Neuron::deriv_activation_Linear(double netInput)
{
    return 1;
}

double Neuron::activation_ReLu(double netInput)
{
    if(netInput <= 0)
    {
        return 0;
    }
    return netInput;
}
double Neuron::deriv_activation_ReLu(double netInput)
{
    if(netInput < 0)
    {
        return 0;
    }
    return 1;
}

double Neuron::activation_Binary(double netInput)
{
    if(netInput < 0)
        return 0;
    else
        return 1;
}
/*double Neuron::deriv_activation_Binary(double netInput)
{
    //Not possible
}*/


double Neuron::activation_Gaussian(double netInput)
{
    //https://www.wolframalpha.com/input/?i=exp%28-pow%28x%2C2%29%29*2-1
    return 2*pow(EULER,-pow(netInput,2))-1;
    //return exp(-pow(netInput,2))*2-1;
    //return 2*double(exp(-(pow(netInput,2)) * 4)-1);
}
double Neuron::deriv_activation_Gaussian(double netInput)
{
    //https://www.wolframalpha.com/input/?i=derivative+of+exp%28-pow%28x%2C2%29%29*2-1
    return -4*pow(EULER,-pow(netInput,2))*netInput;
    //return netInput * (1+activation_Gaussian(netInput))/-0.43;
}


double Neuron::activation_Sigmoid(double netInput)
{
    // PI / 5 = 0.628318531
    return atan(netInput)* 0.628318531;
}
double Neuron::deriv_activation_Sigmoid(double netInput)
{
    return 0.628318531/(pow(netInput,2)+1);
    //return double(1) - (activation_Sigmoid(netInput) * activation_Sigmoid(netInput));
}
