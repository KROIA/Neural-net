#ifndef NET_H
#define NET_H
//              Autor   Alex Krieg
#define    NET_VERSION "02.03.01"
//              Datum   26.08.2019

#include "neuron.h"
#include <math.h>
#include <random>
#include <iostream>
#include <time.h>
#include <QDebug>

#define NET_MIN_INPUTNEURONS 1
#define NET_MIN_HIDDENNEURONS_X 0
#define NET_MIN_HIDDENNEURONS_Y 0
#define NET_MIN_OUTPUTNEURONS 1

#define NET_MAX_INPUTNEURONS 200
#define NET_MAX_HIDDENNEURONS_X 100
#define NET_MAX_HIDDENNEURONS_Y 100
#define NET_MAX_OUTPUTNEURONS 200

#define NET_MIN_NEURONS 1
#define NET_MAX_NEURONS 1000




class Net
{
    public:

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
        ~Net();

        void ID(unsigned int ID);
        unsigned int ID();

        void         inputNeurons(unsigned int inputs);
        unsigned int inputNeurons();
        void         hiddenNeuronsX(unsigned int hiddenX);
        unsigned int hiddenNeuronsX();
        void         hiddenNeuronsY(unsigned int hiddenY);
        unsigned int hiddenNeuronsY();
        void         outputNeurons(unsigned int outputs);
        unsigned int outputNeurons();
        void         costumNeurons(unsigned int costum);
        unsigned int costumNeurons();
        unsigned int connections();
        unsigned int costumConnections();
        void         costumConnections(unsigned int connections);
        void         neurons(unsigned int neurons,unsigned int hiddenNeurons,unsigned int outputNeurons,unsigned int costumNeurons);

        void bias(bool enableBias);
        bool bias();
        void enableAverage(bool enableAverage);
        bool enableAverage();
        void biasValue(float value);
        float biasValue();
        void activationFunction(Activation func);
        Activation activationFunction();
        bool    noHiddenLayer();

        void randomGenom();
        void genom(std::vector<float> genom);
        std::vector<float>  genom();
        std::vector<float*> *ptr_genom();
        unsigned int genomsize();

        void                input(unsigned int input, float signal);
        float               input(unsigned int input);
        void                input(std::vector<float> inputList);
        std::vector<float>  input();

        float               hidden(unsigned int hiddenX, unsigned int hiddenY);
        std::vector<float>  hiddenX(unsigned int hiddenX);  // |    Alle in einer Spalte
        std::vector<float>  hiddenY(unsigned int hiddenY);  // --   Alle in einer Reihe

        Neuron              *neuron_viaID(unsigned int ID);
        Neuron              *neuron_viaID(NeuronID ID);
        Neuron              *hiddenNeuron(unsigned int hiddenX, unsigned int hiddenY);
        std::vector<Neuron*> hiddenNeuronX(unsigned int hiddenX);    // |    Alle in einer Spalte
        std::vector<Neuron*> hiddenNeuronY(unsigned int hiddenY);    // --   Alle in einer Reihe
        std::vector<std::vector<Neuron*> > *hiddenNeuron();
        Neuron              *outputNeuron(unsigned int output);
        std::vector<Neuron*> *outputNeuron();
        std::vector<Neuron*> *allNeurons();


        float               output(unsigned int output);
        std::vector<float>  output();

        void                run();
        void                updateNetConfiguration();
        /*  Needed after every change in the Net-structure like
         *  inputNeurons()
         *  sins V02.01.00
         */
        void                connectNeuronViaID(unsigned int fromNeuron,unsigned int toNeuron,bool forward = true);
        void                connectionList(std::vector<Connection> connections);
        std::vector<Connection> *connectionList();
    private:
        void init(unsigned int inputs,
                  unsigned int hiddenX,
                  unsigned int hiddenY,
                  unsigned int outputs,
                  bool enableBias,
                  bool enableAverage,
                  Activation func);

        void prepareConnectionList();
   //     void setGenomToNeuron();
   //     void getGenomFromNeuron();

        //----------ERROR
        std::string error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max);
        std::string error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max);
        std::string error_paramOutOfRange(unsigned int paramPos,int value,int min, int max);
        std::string error_paramOutOfRange(unsigned int paramPos,float value,float min, float max);
        void        error_general(std::string function, std::string cause);
        void        error_general(std::string function, std::runtime_error &e);
        void        error_general(std::string function, std::string cause, std::runtime_error &e);
        //---------------

        unsigned int _inputs;
        unsigned int _hiddenX;
        unsigned int _hiddenY;
        unsigned int _outputs;
        unsigned int _connections;
        unsigned int  _costumConnections;
        bool         _bias;
        bool         _enableAverage;
        Activation   _activationFunction;
        float        _biasValue;
        unsigned int _neurons;
        unsigned int  _hiddenNeurons;
        unsigned int  _outputNeurons;
        unsigned int  _costumNeurons;
        //float*       _ptr_biasValue;



      //  std::vector<float*>                 _ptr_inputSignalList;     // Input of Net
        std::vector<float>                  _inputSignalList;
        //std::vector< std::vector<float> >   _hiddenSignalList;    // Output of hidden neuron x y
        bool                                _noHiddenLayer;

        bool                                _update;
        bool                                _updateNetConfiguration;
        std::default_random_engine          _randEngine;

        //std::vector<std::vector<Neuron*> >  _hiddenNeuronList;
        //std::vector<Neuron*>                _outputNeuronList;
        std::vector<float*>                 _ptr_genom;

        std::vector<Neuron*>                _allNeuronList;

        std::vector<Neuron*>                _hiddenNeuronList;
        std::vector<Neuron*>                _outputNeuronList;
        std::vector<Neuron*>                _costumNeuronList;

        std::vector<Connection>             _connectionList;


        unsigned int _ID;

};
#endif // NET_H
