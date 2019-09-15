#ifndef BACKPROPNET_H
#define BACKPROPNET_H
//                      Autor   Alex Krieg
#define    BACKPROPNET_VERSION "02.04.00"
//                      Datum   15.09.2019

#include "net.h"
#include "savenet.h"

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

        // Functions which aren't allowed for backpropagation yet:
        void         costumNeurons(unsigned int costum);
        unsigned int costumNeurons();
        unsigned int costumConnections();
        void         costumConnections(unsigned int connections);
        void         neurons(unsigned int neurons,unsigned int hiddenNeurons,unsigned int outputNeurons,unsigned int costumNeurons);
        Neuron               *costumNeuron(NeuronID ID);
        std::vector<Neuron*> *costumNeuron();
        bool                connectNeuronViaID(unsigned int fromNeuron,unsigned int toNeuron,ConnectionDirection direction = ConnectionDirection::forward);
        bool                connectNeuron(Connection *connection);
        bool                connectNeuron(std::vector<Connection> *connections);
        void                connectionList(std::vector<Connection> *connections);
        std::vector<Connection> *connectionList();
        void                clearConnectionList();
        NeuronID            addNeuron();
        NeuronID            addNeuron(Neuron *neuron);
        NeuronID            addNeuron(Connection connection);
        NeuronID            addNeuron(std::vector<Connection> inputConnections);
        void printIllegalFunctionMessage(std::string func);
        //--------------------------------------------------------

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

inline void __DEBUG_BACKPROPNET_(Net *ptr_net,std::string func,std::string message)
{
#ifdef _DEBUG_NET_ONLY_ID
    if(ptr_net->ID() != _DEBUG_NET_ONLY_ID)
        return;
#endif
#ifdef QT_APPLICATION
    qDebug() << "["+QString::number(ptr_net->ID())+"] BackpropNet::"+QString::fromStdString(func)+" "+QString::fromStdString(message);
#else
    std::cout << "["<<ptr_net->ID()<<"]"<<"Net::"<<func<<" "<<message << std::endl;
#endif

#define __DEBUG_BACKPROPNET(net,func,message)(__DEBUG_BACKPROPNET_(net,func,message));

}
#endif // BACKPROPNET_H
