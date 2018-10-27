#include <geneticnet.h>


GeneticNet::GeneticNet()
{
    this->init(GENETICNET_MIN_ANIMALS,NET_MIN_INPUTNEURONS,NET_MIN_HIDDENNEURONS_X,NET_MIN_HIDDENNEURONS_Y,NET_MIN_OUTPUTNEURONS,true,false,Activation::Sigmoid);
}
GeneticNet::GeneticNet(unsigned int animals)
{
    this->init(animals,NET_MIN_INPUTNEURONS,NET_MIN_HIDDENNEURONS_X,NET_MIN_HIDDENNEURONS_Y,NET_MIN_OUTPUTNEURONS,true,false,Activation::Sigmoid);
}
GeneticNet::GeneticNet(unsigned int animals,
                       unsigned int inputs,
                       unsigned int hiddenX,
                       unsigned int hiddenY,
                       unsigned int outputs)
{
    this->init(animals,inputs,hiddenX,hiddenY,outputs,true,false,Activation::Sigmoid);
}
GeneticNet::GeneticNet(unsigned int animals,
                       unsigned int inputs,
                       unsigned int hiddenX,
                       unsigned int hiddenY,
                       unsigned int outputs,
                       bool enableBias,
                       bool enableAverage,
                       Activation func)
{
    this->init(animals,inputs,hiddenX,hiddenY,outputs,enableBias,enableAverage,func);
}
GeneticNet::~GeneticNet()
{
    for(unsigned int b=_netList.size(); b>0; b--)
    {
        try {
            delete _netList[b-1];
        } catch (std::exception &e) {
            qDebug() << "error: "<<e.what();
        }
    }
    _scoreList.clear();
}
void                    GeneticNet::set(unsigned int animals,
                            unsigned int inputs,
                            unsigned int hiddenX,
                            unsigned int hiddenY,
                            unsigned int outputs,
                            bool enableBias,
                            bool enableAverage,
                            Activation func)
{
    this->animals(animals);
    this->inputNeurons(inputs);
    this->hiddenNeuronsX(hiddenX);
    this->hiddenNeuronsY(hiddenY);
    this->outputNeurons(outputs);
    this->bias(enableBias);
    this->enableAverage(enableAverage);
    this->activationFunction(func);


}

void                    GeneticNet::netFileName(std::string filename)
{
    _saveNet.filename(filename);
}
std::string             GeneticNet::netFileName()
{
    return _saveNet.filename();
}
void                    GeneticNet::netFileEnding(std::string fileEnding)
{
    _saveNet.fileEnding(fileEnding);
}
std::string             GeneticNet::netFileEnding()
{
    return _saveNet.fileEnding();
}
void                    GeneticNet::loadFromNetFile()
{
    try {
        _saveNet.loadFile();
    } catch (std::runtime_error &e) {
        qDebug() << "Warning: "<< e.what();
        return;
    }
    try {
        this->set(_saveNet.animals(),_saveNet.inputNeurons(),_saveNet.hiddenNeuronsX(),_saveNet.hiddenNeuronsY(),_saveNet.outputNeurons(),
                  _saveNet.bias(),_saveNet.enableAverage(),_saveNet.activationFunction());
        this->biasValue(_saveNet.biasValue());
        this->genom(_saveNet.genom());
    } catch (std::runtime_error &e) {
        error_general("loadFromNetFile(std::string ["+_saveNet.filename()+"] , std::string ["+_saveNet.fileEnding()+"] )",
                      "unable to apply the settings. Maybe the file is damaged.",e);
    }
}
void                    GeneticNet::loadFromNetFile(std::string filename)
{
    this->netFileName(filename);
    this->loadFromNetFile();
}
void                    GeneticNet::loadFromNetFile(std::string filename,std::string fileEnding)
{
    this->netFileName(filename);
    this->netFileEnding(fileEnding);
    this->loadFromNetFile();
}
void                    GeneticNet::saveToNetFile()
{
    try {
        _saveNet.set(this->inputNeurons(),this->hiddenNeuronsX(),this->hiddenNeuronsY(),this->outputNeurons(),
                     this->bias(),this->enableAverage(),this->activationFunction(),this->biasValue());
        _saveNet.setGenom(this->genom());
        _saveNet.saveFile();
    } catch (std::runtime_error &e) {
        error_general("saveToNetFile()",e);
    }
}
void                    GeneticNet::saveToNetFile(std::string filename)
{
    this->netFileName(filename);
    this->saveToNetFile();
}
void                    GeneticNet::saveToNetFile(std::string filename,std::string fileEnding)
{
    this->netFileName(filename);
    this->netFileEnding(fileEnding);
    this->saveToNetFile();
}


