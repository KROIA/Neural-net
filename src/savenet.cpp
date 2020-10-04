#include <savenet.h>


SaveNet::SaveNet()
{
    set_filename("netFile");
    set_fileEnding("net");
    _saves = 0;
    _genomsize = 0;
    this->removeAllNets();
}
SaveNet::~SaveNet()
{
    _neuronList.clear();
    _extraParamName.clear();
    _extraParamValue.clear();
    _genomList.clear();
    _ID_list.clear();
}

void            SaveNet::set_filename(std::string filename)
{
    _filename = filename;
}
std::string         SaveNet::get_filename()
{
    return _filename;
}
void            SaveNet::set_fileEnding(std::string fileEnding)
{
    _fileEnding = fileEnding;
}
std::string         SaveNet::get_fileEnding()
{
    return _fileEnding;
}
void            SaveNet::set_inputNeurons(unsigned int inputs)
{
    _inputs  = inputs;
    _check_inputs = true;
    clearGenomList();
}
unsigned int    SaveNet::get_inputNeurons()
{
    return _inputs;
}
void            SaveNet::set_hiddenNeuronsX(unsigned int hiddenX)
{
    _hiddenX = hiddenX;
    _check_hiddenX = true;
    clearGenomList();
}
unsigned int    SaveNet::get_hiddenNeuronsX()
{
    return _hiddenX;
}
void            SaveNet::set_hiddenNeuronsY(unsigned int hiddenY)
{
    _hiddenY = hiddenY;
    _check_hiddenY = true;
    clearGenomList();
}
unsigned int    SaveNet::get_hiddenNeuronsY()
{
    return _hiddenY;
}
void            SaveNet::set_outputNeurons(unsigned int outputs)
{
    _outputs = outputs;
    _check_outputs = true;
    clearGenomList();
}
unsigned int    SaveNet::get_outputNeurons()
{
    return _outputs;
}
unsigned int    SaveNet::get_neurons()
{
    return _neurons;
}
unsigned int    SaveNet::get_hiddenNeurons()
{
    return _hiddenNeurons;
}
unsigned int    SaveNet::get_costumNeurons()
{
    return _costumNeurons;
}
unsigned int    SaveNet::get_connections()
{
    return _connections;
}
unsigned int    SaveNet::get_costumConnections()
{
    return _costumConnections;
}
void            SaveNet::set_bias(bool bias)
{
    _bias = bias;
    _check_bias = true;
    clearGenomList();
}
bool            SaveNet::get_bias()
{
    return _bias;
}
void            SaveNet::set_biasValue(double value)
{
    _biasValue = value;
    _check_biasValue = true;
    clearGenomList();
}
double          SaveNet::get_biasValue()
{
    return _biasValue;
}
void            SaveNet::set_enableAverage(bool average)
{
    _average = average;
    _check_average = true;
    clearGenomList();
}
bool            SaveNet::get_enableAverage()
{
    return _average;
}
void            SaveNet::set_activationFunction(Activation func)
{
    _activationFuncton = func;
    _check_activationFunction = true;
    clearGenomList();
}
Activation      SaveNet::get_activationFunction()
{
    return _activationFuncton;
}
void            SaveNet::set_ExtraParam(std::string name,double value)
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
void            SaveNet::get_ExtraParam(std::string name, double &value)
{
    for(unsigned int a=0; a<_extraParamName.size(); a++)
    {
        if(_extraParamName[a] == name)
        {
            value = _extraParamValue[a];
            return;
        }
    }
    addError(Error("get_ExtraParam(std::string ["+name+"] , double &["+std::to_string(value)+"] )",
                   std::string("Unknown parameter: "+name)));
}
void            SaveNet::set_ExtraParam(std::vector<std::string> name,std::vector<double> value)
{
    if(name.size() != value.size())
    {
        addError(Error("set_ExtraParam(std::vector<std::string> name , std::vector<double> value )",
                       std::string("name.size() != value.size()")));
        return;
    }
    for(unsigned int a=0; a<name.size(); a++)
    {
        set_ExtraParam(name[a],value[a]);
    }
}
void            SaveNet::get_ExtraParam(std::vector<std::string> &name,std::vector<double> &value)
{
    name    = _extraParamName;
    value   = _extraParamValue;
}


