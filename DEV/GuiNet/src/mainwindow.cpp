#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Global Parameters
    timer_loop_interval = 100; //100 ms for the loop
    sinTest_weightIndex = 0;
    sinTest_angle       = 0;


    // Timers
    ptr_timer_loop = new QTimer(this);
    connect(ptr_timer_loop,&QTimer::timeout,this,&MainWindow::loop);
    ptr_timer_loop->start(signed(timer_loop_interval));

    // Neural net's
    net_ID              = 0;
    net_inputs          = 4;
    net_hiddenX         = 1;
    net_hiddenY         = 1;
    net_outputs         = 1;
    net_enableBias      = true;
    net_enableAverage   = false;
    net_activation      = Activation::Sigmoid;

    ptr_net = new Net(  net_ID,net_inputs,net_hiddenX,net_hiddenY,net_outputs,
                        net_enableBias,net_enableAverage,net_activation,this);

    connect(ptr_net,&Net::errorOccured,this,&MainWindow::netErrorOccured);

    Connection con;
    con.netID = 0;
    con.direction = ConnectionDirection::backward;

    con.source_ID.ID = 1;
    con.source_ID.TYPE = NeuronType::hidden;

    con.destination_ID.ID = 0;
    con.destination_ID.TYPE = NeuronType::hidden;

    con.weight = 0.5;

    //ptr_net->addConnection(con);
    ptr_net->addConnection(Connection{.netID=0,
                                      .source_ID=NeuronID{.ID=0,.TYPE=NeuronType::input},
                                      .destination_ID=NeuronID{.ID=2,.TYPE=NeuronType::costum},
                                      .weight=0.5,
                                      .direction=ConnectionDirection::forward});

    ptr_net->addConnection(Connection{.netID=0,
                                      .source_ID=NeuronID{.ID=2,.TYPE=NeuronType::costum},
                                      .destination_ID=NeuronID{.ID=1,.TYPE=NeuronType::output},
                                      .weight=0.75,
                                      .direction=ConnectionDirection::forward});

    ptr_net->updateNetConfiguration();
    //ptr_net_visu = new NetVisu(ptr_net,this);
    //ptr_net_visu->loadNetInUi(ui->net_view_widget);
    //ptr_net_visu->setUpdateTime(timer_loop_interval);
    ptr_net_genom = ptr_net->get_ptr_genom();
    //ptr_net_visu->showWindow();

}

MainWindow::~MainWindow()
{
    /*delete ui;

    ptr_net_visu->deleteLater();
    delete ptr_net;
    ptr_backpropNet_visu->deleteLater();
    delete ptr_backpropNet;
    ptr_geneticNet_visu->deleteLater();
    delete ptr_geneticNet;

    ptr_timer_loop->deleteLater();*/
}


void MainWindow::loop()
{
    //qDebug() << "Loop start";

    ptr_net->set_input({1,1,1,1});
    sinusTestForWeights(*ptr_net_genom);
    ptr_net->run();

    //qDebug() << "Loop end";
}

void MainWindow::sinusTestForWeights(std::vector<double*> &genom)
{
    double angleIncrement = 6.283185307/40;

    if(sinTest_weightIndex == 0)
        *genom[genom.size()-1] = 1;
    else
        *genom[sinTest_weightIndex-1] = 1;


    if(genom.size() <= sinTest_weightIndex)
    {
        sinTest_weightIndex = 0;
    }

    *genom[sinTest_weightIndex] = sin(sinTest_angle+6.283185307/4);
    //qDebug() << *genom[sinTest_weightIndex];



    sinTest_angle = sinTest_angle+angleIncrement;
    if(sinTest_angle > 6.283185307)
    {
        sinTest_angle = 0;
        sinTest_weightIndex++;
        qDebug() << " sinTest_weightIndex: "<<sinTest_weightIndex;
    }
}
void MainWindow::netErrorOccured(unsigned int netID, Error &e)
{
    //qDebug() << "Error in Net ID: "<<netID;
    e.print();
}
