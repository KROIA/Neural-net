#include "neuron.h"

unsigned int Neuron::_globalNeurons = 0;

#ifdef QT_APP
Neuron::Neuron(QObject *parent)
    : QObject (parent)
#else
Neuron::Neuron()
#endif
{
    init(NEURON_MIN_INPUTS,Activation::Sigmoid,false);
}

#ifdef QT_APP
Neuron::Neuron(unsigned int inputs,
               QObject *parent)
    : QObject (parent)
#else
Neuron::Neuron(unsigned int inputs)
#endif
{
    init(inputs,Activation::Sigmoid,false);
}

#ifdef QT_APP
Neuron::Neuron(unsigned int inputs,
               Activation activationFunction,
               QObject *parent)
    : QObject (parent)
#else
Neuron::Neuron(unsigned int inputs,
               Activation activationFunction)
#endif
{
    init(inputs,activationFunction,false);
}

#ifdef QT_APP
Neuron::Neuron(unsigned int inputs,
               Activation activationFunction,
               bool enableAverage,
               QObject *parent)
    : QObject (parent)
#else
Neuron::Neuron(unsigned int inputs,
               Activation activationFunction,
               bool enableAverage)
#endif
{
    init(inputs,activationFunction,enableAverage);
}

Neuron::~Neuron()
{
   _globalNeurons--;
   _weightList.clear();
   _ptr_inputList.clear();
   _inputConnectionList.clear();
   _inputConnectionID_list.clear();
   _inputConnectionDirection_List.clear();
}
void Neuron::set_ID(NeuronID ID)
{
    _ID = ID;
}
void Neuron::set_ID(unsigned int ID)
{
    _ID.ID = ID;
}
void Neuron::set_TYPE(NeuronType TYPE)
{
    _ID.TYPE = TYPE;
}
NeuronID Neuron::get_ID()
{
    return _ID;
}


void Neuron::set_inputs(unsigned int inputs)
{
#if NEURON_MIN_INPUTS == 0
    if(inputs > NEURON_MAX_INPUTS)
#else
    if(inputs < NEURON_MIN_INPUTS || inputs > NEURON_MAX_INPUTS)
#endif
    {
        addError(Error("set_inputs(unsigned int ["+std::to_string(inputs)+"] )",
                 ErrorMessage::outOfRange<unsigned int>('[',static_cast<unsigned int>(NEURON_MIN_INPUTS),inputs,static_cast<unsigned int>(NEURON_MAX_INPUTS),']')));
        return;
    }
    if(inputs != _inputs)
    {
        if(inputs > _inputs)
        {
            try{
                _weightList.reserve(inputs);
                _inputConnectionList.reserve(inputs);
                _ptr_inputList.reserve(inputs);
                _inputConnectionID_list.reserve(inputs);
                _inputConnectionDirection_List.reserve(inputs);
            }
            catch (std::exception &e) {
                addError(Error("inputs(unsigned int ["+std::to_string(inputs)+"])",std::string(e.what())));
                return;
                //error_general("inputs(unsigned int ["+std::to_string(inputs)+"])",e.what());
            }
        }
        while(inputs > _inputs)
        {
            try {
                _ptr_inputList.push_back(nullptr);
                _inputConnectionList.push_back(false);
                _weightList.push_back(0);
                NeuronID id;
                id.ID = NEURON_ID_INVALID;
                id.TYPE = NeuronType::none;
                _inputConnectionID_list.push_back(id);
                _inputConnectionDirection_List.push_back(ConnectionDirection::forward);
                _inputs++;
                set_randWeight(_inputs-1);
            } catch (std::exception &e) {
                addError(Error("inputs(unsigned int ["+std::to_string(inputs)+"])",std::string(e.what())));
                return;
            }catch (...) {
                addError(Error("inputs(unsigned int ["+std::to_string(inputs)+"])","unnkown"));
                return;
            }
        }
        while(inputs < _inputs)
        {
            try{
                deleteInput(_inputs-1);
            } catch (std::exception &e) {
                //error_general("inputs(unsigned int ["+std::to_string(inputs)+"])",e.what());
                addError(Error("inputs(unsigned int ["+std::to_string(inputs)+"])",std::string(e.what())));
                return;
            }catch (...) {
                addError(Error("inputs(unsigned int ["+std::to_string(inputs)+"])","unnkown"));
                return;
               // error_general("inputs(unsigned int ["+std::to_string(inputs)+"])","unnkown");
            }
        }
        _needsCalculationUpdate     = true;
    }
}
unsigned int Neuron::get_inputs()
{
    return _inputs;
}
int Neuron::deleteInput(unsigned int input)
{
    if(input >= _inputs)
    {
        addError(Error("deleteInput(usnigned int ["+std::to_string(input)+"] )",
                 ErrorMessage::outOfRange<unsigned int>('[',0,input,static_cast<unsigned int>(_inputs-1),']')));
        /*
#ifdef _DEBUG_NEURON_DELETE_INPUT
        __DEBUG_NEURON(this,"deleteInput(unsigned int ["+std::to_string(input)+"])",error_paramOutOfRange((unsigned int)0,(unsigned int)input,(unsigned int)0,(unsigned int)_inputs-1));
#endif*/
        return 2;
    }
    try{
        _ptr_inputList.erase(_ptr_inputList.begin()+static_cast<int>(input));
        _inputConnectionList.erase(_inputConnectionList.begin()+static_cast<int>(input));
        _weightList.erase(_weightList.begin()+static_cast<int>(input));
        _inputConnectionID_list.erase(_inputConnectionID_list.begin()+static_cast<int>(input));
        _inputConnectionDirection_List.erase(_inputConnectionDirection_List.begin()+static_cast<int>(input));
        _inputs--;
    } catch (std::exception &e) {
        //error_general("deleteInput(unsigned int ["+std::to_string(input)+"])",e.what());
        addError(Error("deleteInput(unsigned int ["+std::to_string(input)+"] )",std::string(e.what())));
        return 0;
    }
    return 1;
}