bool SaveNet::loadFile()
{
    if(_filename == "")
    {
        addError(Error("loadFile())",
                       std::string("No filename declared")));
        return false;
    }
    _file = fopen((_filename+"."+_fileEnding).c_str(),"r");
    if(!_file)
    {
        addError(Error("loadFile())",
                       std::string("can't open file: \""+_filename+"."+_fileEnding+"\"")));
        return false;
    }
    this->removeAllNets();
    std::vector<std::string> fileBuffer;
    std::string tmpBuffer = "";
    unsigned int tmpAnimals = 0;
    while(tmpBuffer.find("[NETS]") == std::string::npos)
    {
        char list[255];
        fgets(list,255,_file);
        tmpBuffer = list;
        fileBuffer.push_back(list);
    }
    for(unsigned int a=0; a<fileBuffer.size(); a++)
    {
        std::string buffLine = fileBuffer[a].substr(fileBuffer[a].find_last_of(" ")+1,fileBuffer[a].find("\n"));
        //qDebug() << buffLine.c_str();
        unsigned int uInt_buffLine;
        if(buffLine == "" || fileBuffer[a].find(" ") == std::string::npos)
            continue;
        try {
            uInt_buffLine = stoul(buffLine);
        } catch (std::invalid_argument &e) {
            CONSOLE << e.what();

        }

        if(fileBuffer[a].find("SAVES") == 0)
        {
            _saves = uInt_buffLine;
            continue;
        }else if(fileBuffer[a].find("ANIMALS ") == 0)
        {
            tmpAnimals = uInt_buffLine;
            continue;
        }else if(fileBuffer[a].find("__INPUTNEURONS ") == 0)
        {
            set_inputNeurons(uInt_buffLine);
            continue;
        }else if(fileBuffer[a].find("__HIDDENNEURONSX ") == 0)
        {
            set_hiddenNeuronsX(uInt_buffLine);
            continue;
        }else if(fileBuffer[a].find("__HIDDENNEURONSY ") == 0)
        {
            set_hiddenNeuronsY(uInt_buffLine);
            continue;
        }else if(fileBuffer[a].find("__OUTPUTNEURONS ") == 0)
        {
            set_outputNeurons(uInt_buffLine);
            continue;
        }else if(fileBuffer[a].find("__BIAS ") == 0)
        {
            set_bias(uInt_buffLine);
            continue;
        }else if(fileBuffer[a].find("__AVERAGE ") == 0)
        {
            set_enableAverage(uInt_buffLine);
            continue;
        }else if(fileBuffer[a].find("__BIASVALUE ") == 0)
        {
            set_biasValue(stod(buffLine));
            continue;
        }else if(fileBuffer[a].find("__ACTIVATIONFUNCTION ") == 0)
        {
            set_activationFunction(static_cast<Activation>(uInt_buffLine));
            continue;
        }else if(fileBuffer[a].find("__NEURONS ") == 0)
        {
            _neurons = uInt_buffLine;
            continue;
        }else if(fileBuffer[a].find(" __HIDDEN_NEURONS ") == 0)
        {
            fileBuffer[a] = fileBuffer[a].substr(1);
            _hiddenNeurons = stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")));
            continue;
        }else if(fileBuffer[a].find(" __OUTPUT_NEURONS ") == 0)
        {
            fileBuffer[a] = fileBuffer[a].substr(1);
            _outputNeurons = stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")));
            continue;
        }else if(fileBuffer[a].find(" __COSTUM_NEURONS ") == 0)
        {
            fileBuffer[a] = fileBuffer[a].substr(1);
            _costumNeurons = stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")));
            continue;
        }else if(fileBuffer[a].find("__CONNECTIONS ") == 0)
        {
            _connections = uInt_buffLine;
            continue;
        }else if(fileBuffer[a].find(" __SPECIAL_CONNECTIONS ") == 0)
        {
            fileBuffer[a] = fileBuffer[a].substr(1);
            _costumConnections = stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")));
            continue;
        }else if(fileBuffer[a].find("[P] ") == 0)
        {
            fileBuffer[a]   = fileBuffer[a].substr(4);
            std::string name    = fileBuffer[a].substr(0,fileBuffer[a].find(" "));
            fileBuffer[a]   = fileBuffer[a].substr(fileBuffer[a].find(" ")+1);
            double value    = stod(fileBuffer[a].substr(0,fileBuffer[a].find("\n")));
            set_ExtraParam(name,value);
        }
    }
    //this->genomsize();

    fileBuffer.clear();
    std::vector<std::vector<std::string>  >connectionBuffer(1);
    const int bufferSize = 100;
    tmpBuffer = "";
    tmpBuffer.resize(bufferSize);
#ifdef _DEBUG_READ_TIMING
    __debug_timer1_start  = std::chrono::high_resolution_clock::now();
    double secCounter    = 0;
#endif
    while(!feof(_file))
    {
        char list[bufferSize];
        if(fgets(list,bufferSize,_file) == nullptr)
        {
            break;
        }
        connectionBuffer[connectionBuffer.size()-1].push_back(list);
        if(std::string(list).find("\n") != std::string::npos)
        {
            //fileBuffer.push_back(tmpBuffer);
            connectionBuffer.push_back(std::vector<std::string>());
            tmpBuffer = "";
        }
#ifdef _DEBUG_READ_TIMING
        __debug_timer1_end  = std::chrono::high_resolution_clock::now();
        __debug_time_span   = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer1_end - __debug_timer1_start);
        if(__debug_time_span.count() >= debugInterval)//sec
        {
            secCounter+=debugInterval;
            __debug_timer1_start = __debug_timer1_end;
            CONSOLE<< "SaveNet::loadFile() Reading File... Time elapsed: "<<secCounter<<" sec";
        }
#endif
    }
    connectionBuffer.erase(connectionBuffer.end());

    fclose(_file);

    enum ReadPos{
        nothing = 0,
        NeuronID = 1,
        Inputs = 2,
        Connections = 3,
    };


    ReadPos readPos = ReadPos::nothing;


    struct NeuronID    neuronID;
    unsigned int netID = 0;
    unsigned int inputs = 0;
    unsigned int inputCounter = 0;
    _connectionListFromFile.reserve(tmpAnimals);
