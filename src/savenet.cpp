#include <savenet.h>


SaveNet::SaveNet()
{
    filename("netFile");
    fileEnding("net");
    _saves = 0;
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
void            SaveNet::inputNeurons(unsigned int inputs)
{
    _inputs  = inputs;
    _check_inputs = true;
    clearGenomList();
}
unsigned int    SaveNet::inputNeurons()
{
    return _inputs;
}
void            SaveNet::hiddenNeuronsX(unsigned int hiddenX)
{
    _hiddenX = hiddenX;
    _check_hiddenX = true;
    clearGenomList();
}
unsigned int    SaveNet::hiddenNeuronsX()
{
    return _hiddenX;
}
void            SaveNet::hiddenNeuronsY(unsigned int hiddenY)
{
    _hiddenY = hiddenY;
    _check_hiddenY = true;
    clearGenomList();
}
unsigned int    SaveNet::hiddenNeuronsY()
{
    return _hiddenY;
}
void            SaveNet::outputNeurons(unsigned int outputs)
{
    _outputs = outputs;
    _check_outputs = true;
    clearGenomList();
}
unsigned int    SaveNet::outputNeurons()
{
    return _outputs;
}
void            SaveNet::bias(bool bias)
{
    _bias = bias;
    _check_bias = true;
    clearGenomList();
}
bool            SaveNet::bias()
{
    return _bias;
}
void            SaveNet::biasValue(float value)
{
    _biasValue = value;
    _check_biasValue = true;
    clearGenomList();
}
float           SaveNet::biasValue()
{
    return _biasValue;
}
void            SaveNet::enableAverage(bool average)
{
    _average = average;
    _check_average = true;
    clearGenomList();
}
bool            SaveNet::enableAverage()
{
    return _average;
}
void            SaveNet::activationFunction(Activation func)
{
    _activationFuncton = func;
    _check_activationFunction = true;
    clearGenomList();
}
Activation      SaveNet::activationFunction()
{
    return _activationFuncton;
}
void            SaveNet::setExtraParam(std::string name,float value)
{
    for(unsigned int a=0; a<_extraParamName.size(); a++)
    {
        if(_extraParamName[a] == name)
        {
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
void            SaveNet::setExtraParam(std::vector<std::string> name,std::vector<float> value)
{
    if(name.size() != value.size())
    {
        error_general("setExtraParam(std::vector<std::string> , std::vector<float> )","Vectorsize is not the same");
    }
    for(unsigned int a=0; a<name.size(); a++)
    {
        try {
            setExtraParam(name[a],value[a]);
        } catch (std::runtime_error &e) {
            error_general("setExtraParam(std::vector<std::string> , std::vector<float> )","addExtraParam("+name[a]+","+std::to_string(value[a])+")",e);
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
    if(_filename == "")
    {
        error_general("loadFile()","no filename declared");
    }
    _file = fopen((_filename+"."+_fileEnding).c_str(),"r");
    if(!_file)
    {
        error_general("loadFile()","can't open file: \""+_filename+"."+_fileEnding+"\"");
    }
    this->clear();
    std::vector<std::string> fileBuffer;
    std::string tmpBuffer = "";
    unsigned int tmpAnimals = 0;
    while(tmpBuffer.find("[GENLIST]") == -1)
    {
        char list[255];
        fgets(list,255,_file);
        tmpBuffer = list;
        fileBuffer.push_back(list);
    }
    for(unsigned int a=0; a<fileBuffer.size(); a++)
    {
        if(fileBuffer[a].find("SAVES") == 0)
        {
            _saves = std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str());
        }
        if(fileBuffer[a].find("ANIMALS") == 0)
        {
            tmpAnimals = std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str());
        }
        if(fileBuffer[a].find("__INPUTNEURONS ") == 0)
        {
            inputNeurons(std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
        }
        if(fileBuffer[a].find("__HIDDENNEURONSX ") == 0)
        {
            hiddenNeuronsX(std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
        }
        if(fileBuffer[a].find("__HIDDENNEURONSY ") == 0)
        {
            hiddenNeuronsY(std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
        }
        if(fileBuffer[a].find("__OUTPUTNEURONS ") == 0)
        {
            outputNeurons(std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
        }
        if(fileBuffer[a].find("__BIAS ") == 0)
        {
            bias(std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
        }
        if(fileBuffer[a].find("__AVERAGE ") == 0)
        {
            enableAverage(std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
        }
        if(fileBuffer[a].find("__BIASVALUE ") == 0)
        {
            biasValue(std::stof(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
        }
        if(fileBuffer[a].find("__ACTIVATIONFUNCTION ") == 0)
        {
            activationFunction((Activation)std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
        }
        if(fileBuffer[a].find("[P] ") == 0)
        {
            fileBuffer[a] = fileBuffer[a].substr(4);
            std::string name = fileBuffer[a].substr(0,fileBuffer[a].find(" "));
            fileBuffer[a] = fileBuffer[a].substr(fileBuffer[a].find(" ")+1);
            float value = std::stof(fileBuffer[a].substr(0,fileBuffer[a].find("\n")).c_str());
            setExtraParam(name,value);
        }
    }
    this->genomsize();

    for(unsigned int y=0; y<tmpAnimals; y++)
    {
        std::vector<float>  addGen;

        char  val[255];
        std::string val2;
        fscanf(_file,"%s",val);
        val2 = val;
        if(val2.find("[G]") == -1)
        {
          //  qDebug() << "ERROR";
        }
        for(unsigned int x=0; x<_genomsize; x++)
        {
            fscanf(_file,"%s",val);
            val2 = val;
            if(val2.find("[G]") != -1 || feof(_file))
            {
                break;
            }
            addGen.push_back(stof(val2));
        }
        this->addGenom(addGen);
    }
    fclose(_file);
}
void SaveNet::loadFile(std::string filename)
{
    this->filename(filename);
    loadFile();
}
void SaveNet::saveFile()
{
    if(_filename == "")
    {
        error_general("saveFile()","no filename declared");
    }
    checkParam();
    if(_genomList.size() == 0)
    {
        error_general("saveFile()","no genome defined");
    }
    _file = fopen((_filename+"."+_fileEnding).c_str(),"w");
    if(!_file)
    {
        error_general("saveFile()","can't create file: \""+_filename+"."+_fileEnding+"\"");
    }

    time_t timer;
    time(&timer);
    struct tm *ti = localtime(&timer);

    _saves++;
    //--------------------SAVE INFO---------------------
    fprintf(_file,"SAVENET_VERSION: %s\t",SAVENET_VERSION);
    fprintf(_file,"DATE: %i.%i.%i\tTIME: %i:%i:%i\n",ti->tm_mday,ti->tm_mon,ti->tm_year+1900,ti->tm_hour,ti->tm_min,ti->tm_sec);
    fprintf(_file,"SAVES: %i\n",this->_saves);
    fprintf(_file,"ANIMALS: %i\n\n",this->animals());
    //--------------------SAVE NET CONFIG---------------------
    fprintf(_file,"__INPUTNEURONS %i\n",this->_inputs);
    fprintf(_file,"__HIDDENNEURONSX %i\n",this->_hiddenX);
    fprintf(_file,"__HIDDENNEURONSY %i\n",this->_hiddenY);
    fprintf(_file,"__OUTPUTNEURONS %i\n",this->_outputs);
    fprintf(_file,"__BIAS %i\n",(unsigned int)this->_bias);
    fprintf(_file,"__AVERAGE %i\n",(unsigned int)this->_average);
    fprintf(_file,"__BIASVALUE %.8f\n",this->_biasValue);
    fprintf(_file,"__ACTIVATIONFUNCTION %i\n",(unsigned int)this->_activationFuncton);
    //--------------------------------------------------------
    fprintf(_file,"\n");
    fprintf(_file,"[PARAM]\n");
    for(unsigned int a=0; a<_extraParamName.size(); a++)
    {
        fprintf(_file,"[P] %s %f\n",_extraParamName[a].c_str(),_extraParamValue[a]);
    }

    fprintf(_file,"\n");
    fprintf(_file,"[GENLIST]\n");

    for(unsigned int y=0; y<_genomList.size(); y++)
    {
        fprintf(_file,"[G] ");
        for(unsigned int x=0; x<_genomList[y].size(); x++)
        {
            fprintf(_file,"%.8f ",_genomList[y][x]);
        }
        fprintf(_file,"\n");
    }

    fclose(_file);

}
void SaveNet::saveFile(std::string filename)
{
    this->filename(filename);
    saveFile();
}
void SaveNet::setGenom(std::vector<float>   genom)
{
    checkParam();
    this->clearGenomList();
    this->addGenom(genom);
}
void SaveNet::setGenom(unsigned int index,std::vector<float>   genom)
{
    if(_genomList.size() == 0)
    {
        error_general("setGenom(unsigned int ["+std::to_string(index)+"] , std::vector<float> )","no genome defined");
    }
    if(index >= _genomList.size())
    {
        error_general("setGenom(unsigned int ["+std::to_string(index)+"] , std::vector<float> )",error_paramOutOfRange((unsigned int)0,index,(unsigned int)0,_genomList.size()-1));
    }
    if(genom.size() != genomsize())
    {
        error_general("setGenom(unsigned int ["+std::to_string(index)+"] , std::vector<float> )",error_paramOutOfRange((unsigned int)1,genom.size(),_genomsize,_genomsize));
    }
    _genomList[index] = genom;
}
void SaveNet::setGenom(std::vector<std::vector<float>   > genomList)
{
    qDebug() << "size: "<< genomList.size();
    if(genomList.size() == 0)
    {
        error_general("setGenom(std::vector<float> )","no genomes in parameter 0");
    }
    clearGenomList();
    try {
        addGenom(genomList);
    } catch (std::runtime_error &e) {
        error_general("setGenom(std::vector<float> )","addGenom(genomList)",e);
    }
}
void SaveNet::addGenom(std::vector<float>   genom)
{
    checkParam();
    if(this->genomsize() != genom.size())
    {
        std::string error = "genomsize is wrong\n";
                    error+= "genomsize is: "+std::to_string(genom.size()) + " but the Net has only place for an amount of: "+ std::to_string(_genomsize) + " weights.\n";
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
    if(index >= _genomList.size())
    {
        if(_genomList.size() == 0)
        {
            error_general("genom(unsigned int ["+std::to_string(index)+"])","no genom in buffer");
        }
        error_general("genom(unsigned int ["+std::to_string(index)+"])",error_paramOutOfRange((unsigned int)0,index,(unsigned int)0,_genomList.size()-1));
    }
    return _genomList[index];
}
std::vector<std::vector<float>  > SaveNet::genom()
{
    return _genomList;
}
unsigned int SaveNet::animals()
{
    return _genomList.size();
}
void SaveNet::clear()
{
    set(0,0,0,0,false,false,Activation::Binary,0);
    _check_inputs               = false;
    _check_hiddenX              = false;
    _check_hiddenY              = false;
    _check_outputs              = false;
    _check_bias                 = false;
    _check_biasValue            = false;
    _check_average              = false;
    _check_activationFunction   = false;
    clearGenomList();
    clearExternParam();
}
void SaveNet::set(unsigned int inputs,
         unsigned int hiddenX,
         unsigned int hiddenY,
         unsigned int outputs,
         bool         bias,
         bool         average,
         Activation   activationFunction,
         float        biasValue)
{
    this->inputNeurons(inputs);
    this->hiddenNeuronsX(hiddenX);
    this->hiddenNeuronsY(hiddenY);
    this->outputNeurons(outputs);
    this->bias(bias);
    this->enableAverage(average);
    this->biasValue(biasValue);
    this->activationFunction(activationFunction);
}
void SaveNet::checkParam()
{
    if(!_check_inputs)
    {
        error_general("checkParam()","Param \"inputNeurons\" is not defined");
    }
    if(!_check_hiddenX)
    {
        error_general("checkParam()","Param \"hiddenNeuronsX\" is not defined");
    }
    if(!_check_hiddenY)
    {
        error_general("checkParam()","Param \"hiddenNeuronsY\" is not defined");
    }
    if(!_check_outputs)
    {
        error_general("checkParam()","Param \"outputNeurons\" is not defined");
    }
    if(!_check_bias)
    {
        error_general("checkParam()","Param \"bias\" is not defined");
    }
    if(!_check_biasValue)
    {
        error_general("checkParam()","Param \"biasValue\" is not defined");
    }
    if(!_check_average)
    {
        error_general("checkParam()","Param \"enableAverage\" is not defined");
    }
    if(!_check_activationFunction)
    {
        error_general("checkParam()","Param \"activationFunction\" is not defined");
    }
}
unsigned int SaveNet::genomsize()
{
    if(_hiddenX == 0 || _hiddenY == 0)
    {
        _genomsize = (_inputs+(unsigned int)_bias) * _outputs;
    }
    else
    {
        _genomsize = (_inputs+(unsigned int)_bias) * _hiddenY + (_hiddenY+(unsigned int)_bias) * _hiddenY * (_hiddenX-1) + (_hiddenY+(unsigned int)_bias) * _outputs;
    }
    return _genomsize;
}
void SaveNet::clearGenomList()
{
    _genomList = std::vector<std::vector<float> >();
}
void SaveNet::clearExternParam()
{
    _extraParamName = std::vector<std::string> ();
    _extraParamValue = std::vector<float> ();
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
