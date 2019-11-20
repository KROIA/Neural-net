#include <savenet.h>


SaveNet::SaveNet()
{
    filename("netFile");
    fileEnding("net");
    _saves = 0;
    _genomsize = 0;
    this->clear();
}
SaveNet::~SaveNet()
{
    _neuronList.clear();
    _extraParamName.clear();
    _extraParamValue.clear();
    _genomList.clear();
    _ID_list.clear();
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
unsigned int    SaveNet::neurons()
{
    return _neurons;
}
unsigned int    SaveNet::hiddenNeurons()
{
    return _hiddenNeurons;
}
unsigned int    SaveNet::costumNeurons()
{
    return _costumNeurons;
}
unsigned int    SaveNet::connections()
{
    return _connections;
}
unsigned int    SaveNet::costumConnections()
{
    return _costumConnections;
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
    while(tmpBuffer.find("[NETS]") == -1)
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
            continue;
        }else if(fileBuffer[a].find("ANIMALS ") == 0)
        {
            tmpAnimals = std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str());
            continue;
        }else if(fileBuffer[a].find("__INPUTNEURONS ") == 0)
        {
            inputNeurons(std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
            continue;
        }else if(fileBuffer[a].find("__HIDDENNEURONSX ") == 0)
        {
            hiddenNeuronsX(std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
            continue;
        }else if(fileBuffer[a].find("__HIDDENNEURONSY ") == 0)
        {
            hiddenNeuronsY(std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
            continue;
        }else if(fileBuffer[a].find("__OUTPUTNEURONS ") == 0)
        {
            outputNeurons(std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
            continue;
        }else if(fileBuffer[a].find("__BIAS ") == 0)
        {
            bias(std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
            continue;
        }else if(fileBuffer[a].find("__AVERAGE ") == 0)
        {
            enableAverage(std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
            continue;
        }else if(fileBuffer[a].find("__BIASVALUE ") == 0)
        {
            biasValue(std::stof(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n"))));
            continue;
        }else if(fileBuffer[a].find("__ACTIVATIONFUNCTION ") == 0)
        {
            activationFunction((Activation)std::stoul(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str()));
            continue;
        }else if(fileBuffer[a].find("__NEURONS ") == 0)
        {
            _neurons = std::stol(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str());
            continue;
        }else if(fileBuffer[a].find(" __HIDDEN_NEURONS ") == 0)
        {
            fileBuffer[a] = fileBuffer[a].substr(1);
            _hiddenNeurons = std::stol(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str());
            continue;
        }else if(fileBuffer[a].find(" __OUTPUT_NEURONS ") == 0)
        {
            fileBuffer[a] = fileBuffer[a].substr(1);
            _outputNeurons = std::stol(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str());
            continue;
        }else if(fileBuffer[a].find(" __COSTUM_NEURONS ") == 0)
        {
            fileBuffer[a] = fileBuffer[a].substr(1);
            _costumNeurons = std::stol(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str());
            continue;
        }else if(fileBuffer[a].find("__CONNECTIONS ") == 0)
        {
            _connections = std::stol(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str());
            continue;
        }else if(fileBuffer[a].find(" __SPECIAL_CONNECTIONS ") == 0)
        {
            fileBuffer[a] = fileBuffer[a].substr(1);
            _costumConnections = std::stol(fileBuffer[a].substr(fileBuffer[a].find(" ")+1,fileBuffer[a].find("\n")).c_str());
            continue;
        }else if(fileBuffer[a].find("[P] ") == 0)
        {
            fileBuffer[a] = fileBuffer[a].substr(4);
            std::string name = fileBuffer[a].substr(0,fileBuffer[a].find(" "));
            fileBuffer[a] = fileBuffer[a].substr(fileBuffer[a].find(" ")+1);
            float value = std::stof(fileBuffer[a].substr(0,fileBuffer[a].find("\n")).c_str());
            setExtraParam(name,value);
        }
    }
    //this->genomsize();

    fileBuffer.clear();
    tmpBuffer = "";
    while(!feof(_file))
    {
        char list[1024];
        if(fgets(list,1024,_file) == nullptr)
        {
            break;
        }
        tmpBuffer += list;
        if(tmpBuffer.find("\n") != -1)
        {
            fileBuffer.push_back(tmpBuffer);
            tmpBuffer = "";
        }
    }
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

    for(unsigned int a=0; a<fileBuffer.size(); a++)
    {
        //if(fileBuffer[a].find("[NET]") != -1)
        if(a == 50)
        {
            a=50;
        }
        while(fileBuffer[a] != "\n")
        {
           // qDebug() << "a: "<<a<<" "<<QString::fromStdString(fileBuffer[a]);
            if(fileBuffer[a].find("N") == 0)
            {
                //netID = atoi(fileBuffer[a].substr(fileBuffer[a].find("<")+1,fileBuffer[a].find(">")).c_str());
                netID = atoi(fileBuffer[a].substr(fileBuffer[a].find("N")+1,fileBuffer[a].find("D")).c_str());
                fileBuffer[a] = fileBuffer[a].substr(fileBuffer[a].find("D"),fileBuffer[a].size());
                _connectionListFromFile.push_back(std::vector<Connection>());

            }else if(fileBuffer[a].find("D") == 0) //if(fileBuffer[a].find("[NeuronID]") != -1)
            {
                //fileBuffer[a] = fileBuffer[a].substr(1,fileBuffer[a].size());
                readPos   = ReadPos::NeuronID;
               // ptr_tmpNeuron = new Neuron();
            }else if(fileBuffer[a].find("I") == 0) //if(fileBuffer[a].find("[Inputs]") != -1)
            {
                //fileBuffer[a] = fileBuffer[a].substr(1,fileBuffer[a].size());
                readPos   = ReadPos::Inputs;
            }else if(fileBuffer[a].find("C") == 0) //if(fileBuffer[a].find("[Connections]") != -1)
            {
               // fileBuffer[a] = fileBuffer[a].substr(1,fileBuffer[a].size());
                readPos   = ReadPos::Connections;
                _connectionListFromFile[_connectionListFromFile.size()-1].reserve(_connections);
            }

            switch(readPos)
            {
                case ReadPos::NeuronID:
                {
                    if(fileBuffer[a].find("D") == 0) //if(fileBuffer[a].find("[ID]") != -1)
                    {
                        //neuronID.ID = atoi(fileBuffer[a].substr(fileBuffer[a].find("<")+1,fileBuffer[a].find(">")).c_str());
                        neuronID.ID = atoi(fileBuffer[a].substr(fileBuffer[a].find("D")+1,fileBuffer[a].find("T")).c_str());
                        //a++;
                        //neuronID.TYPE = (NeuronType)atoi(fileBuffer[a].substr(fileBuffer[a].find("<")+1,fileBuffer[a].find(">")).c_str());
                        neuronID.TYPE = (NeuronType)atoi(fileBuffer[a].substr(fileBuffer[a].find("T")+1,fileBuffer[a].find("I")).c_str());
                        fileBuffer[a] = fileBuffer[a].substr(fileBuffer[a].find("I"),fileBuffer[a].size());
                        readPos = ReadPos::nothing;
                    }
                    break;
                }
                case ReadPos::Inputs:
                {
                    //ptr_tmpNeuron->inputs(atoi(fileBuffer[a+2].substr(fileBuffer[a+2].find("<")+1,fileBuffer[a+2].find(">")).c_str()));
                    inputs = atoi(fileBuffer[a].substr(fileBuffer[a].find("I")+1,fileBuffer[a].find("C")).c_str());
                    fileBuffer[a] = fileBuffer[a].substr(fileBuffer[a].find("C"),fileBuffer[a].size());
                    readPos = ReadPos::nothing;
                    break;
                }
                case ReadPos::Connections:
                {
                    //connections einlesen
                    //if(fileBuffer[a].find("i") == 0)
                    {
                        _connectionListFromFile[_connectionListFromFile.size()-1].push_back(Connection());
                        inputCounter++;
                        /*_connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].netID = netID;
                        _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].destination_ID = neuronID;
                        _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].source_ID.ID = atoi(fileBuffer[a].substr(fileBuffer[a].find("<")+1,fileBuffer[a].find(">")).c_str());
                        a++;
                        _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].source_ID.TYPE = (NeuronType)atoi(fileBuffer[a].substr(fileBuffer[a].find("<")+1,fileBuffer[a].find(">")).c_str());
                        a++;
                        _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].weight = stof(fileBuffer[a].substr(fileBuffer[a].find("<")+1,fileBuffer[a].find(">")));
                        a++;
                        _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].direction = atoi(fileBuffer[a].substr(fileBuffer[a].find("<")+1,fileBuffer[a].find(">")).c_str());
                        a++;*/
                        _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].netID = netID;
                        _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].destination_ID = neuronID;
                        _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].source_ID.ID = atoi(fileBuffer[a].substr(fileBuffer[a].find("i")+1,fileBuffer[a].find("t")).c_str());
                        fileBuffer[a] = fileBuffer[a].substr(fileBuffer[a].find("i")+1,fileBuffer[a].size());
                        _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].source_ID.TYPE = (NeuronType)atoi(fileBuffer[a].substr(fileBuffer[a].find("t")+1,fileBuffer[a].find("w")).c_str());
                        _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].weight = stof(fileBuffer[a].substr(fileBuffer[a].find("w")+1,fileBuffer[a].find("d")));
                        _connectionListFromFile[_connectionListFromFile.size()-1][_connectionListFromFile[_connectionListFromFile.size()-1].size()-1].direction = (ConnectionDirection)atoi(fileBuffer[a].substr(fileBuffer[a].find("d")+1,fileBuffer[a].find("i")).c_str());
                        fileBuffer[a] = fileBuffer[a].substr(fileBuffer[a].find("i"),fileBuffer[a].size());
                    }

                    if(inputCounter == inputs)
                    {
                        inputCounter = 0;
                        readPos = ReadPos::nothing;
                        fileBuffer[a] = fileBuffer[a].substr(fileBuffer[a].find("|")+1,fileBuffer[a].size());
                    }
                    break;
                }
                default:
                {

                }
            }
        }
        _animals = _connectionListFromFile.size();
       /* _genomList[index].clear();
        for(unsigned int a=0; a<_neuronList[index].size(); a++)
        {
            for(unsigned int b=0; b<_neuronList[index][a]->inputs(); b++)
            {
                _genomList[index].push_back(_neuronList[index][a]->weight(b));
            }
        }
        _genomsize = _genomList.size();*/
        /*std::vector<float>  addGen;

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
        this->addGenom(addGen);*/
    }
    qDebug() << "read finish";
    qDebug() << "nets: "<<_connectionListFromFile.size() << " connections: "<<_connectionListFromFile[0].size();
    inputCounter = 0;
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
    if(_neuronList.size() == 0)
    {
        error_general("saveFile()","No nets defined");
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
            fprintf(_file,"D%i",_neuronList[net][neuron]->ID().ID);
            fprintf(_file,"T%i",_neuronList[net][neuron]->ID().TYPE);

            //[Inputs]      -> amount of inputs of the Neuron
            /*fprintf(_file,"\t\t[Inputs]<%i>\n",_neuronList[net][neuron]->inputs());
            fprintf(_file,"\t\t[Connections]\n\t\t{\n",_neuronList[net][neuron]->inputs());*/
            fprintf(_file,"I%i",_neuronList[net][neuron]->inputs());
            fprintf(_file,"C%i",_neuronList[net][neuron]->inputs());
            for(unsigned int input=0; input<_neuronList[net][neuron]->inputs(); input++)
            {
                // connectionAddress to the following weight, has Parameter of struct NeuronID
                /*fprintf(_file,"\t\t\t{\n");
                fprintf(_file,"\t\t\t\t[ID]<%i>\n",_neuronList[net][neuron]->inputID()[input].ID);
                fprintf(_file,"\t\t\t\t[NeuronType]<%i>\"%s\"\n",_neuronList[net][neuron]->inputID()[input].TYPE,Neuron::typeString(_neuronList[net][neuron]->inputID()[input].TYPE).c_str());
                //[w]   -> weight
                fprintf(_file,"\t\t\t\t[w]<%.8f>\n",(double)_neuronList[net][neuron]->weight(input));
                fprintf(_file,"\t\t\t\t[dir]<%i>\n",_neuronList[net][neuron]->inputConnectionDirection(_neuronList[net][neuron]->inputID(input)));
                fprintf(_file,"\t\t\t}\n");*/
                fprintf(_file,"i%i",_neuronList[net][neuron]->inputID()[input].ID);
                fprintf(_file,"t%i",_neuronList[net][neuron]->inputID()[input].TYPE);
                //[w]   -> weight
                fprintf(_file,"w%.4f",(double)_neuronList[net][neuron]->weight(input));
                fprintf(_file,"d%i",_neuronList[net][neuron]->inputConnectionDirection(_neuronList[net][neuron]->inputID(input)));
            }
            /*fprintf(_file,"\t\t}\n");
            fprintf(_file,"\t}\n");*/
            fprintf(_file,"i|");
        }
        //fprintf(_file,"}\n");
        fprintf(_file,"\n");
    }

    fclose(_file);

}
void SaveNet::saveFile(std::string filename)
{
    this->filename(filename);
    saveFile();
}
/*void SaveNet::setGenom(std::vector<float>   genom)
{
    checkParam();
    this->clearGenomList();
    this->addGenom(genom);
}*/
/*void SaveNet::setGenom(unsigned int index,std::vector<float>   genom)
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
}*/
/*void SaveNet::setGenom(std::vector<std::vector<float>   > genomList)
{
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
}*/
/*void SaveNet::addGenom(std::vector<float>   genom)
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
        //error_general("addGenom(std::vector<float>)",error);
        qDebug() << "WARNING: "<< QString::fromStdString(error);        //only to test the aditional connections
    }
    _genomList.push_back(genom);
}*/
/*void SaveNet::addGenom(std::vector<std::vector<float>   > genomList)
{
    for(unsigned int a=0; a<genomList.size(); a++)
    {
        try {
            addGenom(genomList[a]);
        } catch (std::runtime_error &e) {
            error_general("addGenom(std::vector<std::vecor<float>   >)","addGenom(genomList["+std::to_string(a)+"])",e);
        }
    }
}*/
std::vector<float>  SaveNet::genom(unsigned int ID)
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
        error_general("genom(unsigned int ["+std::to_string(ID)+"])","No net with such an ID");
    }
}
std::vector<std::vector<float>  > SaveNet::genom()
{
    return _genomList;
}
unsigned int SaveNet::animals()
{
    return _animals;//_genomList.size();
}
void SaveNet::clear()
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

