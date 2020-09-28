#ifndef NEURON_H
#define NEURON_H
//              Autor   Alex Krieg
#define NEURON_VERSION "02.06.01"
//              Datum   25.09.2020

/*
 Some functions may throw errors.
 All thrwon errors are std::runtime_error
*/

#define QT_APP

//  Debuging
//#define _DEBUG_NEURON_RUN
//#define _DEBUG_NEURON_ALL
//#define _DEBUG_NEURON_TIMING
#define _DEBUG_NEURON_CONNECT
#define _DEBUG_NEURON_DELETE_INPUT

#ifdef _DEBUG_NEURON_ALL
#ifndef _DEBUG_NEURON_RUN
#define _DEBUG_NEURON_RUN
#endif
#ifndef _DEBUG_NEURON_TIMING
#define _DEBUG_NEURON_TIMING
#endif
#ifndef _DEBUG_NEURON_CONNECT
#define _DEBUG_NEURON_CONNECT
#endif
#ifndef _DEBUG_NEURON_DELETE_INPUT
#define _DEBUG_NEURON_DELETE_INPUT
#endif
#endif

#include <vector>
#include <math.h>
#include <random>
#include <iostream>
#include <time.h>

#ifdef QT_APP
    #include <QDebug>
    #include <QObject>
#endif
#include <error.h>

#if defined(_DEBUG_NEURON_TIMING)
#include <ctime>
#include <ratio>
#include <chrono>
#endif

#ifdef QDEBUG_H
#ifndef CONSOLE
#define CONSOLE qDebug()
#endif
#else
#include <iostream>
#include <stdio.h>
#ifndef CONSOLE
#define CONSOLE std::cout
#endif
#endif


#define NEURON_MIN_INPUTS 0
#define NEURON_MAX_INPUTS 10000
#define __FLOATINGPOINT_TOLERANCE 0.0000001
#define NEURON_ID_INVALID 65535

const double EULER = 2.71828182845904523536;

#define neuron_activationFunctionAmount 5
enum Activation{        // https://en.wikipedia.org/wiki/Activation_function
    Linear   = 0,       // https://en.wikipedia.org/wiki/Identity_function
    ReLu     = 1,       // https://en.wikipedia.org/wiki/Rectifier_(neural_networks)
    Binary   = 2,       // https://en.wikipedia.org/wiki/Heaviside_step_function
    Gaussian = 3,       // https://en.wikipedia.org/wiki/Gaussian_function
    Sigmoid  = 4,       // https://en.wikipedia.org/wiki/Activation_function
};


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
       This is necessary because you may wanna connect a custom double variable to a Neuron,
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
    double weight;
    ConnectionDirection direction;
};

#ifdef QT_APP
    class Neuron : public QObject
#else
   class Neuron
