#ifndef NEURON_H
#define NEURON_H
//              Autor   Alex Krieg
#define NEURON_VERSION "02.04.01"
//              Datum   16.08.2019

#include <vector>
#include <math.h>
#include <random>
#include <iostream>
#include <time.h>
#include <QDebug>

#define NEURON_MIN_INPUTS 1
#define NEURON_MAX_INPUTS 500
#define __FLOATINPOINT_TOLERANCE 0.0000001
#define NEURON_ID_INVALID 65535
#define NEURON_ID_NOTDEFINED 65534


enum Activation{        // https://en.wikipedia.org/wiki/Activation_function
    Linear   = 0,       // https://en.wikipedia.org/wiki/Identity_function
    ReLu     = 1,       // https://en.wikipedia.org/wiki/Rectifier_(neural_networks)
    Binary   = 2,       // https://en.wikipedia.org/wiki/Heaviside_step_function
    Gaussian = 3,       // https://en.wikipedia.org/wiki/Gaussian_function
    Sigmoid  = 4,       // https://en.wikipedia.org/wiki/Activation_function
};
#define neuron_activationFunctionAmount 5

enum NeuronType{
    none   = 0,         // Initial value
    input  = 1,         // It's a Neuron in the inputlayer
    hidden = 2,         // It's a Neuron in the hiddenlayer
    output = 3,         // It's a Neuron in the outputlayer
    costum = 4,         // It's a costum Neuron and can be everywere except the inputlayer
    bias   = 5,
};

struct NeuronID{
    unsigned int ID;
    NeuronType  TYPE;
};

struct Connection
{
    unsigned int netID;
    NeuronID source_ID;
    NeuronID destination_ID;
    float weight;
};

class Neuron
{
    public:
        Neuron();
        Neuron(unsigned int inputs);
        Neuron(unsigned int inputs, Activation activationFunction);
        Neuron(unsigned int inputs, Activation activationFunction, bool enableAverage);
        ~Neuron();

        void ID(NeuronID ID);
        void ID(unsigned int ID);
        void TYPE(NeuronType TYPE);
        NeuronID ID();

        void inputs(unsigned int inputs);
        unsigned int inputs();
        bool deleteInput(unsigned int input);


        void activationFunction(Activation activationFunction);
        Activation activationFunction();

        void enableAverage(bool enableAverage);
        bool enableAverage();

        static float calcRandWeight(std::default_random_engine &randEngine);
        void randWeight();
        void randWeight(unsigned int input);
        void weight(unsigned int pos, float weight);
        void weight(std::vector<float>  weightList);
        void weight(NeuronID ID,float weight);

        float weight(unsigned int pos);
        std::vector<float> weight();

     //   void input(unsigned int pos, float input);
     //   void input(std::vector<float> inputList);
        float input(unsigned int input);
        std::vector<float> input();

        //void connectInput(float *ptr_float);
        bool connectInput(NeuronID ID, float *ptr_float);
        //void connectInput(NeuronType type, float *ptr_float);
        bool connectInput(unsigned int input, NeuronID ID, float *ptr_float);
        bool connectInput(Neuron *ptr_neuron);
        bool connectInput(unsigned int input, Neuron *ptr_neuron);
        /*  Connect a Neuron's output to this Neuron's input.
         *  This lets the Neuron communicate between without copys of the output-values
         *  sins V02.02.01
         */
        bool disconnect(unsigned int input);


        float netInput();
        float output();
        float *ptr_output();
        NeuronID inputID(unsigned int input);
        std::vector<NeuronID> inputID();
        float *ptr_weight(unsigned int input);
        float *ptr_weight(NeuronID  connectionID);

        void run();

        //first derivative
        static float deriv_activation_Linear(float netInput);
        static float deriv_activation_ReLu(float netInput);
      //  static float deriv_activation_Binary(float netInput);  //Not possible
        static float deriv_activation_Gaussian(float netInput);
        static float deriv_activation_Sigmoid(float netInput);

        static const std::string typeString(NeuronType TYPE);
        static const std::string activationString(Activation activationFunction);

        void needsUpdate();
        bool isUpdated();

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

        NeuronID                    _ID;
        std::vector<NeuronID>       _inputConnectionID_list;

};
#endif // NEURON_H