#ifdef _DEBUG_READ_TIMING
    __debug_timer1_start  = std::chrono::high_resolution_clock::now();
    secCounter = 0;
#endif
    for(unsigned int a=0; a<connectionBuffer.size(); a++)
    {
        if(a == 50)
        {
            a=50;
        }
        std::string tempBuff = "";
        size_t largeBufferIndex = 0;
        size_t smallBufferSize = 20;
        unsigned int connectionListIndex = 0;
        bool endOfList = false;
        while(tempBuff != "\n")
        {

            if(tempBuff.size() < smallBufferSize && !endOfList)
            {
                if(largeBufferIndex > connectionBuffer[a][connectionListIndex].size()-smallBufferSize)
                {
                    tempBuff.append(connectionBuffer[a][connectionListIndex].substr(largeBufferIndex,smallBufferSize));
                    connectionListIndex++;
                    largeBufferIndex = 0;
                    if(connectionBuffer[a].size()>connectionListIndex)
                    {

                        tempBuff.append(connectionBuffer[a][connectionListIndex].substr(largeBufferIndex,smallBufferSize));
                        largeBufferIndex += smallBufferSize;
                    }else {
                        endOfList = true;
                    }
                }else{
                    tempBuff.append(connectionBuffer[a][connectionListIndex].substr(largeBufferIndex,smallBufferSize));
                    largeBufferIndex += smallBufferSize;
                }
            }
            if(tempBuff == "")
                break;
            if(tempBuff.find("N") == 0)
            {
                netID = stoul(tempBuff.substr(tempBuff.find("N")+1,tempBuff.find("D")-(tempBuff.find("N")+1)));
                tempBuff = tempBuff.substr(tempBuff.find("D"),tempBuff.size());
                _connectionListFromFile.push_back(std::vector<Connection>());

            }else if(tempBuff.find("D") == 0)
            {
                readPos   = ReadPos::NeuronID;
            }else if(tempBuff.find("I") == 0)
            {
                readPos   = ReadPos::Inputs;
            }else if(tempBuff.find("C") == 0)
            {
                readPos   = ReadPos::Connections;
                _connectionListFromFile[_connectionListFromFile.size()-1].reserve(_connections);
            }

            switch(readPos)
            {
                case ReadPos::NeuronID:
                {
                    if(tempBuff.find("D") == 0)
                    {
                        neuronID.ID = stoul(tempBuff.substr(tempBuff.find("D")+1,tempBuff.find("T")-(tempBuff.find("D")+1)));
                        neuronID.TYPE = static_cast<NeuronType>(stoul(tempBuff.substr(tempBuff.find("T")+1,tempBuff.find("I")-(tempBuff.find("T")+1))));
                        tempBuff = tempBuff.substr(tempBuff.find("I"),tempBuff.size());
                        readPos = ReadPos::nothing;
                    }
                    break;
                }
                case ReadPos::Inputs:
                {
                    //ptr_tmpNeuron->inputs(atoi(fileBuffer[a+2].substr(fileBuffer[a+2].find("<")+1,fileBuffer[a+2].find(">")).c_str()));
                    inputs = stoul(tempBuff.substr(tempBuff.find("I")+1,tempBuff.find("C")-(tempBuff.find("I")+1)));
                    tempBuff = tempBuff.substr(tempBuff.find("C"),tempBuff.size());
                    readPos = ReadPos::nothing;
                    break;
                }
                case ReadPos::Connections:
                {
                    _connectionListFromFile[_connectionListFromFile.size()-1].push_back(Connection());
                    inputCounter++;
                    _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].netID = netID;
                    _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].destination_ID = neuronID;
                    _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].source_ID.ID = stoul(tempBuff.substr(tempBuff.find("i")+1,tempBuff.find("t")-(tempBuff.find("i")+1)));
                    tempBuff = tempBuff.substr(tempBuff.find("i")+1,tempBuff.size());
                    _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].source_ID.TYPE = static_cast<NeuronType>(stoul(tempBuff.substr(tempBuff.find("t")+1,tempBuff.find("w")-(tempBuff.find("t")+1))));
                    _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].weight = stod(tempBuff.substr(tempBuff.find("w")+1,tempBuff.find("d")-(tempBuff.find("w")+1)));
                    _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].direction = static_cast<ConnectionDirection>(stoul(tempBuff.substr(tempBuff.find("d")+1,tempBuff.find("i")-(tempBuff.find("d")+1))));
                    tempBuff = tempBuff.substr(tempBuff.find("i"),tempBuff.size());
                    if(inputCounter == inputs)
                    {
                        inputCounter = 0;
                        readPos = ReadPos::nothing;
                        tempBuff = tempBuff.substr(tempBuff.find("|")+1,tempBuff.size());
                    }
                    break;
                }
                default:
                {

                }
            }
