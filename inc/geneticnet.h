#ifndef GENETICNET_H
#define GENETICNET_H
//                      Autor   Alex Krieg
#define    GENETICNET_VERSION "02.05.00"
//                      Datum  26.08.2019

#include "net.h"
#include "savenet.h"

#include <QDebug>

#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define GENETICNET_MIN_ANIMALS 2
#define GENETICNET_MAX_ANIMALS 1000

#define __enableGeneticNetThread

struct thread_data_geneticNet {
   int  thread_id;
   Net *net;
   bool *exit;
   bool *pause;
   pthread_mutex_t *lock;
   bool isPaused;
   long *delayMicros;
};

class GeneticNet
{
    public:
        GeneticNet();
        GeneticNet(unsigned int animals);
        GeneticNet(unsigned int animals,
                   unsigned int inputs,
                   unsigned int hiddenX,
                   unsigned int hiddenY,
                   unsigned int outputs);
        GeneticNet(unsigned int animals,
                   unsigned int inputs,
                   unsigned int hiddenX,
                   unsigned int hiddenY,
                   unsigned int outputs,
                   bool enableBias,
                   bool enableAverage,
                   Activation func);
        ~GeneticNet();

        void                    set(unsigned int animals,
                                    unsigned int inputs,
                                    unsigned int hiddenX,
                                    unsigned int hiddenY,
                                    unsigned int outputs,
                                    bool enableBias,
                                    bool enableAverage,
                                    Activation func);

        void                    netFileName(std::string filename);
        std::string             netFileName();
        void                    netFileEnding(std::string fileEnding);
        std::string             netFileEnding();
        void                    loadFromNetFile();
        void                    loadFromNetFile(std::string filename);
        void                    loadFromNetFile(std::string filename,std::string fileEnding);
        void                    saveToNetFile();
        void                    saveToNetFile(std::string filename);
        void                    saveToNetFile(std::string filename,std::string fileEnding);

        void                    animals(unsigned int animals);
        unsigned int            animals();
        void                    inputNeurons(unsigned int inputs);
        unsigned int            inputNeurons();
        void                    hiddenNeuronsX(unsigned int hiddenX);
        unsigned int            hiddenNeuronsX();
        void                    hiddenNeuronsY(unsigned int hiddenY);
        unsigned int            hiddenNeuronsY();
        void                    outputNeurons(unsigned int outputs);
        unsigned int            outputNeurons();
        void                    costumNeurons(unsigned int costum);
        unsigned int            costumNeurons();
        void                    costumConnections(unsigned int connections);
        void                    neurons(unsigned int neurons,unsigned int hiddenNeurons,unsigned int outputNeurons,unsigned int costumNeurons);


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
        void                    genom(std::vector<std::vector<float>    > genomList);
        void                    genom(unsigned int animal, std::vector<float> genom);
        std::vector<float>      genom(unsigned int animal);
        std::vector<std::vector<float>  >genom();
        unsigned int            genomsize();
        void                    genomFromNetFile();

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
        std::vector<Neuron*>     hiddenNeuronX(unsigned int animal, unsigned int hiddenX);    // |    Alle in einer Spalte
        std::vector<Neuron*>     hiddenNeuronY(unsigned int animal, unsigned int hiddenY);    // --   Alle in einer Reihe
        std::vector<std::vector<Neuron*> > *hiddenNeuron(unsigned int animal);
        Neuron                 *outputNeuron(unsigned int animal, unsigned int output);
        std::vector<Neuron*>   *outputNeuron(unsigned int animal);


        float                   output(unsigned int animal, unsigned int output);
        std::vector<float>      output(unsigned int animal);
        std::vector<std::vector<float>  >output();


        void                    run();
        void                    run(unsigned int animal);



        Net *operator[](unsigned int animal);

        void                mutationFactor(float factor);
        void                mutationChangeWeight(float weight);

        void                score(unsigned int animal, float score);
        float               score(unsigned int animal);
        void                score(std::vector<float>   scoreList);
        std::vector<float>  score();

        void                learn(std::vector<float>   scoreList);
        void                learn();

        void                updateNetConfiguration();
        /*  Needed after every change in the Net-structure like
         *  inputNeurons()
         *  sins V02.03.00
         */
        void                connectNeuronViaID(unsigned int fromNeuron,unsigned int toNeuron,bool forward = true);
        void                connectionList(std::vector<std::vector<Connection> >connections);
        std::vector<Connection> *connectionList(unsigned int netID);
        std::vector<std::vector<Connection>* > connectionList();

    private:

        void                    init(unsigned int animals,
                                     unsigned int inputs,
                                     unsigned int hiddenX,
                                     unsigned int hiddenY,
                                     unsigned int outputs,
                                     bool enableBias,
                                     bool enableAverage,
                                     Activation func);

        void learn_selectAnimal(float gesScore,unsigned int &selection1,unsigned int &selection2);
        void learn_crossover(unsigned int selection1,unsigned int selection2,std::vector<float> &newGen1,std::vector<float> &newGen2);
        void learn_mutate(std::vector<float> &genom);

        //Threads
        static void *runThread(void *threadarg);
        //----------ERROR
        std::string error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max);
        std::string error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max);
        std::string error_paramOutOfRange(unsigned int paramPos,int value,int min, int max);
        std::string error_paramOutOfRange(unsigned int paramPos,float value,float min, float max);
        void        error_general(std::string function, std::string cause);
        void        error_general(std::string function, std::runtime_error &e);
        void        error_general(std::string function, std::string cause, std::runtime_error &e);
        //---------------

        bool         _update;

        unsigned int _animals;
        unsigned int _currentAnimal;

        float        _mutationFactor;
        float        _mutationChangeWeight;

        std::vector<Net*>    _netList;
        std::vector<float>  _scoreList;

        std::default_random_engine          _randEngine;

        SaveNet _saveNet;

        //Threads
        pthread_mutex_t _threadLock=PTHREAD_MUTEX_INITIALIZER;
        std::vector<thread_data_geneticNet> _threadData;
        std::vector<pthread_t>  _threadList;
        bool                    _threadExit;
        bool                    _threadPause;
        long                    _threadDelayMicros;

};
#endif // GENETICNET_H
