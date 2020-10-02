#ifndef NET_H
#define NET_H
//              Autor   Alex Krieg
#define    NET_VERSION "02.06.02"
//              Datum   02.10.2020

/*
 Some functions may throw errors.
 All thrwon errors are std::runtime_error
*/

//  DEBUGING
#define _DEBUG_NET_ONLY_ID 0
//#define _DEBUG_NET_ALL
//#define _DEBUG_NET_RUN
//#define _DEBUG_NET_UPDATE_NET_CONFIGURATION
#define _DEBUG_NET_TIMING
//#define _DEBUG_NET_CONNECT
//#define _DEBUG_NET_CALCULATION_ORDER_LIST


//#ifdef _DEBUG_NET_ALL
//#ifndef _DEBUG_NET_RUN
//#define _DEBUG_NET_RUN
//#endif
//#ifndef _DEBUG_NET_UPDATE_NET_CONFIGURATION
//#define _DEBUG_NET_UPDATE_NET_CONFIGURATION
//#endif
#ifndef _DEBUG_NET_TIMING
#define _DEBUG_NET_TIMING
//#endif
#ifndef _DEBUG_NET_CONNECT
#define _DEBUG_NET_CONNECT
#endif
#endif
//---------

#include "neuron.h"
#include <math.h>
#include <random>
#include <time.h>

#ifdef QT_APP
  #include <QDebug>
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

#include <error.h>

#ifdef _DEBUG_NET_TIMING
#include <ctime>
#include <ratio>
#include <chrono>
#endif

#define NET_MIN_INPUTNEURONS 1
#define NET_MIN_HIDDENNEURONS_X 0
#define NET_MIN_HIDDENNEURONS_Y 0
#define NET_MIN_OUTPUTNEURONS 1

#define NET_MAX_INPUTNEURONS 10000
#define NET_MAX_HIDDENNEURONS_X 1000
#define NET_MAX_HIDDENNEURONS_Y 1000
#define NET_MAX_OUTPUTNEURONS 200

#define NET_MIN_NEURONS 1
#define NET_MAX_NEURONS 1000




