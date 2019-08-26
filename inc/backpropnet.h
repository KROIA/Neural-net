#ifndef BACKPROPNET_H
#define BACKPROPNET_H
//                      Autor   Alex Krieg
#define    BACKPROPNET_VERSION "02.03.00"
//                      Datum   26.08.2019

#include "net.h"
#include "savenet.h"

#include <QDebug>

class BackpropNet : public Net
{
    public:
        BackpropNet(unsigned int ID = 0);
        BackpropNet(unsigned int ID,
                    unsigned int inputs,
                    unsigned int hiddenX,
                    unsigned int hiddenY,
                    unsigned int outputs);
        BackpropNet(unsigned int ID,
                    unsigned int inputs,
                    unsigned int hiddenX,
                    unsigned int hiddenY,
                    unsigned int outputs,
                    bool enableBias,
                    bool enableAverage,
                    Activation func);

        void        set(unsigned int inputs,
                        unsigned int hiddenX,
                        unsigned int hiddenY,
                        unsigned int outputs,
                        bool enableBias,
                        bool enableAverage,
                        Activation func);


        void        netFileName(std::string filename);
        std::string netFileName();
        void        netFileEnding(std::string fileEnding);
        std::string netFileEnding();
        void        loadFromNetFile();
        void        loadFromNetFile(std::string filename);
        void        loadFromNetFile(std::string filename,std::string fileEnding);
        void        saveToNetFile();
        void        saveToNetFile(std::string filename);
        void        saveToNetFile(std::string filename,std::string fileEnding);



        float               netError();
        std::vector<float>  outputError();
        float               outputError(unsigned int output);
        void                mutationFactor(float mutationFactor);
        float               mutationFactor();

        void expected(std::vector<float> expected);
        void learn();
        void learn(std::vector<float> expected);
    private:
        void init();
        //----------ERROR
        std::string error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max);
        std::string error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max);
        std::string error_paramOutOfRange(unsigned int paramPos,int value,int min, int max);
        std::string error_paramOutOfRange(unsigned int paramPos,float value,float min, float max);
        void        error_general(std::string function, std::string cause);
        void        error_general(std::string function, std::runtime_error &e);
        void        error_general(std::string function, std::string cause, std::runtime_error &e);
        //---------------


        void calc_netError();
        float derivative(float netinput);
        float _netError;
        float _mutationFactor;
        bool  _update;
        std::vector<float> _outputError;
        std::vector<float> _expected;

        SaveNet _saveNet;
};
#endif // BACKPROPNET_H