void SaveNet::neuronsOfNet(unsigned int ID,std::vector<Neuron*> *neurons)
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
        if(_neurons == neurons->size())
        {
            _neuronList[index] = *neurons;
            saveGenomOfNet(index);
        }
        else
        {
            error_general("neuronsOfNet(unsigned int ["+std::to_string(ID)+"], std::vector<Neuron*> neurons)","There are already some nets saved which have not the same amount of neurons: "+std::to_string(_neurons)+" as parameter 2: "+std::to_string(neurons->size()));
        }
    }
    else
    {
        _neuronList.push_back(*neurons);

        _ID_list.push_back(ID);
        _animals = _ID_list.size();
        _check_neurons = true;

        _neurons        = neurons->size();
        _hiddenNeurons  = 0;
        _outputNeurons  = 0;
        _costumNeurons  = 0;

        for(unsigned int a=0; a<neurons->size(); a++)
        {
            if((*neurons)[a]->ID().TYPE == NeuronType::hidden)
            {
                _hiddenNeurons++;
            }else if((*neurons)[a]->ID().TYPE == NeuronType::output)
            {
                _outputNeurons++;
            }else if((*neurons)[a]->ID().TYPE == NeuronType::costum)
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
            _connections += (*neurons)[a]->inputs();
            for(unsigned int b=0; b<(*neurons)[a]->inputs(); b++)
            {
                if((*neurons)[a]->inputID(b).TYPE == NeuronType::costum)
                {
                    _costumConnections++;
                }
            }
        }
        _check_connections       = true;
        _check_costumConnections = true;
        _genomList.push_back(std::vector<float>());
        saveGenomOfNet(_neuronList.size()-1);
    }
}
std::vector<Neuron*> SaveNet::neuronsOfNet(unsigned int ID)
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
        error_general("neuronsOfNet(unsigned int ["+std::to_string(ID)+"])","Net not available. There is no Net with such an ID");
    }
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
    if(!_check_neurons)
    {
        error_general("checkParam()","Param \"neurons\" is not defined");
    }
    if(!_check_hiddenNeurons)
    {
        error_general("checkParam()","Param \"_check_hiddenNeurons\" is not defined");
    }
    if(!_check_outputNeurons)
    {
        error_general("checkParam()","Param \"_check_outputNeurons\" is not defined");
    }
    if(!_check_costumNeuron)
    {
        error_general("checkParam()","Param \"_check_costumNeuron\" is not defined");
    }
    if(!_check_connections)
    {
        error_general("checkParam()","Param \"_check_connections\" is not defined");
    }
    if(!_check_costumConnections)
    {
        error_general("checkParam()","Param \"_check_costumConnections\" is not defined");
    }
}
unsigned int SaveNet::genomsize()
{
   /* _genomsize = 0;
    if(_neuronList.size() == 0)
    {
        //no nets available
        return 0;
    }
    for(unsigned int a=0; a<_neuronList[0].size(); a++)
    {
        _genomsize += _neuronList[0][a]->inputs();
    }*/

    /*if(_hiddenX == 0 || _hiddenY == 0)
    {
        _genomsize = (_inputs+(unsigned int)_bias) * _outputs;
    }
    else
    {
        _genomsize = (_inputs+(unsigned int)_bias) * _hiddenY + (_hiddenY+(unsigned int)_bias) * _hiddenY * (_hiddenX-1) + (_hiddenY+(unsigned int)_bias) * _outputs;
    }*/
    return _genomsize;
}
void SaveNet::saveGenomOfNet(unsigned int ID)
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
            for(unsigned int b=0; b<_neuronList[index][a]->inputs(); b++)
            {
                _genomList[index].push_back(_neuronList[index][a]->weight(b));
            }
        }
        _genomsize = _genomList.size();
    }
}
std::vector<Connection> *SaveNet::connectionList(unsigned int animal)
{
    if(animal >= animals())
    {
        error_general("connectionList(unsigned int ["+std::to_string(animal)+"])",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,animals()));
    }
    return &_connectionListFromFile[animal];
}
std::vector<std::vector<Connection> > *SaveNet::connectionList()
{
    return &_connectionListFromFile;
}
void SaveNet::clearGenomList()
{
    _genomList = std::vector<std::vector<float> >(_animals,std::vector<float>(0));
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
