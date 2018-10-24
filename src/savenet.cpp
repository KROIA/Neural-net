#include <savenet.h>


SaveNet::SaveNet()
{
    filename("netFile");
    fileEnding("net");
}
SaveNet::~SaveNet()
{

}

void            SaveNet::filename(std::string filename)
{
    _filename = filename;
}
std::string     SaveNet::filename()
{
    return _filename;
}
void            SaveNet::fileEnding(std::string fileEnding)
{
    _fileEnding = fileEnding;
}
std::string     SaveNet::fileEnding()
{
    return _fileEnding;
}
std::string     SaveNet::netConfiguration()
{
    return _netConfiguration;
}
void            SaveNet::inputNeurons(unsigned int inputs)
{
    _inputs  = inputs;
}
unsigned int    SaveNet::inputNeurons()
{
    return _inputs;
}
void            SaveNet::hiddenNeuronsX(unsigned int hiddenX)
{
    _hiddenX = hiddenX;
}
unsigned int    SaveNet::hiddenNeuronsX()
{
    return _hiddenX;
}
void            SaveNet::hiddenNeuronsY(unsigned int hiddenY)
{
    _hiddenY = hiddenY;
}
unsigned int    SaveNet::hiddenNeuronsY()
{
    return _hiddenY;
}
void            SaveNet::outptNeurons(unsigned int outputs)
{
    _outputs = outputs;
}
unsigned int    SaveNet::outputNeurons()
{
    return _outputs;
}
void            SaveNet::bias(bool bias)
{
    _bias = bias;
}
bool            SaveNet::bias()
{
    return _bias;
}
void            SaveNet::biasValue(float value)
{
    _biasValue = value;
}
float           SaveNet::biasValue()
{
    return _biasValue;
}
void            SaveNet::enableAverage(bool average)
{
    _average = average;
}
bool            SaveNet::enableAverage()
{
    return _average;
}
void            SaveNet::activationFunction(Activation func)
{
    _activationFuncton = func;
}
Activation      SaveNet::activationFunction()
{
    return _activationFuncton;
}
void            SaveNet::addExtraParam(std::string name,float value)
{
    for(unsigned int a=0; a<_extraParamName.size(); a++)
    {
        if(_extraParamName[a] == name)
        {
            //error_general("addExtraParam(std::string ["+name+"] , float ["+std::to_string(value)+"] )","Multiple parameter: "+name);
            _extraParamValue[a] = value;
            return;
        }
    }
    _extraParamName.push_back(name);
    _extraParamValue.push_back(value);
}
void            SaveNet::getExtraParam(std::string name, float &value)
{
    for(unsigned int a=0; a<_extraParamName.size(); a++)
    {
        if(_extraParamName[a] == name)
        {
            value = _extraParamValue[a];
            return;
        }
    }
    error_general("getExternParam(std::string ["+name+"] , float & ["+std::to_string(value)+"] )","Unknown parameter: "+name);
}
void            SaveNet::addExtraParam(std::vector<std::string> name,std::vector<float> value)
{
    if(name.size() != value.size())
    {
        error_general("addExternParam(std::vector<std::string> , std::vector<float> )","Vectorsize is not the same");
    }
    for(unsigned int a=0; a<name.size(); a++)
    {
        try {
            addExtraParam(name[a],value[a]);
        } catch (std::runtime_error &e) {
            error_general("addExternParam(std::vector<std::string> , std::vector<float> )","addExtraParam("+name[a]+","+std::to_string(value[a])+")",e);
        }
    }
}
void            SaveNet::getExtraParam(std::vector<std::string> &name,std::vector<float> &value)
{
    name    = _extraParamName;
    value   = _extraParamValue;
}


void SaveNet::loadFile()
{

}
void SaveNet::loadFile(std::string filename)
{
    this->filename(filename);
    loadFile();
}
void SaveNet::saveFile()
{

}
void SaveNet::saveFile(std::string filename)
{
    this->filename(filename);
    saveFile();
}

void SaveNet::addGenom(std::vector<float>   genom)
{
    unsigned int genomsize = (_inputs + (unsigned int)_bias) +
                             ((_hiddenY + (unsigned int)_bias)*_hiddenX) +
                             (_outputs + (unsigned int)_bias);
    if(genomsize != genom.size())
    {
        std::string error = "geonomsize is wrong\n";
                    error+= "genomsize is: "+std::to_string(genom.size()) + " but the Net has only place for an amount of: "+ std::to_string((unsigned int)genomsize) + " weights.\n";
                    error+= "Check your net configuration:\n";
                    error+= "\tInput Neurons Y:\t"+std::to_string(_inputs)+"\n";
                    error+= "\tHiddenNeurons X:\t"+std::to_string(_hiddenX)+"\n";
                    error+= "\tHiddenNeurons Y:\t"+std::to_string(_hiddenY)+"\n";
                    error+= "\tOutputNeurons Y:\t"+std::to_string(_outputs)+"\n";
                    error+= "\tBias"+ std::to_string(_bias);
        error_general("addGenom(std::vector<float>)",error);
    }
    _genomList.push_back(genom);
}
void SaveNet::addGenom(std::vector<std::vector<float>   > genomList)
{
    for(unsigned int a=0; a<genomList.size(); a++)
    {
        try {
            addGenom(genomList[a]);
        } catch (std::runtime_error &e) {
            error_general("addGenom(std::vector<std::vecor<float>   >)","addGenom(genomList["+std::to_string(a)+"])",e);
        }
    }
}
std::vector<float>  SaveNet::genom(unsigned int index)
{

}
std::vector<std::vector<float>  > SaveNet::genom()
{

}


//----------ERROR

std::string SaveNet::error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max)
{
    return " parameter "+std::to_string(paramPos)+" is out of range: "+value+"     minimum is: "+min+"     maximum is: "+max;
}
std::string SaveNet::error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string SaveNet::error_paramOutOfRange(unsigned int paramPos,int value,int min, int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string SaveNet::error_paramOutOfRange(unsigned int paramPos,float value,float min, float max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
void        SaveNet::error_general(std::string function, std::string cause)
{
    throw std::runtime_error("ERROR: SaveNet::" + function + "     " + cause);
}
void        SaveNet::error_general(std::string function, std::runtime_error &e)
{
    error_general(function,"",e);
}
void        SaveNet::error_general(std::string function, std::string cause, std::runtime_error &e)
{
    std::string error = "ERROR: SaveNet::" + function + "     " + cause;
    error += "     --> "+std::string(e.what());
    throw std::runtime_error(error);
}