void                    GeneticNet::init(unsigned int animals,
                                         unsigned int inputs,
                                         unsigned int hiddenX,
                                         unsigned int hiddenY,
                                         unsigned int outputs,
                                         bool enableBias,
                                         bool enableAverage,
                                         Activation func)
{
    _randEngine = std::default_random_engine(rand()%100);
    _animals = 0;
    _netList = std::vector<Net * >();
    _scoreList = std::vector<float>();
    this->mutationChangeWeight((float)0.01);
    this->mutationFactor((float)0.01);
    this->netFileName("netFile");
    this->netFileEnding("gnet");
    this->set(animals,inputs,hiddenX,hiddenY,outputs,enableBias,enableAverage,func);
}

void                    GeneticNet::animals(unsigned int animals)
{
    if(animals < GENETICNET_MIN_ANIMALS || animals > GENETICNET_MAX_ANIMALS)
    {
        error_general("animals(unsigned int ["+std::to_string(animals)+"] )","Parameter 0 is out of range. Min: "+ std::to_string(GENETICNET_MIN_ANIMALS)+" Max: "+std::to_string(GENETICNET_MAX_ANIMALS));
    }
    if(animals == _animals)
    {
        return;
    }
    if(animals > _animals)
    {
       for(unsigned int a=0; a<animals - _animals; a++)
        {
            _netList.push_back(new Net());
            _scoreList.push_back(0);
        }

    }
    else
    {
        for(unsigned int a=0; a<_animals - animals; a++)
        {
            delete _netList[_netList.size()-1];
            _netList.erase(_netList.end()-1);
            _scoreList.erase(_scoreList.end()-1);
        }
    }
    _animals = animals;
    _update  = true;
}
unsigned int            GeneticNet::animals()
{
    return _animals;
}
void                    GeneticNet::inputNeurons(unsigned int inputs)
{
    if(inputs != _netList[0]->inputNeurons())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->inputNeurons(inputs);
            } catch (std::runtime_error &e) {
                error_general("inputNeurons(unsigned int ["+std::to_string(inputs)+"] )","_netList["+std::to_string(a)+"].inputNeurons("+std::to_string(inputs)+")",e);
            }
        }
        _update = true;
    }
}
unsigned int            GeneticNet::inputNeurons()
{
    return _netList[0]->inputNeurons();
}
void                    GeneticNet::hiddenNeuronsX(unsigned int hiddenX)
{
    if(hiddenX != _netList[0]->hiddenNeuronsX())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->hiddenNeuronsX(hiddenX);
            } catch (std::runtime_error &e) {
                error_general("hiddenNeuronsX(unsigned int ["+std::to_string(hiddenX)+"] )","_netList["+std::to_string(a)+"].hiddenNeuronsX("+std::to_string(hiddenX)+")",e);
            }
        }
        _update = true;
    }
}
unsigned int            GeneticNet::hiddenNeuronsX()
{
    return _netList[0]->hiddenNeuronsX();
}
void                    GeneticNet::hiddenNeuronsY(unsigned int hiddenY)
{
    if(hiddenY != _netList[0]->hiddenNeuronsY())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->hiddenNeuronsY(hiddenY);
            } catch (std::runtime_error &e) {
                error_general("hiddenNeuronsY(unsigned int ["+std::to_string(hiddenY)+"] )","_netList["+std::to_string(a)+"].hiddenNeuronsY("+std::to_string(hiddenY)+")",e);
            }
        }
        _update = true;
    }
}
unsigned int            GeneticNet::hiddenNeuronsY()
{
    return _netList[0]->hiddenNeuronsY();
}
void                    GeneticNet::outputNeurons(unsigned int outputs)
{
    if(outputs != _netList[0]->outputNeurons())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->outputNeurons(outputs);
            } catch (std::runtime_error &e) {
                error_general("outputNeurons(unsigned int ["+std::to_string(outputs)+"] )","_netList["+std::to_string(a)+"].outputNeurons("+std::to_string(outputs)+")",e);
            }
        }
        _update = true;
    }
}
unsigned int            GeneticNet::outputNeurons()
{
    return _netList[0]->outputNeurons();
}

