#ifndef BACKPROPNET_H
#define BACKPROPNET_H
//                      Autor   Alex Krieg
#define    BACKPROPNET_VERSION "02.00.01"
//                      Datum   11.10.2018

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
        std::vector<float>  netError();
        float               netError(unsigned int output);
        void                mutationFactor(float mutationFactor);
        float               mutationFactor();

        void expected(std::vector<float> expected);
        void learn(std::vector<float> expected);
    private:
        void calc_netError();
        float _netError;
        float _mutationFactor;
        std::vector<float> _outputError;
        std::vector<float> _expected;
};
#endif // BACKPROPNET_H