#endif
{
        #ifdef QT_APP
            Q_OBJECT
        #endif
    public:
        #ifdef QT_APP
            Neuron(QObject *parent = nullptr);
        #else
           Neuron();
        #endif
        /*Constructor of Neuron
            Parameter:
            none

          setup as default:
           | inputs: NEURON_MIN_INPUTS
           | activation: Activation::Sigmoid
           | enableAverage: false

          ERROR:
           | same as Neuron::init(init(unsigned int inputs, Activation activationFunction, bool enableAverage);
         */

        #ifdef QT_APP
           Neuron(unsigned int inputs,
                  QObject *parent = nullptr);
        #else
           Neuron(unsigned int inputs);
        #endif
        /* Constructor of Neuron
          Parameter:
           | inputs: input connection, have to be connected later

          setup as default:
           | activation: Activation::Sigmoid
           | enableAverage: false

          ERROR:
           | same as Neuron::init(init(unsigned int inputs, Activation activationFunction, bool enableAverage);

         */

        #ifdef QT_APP
           Neuron(unsigned int inputs,
                  Activation activationFunction,
                  QObject *parent = nullptr);
        #else
           Neuron(unsigned int inputs,
                  Activation activationFunction);
        #endif
        /* Constructor of Neuron
          Parameter:
           | inputs: input connection, have to be connected later
           | activation: set your chosen Activationfunction. see enum Activation above.

          setup as default:
           | enableAverage: false

          ERROR:
           | same as Neuron::init(init(unsigned int inputs, Activation activationFunction, bool enableAverage);

         */

        #ifdef QT_APP
           Neuron(unsigned int inputs,
                  Activation activationFunction,
                  bool enableAverage,
                  QObject *parent = nullptr);
        #else
           Neuron(unsigned int inputs,
                  Activation activationFunction,
                  bool enableAverage);
        #endif
        /* Constructor of Neuron
          Parameter:
           | inputs: input connection, have to be connected later
           | activation: set your chosen Activationfunction. see enum Activation above.
           | enableAverage: if true  ->  The netinput will be averaged out before it is given to the Activation Function.
                                         This may make sense if the Neuron has lots of inputs eg. inputs > 50.
                               false ->  No averaging will be done.

          ERROR:
           | same as Neuron::init(init(unsigned int inputs, Activation activationFunction, bool enableAverage);

         */
        ~Neuron();

        void set_ID(NeuronID ID);
        /* Set ID of the Neuron to identify it in your Net
          Parameter:
           | ID: See struct NeuronID above

          ERROR:
           | none
         */
        void set_ID(unsigned int ID);
        /* Set the ID number of the NeuronID
          Parameter:
           | ID: ID number of the NeuronID struct.

           ERROR:
            | none
         */
        void set_TYPE(NeuronType TYPE);
        /* Set the TYPE of the NeuronID
          Parameter:
           | TYPE: TPYE of the NeuronID struct.

          ERROR:
           | none
         */
        NeuronID get_ID();
        /* Returns the NeuronID

          ERROR:
           | none
         */

        void set_inputs(unsigned int inputs);
        /* Set the amount of inputs. They have to be connected later.
          Parameter:
           | inputs: amount of inputs.

           ERROR:
            | from deleteInput(unsigned int input)
            | std::vector errors
         */
        unsigned int get_inputs();
        /* Returns the amount of inputs. Also those who aren't connected.

          ERROR:
           | none
         */
        int deleteInput(unsigned int input);
        /* Delete the Input at index 'input'.

          returns:
          [0] -> error, couldn't delete connection
          [1] -> deletion of the connection succeed
          [2] -> connection already doesn't exist

          ERROR:
           | std::vector errors
         */

        void set_activationFunction(Activation activationFunction);
        /* Sets the Activation function of the Neuron.
          Parameter:
           | ActivationFunction: See enum Activation above

          ERROR:
           | invalid activationfunction
         */
        Activation get_activationFunction();
        /* Returns the current activationfunction.

          ERROR:
           | none
         */

        void set_enableAverage(bool enableAverage);
        /* Determines whether the netinput should be averaged before being passed to the activation function.

          ERROR:
           | none
         */
        bool get_enableAverage();
        /* Returns if averaging is enabled.

          ERROR:
           | none
         */

        static double get_calcRandWeight(std::default_random_engine &randEngine);
        /* Retruns a random weight in the range -1 to 1 resolution: 1/1000

          ERROR:
           | none
         */
        void set_randWeight();
        /* Calculates a random weight for all inputconnections.

          ERROR:
           | none
         */
        void set_randWeight(unsigned int input);
        /* Sets a random weight for the input 'input'.

          ERROR:
           | from weight(unsigned int pos, double weight).
         */
        void set_weight(unsigned int pos, double weight);
        /* Sets the weight 'weight' for the input 'input'.

          ERROR:
           | Out of range error, if the 'input'-indx is grater than the amount of inputs-1.
         */
        void set_weight(std::vector<double>  weightList);
        /* Sets all weights index 0 from 'weightList' is assigned to input 0 and so on.

          ERROR:
           | Wrong size of 'weightList' when the vector has not the same amount of elements as inputs available.
         */
        void set_weight(NeuronID ID,double weight);
        /* Sets the weight 'weight' for the input on which the neuron in the ID is connected to.

          ERROR:
           | No input is coming from a Neuron with the ID 'ID'.
         */

        double get_weight(unsigned int input);
        /* Returns the weight on input 'input'.

          ERROR:
           | Out of range error, if the 'input'-indx is grater than the amount of inputs-1.
         */
        std::vector<double> get_weight();
        /* Returns a vector of all weights.
         */

        void set_input(unsigned int input, double value);
        /* Set the inputvalue 'value' of the input 'input'
           Caution: this is only possible when a variable is assigned to the input.
           This function overwrites the value of the assigned variable.

          ERROR:
           | 'input' out of range. If the 'input' is grater than the amount of inputs-1 in the neuron.
           | nullptr exception. If no variable is assigned to the chosen input.
        */
        void set_input(std::vector<double> inputList);
        /* Sets the inputvalue of all inputs. Same as Neuron::input(unsigned int input, double value)
           but for all inputs.

          ERROR:
           | same as Neuron::input(unsigned int input, double value)
           | 'inputList' hasn't the same amount of elements as inputs available.
        */
        double get_input(unsigned int input);
        /* Returns the inputvalue at the input 'input'.

          ERROR:
           | 'input' out of range. If the 'input' is grater than the amount of inputs-1 in the neuron.
           | nullptr exception. If no variable is assigned to the chosen input.
         */
        std::vector<double> get_input();
        /* Returns a list of all inputvalues.
           Returns only the values of the connected inputs.

          ERROR:
           | none
        */

        bool connectInput(NeuronID ID, double *ptr_sourceNeuronOutput,ConnectionDirection direction = ConnectionDirection::forward);
        /* Connects an input of the neuron 'this' with the output of the neuron with the ID 'ID'.
           The parameter 'ptr_sourceNeuronOutput' is a pointer to the the output of the neuron with the ID 'ID'.
           The ptr_sourceNeuronOutput is provided through the function Neuron::ptr_output() or Neuron::ptr_loopBackOutput()
           How to chose between the ptr_output() and ptr_loopBackOutput()?
            When you connect a Neuron you have to specify in which direction the connection is going to be.
            eg.
            +------------------------------------------------------------------------------------------------+
            |   Neuron neuron_1;
            |   Neuron neuron_2;
            |
            |   neuron_2.connect(neuron_1.ID(),neuron_1.ptr_output(),ConnectionDirection::forward);
            +------------------------------------------------------------------------------------------------+
            |
            +--> This code connects the neuron 'neuron_1' to the neuron 'neuron_2'.
                 The first and only input connection of neuron 'neuron_2' is comming from neuron 'neuron_1'.

                  ******            ******
                 *      *          *      *
                *    1   *->---->-*    2   *
                 *      *          *      *
                  ******            ******

                  Flow of computation: --->
            +------------------------------------------------------------------------------------------------+
            |   Neuron neuron_1;
            |   Neuron neuron_2;
            |
            |   neuron_2.connect(neuron_1.ID(),neuron_1.ptr_loopBackOutput(),ConnectionDirection::backward);
            +------------------------------------------------------------------------------------------------+
            |
            +--> This code connects the neuron 'neuron_1' to the neuron 'neuron_2'.
                 The first and only input connection of neuron 'neuron_2' is comming from neuron 'neuron_1'.

                  ******            ******
                 *      *          *      *
            +->-*    2   *        *    1   *->-+
            |    *      *          *      *    |
            |     ******            ******     |
            |                                  |
            +------------------<---------------+

                  Flow of computation: --->
            It looks the same but the importent thing is, the neuron 1 is now in the second layer.
            In the first calculation of the net, the neuron 2 wil not become any input signals.
            Neuron 1 wil be calculated first. In the second sycle, the neuron 2 will reseave the input of neuron 1 from the last sycle.

            Sounds complicated?

            +------------------------------------------------------------------------------------------------+
            |   Neuron neuron_1;
            |   Neuron neuron_2;
            |
            |   neuron_2.connect(neuron_1.ID(),neuron_1.ptr_loopBackOutput(),ConnectionDirection::backward);
            |   neuron_1.connect(neuron_2.ID(),neuron_2.ptr_output(),ConnectionDirection::forward);
            +------------------------------------------------------------------------------------------------+
            |
            +--> This code connects the neuron 'neuron_1' to the neuron 'neuron_2'.
                 The first and only input connection of neuron 'neuron_2' is comming from neuron 'neuron_1'.

                  ******            ******
                 *      *          *      *
            +->-*    2   *->---->-*    1   *->-+
            |    *      *          *      *    |
            |     ******            ******     |
            |                                  |
            +------------------<---------------+

                  Flow of computation: --->
            In this example we have a closed loop but the simulation isn't continuous, it's discreet.
            That means, the simulation calculates each net once per timeinterval.
            In this closed loop we have some problems
                1.  At the beginning of the simulation we
                    down't know which signal is comming into the neuron 2.
                2.  Which neuron shuld be calculated first? Both are interfering each other.

            The solution:
                    The flow of calculation is from left to right.
                    Backlooped inputs are at value '0' at the start of the simulation.
                    Each neuron which shuld be calculated is checked firs if all neurons at their inputs are calculated.
                    That means, a neuron only can be calculated, if all neurons connected to it are already calculated or
                    the neurons connected to it are in a higher level (backlooped connection).

            returns:
             [true]     -> Connection successfully created.
             [false]    -> No connection was made.
                           Enable the Neuron debugmode to know why no connection has been made.
                           #define _DEBUG_NEURON above

            ERROR:
             | from Neuron::inputs(unsigned int inputs)
             | from Neuron::connectInput(unsigned int input, NeuronID ID, double *ptr_sourceNeuronOutput,ConnectionDirection direction)
         */
        bool connectInput(unsigned int input, NeuronID ID, double *ptr_sourceNeuronOutput,ConnectionDirection direction = ConnectionDirection::forward);
        /* For infos see above.
           Different input parameter:
           'input'                      -> inputArrayIndex of the inputVector. (Not the ID.ID value)
           'ID'                         -> NeuronID from the sourceNeuron. (same as above)
           'ptr_sourceNeuronOutput'     -> output pointer from the sourceNeuron. (same as above)
           'direction'                  -> forward if sourceNeuron is in a lower layer.

           returns:
            [true]     -> Connection successfully created.
            [false]    -> No connection was made.
                          Enable the Neuron debugmode to know why no connection has been made.
                          #define _DEBUG_NEURON above

           ERROR:
            | 'input' is greater or equal like amount of inputs available on the neuron.
            | 'ptr_sourceNeuronOutput' is a nullptr
         */
        bool connectInput(Neuron *ptr_neuron,ConnectionDirection direction = ConnectionDirection::forward);
        /* For infos see above.
           Different input parameter:
           'ptr_neuron'                 -> the pointer from the sourceNeuron.
           'direction'                  -> forward if sourceNeuron is in a lower layer.

           returns:
            [true]     -> Connection successfully created.
            [false]    -> No connection was made.
                          Enable the Neuron debugmode to know why no connection has been made.
                          #define _DEBUG_NEURON above

           ERROR:
            | from Neuron::connectInput(unsigned int input,Neuron *ptr_neuron,ConnectionDirection direction)
            | from Neuron::inputs(unsigned int inputs)
         */
        bool connectInput(unsigned int input, Neuron *ptr_neuron,ConnectionDirection direction = ConnectionDirection::forward);
        /* For infos see above.
           Different input parameter:
           'input'                      -> inputArrayIndex of the inputVector. (Not the ID.ID value)
           'ptr_neuron'                 -> the pointer from the sourceNeuron.
           'direction'                  -> forward if sourceNeuron is in a lower layer.

           returns:
            [true]     -> Connection successfully created.
            [false]    -> No connection was made.
                          Enable the Neuron debugmode to know why no connection has been made.
                          #define _DEBUG_NEURON above

           ERROR:
            | 'input' is greater or equal like amount of inputs available on the neuron.
            | 'ptr_neuron' is a nullptr.
         */
        bool disconnect(unsigned int input);
        /* Disconnects the inputconnection at arrayIndex 'input'
           This won't delete the input, it will only be disconnected and
           the weight will stay the same even if it is connected to an other neuron.

           returns:
            [true]      -> Connection successfully removed.
            [false]     -> Connection already doesen't exist.
                           Enable the Neuron debugmode to know why no connection has been made.
                           #define _DEBUG_NEURON above

           ERROR:
            | none
         */
        bool disconnect(NeuronID sourceNeuron);
        /* Disconnects the inputconnection comming from the neuron with the NeuronID 'sourceNeuron'.
           This won't delete the input, it will only be disconnected and
           the weight will stay the same even if it is connected to an other neuron.

           returns:
            [true]      -> Connection successfully removed.
            [false]     -> Connection already doesen't exist.
                           Enable the Neuron debugmode to know why no connection has been made.
                           #define _DEBUG_NEURON above

           ERROR:
            | from Neuron::disconnect(unsigned int input)

         */
        ConnectionDirection get_inputConnectionDirection(NeuronID inputID);
        /* Returns the direction from the connection from neuron with the ID 'inputID'
           to 'this' neuron.

           'inputID'    -> The neuron ID which is connected to 'this' neuron.

           returns:
            [ConnectionDirection::forward]  -> It is a forward connection.
            [ConnectionDirection::backward] -> It is a backward connection.

           ERROR:
            | No connection is comming from the neuron with the ID 'inputID'
         */
        std::vector<ConnectionDirection>   get_inputConnectionDirection();
        /* Returns a vector with all directions of all connections.

           ERROR:
            | none
         */


        double get_netInput();
        /* Returns the calculated netInput of the neuron.
           First call Neuron::run() to get a updated value here.

           ERROR:
            | none
         */
        double get_output();
        /* Returns the calculated output of the neuron.
           First call Neuron::run() to get a updated value here.

           ERROR:
            | none
         */
        double *get_ptr_output();        //use for forward connections
        /* Returns the pointer to the output.
           Used to connect neurons with the Neuron::connectInput(NeuronID ID, double *ptr_sourceNeuronOutput,bool forward = ConnectionDirection::forward)
           function. Only use this functions for forward connections.

           ERROR:
            | none
         */
        double *get_ptr_loopBackOutput();//use for backward connections
        /* Returns the pointer to the output.
           Used to connect neurons with the Neuron::connectInput(NeuronID ID, double *ptr_sourceNeuronOutput,bool forward = ConnectionDirection::forward)
           function. Only use this functions for backward connections.

           ERROR:
            | none
         */

        NeuronID get_inputID(unsigned int input);
        /* Returns the neuronID which is connected to the input at pos 'input'.

           ERROR:
            | parameter 'input' >= amount of inputs available.
         */
        std::vector<NeuronID> get_inputIDList();
        /* Returns a vector with all inputID's of the neurons which are connected to 'this' neuron.

          ERROR:
           | none
         */
        double *get_ptr_weight(unsigned int input);
        /* Returns the pointer to the weight variable of the input 'input'.

          ERROR:
           | parameter 'input' >= amount of inputs available.
         */
        double *get_ptr_weight(NeuronID  connectionID);
        /* Returns the pointer to the weight variable of the input
           on which the neuron with the neuronID 'connectionID' is connected to.

          ERROR:
           | from ptr_weight(unsigned int input)
           | No connection is comming from the neuron with the ID 'connectionID'
         */

        void run();
        /* Calculates the next output.
           First set all inputs and than call this function to get a updated value.
           Then call the function Neuron::needsUpdate()

          ERROR:
           | Unknown activationfunction.
         */

        static double activation_Linear(double netInput);
        static double activation_ReLu(double netInput);
        static double activation_Binary(double netInput);
        static double activation_Gaussian(double netInput);
        static double activation_Sigmoid(double netInput);

        //first derivative
        static double deriv_activation_Linear(double netInput);
        static double deriv_activation_ReLu(double netInput);
      //  static double deriv_activation_Binary(double netInput);  //Not possible
        static double deriv_activation_Gaussian(double netInput);
        static double deriv_activation_Sigmoid(double netInput);

        static const std::string toIDString(NeuronID ID);
        static const std::string toTypeString(NeuronType TYPE);
        static const std::string toActivationString(Activation activationFunction);
        static const std::string toConnectionString(Connection connection);
        static const std::string toDirectionString(ConnectionDirection dir);
        std::string              toString();
        std::vector<std::string>          toStringList();

        void needsUpdate();
        /* Enables the Neuron::run() function once.
           This is for performance purpose, the Neuron::run() function can only be called once every sycleonly be called once every cycle
           or every Neuron::needsUpdate() call.

           ERROR:
            | none
         */
        bool isUpdated();
        /* Returns true if the neuron is up to date.

          ERROR:
           | none
         */
#if defined(_DEBUG_NEURON_TIMING)
        double get_runtime();
        /* Retruns the time, it takes to update the neuron.

          ERROR:
           | none
         */
#endif
        void set_seed(unsigned int seed);
        void clearErrors();
        Error get_lastError() const;
        Error get_error(unsigned int index);
        ErrorList get_errorList() const;
        unsigned int get_errorAmount() const;
#ifdef QT_APP
    signals:
        void errorOccured(NeuronID ID,Error &e);
#endif
    protected:
        static unsigned int _globalNeurons;

    private:
        void init(unsigned int inputs, Activation activationFunction, bool enableAverage);
        /* Setup the neuron

           ERROR:
            | from Neuron::inputs(unsigned int inputs)
            | from Neuron::activationFunction(Activation activationFunction)
            | from Neuron::enableAverage(bool enableAverage)
         */

        void addError(const Error &e);






        void calc_netInput();
        void calc_output();

  /*      //----------ERROR
        std::string error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max);
        std::string error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max);
        std::string error_paramOutOfRange(unsigned int paramPos,int value,int min, int max);
        std::string error_paramOutOfRange(unsigned int paramPos,double value,double min, double max);

        void        error_general(std::string function, std::string cause);
        void        error_general(std::string function, std::runtime_error &e);
        void        error_general(std::string function, std::string cause, std::runtime_error &e);
        //---------------
*/

        std::vector<double>          _weightList;
        std::vector<double*>         _ptr_inputList;
        std::vector<bool>            _inputConnectionList;
        double                       _netInput;
        double                       _output;
        double                       _delayedOutput; //for loopbacksignal

        unsigned int                _inputs;
        bool                        _enableAverage;
        std::default_random_engine  _randEngine;
        Activation                  _activationFunction;
        bool                        _needsCalculationUpdate;

        NeuronID                    _ID;
        std::vector<NeuronID>       _inputConnectionID_list;
        std::vector<ConnectionDirection>           _inputConnectionDirection_List;

        ErrorList _errorList;


#if defined(_DEBUG_NEURON_TIMING)
        std::chrono::high_resolution_clock::time_point __debug_timer1;
        std::chrono::high_resolution_clock::time_point __debug_timer2;
        std::chrono::duration<double> __debug_time_span;
        double __debug_run_time;
#endif

};

/*inline void __DEBUG_NEURON_(Neuron *ptr_neuron,std::string func,std::string message)
{
#ifdef QT_APPLICATION
    CONSOLE<< "["+std::to_string(ptr_neuron->ID().ID)+" | "+std::string::fromStdString(Neuron::typeString(ptr_neuron->ID().TYPE))+"] Neuron::"+std::string::fromStdString(func)+" "+std::string::fromStdString(message);
#else
    CONSOLE << "["<<ptr_neuron->ID().ID<<" | "<<Neuron::typeString(ptr_neuron->ID().TYPE)<<"]"<<"Net::"<<func<<" "<<message << std::endl;
#endif
}*/
//#define __DEBUG_NEURON(neuron,func,message)(__DEBUG_NEURON_(neuron,func,message));

#endif // NEURON_H