void                    GeneticNet::bias(bool enableBias)
{
    if(enableBias != _netList[0]->bias())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->bias(enableBias);
            } catch (std::runtime_error &e) {
                error_general("bias(unsigned int ["+std::to_string(enableBias)+"] )","_netList["+std::to_string(a)+"].bias("+std::to_string(enableBias)+")",e);
            }
        }
        _update = true;
    }
}
bool                    GeneticNet::bias()
{
    return _netList[0]->bias();
}
void                    GeneticNet::enableAverage(bool enableAverage)
{
    if(enableAverage != _netList[0]->enableAverage())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->enableAverage(enableAverage);
            } catch (std::runtime_error &e) {
                error_general("enableAverage(unsigned int ["+std::to_string(enableAverage)+"] )","_netList["+std::to_string(a)+"].enableAverage("+std::to_string(enableAverage)+")",e);
            }
        }
        _update = true;
    }
}
bool                    GeneticNet::enableAverage()
{
    return _netList[0]->enableAverage();
}
void                    GeneticNet::biasValue(float value)
{
    if(value != _netList[0]->biasValue())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->biasValue(value);
            } catch (std::runtime_error &e) {
                error_general("biasValue(unsigned int ["+std::to_string(value)+"] )","_netList["+std::to_string(a)+"].biasValue("+std::to_string(value)+")",e);
            }
        }
        _update = true;
    }
}
float                   GeneticNet::biasValue()
{
    return _netList[0]->biasValue();
}
void                    GeneticNet::activationFunction(Activation func)
{
    if(func != _netList[0]->activationFunction())
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                _netList[a]->activationFunction(func);
            } catch (std::runtime_error &e) {
                error_general("activationFunction(unsigned int ["+std::to_string(func)+"] )","_netList["+std::to_string(a)+"].activationFunction("+std::to_string(func)+")",e);
            }
        }
        _update = true;
    }
}
Activation              GeneticNet::activationFunction()
{
    return _netList[0]->activationFunction();
}
bool                    GeneticNet::noHiddenLayer()
{
    return _netList[0]->noHiddenLayer();
}

