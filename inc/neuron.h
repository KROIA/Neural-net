#ifndef NEURON_H
#define NEURON_H
//              Autor   Alex Krieg
#define NEURON_VERSION "02.02.02"
//              Datum   20.08.2019

#include <vector>
#include <math.h>
#include <random>
#include <iostream>
#include <time.h>
#include <QDebug>

#define NEURON_MIN_INPUTS 1
#define NEURON_MAX_INPUTS 500
#define __FLOATINPOINT_TOLERANCE 0.0000001


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
        ~Neuron();

        void inputs(unsigned int inputs);
        unsigned int inputs();
        void deleteInput(unsigned int input);


        void activationFunction(Activation activationFunction);
        Activation activationFunction();

        void enableAverage(bool enableAverage);
        bool enableAverage();

        void randWeight();
        void randWeight(unsigned int input);
        void weight(unsigned int pos, float weight);
        void weight(std::vector<float>  weightList);

        float weight(unsigned int pos);
        std::vector<float> weight();

        void input(unsigned int pos, float input);
        void input(std::vector<float> inputList);
        void disconnect(unsigned int input);
        void connectInput(float *ptr);
        void connectInput(unsigned int input, float *ptr);
        /*  Connect a output(reference) from an other Neuron or a dummy float* for the first Input.
         *  This lets the Neuron communicate between without copys of the output-values
         *  sins V02.02.01
         */

        float input(unsigned int pos);
        std::vector<float> input();

        float netInput();
        float output();
        float *ptr_output();

        void run();

        //first derivative
        static float deriv_activation_Linear(float netInput);
        static float deriv_activation_ReLu(float netInput);
      //  static float deriv_activation_Binary(float netInput);  //Not possible
        static float deriv_activation_Gaussian(float netInput);
        static float deriv_activation_Sigmoid(float netInput);

    private:
        void init(unsigned int inputs, Activation activationFunction, bool enableAverage);
        static float activation_Linear(float netInput);
        static float activation_ReLu(float netInput);
        static float activation_Binary(float netInput);
        static float activation_Gaussian(float netInput);
        static float activation_Sigmoid(float netInput);



        void calc_netInput();
        void calc_output();

        //----------ERROR
        std::string error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max);
        std::string error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max);
        std::string error_paramOutOfRange(unsigned int paramPos,int value,int min, int max);
        std::string error_paramOutOfRange(unsigned int paramPos,float value,float min, float max);

        void        error_general(std::string function, std::string cause);
        void        error_general(std::string function, std::runtime_error &e);
        void        error_general(std::string function, std::string cause, std::runtime_error &e);
        //---------------


        std::vector<float>          _weightList;
        std::vector<float*>         _ptr_inputList;
        std::vector<bool>           _inputConnectionList;
        //std::vector<float>          _inputList;
        float                       _netInput;
        float                       _output;

        unsigned int                _inputs;
        bool                        _enableAverage;
        std::default_random_engine  _randEngine;
        Activation                  _activationFunction;
        bool                        _update;



};
#endif // NEURON_H
