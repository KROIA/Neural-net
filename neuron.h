#ifndef NEURON_H
#define NEURON_H
//              Autor   Alex Krieg
#define NEURON_VERSION "02.00.01"
//              Datum   11.10.2018

#include <vector>
#include <math.h>
#include <random>
#include <iostream>
#include <time.h>


enum Activation{        // https://en.wikipedia.org/wiki/Activation_function
    Linear   = 0,       // https://en.wikipedia.org/wiki/Identity_function
    ReLu     = 1,       // https://en.wikipedia.org/wiki/Rectifier_(neural_networks)
    Binary   = 2,       // https://en.wikipedia.org/wiki/Heaviside_step_function
    Gaussian = 3,       // https://en.wikipedia.org/wiki/Gaussian_function
    Sigmoid  = 4,       // https://en.wikipedia.org/wiki/Activation_function
};
#define neuron_activationFunctionAmount 5


class Neuron
{
    public:
        Neuron();
        Neuron(unsigned int inputs);
        Neuron(unsigned int inputs, Activation activationFunction);
        Neuron(unsigned int inputs, Activation activationFunction, bool enableAverage);

        void inputs(unsigned int inputs);
        unsigned int inputs();

        void activationFunction(Activation activationFunction);
        Activation activationFunction();

        void enableAverage(bool enableAverage);
        bool enableAverage();

        void randWeight();
        void weight(unsigned int pos, float weight);
        void weight(std::vector<float>  weightList);

        float weight(unsigned int pos);
        std::vector<float> weight();

        void input(unsigned int pos, float input);
        void input(std::vector<float> inputList);

        float input(unsigned int pos);
        std::vector<float> input();

        float netInput();
        float output();

        void run();



    private:
        static float activation_Linear(float netInput);
        static float activation_ReLu(float netInput);
        static float activation_Binary(float netInput);
        static float activation_Gaussian(float netInput);
        static float activation_Sigmoid(float netInput);

        //first derivative
        static float deriv_activation_Linear(float netInput);
        static float deriv_activation_ReLu(float netInput);
      //  static float deriv_activation_Binary(float netInput);  //Not possible
        static float deriv_activation_Gaussian(float netInput);
        static float deriv_activation_Sigmoid(float netInput);

        void calc_netInput();
        void calc_output();


        std::vector<float>          _weightList;
        std::vector<float>          _inputList;
        float                       _netInput;
        float                       _output;

        unsigned int                _inputs;
        bool                        _enableAverage;
        std::default_random_engine 	_randEngine;
        Activation                  _activationFunction;
        bool                        _update;

        struct tm *ti;

};
#endif // NEURON_H