void                    GeneticNet::randomGenom()
{
    _update = true;
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            _netList[a]->randomGenom();
        } catch (std::runtime_error &e) {
            error_general("randomGenom()","_netList["+std::to_string(a)+"]->randomGenom()",e);
        }
    }
}
void                    GeneticNet::genom(std::vector<std::vector<float>    > genomList)
{
    if(genomList.size() != _animals)
    {
        error_general("genom(std::vector<std::vector<float> >)","std::vector<std::vector<float> >.size() == "+std::to_string(genomList.size())+" "+error_paramOutOfRange(0,genomList.size(),_animals,_animals));
    }
    _update = true;
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            _netList[a]->genom(genomList[a]);
        } catch (std::runtime_error &e) {
            error_general("genom(std::vector<std::vector<float> >)","_netList["+std::to_string(a)+"]->genom(genomList["+std::to_string(a)+"])",e);
        }
    }
}
void                    GeneticNet::genom(unsigned int animal, std::vector<float> genom)
{
    if(animal > _animals-1)
    {
        error_general("genom(unsigned int ["+std::to_string(animal)+"] , std::vector<float> )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    if(genom.size() != genomsize())
    {
        error_general("genom(unsigned int ["+std::to_string(animal)+"] , std::vector<float> )",error_paramOutOfRange((unsigned int)1,genom.size(),genomsize(),genomsize()));
    }
    _update = true;
    try {
        _netList[animal]->genom(genom);
    } catch (std::runtime_error &e) {
        error_general("genom(unsigned int ["+std::to_string(animal)+"] , std::vector<float> )","_netList["+std::to_string(animal)+"].genom(std::vector<float>)",e);
    }
}
std::vector<float>      GeneticNet::genom(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("genom(unsigned int ["+std::to_string(animal)+"])",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal]->genom();
}
std::vector<std::vector<float>  >GeneticNet::genom()
{
    std::vector<std::vector<float>  > gen(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        gen[a] = _netList[a]->genom();
    }
    return gen;
}
unsigned int            GeneticNet::genomsize()
{
    return _netList[0]->genomsize();
}

void                    GeneticNet::input(unsigned int animal, unsigned int input, float signal)
{
    if(animal > _animals-1)
    {
        error_general("input(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(input)+"] , float ["+std::to_string(signal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    _update = true;
    try {
        _netList[animal]->input(input,signal);
    } catch (std::runtime_error &e) {
        error_general("input(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(input)+"] , float ["+std::to_string(signal)+"] )","_netList["+std::to_string(animal)+"]->input("+std::to_string(input)+","+std::to_string(signal)+")",e);
    }
}
float                   GeneticNet::input(unsigned int animal, unsigned int input)
{
    if(animal > _animals-1)
    {
        error_general("input(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    try {
        return _netList[animal]->input(input);
    } catch (std::runtime_error &e) {
        error_general("input(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(input)+"] )","return _netList["+std::to_string(animal)+"]->input("+std::to_string(input)+")",e);
    }
}
void                    GeneticNet::input(unsigned int animal, std::vector<float> inputList)
{
    if(animal > _animals-1)
    {
        error_general("input(unsigned int ["+std::to_string(animal)+"] , std::vector<float> )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    _update = true;
    try {
        _netList[animal]->input(inputList);
    } catch (std::runtime_error &e) {
        error_general("input(unsigned int ["+std::to_string(animal)+"] , std::vector<float> )","_netList["+std::to_string(animal)+"]->input(std::vector<float>)",e);
    }
}
std::vector<float>      GeneticNet::input(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("input(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    try {
        return _netList[animal]->input();
    } catch (std::runtime_error &e) {
        error_general("input(unsigned int ["+std::to_string(animal)+"] )","return _netList["+std::to_string(animal)+"]->input()",e);
    }
}
void                    GeneticNet::input(std::vector<std::vector<float>    > input)
{
    if(input.size() != _animals)
    {
        error_general("input(std::vector<std::vector<float> >)","std::vector<>.size() == "+std::to_string(input.size()) +" "+ error_paramOutOfRange((unsigned int)0,input.size(),_animals,_animals));
    }
    _update = true;
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            _netList[a]->input(input[a]);
        } catch (std::runtime_error &e) {
            error_general("input(std::vector<std::vector<float> >)","_netList["+std::to_string(a)+"]->input(std::vector<float>["+std::to_string(a)+"])",e);
        }
    }
}
std::vector<std::vector<float>  >GeneticNet::input()
{
    std::vector<std::vector<float>  > inp(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        inp[a] = _netList[a]->input();
    }
    return inp;
}

float                   GeneticNet::hidden(unsigned int animal, unsigned int hiddenX, unsigned int hiddenY)
{
    if(animal > _animals-1)
    {
        error_general("hidden(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    this->run();
    try {
        return _netList[animal]->hidden(hiddenX,hiddenY);
    } catch (std::runtime_error &e) {
        error_general("hidden(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )","return _netList["+std::to_string(animal)+"]->hidden("+std::to_string(hiddenX)+","+std::to_string(hiddenY)+")",e);
    }
}
std::vector<float>      GeneticNet::hidden(unsigned int hiddenX, unsigned int hiddenY)
{
    this->run();
    std::vector<float> ret(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            ret[a] = _netList[a]->hidden(hiddenX,hiddenY);
        } catch (std::runtime_error &e) {
            error_general("hidden(unsigned int ["+std::to_string(hiddenX)+"] , unsigned int ["+std::to_string(hiddenY)+"] )","ret["+std::to_string(a)+"] = _netList["+std::to_string(a)+"]->hidden("+std::to_string(hiddenX)+","+std::to_string(hiddenY)+")",e);
        }
    }
    return ret;
}
std::vector<float>      GeneticNet::hiddenX(unsigned int animal, unsigned int hiddenX) // |    Alle in einer Spalte
{
    if(animal > _animals-1)
    {
        error_general("hiddenX(unsigned int ["+std::to_string(animal)+"] , unsinged int ["+std::to_string(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    this->run();
    try {
        return _netList[animal]->hiddenX(hiddenX);
    } catch (std::runtime_error &e) {
        error_general("hiddenX(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] )","return _netList["+std::to_string(animal)+"]->hiddenX("+std::to_string(hiddenX)+")",e);
    }
}
std::vector<std::vector<float>  >GeneticNet::hiddenX(unsigned int hiddenX)
{
    this->run();
    std::vector<std::vector<float>  > hidX(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            hidX[a] = _netList[a]->hiddenX(hiddenX);
        } catch (std::runtime_error &e) {
            error_general("hiddenX(unsigned int ["+std::to_string(hiddenX)+"] )","hidX["+std::to_string(a)+"] = _netList["+std::to_string(a)+"]->hiddenX("+std::to_string(hiddenX)+");",e);
        }
    }
    return hidX;
}
std::vector<float>      GeneticNet::hiddenY(unsigned int animal, unsigned int hiddenY)// --   Alle in einer Reihe
{
    if(animal > _animals-1)
    {
        error_general("hiddenY(unsigned int ["+std::to_string(animal)+"] , unsinged int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    this->run();
    try {
        return _netList[animal]->hiddenY(hiddenY);
    } catch (std::runtime_error &e) {
        error_general("hiddenY(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenY)+"] )","return _netList["+std::to_string(animal)+"]->hiddenY("+std::to_string(hiddenY)+")",e);
    }
}
std::vector<std::vector<float>  >GeneticNet::hiddenY(unsigned int hiddenY)
{
    this->run();
    std::vector<std::vector<float>  > hidY(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            hidY[a] = _netList[a]->hiddenY(hiddenY);
        } catch (std::runtime_error &e) {
            error_general("hiddenY(unsigned int ["+std::to_string(hiddenY)+"] )","hidX["+std::to_string(a)+"] = _netList["+std::to_string(a)+"]->hiddenY("+std::to_string(hiddenY)+");",e);
        }
    }
    return hidY;
}

Neuron                 *GeneticNet::hiddenNeuron(unsigned int animal, unsigned int hiddenX, unsigned int hiddenY)
{
    if(animal > _animals-1)
    {
        error_general("hiddenNeuron(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] , unsinged int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    this->run();
    try {
        return _netList[animal]->hiddenNeuron(hiddenX,hiddenY);
    } catch (std::runtime_error &e) {
        error_general("hiddenNeuron(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] , unsinged int ["+std::to_string(hiddenY)+"] )","return _netList["+std::to_string(animal)+"]->hiddenNeuron("+std::to_string(hiddenX)+","+std::to_string(hiddenY)+")",e);
    }
}
std::vector<Neuron*>    GeneticNet::hiddenNeuronX(unsigned int animal, unsigned int hiddenX)// |    Alle in einer Spalte
{
    if(animal > _animals-1)
    {
        error_general("hiddenNeuronX(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    this->run();
    try {
        return _netList[animal]->hiddenNeuronX(hiddenX);
    } catch (std::runtime_error &e) {
        error_general("hiddenNeuronX(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenX)+"] )","return _netList["+std::to_string(animal)+"]->hiddenNeuronX("+std::to_string(hiddenX)+");",e);
    }
}
std::vector<Neuron*>    GeneticNet::hiddenNeuronY(unsigned int animal, unsigned int hiddenY)// --   Alle in einer Reihe
{
    if(animal > _animals-1)
    {
        error_general("hiddenNeuronY(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenY)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    this->run();
    try {
        return _netList[animal]->hiddenNeuronY(hiddenY);
    } catch (std::runtime_error &e) {
        error_general("hiddenNeuronY(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(hiddenY)+"] )","return _netList["+std::to_string(animal)+"]->hiddenNeuronY("+std::to_string(hiddenY)+")",e);
    }
}
std::vector<std::vector<Neuron*> > *GeneticNet::hiddenNeuron(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("hiddenNeuron(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    this->run();
    return _netList[animal]->hiddenNeuron();
}
Neuron                 *GeneticNet::outputNeuron(unsigned int animal, unsigned int output)
{
    if(animal > _animals-1)
    {
        error_general("outputNeuron(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    this->run();
    try {
        return _netList[animal]->outputNeuron(output);
    } catch (std::runtime_error &e) {
        error_general("outputNeuron(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(output)+"] )","return _netList["+std::to_string(animal)+"]->outputNeuron("+std::to_string(output)+")",e);
    }
}
std::vector<Neuron*>   *GeneticNet::outputNeuron(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("outputNeuron(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    this->run();
    try {
        return _netList[animal]->outputNeuron();
    } catch (std::runtime_error &e) {
        error_general("outputNeuron(unsigned int ["+std::to_string(animal)+"] , )","return _netList["+std::to_string(animal)+"]->outputNeuron()",e);
    }
}


float                   GeneticNet::output(unsigned int animal, unsigned int output)
{
    if(animal > _animals-1)
    {
        error_general("output(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(output)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    this->run();
    try {
        return _netList[animal]->output(output);
    } catch (std::runtime_error &e) {
        error_general("output(unsigned int ["+std::to_string(animal)+"] , unsigned int ["+std::to_string(output)+"] )","return _netList["+std::to_string(animal)+"]->output("+std::to_string(output)+")",e);
    }
}
std::vector<float>      GeneticNet::output(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("output(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    this->run();
    try {
        return _netList[animal]->output();
    } catch (std::runtime_error &e) {
        error_general("output(unsigned int ["+std::to_string(animal)+"] , )","return _netList["+std::to_string(animal)+"]->output()",e);
    }
}
std::vector<std::vector<float>  >GeneticNet::output()
{
    this->run();
    std::vector<std::vector<float>  > out(_animals);
    for(unsigned int a=0; a<_animals; a++)
    {
        try {
            out[a] = _netList[a]->output();
        } catch (std::runtime_error &e) {
            error_general("output()","out["+std::to_string(a)+"] = _netList["+std::to_string(a)+"]->output()",e);
        }
    }
    return out;
}


void                    GeneticNet::run()
{
    if(_update)
    {
        for(unsigned int a=0; a<_animals; a++)
        {
            try {
                this->run(a);
            } catch (std::runtime_error &e) {
                error_general("run()","this->run("+std::to_string(a)+");",e);
            }
        }
        _update = false;
    }
}
void                    GeneticNet::run(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("run(unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    try {
        _netList[animal]->run();
    } catch (std::runtime_error &e) {
        error_general("run(unsigned int ["+std::to_string(animal)+"] )","_netList["+std::to_string(animal)+"]->run()",e);
    }
}


Net                    *GeneticNet::operator[](unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("operator[](unsigned int ["+std::to_string(animal)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _netList[animal];
}

void                    GeneticNet::mutationFactor(float factor)
{
    if(factor <= __FLOATINPOINT_TOLERANCE)
    {
        error_general("mutationFactor(float ["+std::to_string(factor)+"] )","mutationFactor must be greater than 0");
    }
    _mutationFactor = factor;
}
void                    GeneticNet::mutationChangeWeight(float weight)
{
    if(weight <= __FLOATINPOINT_TOLERANCE)
    {
        error_general("mutationChangeWeight(float ["+std::to_string(weight)+"] )","mutationChangeWeight must be greater than 0");
    }
    _mutationChangeWeight = weight;
}

void                    GeneticNet::score(unsigned int animal, float score)
{
    if(animal > _animals-1)
    {
        error_general("score(unsigned int ["+std::to_string(animal)+"] , float ["+std::to_string(score)+"] )",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    _scoreList[animal] = score;
}
float                   GeneticNet::score(unsigned int animal)
{
    if(animal > _animals-1)
    {
        error_general("score(unsigned int ["+std::to_string(animal)+"])",error_paramOutOfRange((unsigned int)0,animal,(unsigned int)0,_animals-1));
    }
    return _scoreList[animal];
}
void                    GeneticNet::score(std::vector<float>   scoreList)
{
    if(scoreList.size() != _scoreList.size())
    {
        qDebug() << "scoreList.size() "<<_scoreList.size();
        error_general("score(std::vector<float>)","std::vector<float>.size() == "+std::to_string(scoreList.size())+"  "+error_paramOutOfRange(0,scoreList.size(),_animals,_animals));
    }
    _scoreList = scoreList;
}
std::vector<float>      GeneticNet::score()
{
    return _scoreList;
}

void                    GeneticNet::learn(std::vector<float>   scoreList)
{
    this->score(scoreList);
    this->learn();
}
void                    GeneticNet::learn()
{
    float gesScore      = 0;
    float scoreOffset   = 0;
    unsigned int selection1;
    unsigned int selection2;
    std::vector<std::vector<float>  >   newGenom(_animals,std::vector<float>(this->genomsize(),0));


    for(unsigned int a=0; a<_animals; a++)
    {
        if(_scoreList[a] < scoreOffset)
        {
            scoreOffset = _scoreList[a];
        }
    }
    scoreOffset         = -scoreOffset;
    for(unsigned int a=0; a<_animals; a++)
    {
        gesScore       += scoreOffset + _scoreList[a];
        _scoreList[a]  += scoreOffset+0.1;
    }
    for(unsigned int a=0; a<_animals; a+=2)
    {
        learn_selectAnimal(gesScore,selection1,selection2);

        if(a+1 == _animals)
        {
            learn_crossover(_netList[selection1]->genom(),_netList[selection2]->genom(),newGenom[a],newGenom[a-1]);
            learn_mutate(newGenom[a-1]);
        }
        else
        {
            learn_crossover(_netList[selection1]->genom(),_netList[selection2]->genom(),newGenom[a],newGenom[a+1]);
            learn_mutate(newGenom[a+1]);
        }
        learn_mutate(newGenom[a]);
    }
    this->genom(newGenom);
}
void                    GeneticNet::learn_selectAnimal(float gesScore,unsigned int &selection1,unsigned int &selection2)
{
    float random;
    float beginScore;

    random = (float)(_randEngine() % (unsigned int)(gesScore * 100))/100;
    beginScore = 0;
    for(unsigned int a=0; a<_animals; a++)
    {
        if(beginScore <= random && _scoreList[a] + beginScore > random)
        {
            selection1 = a;
        }
        else
        {
            beginScore += _scoreList[a];
        }
    }
    unsigned int counter = 0;
    do{
        random = (float)(_randEngine() % (unsigned int)((gesScore - _scoreList[selection1]) * 100))/100;
        beginScore = 0;
        for(unsigned int a=0; a<_animals; a++)
        {
            if(a == selection1)
            {
                a++;
            }
            if(beginScore <= random && _scoreList[a] + beginScore > random)
            {
                selection2 = a;
            }
            else
            {
                beginScore += _scoreList[a];
            }
            if(counter > 100)
            {
                qDebug() << "warning: learn_selectAnimal(...) counter > 100";
            }
        }
        counter++;

    }while(selection1 == selection2);
}
void                    GeneticNet::learn_crossover(std::vector<float> oldGen1,std::vector<float> oldGen2,std::vector<float> &newGen1,std::vector<float> &newGen2)
{
    unsigned int random =  1 + _randEngine() % (genomsize() - 2);
    newGen1 = oldGen1;
    newGen2 = oldGen2;
    for(unsigned int a=0; a<random; a++)
    {
        newGen1[a] = oldGen2[a];
        newGen2[a] = oldGen1[a];
    }
}
void                    GeneticNet::learn_mutate(std::vector<float> &genom)
{
    if(_mutationFactor <= __FLOATINPOINT_TOLERANCE || _mutationChangeWeight <= __FLOATINPOINT_TOLERANCE)
        return;
    for(unsigned int a=0; a<genomsize(); a++)
    {
        if(_randEngine() % (unsigned int)(1/_mutationFactor) == 0)
        {
            unsigned int random = _randEngine();
            float ran = (float)(random % 2000);
            ran = ran-1000;
            ran = ran/1000;
            genom[a] += _mutationChangeWeight * ran;
        }
    }
}
//----------ERROR

std::string GeneticNet::error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max)
{
    return " parameter "+std::to_string(paramPos)+" is out of range: "+value+"     minimum is: "+min+"     maximum is: "+max;
}
std::string GeneticNet::error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string GeneticNet::error_paramOutOfRange(unsigned int paramPos,int value,int min, int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string GeneticNet::error_paramOutOfRange(unsigned int paramPos,float value,float min, float max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
void        GeneticNet::error_general(std::string function, std::string cause)
{
    throw std::runtime_error("ERROR: GeneticNet::" + function + "     " + cause);
}
void        GeneticNet::error_general(std::string function, std::runtime_error &e)
{
    error_general(function,"",e);
}
void        GeneticNet::error_general(std::string function, std::string cause, std::runtime_error &e)
{
    std::string error = "ERROR: GeneticNet::" + function + "     " + cause;
    error += "     --> "+std::string(e.what());
    throw std::runtime_error(error);
}