#ifdef _DEBUG_READ_TIMING
            __debug_timer1_end  = std::chrono::high_resolution_clock::now();
            __debug_time_span   = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer1_end - __debug_timer1_start);
            if(__debug_time_span.count() >= debugInterval)//sec
            {
                secCounter+=debugInterval;
                __debug_timer1_start = __debug_timer1_end;
                CONSOLE<< "SaveNet::loadFile() Processing data... "<< a+1 << "of:"<<_connectionListFromFile.size() <<"\tConnections: "<< _connectionListFromFile[a].size()<<"\t"<<int(double(_connectionListFromFile[a].size())/double(_connections)*100)<<"%\tTime elapsed: "<<secCounter<<"sec";
            }
#endif
        }
        _animals = static_cast<unsigned int>(_connectionListFromFile.size());
    }
    CONSOLE<< "read finish";
    CONSOLE<< "nets: "<<_connectionListFromFile.size() << " connections: "<<_connectionListFromFile[0].size();
    inputCounter = 0;
    return true;
}
bool SaveNet::loadFile(std::string filename)
{
    this->set_filename(filename);
    return loadFile();
}
bool SaveNet::saveFile()
{
    if(_filename == "")
    {
        addError(Error("saveFile())",
                       std::string("No filename declared")));
        return false;
        //error_general("saveFile()","no filename declared");
    }
    checkParam();
    if(_neuronList.size() == 0)
    {
        addError(Error("saveFile())",
                       std::string("No nets defined")));
        return false;
     //   error_general("saveFile()","No nets defined");
    }
    _file = fopen((_filename+"."+_fileEnding).c_str(),"w");
    if(!_file)
    {
        addError(Error("saveFile())",
                       std::string("can't create file: \""+_filename+"."+_fileEnding+"\"")));
        return false;
      //  error_general("saveFile()","can't create file: \""+_filename+"."+_fileEnding+"\"");
    }

    time_t timer;
    time(&timer);
    struct tm *ti = localtime(&timer);

    _saves++;
    //--------------------SAVE INFO---------------------
    fprintf(_file,"SAVENET_VERSION: %s\t",SAVENET_VERSION);
    fprintf(_file,"DATE: %i.%i.%i\tTIME: %i:%i:%i\n",ti->tm_mday,ti->tm_mon+1,ti->tm_year+1900,ti->tm_hour,ti->tm_min,ti->tm_sec);
    fprintf(_file,"SAVES: %i\n",this->_saves);
    fprintf(_file,"ANIMALS: %i\n\n",this->get_animals());
    //--------------------SAVE NET CONFIG---------------------
    fprintf(_file,"__INPUTNEURONS %i\n",this->_inputs);
    fprintf(_file,"__HIDDENNEURONSX %i\n",this->_hiddenX);
    fprintf(_file,"__HIDDENNEURONSY %i\n",this->_hiddenY);
    fprintf(_file,"__OUTPUTNEURONS %i\n",this->_outputs);
    fprintf(_file,"__BIAS %i\n",this->_bias);
    fprintf(_file,"__AVERAGE %i\n",this->_average);
    fprintf(_file,"__BIASVALUE %.8f\n",this->_biasValue);
    fprintf(_file,"__ACTIVATIONFUNCTION %i\n",this->_activationFuncton);
    fprintf(_file,"__NEURONS %i\n",this->_neurons);
    fprintf(_file," __HIDDEN_NEURONS %i\n",this->_hiddenNeurons);
    fprintf(_file," __OUTPUT_NEURONS %i\n",this->_outputNeurons);
    fprintf(_file," __COSTUM_NEURONS %i\n",this->_costumNeurons);

    fprintf(_file,"__CONNECTIONS %i\n",this->_connections);
    fprintf(_file," __SPECIAL_CONNECTIONS %i\n",this->_costumConnections);
    //--------------------------------------------------------
    fprintf(_file,"\n");
    fprintf(_file,"[PARAM]\n");
    for(unsigned int a=0; a<_extraParamName.size(); a++)
    {
        fprintf(_file,"[P] %s %f\n",_extraParamName[a].c_str(),_extraParamValue[a]);
    }

    fprintf(_file,"\n");
    fprintf(_file,"[NETS]\n");
#ifdef _DEBUG_READ_TIMING
    __debug_timer1_start  = std::chrono::high_resolution_clock::now();
    double secCounter     = 0;
#endif

    for(unsigned int net=0; net<_neuronList.size(); net++)
    {
        //[NET] Net ID
        //fprintf(_file,"[NET]<%i>\n{\n",_ID_list[net]);      //save net ID
        fprintf(_file,"N%i",_ID_list[net]);      //save net ID

        for(unsigned int neuron=0; neuron<_neuronList[net].size(); neuron++)
        {
            //[NeuronID]
            //[ID]          -> ID-value
            //[NeuronType]  -> TYPE
            /*fprintf(_file,"\t{\n");
            fprintf(_file,"\t\t[NeuronID]\n");
            fprintf(_file,"\t\t{\n");
            fprintf(_file,"\t\t\t[ID]<%i>\n",_neuronList[net][neuron]->ID().ID);
            fprintf(_file,"\t\t\t[NeuronType]<%i>\"%s\"\n",_neuronList[net][neuron]->ID().TYPE,Neuron::typeString(_neuronList[net][neuron]->ID().TYPE).c_str());
            fprintf(_file,"\t\t}\n");*/
            fprintf(_file,"D%i",_neuronList[net][neuron]->get_ID().ID);
            fprintf(_file,"T%i",_neuronList[net][neuron]->get_ID().TYPE);

            //[Inputs]      -> amount of inputs of the Neuron
            /*fprintf(_file,"\t\t[Inputs]<%i>\n",_neuronList[net][neuron]->inputs());
            fprintf(_file,"\t\t[Connections]\n\t\t{\n",_neuronList[net][neuron]->inputs());*/
            fprintf(_file,"I%i",_neuronList[net][neuron]->get_inputs());
            fprintf(_file,"C%i",_neuronList[net][neuron]->get_inputs());
            for(unsigned int input=0; input<_neuronList[net][neuron]->get_inputs(); input++)
            {
                // connectionAddress to the following weight, has Parameter of struct NeuronID
                /*fprintf(_file,"\t\t\t{\n");
                fprintf(_file,"\t\t\t\t[ID]<%i>\n",_neuronList[net][neuron]->inputID()[input].ID);
                fprintf(_file,"\t\t\t\t[NeuronType]<%i>\"%s\"\n",_neuronList[net][neuron]->inputID()[input].TYPE,Neuron::typeString(_neuronList[net][neuron]->inputID()[input].TYPE).c_str());
                //[w]   -> weight
                fprintf(_file,"\t\t\t\t[w]<%.8f>\n",(double)_neuronList[net][neuron]->weight(input));
                fprintf(_file,"\t\t\t\t[dir]<%i>\n",_neuronList[net][neuron]->inputConnectionDirection(_neuronList[net][neuron]->inputID(input)));
                fprintf(_file,"\t\t\t}\n");*/
                fprintf(_file,"i%i",_neuronList[net][neuron]->get_inputID(input).ID);
                fprintf(_file,"t%i",_neuronList[net][neuron]->get_inputID(input).TYPE);
                //[w]   -> weight
                fprintf(_file,"w%.4f",_neuronList[net][neuron]->get_weight(input));
                fprintf(_file,"d%i",_neuronList[net][neuron]->get_inputConnectionDirection(_neuronList[net][neuron]->get_inputID(input)));
            }
            /*fprintf(_file,"\t\t}\n");
            fprintf(_file,"\t}\n");*/
            fprintf(_file,"i|");
#ifdef _DEBUG_READ_TIMING
            __debug_timer1_end  = std::chrono::high_resolution_clock::now();
            __debug_time_span   = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer1_end - __debug_timer1_start);
            if(__debug_time_span.count() >= debugInterval)//sec
            {
                secCounter+=debugInterval;
                __debug_timer1_start = __debug_timer1_end;
                CONSOLE<< "SaveNet::saveFile() Saving data... Net: "<< net << "of:"<<_neuronList.size() <<"\tNeuron: "<<neuron<<"of:"<<_neuronList[net].size() <<"\tTime elapsed: "<<secCounter<<"sec";
            }
#endif
        }
        //fprintf(_file,"}\n");
        fprintf(_file,"\n");
    }

    fclose(_file);
    return true;
}
bool SaveNet::saveFile(std::string filename)
{
    this->set_filename(filename);
    return saveFile();
}
/*void SaveNet::setGenom(std::vector<double>   genom)
{
    checkParam();
    this->clearGenomList();
    this->addGenom(genom);
}*/
/*void SaveNet::setGenom(unsigned int index,std::vector<double>   genom)
{
    if(_genomList.size() == 0)
    {
        error_general("setGenom(unsigned int ["+std::to_string(index)+"] , std::vector<double> )","no genome defined");
    }
    if(index >= _genomList.size())
    {
        error_general("setGenom(unsigned int ["+std::to_string(index)+"] , std::vector<double> )",error_paramOutOfRange((unsigned int)0,index,(unsigned int)0,_genomList.size()-1));
    }
    if(genom.size() != genomsize())
    {
        error_general("setGenom(unsigned int ["+std::to_string(index)+"] , std::vector<double> )",error_paramOutOfRange((unsigned int)1,genom.size(),_genomsize,_genomsize));
    }
    _genomList[index] = genom;
}*/
/*void SaveNet::setGenom(std::vector<std::vector<double>   > genomList)
{
    if(genomList.size() == 0)
    {
        error_general("setGenom(std::vector<double> )","no genomes in parameter 0");
    }
    clearGenomList();
    try {
        addGenom(genomList);
    } catch (std::runtime_error &e) {
        error_general("setGenom(std::vector<double> )","addGenom(genomList)",e);
    }
}*/
/*void SaveNet::addGenom(std::vector<double>   genom)
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
        //error_general("addGenom(std::vector<double>)",error);
        CONSOLE<< "WARNING: "<< std::string::fromStdString(error);        //only to test the aditional connections
    }
    _genomList.push_back(genom);
}*/
/*void SaveNet::addGenom(std::vector<std::vector<double>   > genomList)
{
    for(unsigned int a=0; a<genomList.size(); a++)
    {
        try {
            addGenom(genomList[a]);
        } catch (std::runtime_error &e) {
            error_general("addGenom(std::vector<std::vecor<double>   >)","addGenom(genomList["+std::to_string(a)+"])",e);
        }
    }
}*/
std::vector<double>           SaveNet::get_genom(unsigned int ID)
{
   /* if(index >= _genomList.size())
    {
        if(_genomList.size() == 0)
        {
            error_general("genom(unsigned int ["+std::to_string(index)+"])","no genom in buffer");
        }
        error_general("genom(unsigned int ["+std::to_string(index)+"])",error_paramOutOfRange((unsigned int)0,index,(unsigned int)0,_genomList.size()-1));
    }
    return _genomList[index];*/
    bool exists = false;
    unsigned int index = 0;
    for(unsigned int a=0; a<_ID_list.size(); a++)
    {
        if(_ID_list[a] == ID)
        {
            exists = true;
            index  = a;
            break;
        }
    }
    if(exists)
    {
        return _genomList[index];
    }
    else {
        addError(Error("get_genom(unsigned int ["+std::to_string(ID)+"] )",
                       std::string("No net with such an ID")));
        return std::vector<double>();
        //error_general("genom(unsigned int ["+std::to_string(ID)+"])","No net with such an ID");
    }
}
std::vector<std::vector<double>  >  SaveNet::get_genom()
{
    return _genomList;
}
unsigned int            SaveNet::get_animals()
{
    return _animals;//_genomList.size();
}
void SaveNet::removeAllNets()
{

    _check_inputs               = false;
    _check_hiddenX              = false;
    _check_hiddenY              = false;
    _check_outputs              = false;
    _check_bias                 = false;
    _check_biasValue            = false;
    _check_average              = false;
    _check_activationFunction   = false;
    _check_neurons              = false;
     _check_hiddenNeurons       = false;
     _check_outputNeurons       = false;
     _check_costumNeuron        = false;
    _check_connections          = false;
     _check_costumConnections   = false;

    _neurons                    = 0;
     _hiddenNeurons             = 0;
     _outputNeurons             = 0;
     _costumNeurons             = 0;
    _connections                = 0;
    _animals                    = 0;
    _connectionListFromFile.clear();
    set(0,0,0,0,false,false,Activation::Binary,0);

    _neuronList.clear();
    _ID_list.clear();
    clearGenomList();
    clearExternParam();
}
void SaveNet::removeNet(unsigned int ID)
{
    bool exists = false;
    unsigned int index = 0;
    for(unsigned int a=0; a<_ID_list.size(); a++)
    {
        if(_ID_list[a] == ID)
        {
            exists = true;
            index  = a;
            break;
        }
    }
    if(exists)
    {
        _neuronList.erase(_neuronList.begin()+index);
        _ID_list.erase(_ID_list.begin()+index);
        _genomList.erase(_genomList.begin()+index);
    }
    else {
        addError(Error("removeNet(unsigned int ["+std::to_string(ID)+"] )",
                       std::string("No net with such an ID")));
    }
}
void SaveNet::set(unsigned int inputs,
         unsigned int hiddenX,
         unsigned int hiddenY,
         unsigned int outputs,
         bool         bias,
         bool         average,
         Activation   activationFunction,
         double        biasValue)
{
    this->set_inputNeurons(inputs);
    this->set_hiddenNeuronsX(hiddenX);
    this->set_hiddenNeuronsY(hiddenY);
    this->set_outputNeurons(outputs);
    this->set_bias(bias);
    this->set_enableAverage(average);
    this->set_biasValue(biasValue);
    this->set_activationFunction(activationFunction);
}

