#ifndef NET_H
#define NET_H
//              Autor   Alex Krieg
#define    NET_VERSION "02.00.00"
//              Datum   07.10.2018

#include "neuron.h"
#include <math.h>
#include <random>
#include <iostream>
#include <time.h>

#define NET_MIN_INPUTNEURONS 1
#define NET_MIN_HIDDENNEURONS_X 0
#define NET_MIN_HIDDENNEURONS_Y 0
#define NET_MIN_OUTPUTNEURONS 1

#define NET_MAX_INPUTNEURONS 200
#define NET_MAX_HIDDENNEURONS_X 100
#define NET_MAX_HIDDENNEURONS_Y 100
#define NET_MAX_OUTPUTNEURONS 200




class Net
{
    public:

        Net();
        Net(unsigned int inputs,
            unsigned int hiddenX,
            unsigned int hiddenY,
            unsigned int outputs);
        Net(unsigned int inputs,
            unsigned int hiddenX,
            unsigned int hiddenY,
            unsigned int outputs,
            bool enableBias,
            bool enableAverage,
            Activation func);
        ~Net();

        void         inputNeurons(unsigned int inputs);
        unsigned int inputNeurons();
        void         hiddenNeuronsX(unsigned int hiddenX);
        unsigned int hiddenNeuronsX();
        void         hiddenNeuronsY(unsigned int hiddenY);
        unsigned int hiddenNeuronsY();
        void         outputNeurons(unsigned int outputs);
        unsigned int outputNeurons();

        void bias(bool enableBias);
        bool bias();
        void average(bool enableAverage);
        bool average();
        void biasValue(float value);
        float biasValue();
        void activationfunction(Activation func);
        Activation activationfunction();

        void randomGenom();
        void genom(std::vector<float> genom);
        std::vector<float>  genom();
        unsigned int genomsize();

        void                input(unsigned int input, float signal);
        float               input(unsigned int input);
        void                input(std::vector<float> inputList);
        std::vector<float>  input();

        float               hidden(unsigned int hiddenX, unsigned int hiddenY);
        std::vector<float>  hiddenX(unsigned int hiddenX);  // |    Alle in einer Spalte
        std::vector<float>  hiddenY(unsigned int hiddenY);  // --   Alle in einer Reihe

        Neuron              hiddenNeuron(unsigned int hiddenX, unsigned int hiddenY);
        std::vector<Neuron> hiddenNeuronX(unsigned int hiddenX);    // |    Alle in einer Spalte
        std::vector<Neuron> hiddenNeuronY(unsigned int hiddenY);    // --   Alle in einer Reihe
        std::vector<std::vector<Neuron> > hiddenNeuron();
        Neuron              outputNeuron(unsigned int output);
        std::vector<Neuron> outputNeuron();


        float               output(unsigned int output);
        std::vector<float>  output();

        void                run();

    private:

        void setupNeurons();
        void setupInputNeurons();
        void setupHiddenNeurons();
        void setupOutputNeurons();
        void setGenomToNeuron();
        void getGenomFromNeuron();

        //----------ERROR
        std::string error_paramOutOfRange(unsigned int paramPos,std::string value, std::string max);
        std::string error_paramOutOfRange(unsigned int paramPos,unsigned int value, unsigned int max);
        std::string error_paramOutOfRange(unsigned int paramPos,int value, int max);
        std::string error_paramOutOfRange(unsigned int paramPos,float value, float max);
        //---------------

        unsigned int _inputs;
        unsigned int _hiddenX;
        unsigned int _hiddenY;
        unsigned int _outputs;
        bool         _bias;
        bool         _enableAverage;
        Activation   _activationFunction;
        float        _biasValue;


        std::vector<float> _weightList;

        std::vector<float>                  _inputSignalList;     // Input of Net
        std::vector< std::vector<float> >   _hiddenSignalList;    // Output of hidden neuron x y
        std::vector<float>                  _outputSignalList;    // Output of output neuron
        bool                                _noHiddenLayer;

        bool                                _update;
        std::default_random_engine          _randEngine;

        std::vector<std::vector<Neuron> >   _hiddenNeuronList;
        std::vector<Neuron>                 _outputNeuronList;
        std::vector<float>                  _genom;

};


#endif // NET_H
