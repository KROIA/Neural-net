#ifndef BACKPROPNET_H
#define BACKPROPNET_H
//                      Autor   Alex Krieg
#define    BACKPROPNET_VERSION "02.05.00"
//                      Datum   02.02.2020

#include "net.h"
#include "savenet.h"
#include <cmath>       /* isnan, sqrt */


class BackpropNet : public Net
{
        Q_OBJECT
    public:
        BackpropNet(unsigned int ID = 0,
                    QObject *parent = nullptr);
        BackpropNet(unsigned int ID,
                    unsigned int inputs,
                    unsigned int hiddenX,
                    unsigned int hiddenY,
                    unsigned int outputs,
                    QObject *parent = nullptr);
        BackpropNet(unsigned int ID,
                    unsigned int inputs,
                    unsigned int hiddenX,
                    unsigned int hiddenY,
                    unsigned int outputs,
                    bool enableBias,
                    bool enableAverage,
                    Activation func,
                    QObject *parent = nullptr);

        void        set(unsigned int inputs,
                        unsigned int hiddenX,
                        unsigned int hiddenY,
                        unsigned int outputs,
                        bool enableBias,
                        bool enableAverage,
                        Activation func);



        void        set_netFileName(QString filename);
        QString     get_netFileName();
        void        set_netFileEnding(QString fileEnding);
        QString     get_netFileEnding();
        void        loadFromNetFile();
        void        loadFromNetFile(QString filename);
        void        loadFromNetFile(QString filename,QString fileEnding);
        void        saveToNetFile();
        void        saveToNetFile(QString filename);
        void        saveToNetFile(QString filename,QString fileEnding);



        double         get_netError();
        std::vector<double>  get_outputError();
        double         get_outputError(unsigned int output);
        void           set_mutationFactor(double mutationFactor);
        double         get_mutationFactor();

        void set_expected(std::vector<double> expected);
        void learn();
        void learn(std::vector<double> expected);

        // Functions which aren't allowed for backpropagation yet:
        void         set_costumNeurons(unsigned int costum);
        unsigned int get_costumNeurons();
        unsigned int get_costumConnections();
        void         set_costumConnections(unsigned int connections);
        //void         set_neurons(unsigned int neurons,unsigned int hiddenNeurons,unsigned int outputNeurons,unsigned int costumNeurons);
        Neuron      *get_ptr_costumNeuron(NeuronID ID);
        std::vector<Neuron*> *get_ptr_costumNeuron();
        void         addConnection(NeuronID fromNeuron,NeuronID toNeuron,ConnectionDirection direction = ConnectionDirection::forward);
        void         addConnection(Connection connection);
        void         addConnection(std::vector<Connection> connections);
        //void         set_ptr_connectionList(std::vector<Connection> *connections);
        //std::vector<Connection> *get_ptr_connectionList();
        void         clearConnectionList();
        NeuronID     addNeuron();
        NeuronID     addNeuron(Neuron *neuron);
        NeuronID     addNeuron(Connection connection);
        NeuronID     addNeuron(std::vector<Connection> inputConnections);

        void            clearErrors();
        Error           get_lastError() const;
        Error           get_error(unsigned int index);
        ErrorList       get_errorList() const;
        unsigned int    get_errorAmount() const;
        //--------------------------------------------------------
        static double __dbg_time1;
        static double __dbg_time2;
        static double __dbg_time3;
        static double __dbg_time4;
        static double __dbg_time5;
        static double __dbg_time6;
        static double __dbg_time7;
        static double __dbg_time8;
        static double __dbg_time9;
        static double __dbg_time10;
    signals:
        void errorOccured(unsigned int netID, Error &e);

    private slots:
        void onNetConfigurationUpdate();
        void onNetConfigurationUpdateFinished();

    private:
        void printIllegalFunctionMessage(QString func);
        void init();
        void addError(const Error &e);

        //----------ERROR
       /* QString error_paramOutOfRange(unsigned int paramPos,QString value,QString min, QString max);
        QString error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max);
        QString error_paramOutOfRange(unsigned int paramPos,int value,int min, int max);
        QString error_paramOutOfRange(unsigned int paramPos,double value,double min, double max);
        void        error_general(QString function, QString cause);
        void        error_general(QString function, std::runtime_error &e);
        void        error_general(QString function, QString cause, std::runtime_error &e);*/
        //---------------


        void    calc_netError();
        double  derivative(double netinput);
        double  _netError;
        double  _mutationFactor;
        bool    _update;
        std::vector<double> _outputError;
        std::vector<double> _expected;

        SaveNet _saveNet;


        // tempVariables for learn()
        std::vector<double>  __output_error;
        std::vector<std::vector<double>  > __hidden_error;
        // tempVariables for learn() end
};

inline void __DEBUG_BACKPROPNET_(Net *ptr_net,QString func,QString message)
{
#ifdef _DEBUG_NET_ONLY_ID
    if(ptr_net->get_ID() != _DEBUG_NET_ONLY_ID)
        return;
#endif
#ifdef QT_APPLICATION
    qDebug() << "["+QString::number(ptr_net->ID())+"] BackpropNet::"+QString::fromStdString(func)+" "+QString::fromStdString(message);
#else
    qDebug() << "["<<ptr_net->get_ID()<<"]"<<"Net::"<<func<<" "<<message;
#endif

#define __DEBUG_BACKPROPNET(net,func,message)(__DEBUG_BACKPROPNET_(net,func,message));

}
#endif // BACKPROPNET_H