void                    SaveNet::set_ptr_neuronsOfNet(unsigned int ID,std::vector<Neuron*> *neurons)
{
    bool exists = false;
    unsigned int index = 0;
    for(unsigned int a=0; a<_ID_list.size(); a++)
    {
        if(_ID_list[a] == ID)
        {
            exists = true;
            index  = static_cast<unsigned int>(a);
            break;
        }
    }
    if(exists)
    {
        if(_neurons == static_cast<unsigned int>(neurons->size()))
        {
            _neuronList[index] = *neurons;
            saveGenomOfNet(index);
        }
        else
        {
            // overwrite the old net with the new netconfiguration
            this->removeNet(ID);
            this->addNet(ID,neurons);
            //addError(Error("set_ptr_neuronsOfNet(unsigned int ["+std::to_string(ID)+"] , std::vector<Neuron*> *neurons)",
            //               std::string("There are already some nets saved which have not the same amount of neurons: "+
            //                        std::to_string(_neurons)+" as parameter 2: "+std::to_string(neurons->size()))));
            //return;
       }
    }
    else
    {
        this->addNet(ID,neurons);
    }
}
std::vector<Neuron*>          SaveNet::get_neuronsOfNet(unsigned int ID)
{
    bool exists = false;
    unsigned int index;
    for(unsigned int a=0; a<_ID_list.size(); a++)
    {
        if(_ID_list[a] == ID)
        {
            exists = true;
            index  = a;
            break;
        }
    }
    if(exists)
    {
        return _neuronList[index];
    }
    else
    {
        addError(Error("get_neuronsOfNet(unsigned int ["+std::to_string(ID)+"] )",
                       std::string("No net with such an ID")));
        return std::vector<Neuron*>();
        //error_general("neuronsOfNet(unsigned int ["+std::to_string(ID)+"])","Net not available. There is no Net with such an ID");
    }
}
void                    SaveNet::addNet(unsigned int ID,std::vector<Neuron*> *neurons)
{
    _neuronList.push_back(*neurons);

    _ID_list.push_back(ID);
    _genomList.push_back(std::vector<double>());

    _animals = static_cast<unsigned int>(_ID_list.size());
    _check_neurons = true;

    _neurons        = static_cast<unsigned int>(neurons->size());
    _hiddenNeurons  = 0;
    _outputNeurons  = 0;
    _costumNeurons  = 0;

    for(unsigned int a=0; a<neurons->size(); a++)
    {
        if((*neurons)[a]->get_ID().TYPE == NeuronType::hidden)
        {
            _hiddenNeurons++;
        }else if((*neurons)[a]->get_ID().TYPE == NeuronType::output)
        {
            _outputNeurons++;
        }else if((*neurons)[a]->get_ID().TYPE == NeuronType::costum)
        {
            _costumNeurons++;
        }
    }
    _check_hiddenNeurons = true;
    _check_outputNeurons = true;
    _check_costumNeuron  = true;

    _connections         = 0;
    _costumConnections   = 0;

    for(unsigned int a=0; a<neurons->size(); a++)
    {
        _connections += (*neurons)[a]->get_inputs();
        for(unsigned int b=0; b<(*neurons)[a]->get_inputs(); b++)
        {
            if((*neurons)[a]->get_inputID(b).TYPE == NeuronType::costum)
            {
                _costumConnections++;
            }
        }
    }
    _check_connections       = true;
    _check_costumConnections = true;

    saveGenomOfNet(static_cast<unsigned int>(_neuronList.size()-1));
}
void                    SaveNet::checkParam()
{
    if(!_check_inputs)
    {
        addError(Error("checkParam()","Param \"inputNeurons\" is not defined"));
        //addError(Error("checkParam()","Param \"inputNeurons\" is not defined");
    }
    if(!_check_hiddenX)
    {
        addError(Error("checkParam()","Param \"hiddenNeuronsX\" is not defined"));
    }
    if(!_check_hiddenY)
    {
        addError(Error("checkParam()","Param \"hiddenNeuronsY\" is not defined"));
    }
    if(!_check_outputs)
    {
        addError(Error("checkParam()","Param \"outputNeurons\" is not defined"));
    }
    if(!_check_bias)
    {
        addError(Error("checkParam()","Param \"bias\" is not defined"));
    }
    if(!_check_biasValue)
    {
        addError(Error("checkParam()","Param \"biasValue\" is not defined"));
    }
    if(!_check_average)
    {
        addError(Error("checkParam()","Param \"enableAverage\" is not defined"));
    }
    if(!_check_activationFunction)
    {
        addError(Error("checkParam()","Param \"activationFunction\" is not defined"));
    }
    if(!_check_neurons)
    {
        addError(Error("checkParam()","Param \"neurons\" is not defined"));
    }
    if(!_check_hiddenNeurons)
    {
        addError(Error("checkParam()","Param \"_check_hiddenNeurons\" is not defined"));
    }
    if(!_check_outputNeurons)
    {
        addError(Error("checkParam()","Param \"_check_outputNeurons\" is not defined"));
    }
    if(!_check_costumNeuron)
    {
        addError(Error("checkParam()","Param \"_check_costumNeuron\" is not defined"));
    }
    if(!_check_connections)
    {
        addError(Error("checkParam()","Param \"_check_connections\" is not defined"));
    }
    if(!_check_costumConnections)
    {
        addError(Error("checkParam()","Param \"_check_costumConnections\" is not defined"));
    }
}
unsigned int            SaveNet::get_genomsize()
{
    return _genomsize;
}
void                    SaveNet::saveGenomOfNet(unsigned int ID)
{
    bool exists = false;
    unsigned int index = 0;
    for(unsigned int a=0; a<_ID_list.size(); a++)
    {
        if(_ID_list[a] == ID)
        {
            exists = true;
            index  = a;
            break;
        }
    }
    if(exists)
    {
        _genomList[index].clear();
        for(unsigned int a=0; a<_neuronList[index].size(); a++)
        {
            for(unsigned int b=0; b<_neuronList[index][a]->get_inputs(); b++)
            {
                _genomList[index].push_back(_neuronList[index][a]->get_weight(b));
            }
        }
        _genomsize = static_cast<unsigned int>(_genomList.size());
    }
}
std::vector<Connection>      *SaveNet::get_ptr_connectionList(unsigned int animal)
{
    if(animal >= get_animals())
    {
        addError(Error("get_ptr_connectionList(unsigned int ["+std::to_string(animal)+"] )",
                       ErrorMessage::outOfRange('[',static_cast<unsigned int>(0),animal,get_animals()-1,']')));
        //error_general("connectionList(unsigned int ["+std::to_string(animal)+"])",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,animals()));
    }
    return &_connectionListFromFile[animal];
}
std::vector<std::vector<Connection> > *SaveNet::get_ptr_connectionList()
{
    return &_connectionListFromFile;
}
void                    SaveNet::clearGenomList()
{
    //_genomList = std::vector<std::vector<double> >(_animals,std::vector<double>(0));
    _genomList.clear();
    _genomList.reserve(_animals);
    for(int a=0; a<static_cast<int>(_animals); a++)
    {
        _genomList.push_back(std::vector<double>());
    }
}
void                    SaveNet::clearExternParam()
{
    _extraParamName = std::vector<std::string> ();
    _extraParamValue = std::vector<double> ();
}
//----------ERROR
void                    SaveNet::addError(const Error &e)
{
    _errorList.push_back(e);
    _errorList[_errorList.size()-1].setNamespace("SaveNet::"+_errorList[_errorList.size()-1].getNamespace());
    Error::print(_errorList[_errorList.size()-1]);
}
/*
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
std::string SaveNet::error_paramOutOfRange(unsigned int paramPos,double value,double min, double max)
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
*/
