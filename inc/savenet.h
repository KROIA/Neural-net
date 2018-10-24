#ifndef SAVENET_H
#define SAVENET_H

#include <net.h>

class SaveNet
{
    public:
        SaveNet();
        ~SaveNet();

        void filename(std::string filename);
        std::string filename();
        void fileEnding(std::string fileEnding);
        std::string fileEnding();
        std::string netConfiguration();
        void inputNeurons(unsigned int inputs);
        unsigned int inputNeurons();
        void hiddenNeuronsX(unsigned int hiddenX);
        unsigned int hiddenNeuronsX();
        void hiddenNeuronsY(unsigned int hiddenY);
        unsigned int hiddenNeuronsY();
        void outptNeurons(unsigned int outputs);
        unsigned int outputNeurons();
        void bias(bool bias);
        bool bias();
        void biasValue(float value);
        float biasValue();
        void enableAverage(bool average);
        bool enableAverage();
        void activationFunction(Activation func);
        Activation activationFunction();
        void addExtraParam(std::string name,float value);
        void getExtraParam(std::string name, float &value);
        void addExtraParam(std::vector<std::string> name,std::vector<float> value);
        void getExtraParam(std::vector<std::string> &name,std::vector<float> &value);


        void loadFile();
        void loadFile(std::string filename);
        void saveFile();
        void saveFile(std::string filename);

        void addGenom(std::vector<float>   genom);
        void addGenom(std::vector<std::vector<float>   > genomList);
        std::vector<float>  genom(unsigned int index = 0);
        std::vector<std::vector<float>  > genom();

        //void net(Net *net);


    private:

        //----------ERROR
        std::string error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max);
        std::string error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max);
        std::string error_paramOutOfRange(unsigned int paramPos,int value,int min, int max);
        std::string error_paramOutOfRange(unsigned int paramPos,float value,float min, float max);
        void        error_general(std::string function, std::string cause);
        void        error_general(std::string function, std::runtime_error &e);
        void        error_general(std::string function, std::string cause, std::runtime_error &e);
        //---------------

        bool        _update;

        std::string _filename;
        std::string _fileEnding;
        std::string _netConfiguration;

        unsigned int _inputs;
        unsigned int _hiddenX;
        unsigned int _hiddenY;
        unsigned int _outputs;
        bool         _bias;
        bool         _average;
        float        _biasValue;
        Activation   _activationFuncton;
        std::vector<std::string> _extraParamName;
        std::vector<float>       _extraParamValue;

        std::vector<std::vector<float>  >_genomList;
       // std::vector<std::vector<Net>    >_netList;

};
#endif // SAVENET_H