#ifdef QT_APP
class Net : public QObject
#else
class Net
#endif
{
#ifdef QT_APP
        Q_OBJECT
#endif
    public:
#ifdef QT_APP
        Net(unsigned int ID = 0,
            QObject *parent = nullptr);
        Net(unsigned int ID,
            unsigned int inputs,
            unsigned int hiddenX,
            unsigned int hiddenY,
            unsigned int outputs,
            QObject *parent = nullptr);
        Net(unsigned int ID,
            unsigned int inputs,
            unsigned int hiddenX,
            unsigned int hiddenY,
            unsigned int outputs,
            bool enableBias,
            bool enableAverage,
            Activation func,
            QObject *parent = nullptr);
#else
        Net(unsigned int ID = 0);
        Net(unsigned int ID,
            unsigned int inputs,
            unsigned int hiddenX,
            unsigned int hiddenY,
            unsigned int outputs);
        Net(unsigned int ID,
            unsigned int inputs,
            unsigned int hiddenX,
            unsigned int hiddenY,
            unsigned int outputs,
            bool enableBias,
            bool enableAverage,
            Activation func);
#endif
        ~Net();

        void                    set_ID(unsigned int ID);
        unsigned int            get_ID();

        void                    set_inputNeurons(unsigned int inputs);      //update
        unsigned int            get_inputNeurons();
        void                    set_hiddenNeuronsX(unsigned int hiddenX);   //update
        unsigned int            get_hiddenNeuronsX();
        void                    set_hiddenNeuronsY(unsigned int hiddenY);   //update
        unsigned int            get_hiddenNeuronsY();
        unsigned int            get_hiddenNeurons();
        void                    set_outputNeurons(unsigned int outputs);    //update
        unsigned int            get_outputNeurons();
        //void                    set_costumNeurons(unsigned int costum);
        unsigned int            get_costumNeurons();
        unsigned int            get_connections();
        unsigned int            get_costumConnections();
        //void                    set_costumConnections(unsigned int connections);
        //void                    set_neurons(unsigned int neurons,unsigned int hiddenNeurons,unsigned int outputNeurons,unsigned int costumNeurons);
        unsigned int            get_neurons();

        void                    set_bias(bool enableBias);                  //update
        bool                    get_bias();
        void                    set_enableAverage(bool enableAverage);      //update
        bool                    get_enableAverage();
        void                    set_biasValue(double value);
        double                  get_biasValue();
        void                    set_activationFunction(Activation func);    //update
        Activation              get_activationFunction();
        bool                    hasHiddenLayer();

        void                    set_randomGenom();
        void                    set_genom(std::vector<double> genom);
        std::vector<double>     get_genom();
        std::vector<double*> *  get_ptr_genom();
        unsigned int            get_genomsize();

        void                    set_input(unsigned int input, double signal);
        double                  get_input(unsigned int input);
        void                    set_input(std::vector<double> inputList);
        std::vector<double>     get_input();

        std::vector<double>     get_hidden();// Alle outpus der Hiddenneuronen
        double                  get_hidden(unsigned int hiddenX, unsigned int hiddenY);
        std::vector<double>     get_hiddenX(unsigned int hiddenX);  // |    Alle in einer Spalte
        std::vector<double>     get_hiddenY(unsigned int hiddenY);  // --   Alle in einer Reihe

        Neuron                 *get_ptr_neuron_viaID(unsigned int ID);
        Neuron                 *get_ptr_neuron_viaID(NeuronID ID);
        Neuron                 *get_ptr_hiddenNeuron(unsigned int hiddenX, unsigned int hiddenY);
        std::vector<Neuron*>    get_hiddenNeuronX_ptr(unsigned int hiddenX);    // |    Alle in einer Spalte
        std::vector<Neuron*>    get_hiddenNeuronY_ptr(unsigned int hiddenY);    // --   Alle in einer Reihe
        std::vector<std::vector<Neuron*> > get_hiddenNeuron_ptr();
        Neuron                 *get_ptr_costumNeuron(NeuronID ID);
        std::vector<Neuron*>   *get_ptr_costumNeuron();
        Neuron                 *get_ptr_outputNeuron(unsigned int output);
        std::vector<Neuron*>   *get_ptr_outputNeuron();
        std::vector<Neuron*>   *get_ptr_allNeurons();


        double                  get_output(unsigned int output);
        std::vector<double>     get_output();

        void                    run();

        bool                    needsUpdate();
        void                    updateNetConfiguration();
        /*  Needed after every change in the Net-structure like
         *  inputNeurons()
         *  sins V02.01.00
         */
        void                    addConnection(NeuronID fromNeuron,NeuronID toNeuron,ConnectionDirection direction = ConnectionDirection::forward);//update
        void                    addConnection(Connection connection);                              //update
        void                    addConnection(std::vector<Connection> connections);                //update
        void                    set_connectionList(std::vector<Connection> connections);           //update
        std::vector<Connection> get_connectionList();
        std::vector<Connection> get_costumConnectionList();

        NeuronID                addNeuron();                                                        //update
        NeuronID                addNeuron(Neuron *neuron);                                          //update


        std::string                 toString();
        std::vector<std::string>             toStringList();
        bool                    isEqual(Net *toCompare);
#if defined(_DEBUG_NET_TIMING)
        double get_runtime();
        /* Retruns the time, it takes to update the net.

          ERROR:
           | none
         */
#endif
        void            set_seed(unsigned int seed);
        void            clearErrors();
        Error           get_lastError() const;
        Error           get_error(unsigned int index);
        ErrorList       get_errorList() const;
        unsigned int    get_errorAmount() const;
#ifdef QT_APP
    signals:
        void errorOccured(unsigned int netID, Error &e);

        void netConfigurationUpdateNeeded(); //Trigger, for updating the netConfiguration
        void netConfigurationUpdateStarted(); //Trigger, when the updating function gets called
        void netConfigurationUpdated();      //Infosignal when the updating is finished
        void accessLock();                    //do not access functions like: get_input() ... otherwise this error will be shown: "Update required: call updateNetConfiguration() first!"
        void accessUnlock();                  //from now on you can access all functions again

        //void inputsChanged(Net *net);
        //void hiddenOutputsChanged(Net *net);
        //void outputsChanged(Net *net);
        void runDone(Net *net);
        void biasValueChanged(Net *net);
        void weightValuesChanged(Net *net);
#endif

    protected:
        bool    _needsCalculationUpdate;
        bool    _needsConfigurationUpdate;
        bool    _doRegenerateConnectionlist;
        ErrorList _errorList;
#ifdef QT_APP
    private slots:
        void onNeuronError(NeuronID ID,Error &e);
#endif
    private:
        void init(unsigned int inputs,
                  unsigned int hiddenX,
                  unsigned int hiddenY,
                  unsigned int outputs,
                  bool enableBias,
                  bool enableAverage,
                  Activation func);
        void update_ptr_genomList();

        bool set_ptr_intern_connectNeuron(Connection *connection);
        void prepareConnectionList();
        void prepareCalculationOrderList();
        void clearConnectionList();

        void addError(const Error &e);

   //     void setGenomToNeuron();
   //     void getGenomFromNeuron();

        //----------ERROR
   /*     std::string error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max);
        std::string error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max);
        std::string error_paramOutOfRange(unsigned int paramPos,int value,int min, int max);
        std::string error_paramOutOfRange(unsigned int paramPos,double value,double min, double max);
        void        error_general(std::string function, std::string cause);
        void        error_general(std::string function, std::runtime_error &e);
        void        error_general(std::string function, std::string cause, std::runtime_error &e);
        //---------------
*/
        unsigned int    _inputs;
        unsigned int    _hiddenX;
        unsigned int    _hiddenY;
        unsigned int    _outputs;
        unsigned long long    _connections;
        unsigned long long    _costumConnections;
        bool            _bias;
        bool            _enableAverage;
        Activation      _activationFunction;
        double          _biasValue;
        unsigned int    _neurons;       //updated in updateNetConfiguration()
        unsigned int    _hiddenNeurons; //updated in updateNetConfiguration()
        unsigned int    _outputNeurons; //updated in updateNetConfiguration()
        unsigned int    _costumNeurons; //updated in updateNetConfiguration()
        //double*       _ptr_biasValue;



      //  std::vector<double*>                 _ptr_inputSignalList;     // Input of Net
        std::vector<double>                  _inputSignalList;
        //std::vector< std::vector<double> >   _hiddenSignalList;    // Output of hidden neuron x y
        bool                                _hasHiddenLayer;


        std::default_random_engine          _randEngine;

        //std::vector<std::vector<Neuron*> >  _hiddenNeuronList;
        //std::vector<Neuron*>                _outputNeuronList;
        std::vector<double*>                 _ptr_genom;

        std::vector<Neuron*>                _allNeuronList;

        std::vector<Neuron*>                _hiddenNeuronList;
        std::vector<Neuron*>                _outputNeuronList;
        std::vector<Neuron*>                _costumNeuronList;

        std::vector<NeuronID>               _calculationOrderList;

        std::vector<Connection>             _connectionList;        //standard connections
        std::vector<Connection>             _costumConnectionList;  //special connections

        unsigned int _ID;



#ifdef _DEBUG_NET_TIMING
        std::chrono::high_resolution_clock::time_point __debug_timer1_start;
        std::chrono::high_resolution_clock::time_point __debug_timer1_end;

        std::chrono::high_resolution_clock::time_point __debug_timer2_start;
        std::chrono::high_resolution_clock::time_point __debug_timer2_end;

        std::chrono::duration<double> __debug_time_span;
        double __debug_run_time;
        double __debug_average_run_time;
#endif

};

inline void __DEBUG_NET_(Net *ptr_net,std::string func,std::string message)
{
#ifdef _DEBUG_NET_ONLY_ID
    if(ptr_net->get_ID() != _DEBUG_NET_ONLY_ID)
        return;
#endif
    std::string msg = "["+std::to_string(ptr_net->get_ID())+"] Net::"+func+" "+message;
    if(msg.find_last_of("\n") != msg.size()-1)
        msg+="\n";
    CONSOLE << msg.c_str();
}
#define __DEBUG_NET(net,func,message)(__DEBUG_NET_(net,func,message));


const std::string __NET_ERROR_MESSAGE_UPDATE_FIRDST = "update required: call Net::updateNetConfiguration() first.";

#endif // NET_H
