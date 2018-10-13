#ifndef BACKPROPNET_H
#define BACKPROPNET_H
//                      Autor   Alex Krieg
#define    BACKPROPNET_VERSION "02.00.02"
//                      Datum   13.10.2018

#include "net.h"


class BackpropNet : public Net
{
    public:
        BackpropNet();
        BackpropNet(unsigned int inputs,
                    unsigned int hiddenX,
                    unsigned int hiddenY,
                    unsigned int outputs);
        BackpropNet(unsigned int inputs,
                    unsigned int hiddenX,
                    unsigned int hiddenY,
                    unsigned int outputs,
                    bool enableBias,
                    bool enableAverage,
                    Activation func);
        float               netError();
        std::vector<float>  outputError();
        float               outputError(unsigned int output);
        void                mutationFactor(float mutationFactor);
        float               mutationFactor();

        void expected(std::vector<float> expected);
        void learn();
        void learn(std::vector<float> expected);
    private:
        void calc_netError();
        float derivative(float netinput);
        float _netError;
        float _mutationFactor;
        bool  _update;
        std::vector<float> _outputError;
        std::vector<float> _expected;
};
#endif // BACKPROPNET_H