void Neuron::set_activationFunction(Activation activationFunction)
{
    if(activationFunction >= neuron_activationFunctionAmount)
    {
        addError(Error("activationFunction(Activation ["+Neuron::toActivationString(activationFunction)+"] )",
                 ErrorMessage::outOfRange<unsigned int>('[',0,activationFunction,static_cast<unsigned int>(neuron_activationFunctionAmount-1),']')));
        return;
       // error_general("activationFunction(Avtivation ["+toActivationString(activationFunction)+"] )",error_paramOutOfRange((unsigned int)0,(unsigned int)activationFunction,(unsigned int)0,(unsigned int)neuron_activationFunctionAmount));
    }
    _activationFunction = activationFunction;
    _needsCalculationUpdate             = true;
}
Activation Neuron::get_activationFunction()
{
    return _activationFunction;
}

void Neuron::set_enableAverage(bool enableAverage)
{
    if(_enableAverage != enableAverage)
    {
        _enableAverage = enableAverage;
        _needsCalculationUpdate        = true;
    }
}
bool Neuron::get_enableAverage()
{
    return _enableAverage;
}
double Neuron::get_calcRandWeight(std::default_random_engine &randEngine)
{
    double val  = double((randEngine() %2000))/double(1000) - double(1);
    if(val > 1 || val < -1)
    {
        CONSOLE<< val;
    }
    return val;
}
void Neuron::set_randWeight()
{
    for(unsigned int a=0; a<_inputs; a++)
    {
        set_randWeight(a);
    }
}
void Neuron::set_randWeight(unsigned int input)
{
    this->set_weight(input,get_calcRandWeight(_randEngine));
}
void Neuron::set_weight(unsigned int pos, double weight)
{
    if(pos >= _inputs)
    {
        addError(Error("weight",
                 ErrorMessage::outOfRange<unsigned int>('[',0,pos,static_cast<unsigned int>(_inputs-1),']')));
        return;
        //error_general("weight(unsigned int ["+std::to_string(pos)+"] , double ["+std::to_string(weight)+"] )",error_paramOutOfRange((unsigned int)0,pos,(unsigned int)0,_inputs-1));
    }
    _weightList[pos] = weight;
}
void Neuron::set_weight(std::vector<double>  weightList)
{
    if(weightList.size() != _inputs)
    {
        addError(Error("set_weight(std::vector<double>)",
                        {"parameter 0 has the wrong size: "+std::to_string(weightList.size()),
                         " Correct size is "+std::to_string(_inputs)}));
        return;
        //error_general("weight(std::vector<double>)","parameter 0 has the wrong size: "+std::to_string(weightList.size())+" Correct size is "+std::to_string(_inputs));
    }
    _weightList = weightList;
    _needsCalculationUpdate     = true;
}
void Neuron::set_weight(NeuronID ID,double weight)
{
    for(unsigned int a=0; a<_inputConnectionID_list.size(); a++)
    {
        if(_inputConnectionID_list[a].ID == ID.ID && _inputConnectionID_list[a].TYPE == ID.TYPE)
        {
            this->set_weight(static_cast<unsigned int>(a),weight);
            return;
        }
    }
    addError(Error("set_weight("+toIDString(ID)+",double ["+std::to_string(weight)+"])","No input with such an ID"));
    //error_general("weight(NeuronID [.ID="+std::to_string(ID.ID)+",.TYPE="+Neuron::toTypeString(ID.TYPE)+"],double ["+std::to_string(weight)+"])","No input with such an ID");
}

