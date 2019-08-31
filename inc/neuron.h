#ifndef NEURON_H
#define NEURON_H
//              Autor   Alex Krieg
#define NEURON_VERSION "02.04.02"
//              Datum   31.08.2019

#include <vector>
#include <math.h>
#include <random>
#include <iostream>
#include <time.h>
#include <QDebug>

#define NEURON_MIN_INPUTS 0
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
    bias   = 5,         // It's a bias -> a constant value.
};

enum ConnectionDirection{
    forward  = 1,
    backward = 0,
};

struct NeuronID{
    unsigned int ID;
    /* A value which should be only once given to the Neuron.
       Every Neuron must have it's own ID.
       This ID will be used for accesing the Neuron in a Vector so it should begin at 0 and counts up for every new Neuron in the Net
     */
    NeuronType  TYPE;
    /* The TYPE specify which Neuron it is.
       This is necessary because you may wanna connect a custom float variable to a Neuron,
       the variable does not have an Neuron ID so it will be stimulated by an
        NeuronID.ID     = inputnumber
        NeuronID.TYPE   = NeuronType::input

       That means, the Net will have two NeuronID's with the ID==0 but
       the TYPE will always be different.
    */
};

struct Connection
{
    unsigned int netID;
    NeuronID source_ID;
    NeuronID destination_ID;
    float weight;
    bool direction;
};


class Neuron
{
    public:
        Neuron();
        /*Constructor of Neuron
            Parameter:
            none

          setup as default:
           | inputs: NEURON_MIN_INPUTS
           | activation: Activation::Sigmoid
           | enableAverage: false
        */
        Neuron(unsigned int inputs);
        /* Constructor of Neuron
          Parameter:
           | inputs: input connection, have to be connected later

          setup as default:
           | activation: Activation::Sigmoid
           | enableAverage: false
        */
        Neuron(unsigned int inputs, Activation activationFunction);
        /* Constructor of Neuron
          Parameter:
           | inputs: input connection, have to be connected later
           | activation: set your chosen Activationfunction. see enum Activation above.

          setup as default:
           | enableAverage: false
        */
        Neuron(unsigned int inputs, Activation activationFunction, bool enableAverage);
        /* Constructor of Neuron
          Parameter:
           | inputs: input connection, have to be connected later
           | activation: set your chosen Activationfunction. see enum Activation above.
           | enableAverage: if true  ->  The netinput will be averaged out before it is given to the Activation Function.
                                         This may make sense if the Neuron has lots of inputs eg. inputs > 50.
                               false ->  No averaging will be done.
        */
        ~Neuron();

        void ID(NeuronID ID);
        /* Set ID of the Neuron to identify it in your Net
          Parameter:
           | ID: See struct NeuronID above
        */
        void ID(unsigned int ID);
        /* Set the ID number of the NeuronID
        */
        void TYPE(NeuronType TYPE);
        /* Set the TYPE of the NeuronID
        */
        NeuronID ID();
        /* Returns the NeuronID
        */

        void inputs(unsigned int inputs);
        unsigned int inputs();
        bool deleteInput(unsigned int input);
        /* returns
           [1] -> deletion of the connection succeed
           [0] -> error, couldn't delete connection
        */

        void activationFunction(Activation activationFunction);
        Activation activationFunction();

        void enableAverage(bool enableAverage);
        bool enableAverage();

        static float calcRandWeight(std::default_random_engine &randEngine);
        void randWeight();
        void randWeight(unsigned int input);
        void weight(unsigned int pos, float weight);
        void weight(std::vector<float>  weightList);
        void weight(NeuronID ID,float weight);                                  // Sets the weight of the connection between Neuron [ID] and Neuron [this]


        float weight(unsigned int pos);
        std::vector<float> weight();

        void input(unsigned int pos, float input);
        void input(std::vector<float> inputList);
        float input(unsigned int input);
        std::vector<float> input();

        //void connectInput(float *ptr_float);
        bool connectInput(NeuronID ID, float *ptr_float,bool forward = true);
        //void connectInput(NeuronType type, float *ptr_float);
        bool connectInput(unsigned int input, NeuronID ID, float *ptr_float,bool forward = true);
        bool connectInput(Neuron *ptr_neuron,bool forward = true);
        bool connectInput(unsigned int input, Neuron *ptr_neuron,bool forward = true);
        /*  Connect a Neuron's output to this Neuron's input.
         *  This lets the Neuron communicate between without copys of the output-values
         *  sins V02.02.01
         */
        bool disconnect(unsigned int input);
        bool inputConnectionDirection(NeuronID inputID);
        std::vector<bool>   inputConnectionDirection();


        float netInput();
        float output();
        float *ptr_output();        //use for forward connections
        float *ptr_loopBackOutput();//use for backward connections

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

        static const std::string neuronIDString(NeuronID ID);
        static const std::string typeString(NeuronType TYPE);
        static const std::string activationString(Activation activationFunction);
        static const std::string connectionString(Connection connection);
        static const std::string directionSring(bool dir);

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
        float                       _netInput;
        float                       _output;
        float                       _delayedOutput; //for loopbacksignal

        unsigned int                _inputs;
        bool                        _enableAverage;
        std::default_random_engine  _randEngine;
        Activation                  _activationFunction;
        bool                        _update;

        NeuronID                    _ID;
        std::vector<NeuronID>       _inputConnectionID_list;
        std::vector<bool>           _inputConnectionDirection_List;

};
#endif // NEURON_H
