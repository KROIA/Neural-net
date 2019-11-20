#ifndef SAVENET_H
#define SAVENET_H
//              Autor   Alex Krieg
#define SAVENET_VERSION "02.01.00"
//              Datum   16.08.2019

#include <net.h>
#include <time.h>



class SaveNet
{
    public:
        SaveNet();
        ~SaveNet();

        void filename(std::string filename);
        std::string filename();
        void fileEnding(std::string fileEnding);
        std::string fileEnding();
        void inputNeurons(unsigned int inputs);
        unsigned int inputNeurons();
        void hiddenNeuronsX(unsigned int hiddenX);
        unsigned int hiddenNeuronsX();
        void hiddenNeuronsY(unsigned int hiddenY);
        unsigned int hiddenNeuronsY();
        void outputNeurons(unsigned int outputs);
        unsigned int neurons();
        unsigned int outputNeurons();
        unsigned int hiddenNeurons();
        unsigned int costumNeurons();
        unsigned int connections();
        unsigned int costumConnections();


        void bias(bool bias);
        bool bias();
        void biasValue(float value);
        float biasValue();
        void enableAverage(bool average);
        bool enableAverage();
        void activationFunction(Activation func);
        Activation activationFunction();
        void setExtraParam(std::string name,float value);
        void getExtraParam(std::string name, float &value);
        void setExtraParam(std::vector<std::string> name,std::vector<float> value);
        void getExtraParam(std::vector<std::string> &name,std::vector<float> &value);


        void loadFile();
        void loadFile(std::string filename);
        void saveFile();
        void saveFile(std::string filename);

      //  void setGenom(std::vector<float>   genom);
      //  void setGenom(unsigned int index,std::vector<float>   genom);
      //  void setGenom(std::vector<std::vector<float>   > genomList);
      //  void addGenom(std::vector<float>   genom);
      //  void addGenom(std::vector<std::vector<float>   > genomList);
        std::vector<float>  genom(unsigned int ID);
        std::vector<std::vector<float>  > genom();
        unsigned int animals();

        void clear();
        void set(unsigned int inputs,
                 unsigned int hiddenX,
                 unsigned int hiddenY,
                 unsigned int outputs,
                 bool         bias,
                 bool         average,
                 Activation   activationFunction,
                 float        biasValue);
        void neuronsOfNet(unsigned int ID,std::vector<Neuron*> *neurons);
        std::vector<Neuron*> neuronsOfNet(unsigned int ID);

        std::vector<Connection> *connectionList(unsigned int animal);
        std::vector<std::vector<Connection> > *connectionList();


        void clearGenomList();
        void clearExternParam();

    private:
        void checkParam();
        unsigned int genomsize();
        void saveGenomOfNet(unsigned int ID);

        //----------ERROR
        std::string error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max);
        std::string error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max);
        std::string error_paramOutOfRange(unsigned int paramPos,int value,int min, int max);
        std::string error_paramOutOfRange(unsigned int paramPos,float value,float min, float max);
        void        error_general(std::string function, std::string cause);
        void        error_general(std::string function, std::runtime_error &e);
        void        error_general(std::string function, std::string cause, std::runtime_error &e);
        //---------------

        std::string _filename;
        std::string _fileEnding;
        unsigned int _saves;
        FILE *_file;

        unsigned int _animals;
        unsigned int _inputs;
        unsigned int _hiddenX;
        unsigned int _hiddenY;
        unsigned int _outputs;
        bool         _bias;
        bool         _average;
        float        _biasValue;
        Activation   _activationFuncton;
        unsigned int _neurons;
        unsigned int  _hiddenNeurons;
        unsigned int  _outputNeurons;
        unsigned int  _costumNeurons;
        unsigned int _connections;
        unsigned int  _costumConnections;
        std::vector<std::string> _extraParamName;
        std::vector<float>       _extraParamValue;

        std::vector<std::vector<float>  >_genomList;
        unsigned int _genomsize;
        // std::vector<std::vector<Net>    >_netList;

        bool        _check_inputs;
        bool        _check_hiddenX;
        bool        _check_hiddenY;
        bool        _check_outputs;
        bool        _check_bias;
        bool        _check_biasValue;
        bool        _check_average;
        bool        _check_activationFunction;
        bool        _check_neurons;
        bool         _check_hiddenNeurons;
        bool         _check_outputNeurons;
        bool         _check_costumNeuron;
        bool        _check_connections;
        bool         _check_costumConnections;

        std::vector<std::vector<Neuron*>    > _neuronList;
        std::vector<std::vector<Connection> > _connectionListFromFile;
        std::vector<unsigned int>             _ID_list;
};
#endif // SAVENET_H