double Neuron::get_weight(unsigned int input)
{
    if(input >= _inputs)
    {
        addError(Error("get_weight(unsigned int ["+std::to_string(input)+"] )",
                 ErrorMessage::outOfRange<unsigned int>('[',0,input,static_cast<unsigned int>(_inputs-1),']')));
        return 0;
        //error_general("weight(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    return _weightList[input];
}
std::vector<double> Neuron::get_weight()
{
    return _weightList;
}

void Neuron::set_input(unsigned int input, double value)
{
    if(input >= _inputs)
    {
        addError(Error("set_input(unsigned int ["+std::to_string(input)+"] , double ["+std::to_string(value)+"] )",
                 ErrorMessage::outOfRange<unsigned int>('[',0,input,static_cast<unsigned int>(_inputs-1),']')));
        return;
       //error_general("input(unsigned int ["+std::to_string(input)+"] , double ["+std::to_string(value)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    if(_ptr_inputList[input] == nullptr)
    {
        addError(Error("set_input(unsigned int ["+std::to_string(input)+"] , double ["+std::to_string(value)+"] )",
                       "Input: "+std::to_string(input)+" is a nullpr, the Input have to be connected first"));
    }
    try {
        *_ptr_inputList[input] = value;
    } catch (std::runtime_error &e) {
        addError(Error("set_input(unsigned int ["+std::to_string(input)+"] , double ["+std::to_string(value)+"] )",
                       {"input: "+std::to_string(input)+" of Neuron is not assigned to another Neuron or variable. ",
                        std::string(e.what())}));
        return;
        //error_general("input(unsigned int ["+std::to_string(input)+"] , double ["+std::to_string(value)+"] )","input: "+std::to_string(input)+" of Neuron is not assigned to another Neuron or variable. "+e.what());
    }
    _needsCalculationUpdate         = true;
}
void Neuron::set_input(std::vector<double> inputList)
{
    if(inputList.size() != _inputs)
    {
        addError(Error("set_input(std::vector<double>)",
                        {"parameter 0 has the wrong size: "+std::to_string(inputList.size()),
                         " Correct size is "+std::to_string(_inputs)}));
        return;
        //error_general("input(std::vector<double>)","parameter 0 has the wrong size: "+std::to_string(inputList.size())+" Correct size is "+std::to_string(_inputs));
    }
    //try{
    for(unsigned int a=0; a<_ptr_inputList.size(); a++)
    {
        set_input(static_cast<unsigned int>(a),inputList[a]);
    }
    /*}catch(std::runtime_error &e)
    {
         error_general("input(std::vector<double>)",e);
    }*/
    _needsCalculationUpdate     = true;
}
double Neuron::get_input(unsigned int input)
{
    if(input >= _inputs)
    {
        addError(Error("get_input(unsigned int ["+std::to_string(input)+"] )",
                 ErrorMessage::outOfRange<unsigned int>('[',0,input,static_cast<unsigned int>(_inputs-1),']')));
        return 0;
        //error_general("weight(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    //try {
        return *_ptr_inputList[input];
    /*}catch(std::runtime_error &e)
    {
        addError(Error("get_input(unsigned int ["+std::to_string(input)+"] )",
                 {"weight(unsigned int ["+std::to_string(input)+"] )",
                  "no input connected. ",
                  std::string(e.what())}));
        //error_general("weight(unsigned int ["+std::to_string(input)+"] )","no input connected. ",e);
    }*/
    //return 0;
}
std::vector<double> Neuron::get_input()
{
    std::vector<double> retVal(_inputs);
    for(unsigned int a=0; a<_inputs; a++)
    {
       // try {
            retVal[a] = *_ptr_inputList[a];
       /* } catch (std::runtime_error &e) {
            addError(Error("get_input()",std::string(e.what())));
            //__DEBUG_NEURON(this,"std::vector<double> Neuron::input()",e.what());
        }*/
    }
    return retVal;
}

bool Neuron::connectInput(NeuronID ID, double *ptr_sourceNeuronOutput,ConnectionDirection direction)
{
    bool allFull = true;
    for(unsigned int a=0; a<_inputs; a++)
    {
        if(_inputConnectionList[a] == false)
        {
            allFull = false;
            return connectInput(static_cast<unsigned int>(a),ID,ptr_sourceNeuronOutput,direction);
        }else if(_inputConnectionID_list[a].ID == ID.ID && _inputConnectionID_list[a].TYPE == ID.TYPE)
        {
            addError(Error("connectInput("+Neuron::toIDString(ID)+" , double *ptr_sourceNeuronOutput , ConnectionDirection ["+
                           toDirectionString(direction)+"] )"+
                           " Connection: " + std::to_string(ID.ID) +" "+this->toTypeString(ID.TYPE)+
                           " to: "+std::to_string(this->get_ID().ID)+" "+this->toTypeString(this->get_ID().TYPE)+" already exists."));
/*#ifdef _DEBUG_NEURON_CONNECT
            __DEBUG_NEURON(this,"connectInput(NeuronID ["+Neuron::toIDString(ID)+"] , double *ptr_sourceNeuronOutput , ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                           "Connection: " + std::to_string(ID.ID) +" "+this->toTypeString(ID.TYPE)+" to: "+std::to_string(this->ID().ID)+" "+this->toTypeString(this->ID().TYPE)+" already exists.");
#endif*/
            return false;
        }
    }
    if(allFull)                         // If all Inputs are used, create a new one and connect to it.
    {
        set_inputs(_inputs+1);
        return connectInput(_inputs-1,ID,ptr_sourceNeuronOutput,direction);
    }
    return false;
}
bool Neuron::connectInput(unsigned int input, NeuronID ID, double *ptr_sourceNeuronOutput,ConnectionDirection direction)
{
    if(input >= _inputs)
    {
        addError(Error("connectInput(unsigned int ["+std::to_string(input)+"] , "+Neuron::toIDString(ID)+" , double *ptr_sourceNeuronOutput , ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                 ErrorMessage::outOfRange<unsigned int>('[',0,input,static_cast<unsigned int>(_inputs-1),']')));
        return false;
        //error_general("connectInput(unsigned int ["+std::to_string(input)+"],NeuronID ["+std::to_string(ID.ID)+","+Neuron::toTypeString(ID.TYPE)+"], double [ptr])",error_paramOutOfRange(0,std::to_string(input),"0",std::to_string(_inputs-1)));
    }
    if(ptr_sourceNeuronOutput == nullptr)
    {
        addError(Error("connectInput(unsigned int ["+std::to_string(input)+"] , "+Neuron::toIDString(ID)+" , double *ptr_sourceNeuronOutput , ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                 "pointer to the Neuron doesn't exist"));
        return false;
        //error_general("connectInput(unsigned int ["+std::to_string(input)+"],NeuronID ["+std::to_string(ID.ID)+","+Neuron::toTypeString(ID.TYPE)+"], double [nullptr]","pointer to the Neuron doesn't exist");
    }
    _inputConnectionList[input]             = true;
    _inputConnectionID_list[input]          = ID;
    _inputConnectionDirection_List[input]   = direction;
    _ptr_inputList[input]                   = ptr_sourceNeuronOutput;
    return true;
}
bool Neuron::connectInput(Neuron *ptr_neuron,ConnectionDirection direction)
{
    bool allFull = true;
    for(unsigned int input=0; input<_inputs; input++)
    {
        if(_inputConnectionList[input] == false)
        {
            allFull = false;
            return connectInput(input,ptr_neuron,direction);
        }else if(_inputConnectionID_list[input].ID      == ptr_neuron->get_ID().ID &&
                 _inputConnectionID_list[input].TYPE    == ptr_neuron->get_ID().TYPE)
        {
            addError(Error("connectInput(Neuron *ptr_neuron , ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                     "Connection: " + std::to_string(ptr_neuron->get_ID().ID)+" "+
                     this->toTypeString(ptr_neuron->get_ID().TYPE)+" to: "+std::to_string(this->get_ID().ID)+" "+
                     this->toTypeString(this->get_ID().TYPE)+" already exists."));
/*#ifdef _DEBUG_NEURON_CONNECT
            __DEBUG_NEURON(this,"connectInput(Neuron *ptr_neuron , ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                           "Connection: " + std::to_string(ptr_neuron->ID().ID)+" "+this->toTypeString(ptr_neuron->ID().TYPE)+" to: "+std::to_string(this->ID().ID)+" "+this->toTypeString(this->ID().TYPE)+" already exists.");
#endif*/
            return false;
        }
    }
    if(allFull)                         // If all Inputs are used, create a new one and connect to it.
    {
        set_inputs(_inputs+1);
        return connectInput(_inputs-1,ptr_neuron,direction);
    }
    return false;
}
bool Neuron::connectInput(unsigned int input,Neuron *ptr_neuron,ConnectionDirection direction)
{
    if(input >= _inputs)
    {
        addError(Error("connectInput(unsigned int ["+std::to_string(input)+"] , Neuron *ptr_neuron , ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                 ErrorMessage::outOfRange<unsigned int>('[',0,input,static_cast<unsigned int>(_inputs-1),']')));
        return false;
        //error_general("connectInput(unsigned int ["+std::to_string(input)+"],Neuron [ptr])",error_paramOutOfRange(0,std::to_string(input),"0",std::to_string(_inputs-1)));
    }
    if(ptr_neuron == nullptr)
    {
       // error_general("connectInput(unsigned int ["+std::to_string(input)+"],Neuron [nullptr]","pointer to the Neuron doesn't exist");
        addError(Error("connectInput(unsigned int ["+std::to_string(input)+"] , Neuron *ptr_neuron , ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                 "pointer to the Neuron doesn't exist"));
        return false;
    }
    for(unsigned int connection=0; connection<_inputConnectionID_list.size(); connection++)
    {
        if(_inputConnectionID_list[connection].ID     == ptr_neuron->get_ID().ID &&
           _inputConnectionID_list[connection].TYPE   == ptr_neuron->get_ID().TYPE)
        {
            addError(Error("connectInput(unsigned int ["+std::to_string(input)+"] , Neuron *ptr_neuron , ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                           "Connection: " + std::to_string(ptr_neuron->get_ID().ID)+" "+
                           this->toTypeString(ptr_neuron->get_ID().TYPE)+" to: "+std::to_string(this->get_ID().ID)+" "+
                           this->toTypeString(this->get_ID().TYPE)+" already exists."));
/*#ifdef _DEBUG_NEURON_CONNECT
            __DEBUG_NEURON(this,"connectInput(unsigned int ["+std::to_string(input)+"] , Neuron *ptr_neuron , ConnectionDirection ["+Neuron::toDirectionString(direction)+"] )",
                           "Trying to connect a connection which already exists. ignore command. Input: "+std::to_string(input)+" Neuron: "+std::to_string((unsigned int)ptr_neuron)+" direction: "+ Neuron::toDirectionString(direction));
#endif*/
            return false;
        }
    }
    _inputConnectionList[input]             = true;
    _inputConnectionID_list[input]          = ptr_neuron->get_ID();
    _inputConnectionDirection_List[input]   = direction;
    if(direction == ConnectionDirection::forward)
    {
        _ptr_inputList[input] = ptr_neuron->get_ptr_output();
    }else{
        _ptr_inputList[input] = ptr_neuron->get_ptr_loopBackOutput();
    }
    return true;
}
bool Neuron::disconnect(unsigned int input)
{
    if(input >= _inputs)
    {
        addError(Error("disconnect(unsigned int ["+std::to_string(input)+"] )",
                 ErrorMessage::outOfRange<unsigned int>('[',0,input,static_cast<unsigned int>(_inputs-1),']')));
        return false;

/*#ifdef _DEBUG_NEURON_CONNECT
        __DEBUG_NEURON(this,"disconnect(unsigned int ["+std::to_string(input)+"])",
                       error_paramOutOfRange(0,std::to_string(input),"0",std::to_string(_inputs-1)));
#endif*/
    }
    if(!_inputConnectionList[input])
        return false;
    _ptr_inputList[input]           = nullptr;
    _inputConnectionList[input]     = false;
    NeuronID id;
    id.ID = NEURON_ID_INVALID;
    id.TYPE = none;
    _inputConnectionID_list[input] = id;
    _inputs--;
    return true;
}
bool Neuron::disconnect(NeuronID sourceNeuron)
{
    for(unsigned int input=0; input<_inputConnectionID_list.size(); input++)
    {
        if(_inputConnectionID_list[input].ID    == sourceNeuron.ID &&
           _inputConnectionID_list[input].TYPE  == sourceNeuron.TYPE)
        {
            return disconnect(static_cast<unsigned int>(input));
        }
    }
/*#ifdef _DEBUG_NEURON_CONNECT
    __DEBUG_NEURON(this,"disconnect(NeuronID ["+Neuron::toIDString(sourceNeuron)+"])",
                   "No neuron with such an ID is connected to this neuron");
#endif*/
    addError(Error("disconnect( "+Neuron::toIDString(sourceNeuron)+" )",toIDString(sourceNeuron)+" No connection with such an ID"));
    return false;
}
ConnectionDirection Neuron::get_inputConnectionDirection(NeuronID inputID)
{
    for(unsigned int input=0; input<_inputConnectionID_list.size(); input++)
    {
        if(_inputConnectionID_list[input].ID == inputID.ID && _inputConnectionID_list[input].TYPE == inputID.TYPE)
        {
            return _inputConnectionDirection_List[input];
        }
    }

    //error_general("inputConnectionDirection(NeuronID [.ID="+std::to_string(inputID.ID)+",.TYPE="+Neuron::toTypeString(inputID.TYPE)+"])","No connection with such an ID");
    addError(Error("get_inputConnectionDirection( "+Neuron::toIDString(inputID) + " )",Neuron::toIDString(inputID)+" No connection with such an ID"));
    return ConnectionDirection::forward;
}
std::vector<ConnectionDirection>   Neuron::get_inputConnectionDirection()
{
    return _inputConnectionDirection_List;
}



double Neuron::get_netInput()
{
    return _netInput;
}
double Neuron::get_output()
{
    return _output;
}
double *Neuron::get_ptr_output()
{
    return &_output;
}
double *Neuron::get_ptr_loopBackOutput()
{
    return &_delayedOutput;
}
NeuronID Neuron::get_inputID(unsigned int input)
{
    if(input >= _inputs)
    {
        addError(Error("get_InputID(unsigned int ["+std::to_string(input)+"] )",ErrorMessage::outOfRange<unsigned int>('[',0,input,static_cast<unsigned int>(_inputs-1),']')));
        NeuronID id;
        id.ID = NEURON_ID_INVALID;
        id.TYPE = none;
        return id;
         //error_general("inputID(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    return _inputConnectionID_list[input];
}
std::vector<NeuronID> Neuron::get_inputIDList()
{
    return _inputConnectionID_list;
}
double *Neuron::get_ptr_weight(unsigned int input)
{
    if(input >= _inputs)
    {
        addError(Error("get_ptr_weight(unsigned int ["+std::to_string(input)+"] )",ErrorMessage::outOfRange<unsigned int>('[',0,input,static_cast<unsigned int>(_inputs-1),']')));
        return nullptr;
        // error_general("ptr_weight(unsigned int ["+std::to_string(input)+"] )",error_paramOutOfRange((unsigned int)0,input,(unsigned int)0,_inputs-1));
    }
    return &_weightList[input];
}
double *Neuron::get_ptr_weight(NeuronID  connectionID)
{
    for(unsigned int a=0; a<_inputConnectionID_list.size(); a++)
    {
        if(connectionID.ID      == _inputConnectionID_list[a].ID &&
           connectionID.TYPE    == _inputConnectionID_list[a].TYPE)
        {
            return get_ptr_weight(static_cast<unsigned int>(a));
        }
    }
    addError(Error("get_ptr_weight( "+Neuron::toIDString(connectionID)+" )",Neuron::toIDString(connectionID)+" No connection with such an ID"));
    return nullptr;
    //error_general("ptr_weight(NeuronID [.ID="+std::to_string(connectionID.ID)+",.TYPE="+Neuron::toTypeString(connectionID.TYPE)+"])","No connection with such an ID");
}


void Neuron::run()
{
    if(_needsCalculationUpdate == true)
    {
#if defined(_DEBUG_NEURON_RUN)
        __DEBUG_NEURON(this,"run()","begin");
#endif
#if defined(_DEBUG_NEURON_TIMING)
        __debug_timer1 = std::chrono::high_resolution_clock::now();
#endif
        calc_netInput();
        calc_output();
        _needsCalculationUpdate = false;
#if defined(_DEBUG_NEURON_TIMING)
        __debug_timer2 = std::chrono::high_resolution_clock::now();
        __debug_time_span = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer2 - __debug_timer1);
        __debug_run_time = __debug_time_span.count()*1000;
#endif
#if defined(_DEBUG_NEURON_RUN)
#if defined(_DEBUG_NEURON_TIMING)
        __DEBUG_NEURON(this,"run()","end, time: "+std::to_string(__debug_run_time)+" ms");
#else
        __DEBUG_NEURON(this,"run()","end");
#endif
#endif
    }
}
const std::string Neuron::toIDString(NeuronID ID)
{
    return "NeuronID { ID: "+std::to_string(ID.ID)+"\tTYPE: "+Neuron::toTypeString(ID.TYPE)+"}";
}
const std::string Neuron::toTypeString(NeuronType TYPE)
{
    std::string str = "["+std::to_string(TYPE)+"] ";
    switch(TYPE)
    {
        case NeuronType::none:
        {
            str = "none";
            break;
        }
        case NeuronType::input:
        {
            str = "input";
            break;
        }
        case NeuronType::hidden:
        {
            str = "hidden";
            break;
        }
        case NeuronType::output:
        {
            str = "output";
            break;
        }
        case NeuronType::costum:
        {
            str = "costum";
            break;
        }
        case NeuronType::bias:
        {
            str = "bias";
            break;
        }
        default:
        {
            str = "unknown";
            break;
        }
    }
    return str;
}
const std::string Neuron::toActivationString(Activation activationFunction)
{
    std::string str = "["+std::to_string(activationFunction)+"] ";
    switch(activationFunction)
    {
        case Activation::Linear:
        {
            str = "Linear";
            break;
        }
        case Activation::ReLu:
        {
            str = "ReLu";
            break;
        }
        case Activation::Binary:
        {
            str = "Binary";
            break;
        }
        case Activation::Gaussian:
        {
            str = "Gaussian";
            break;
        }
        case Activation::Sigmoid:
        {
            str = "Sigmoid";
            break;
        }
        default:
        {
            str = "unknown";
            break;
        }
    }
    return str;
}
const std::string Neuron::toConnectionString(Connection connection)
{
    std::string message;
    message  = "netID:  "+std::to_string(connection.netID)+"\n"+
               "SourceNeuron {\n"+
               "ID:     "+std::to_string(connection.source_ID.ID)+"\n"+
               "TYPE:   "+Neuron::toTypeString(connection.source_ID.TYPE)+"}\n"+
               "DestinationNeuron {\n"+
               "ID:     "+std::to_string(connection.destination_ID.ID)+"\n"+
               "TYPE:   "+Neuron::toTypeString(connection.destination_ID.TYPE)+"}\n"+
               "weight: "+std::to_string(connection.weight)+"\n"+
               "direction: "+Neuron::toDirectionString(connection.direction);
    return message;
}
const std::string Neuron::toDirectionString(ConnectionDirection dir)
{
    if(dir == ConnectionDirection::forward)
        return "forward";
    return "backward";
}
std::string       Neuron::toString()
{
    std::string str = "";
    std::vector<std::string> list = this->toStringList();
    size_t size = 0;
    for(size_t i=0; i<list.size(); i++)
        size += list[i].size();
    str.reserve(size);

    for(size_t i=0; i<list.size(); i++)
        str += list[i];
    return str;
}
std::vector<std::string> Neuron::toStringList()
{
    std::vector<std::string>    stringList;
    std::string line = "";
    const std::string spacer = "\t";
    const std::string newline = "\n";

    stringList.push_back("+-----------------------------------------------------+"+newline);
    stringList.push_back("| "+toIDString(this->_ID)+newline);

    char buff[50];
    line  = "+-----------------------------------------------------+"+newline;
    line += "|   Inputs | Activation | Is updated | Enable average |"+newline;
    sprintf(buff,"|%9i |%11s |%11s |%15s |",_inputs,toActivationString(this->_activationFunction).c_str(),
                                         (_needsCalculationUpdate==false?"TRUE":"FALSE"),
                                         (_enableAverage==true?"TRUE":"FALSE"));
    line += buff + newline +  "+-----------------------------------------------------+"+newline;
    /*
    line = "Inputs:"+spacer+std::to_string(_inputs)+spacer;
    line += "Activation:"+ spacer + toActivationString(this->_activationFunction)+spacer;
    if(!_needsCalculationUpdate)
        line+= "Is updated:"+spacer+"true"+spacer;
    else
        line+= "Is updated:"+spacer+"false"+spacer;

    if(_enableAverage)
        line+= "Enable average:"+spacer+"true"+spacer;
    else
        line+= "Enable average:"+spacer+"false"+spacer;*/
    stringList.push_back(line);

    line = "|   Weight: ";
    for(unsigned int input=0; input<_inputs; input++)
    {
        char buff[20];
        sprintf(buff,"%8.3f",_weightList[input]);
        line += buff;
        //line += std::to_string(_weightList[input])+spacer;
    }
    stringList.push_back(line+newline);

    line = "|    Value: ";
    for(unsigned int input=0; input<_inputs; input++)
    {
        char buff[20];
        sprintf(buff,"%8.3f",*_ptr_inputList[input]);
        line += buff;
        //line += std::to_string(*_ptr_inputList[input])+spacer;
    }
    stringList.push_back(line+newline);

    sprintf(buff,"| Netinput: %8.3f%s",_netInput,newline.c_str());
    stringList.push_back(buff);
    //stringList.push_back("Netinp:"+spacer+std::to_string(_netInput)+newline);

    sprintf(buff,"|   Output: %8.3f%sDelayed output:%s%8.3f%s",_output,spacer.c_str(),spacer.c_str(),_delayedOutput,newline.c_str());
    stringList.push_back(buff);
    stringList.push_back("+-----------------------------------------------------+"+newline);
   // line = "Output:"+spacer+std::to_string(_output)+spacer;
   // line+= "Delayed output:"+spacer+std::to_string(_delayedOutput)+spacer;
   // stringList.push_back(line+newline);


    return stringList;
}

void Neuron::needsUpdate()
{
    _delayedOutput = _output;
    _needsCalculationUpdate   = true;
}
bool Neuron::isUpdated()
{
    return !_needsCalculationUpdate;
}
void Neuron::calc_netInput()
{
#if defined(_DEBUG_NEURON_RUN)
    __DEBUG_NEURON(this,"calc_netInput()","begin");
#endif
    _netInput = 0;
    for(unsigned int a=0; a<_inputs; a++)
    {
        if(_ptr_inputList[a] == nullptr)
        {
#if defined(_DEBUG_NEURON_RUN)
            __DEBUG_NEURON(this,"calc_netInput()","input: "+std::to_string(a)+" is not connected");
#endif
            continue;
        }
        _netInput += *_ptr_inputList[a] * _weightList[a];
    }
    if(_enableAverage == true)
    {
        _netInput /= _inputs;
    }
#if defined(_DEBUG_NEURON_RUN)
    __DEBUG_NEURON(this,"calc_netInput()","end, netInput: "+std::to_string(_netInput));
#endif
}
void Neuron::calc_output()
{
#if defined(_DEBUG_NEURON_RUN)
    __DEBUG_NEURON(this,"calc_output()","begin, activation: "+Neuron::toActivationString(_activationFunction));
#endif
    switch(_activationFunction)
    {
        case Activation::Linear:
        {
            _output = this->activation_Linear(_netInput);
            break;
        }
        case Activation::ReLu:
        {
            _output = this->activation_ReLu(_netInput);
            break;
        }
        case Activation::Binary:
        {
            _output = this->activation_Binary(_netInput);
            break;
        }
        case Activation::Gaussian:
        {
            _output = this->activation_Gaussian(_netInput);
            break;
        }
        case Activation::Sigmoid:
        {
            _output = this->activation_Sigmoid(_netInput);
            break;
        }
        default:
        {
#if defined(_DEBUG_NEURON_RUN)
             __DEBUG_NEURON(this,"calc_output()","unknown activation function ["+std::to_string(_activationFunction)+"]");
#endif
            //error_general("calc_output()","unknown activation function ["+std::to_string(_activationFunction)+"]");
            addError(Error("calc_output()","unknown activation function ["+std::to_string(_activationFunction)+"]"));
        }
    }
#if defined(_DEBUG_NEURON_RUN)
    __DEBUG_NEURON(this,"calc_output()","end, output: "+std::to_string(_output));
#endif
}
#if defined(_DEBUG_NEURON_TIMING)
double Neuron::get_runtime()
{
    return __debug_run_time;
}
#endif
void Neuron::set_seed(unsigned int seed)
{
    _randEngine             = std::default_random_engine(seed);
}
//----------ERROR
void Neuron::clearErrors()
{
    _errorList.clear();
}
Error Neuron::get_lastError() const
{
    if(static_cast<unsigned int>(_errorList.size()) == 0)
    {
        return Error("get_LastError()",ErrorMessage::listIsEmpty("_errorList"));
    }
    return _errorList[_errorList.size()-1];
}
Error Neuron::get_error(unsigned int index)
{
    if(static_cast<unsigned int>(_errorList.size()) == 0)
    {
        return Error("get_Error(unsigend int ["+std::to_string(index)+"])",ErrorMessage::listIsEmpty("_errorList"));
    }
    if(index >= static_cast<unsigned int>(_errorList.size()))
    {
        return Error("get_Error(unsigend int ["+std::to_string(index)+"])",ErrorMessage::outOfRange<unsigned int>('[',0,index,static_cast<unsigned int>(_errorList.size()-1),']'));
    }
    return _errorList[static_cast<int>(index)];
}
ErrorList Neuron::get_errorList() const
{
    return _errorList;
}
unsigned int Neuron::get_errorAmount() const
{
    return static_cast<unsigned int>(_errorList.size());
}
void Neuron::init(unsigned int inputs, Activation activationFunction, bool enableAverage)
{
    _globalNeurons++;
    _inputs                 = 0;
    _netInput               = 0;
    _output                 = 0;
    _delayedOutput          = 0;
    _needsCalculationUpdate = true;
    time_t now              = time(nullptr);
    struct tm *currentTime  = localtime(&now);
    this->set_seed(unsigned(currentTime->tm_min+currentTime->tm_sec+currentTime->tm_sec+clock()));
    //_randEngine.seed(unsigned(currentTime->tm_min+currentTime->tm_sec+currentTime->tm_sec));
    NeuronID id;
    id.ID   = _globalNeurons;
    id.TYPE = NeuronType::none;
    this->set_ID(id);
    this->set_inputs(inputs);
    this->set_activationFunction(activationFunction);
    this->set_enableAverage(enableAverage);
   /* } catch (std::runtime_error &e) {
        //error_general("init(unsigned int ["+std::to_string(inputs)+"] , Activation ["+
        //              toActivationString(activationFunction)+"] , bool ["+std::to_string(enableAverage)+"])",e);
        addError(Error("init(unsigned int ["+std::to_string(inputs)+"] , Activation ["+
                       toActivationString(activationFunction)+"] , bool ["+std::to_string(enableAverage)+"])",
                       std::string(e.what())));
        return;
    }*/
}
void Neuron::addError(const Error &e)
{
    _errorList.push_back(e);
    _errorList[_errorList.size()-1].setNamespace("Neuron::"+_errorList[_errorList.size()-1].getNamespace());
    Error::print(_errorList[_errorList.size()-1]);
    #ifdef QT_APP
        emit errorOccured(this->get_ID(),_errorList[_errorList.size()-1]);
    #endif
}
/*
std::string Neuron::error_paramOutOfRange(unsigned int paramPos,std::string value,std::string min, std::string max)
{
    return " parameter "+std::to_string(paramPos)+" is out of range: "+value+"     minimum is: "+min+"     maximum is: "+max;
}
std::string Neuron::error_paramOutOfRange(unsigned int paramPos,unsigned int value,unsigned int min, unsigned int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string Neuron::error_paramOutOfRange(unsigned int paramPos,int value,int min, int max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
std::string Neuron::error_paramOutOfRange(unsigned int paramPos,double value,double min, double max)
{
    return error_paramOutOfRange(paramPos,std::to_string(value),std::to_string(min),std::to_string(max));
}
void        Neuron::error_general(std::string function, std::string cause)
{
    throw std::runtime_error("ERROR: Neuron::" + function + "     " + cause);
}
void        Neuron::error_general(std::string function, std::runtime_error &e)
{
    error_general(function,"",e);
}
void        Neuron::error_general(std::string function, std::string cause, std::runtime_error &e)
{
    std::string error = "ERROR: Neuron::" + function + "     " + cause;
    error += "     --> "+std::string(e.what());
    throw std::runtime_error(error);
}
*/
