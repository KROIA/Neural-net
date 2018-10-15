#ifndef GENETICNET_H
#define GENETICNET_H
//                      Autor   Alex Krieg
#define    BACKPROPNET_VERSION "02.00.02"
//                      Datum   13.10.2018

#include "net.h"


class GeneticNet
{
    public:
        GeneticNet();
        GeneticNet(unsigned int inputs,
                    unsigned int hiddenX,
                    unsigned int hiddenY,
                    unsigned int outputs);
        GeneticNet(unsigned int inputs,
                    unsigned int hiddenX,
                    unsigned int hiddenY,
                    unsigned int outputs,
                    bool enableBias,
                    bool enableAverage,
                    Activation func,
                    unsigned int animals);

        void                    inputNeurons(unsigned int inputs);
        unsigned int            inputNeurons();
        void                    hiddenNeuronsX(unsigned int hiddenX);
        unsigned int            hiddenNeuronsX();
        void                    hiddenNeuronsY(unsigned int hiddenY);
        unsigned int            hiddenNeuronsY();
        void                    outputNeurons(unsigned int outputs);
        unsigned int            outputNeurons();

        void                    bias(bool enableBias);
        bool                    bias();
        void                    enableAverage(bool enableAverage);
        bool                    enableAverage();
        void                    biasValue(float value);
        float                   biasValue();
        void                    activationFunction(Activation func);
        Activation              activationFunction();
        bool                    noHiddenLayer();

        void                    randomGenom();
        void                    genom(unsigned int animal, std::vector<float> genom);
        std::vector<float>      genom(unsigned int animal);
        std::vector<std::vector<float>  >genom();
        unsigned int            genomsize();

        void                    input(unsigned int animal, unsigned int input, float signal);
        float                   input(unsigned int animal, unsigned int input);
        void                    input(unsigned int animal, std::vector<float> inputList);
        std::vector<float>      input(unsigned int animal);
        void                    input(std::vector<std::vector<float>    > input);
        std::vector<std::vector<float>  >input();

        float                   hidden(unsigned int animal, unsigned int hiddenX, unsigned int hiddenY);
        std::vector<float>      hidden(unsigned int hiddenX, unsigned int hiddenY);
        std::vector<float>      hiddenX(unsigned int animal, unsigned int hiddenX);  // |    Alle in einer Spalte
        std::vector<std::vector<float>  >hiddenX(unsigned int hiddenX);
        std::vector<float>      hiddenY(unsigned int animal, unsigned int hiddenY);  // --   Alle in einer Reihe
        std::vector<std::vector<float>  >hiddenY(unsigned int hiddenY);

        Neuron                 *hiddenNeuron(unsigned int animal, unsigned int hiddenX, unsigned int hiddenY);
        std::vector<Neuron>     hiddenNeuronX(unsigned int animal, unsigned int hiddenX);    // |    Alle in einer Spalte
        std::vector<Neuron>     hiddenNeuronY(unsigned int animal, unsigned int hiddenY);    // --   Alle in einer Reihe
        std::vector<std::vector<Neuron> > *hiddenNeuron(unsigned int animal);
        Neuron                 *outputNeuron(unsigned int animal, unsigned int output);
        std::vector<Neuron>    *outputNeuron(unsigned int animal);


        float                   output(unsigned int animal, unsigned int output);
        std::vector<float>      output(unsigned int animal);
        std::vector<std::vector<float>  >output();


        void                    run();
        void                    run(unsigned int animal);
        void                    updateNetConfiguration();



        Net *operator[](unsigned int animal);
        Net *operator=(Net *net);

        void score(unsigned int animal, float score);
        float score(unsigned int animal);
        void score(std::vector<float>   scoreList);
        std::vector<float>  score();

        void learn(std::vector<float>   scoreList);
    private:


        unsigned int _animals;
        unsigned int _currentAnimal;

        float        _mutationFactor;
        float        _mutationDivisor;

        std::vector<Net>    _netList;
        std::vector<float>  _scoreList;


};
#endif // GENETICNET_H
