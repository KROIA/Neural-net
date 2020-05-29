#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    net = nullptr;

    input = 2;
    hiddenX = 2;
    hiddenY = 2;
    output = 1;
    ui->hiddenX_spinBox->setValue(hiddenX);
    ui->hiddenY_spinBox->setValue(hiddenY);
    //ui->comboBox->setCurrentIndex(0);
    activation = Activation::Gaussian;

    net = new BackpropNet(0,input,hiddenX,hiddenY,output,true,false,activation); //Makes the Net object
    connect(net,&BackpropNet::errorOccured,this,&MainWindow::onError);
    setupNet();

    timer = new QTimer(this);
   // connect(timer,&QTimer::timeout,this,&MainWindow::timeout);
  //  timer->start(10);
   // ui->speed_slider->setValue(100-timer->interval());

    learnTimer = new QTimer(this);
    connect(learnTimer,&QTimer::timeout,this,&MainWindow::timeout);
    learnTimer->start(0);

    /*trainingsSet.push_back({-1,-1});outputSet.push_back({-1});
    trainingsSet.push_back({-1,1});outputSet.push_back({1});
    trainingsSet.push_back({1,-1});outputSet.push_back({1});
    trainingsSet.push_back({1,1});outputSet.push_back({-1});*/

    setPos = 0;
    sycles = 0;
    averageError = 0;
    learnMode = true;
    learnSize = 200;
    testingSize = 200;

    trainImage      = new QImage(learnSize,learnSize,QImage::Format::Format_RGB16);
    testImage       = new QImage(testingSize,testingSize,QImage::Format::Format_RGB16);
    sollImage       = new QImage(200,200,QImage::Format::Format_RGB16);
    trainPicLabel   = new QLabel(this);
    testLabel       = new QLabel(this);
    sollLabel       = new QLabel(this);
    trainPicLabel->show();
    trainPicLabel->setGeometry(150,50,300,300);
    testLabel->show();
    testLabel->setGeometry(trainPicLabel->geometry().x()+trainPicLabel->geometry().width()+100,
                           trainPicLabel->geometry().y(),
                           600,600);
    sollLabel->show();
    sollLabel->setGeometry(trainPicLabel->geometry().x(),trainPicLabel->geometry().y()+trainPicLabel->geometry().height()+50,300,300);


    for(int x=-learnSize/2; x<learnSize/2; x++)
    {
        for(int y=-learnSize/2; y<learnSize/2; y++)
        {
            trainImage->setPixelColor(x+learnSize/2,y+learnSize/2,getValueColor(0));
        }
    }
    for(int x=-testingSize/2; x<testingSize/2; x++)
    {
        for(int y=-testingSize/2; y<testingSize/2; y++)
        {

            testImage->setPixelColor(x+testingSize/2,y+testingSize/2,getValueColor(0));
        }
    }
    for(int x=0; x<sollImage->width(); x++)
    {
        for(int y=0; y<sollImage->height(); y++)
        {
            sollImage->setPixelColor(x,y,getValueColor(0));

        }
    }
    setupTrainingSet();
    visu = new NetVisu(net);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::timeout()
{

    trainNet();
    if(sycles % displayInterval == 0)
        testNet();


}
void MainWindow::onError(unsigned int netID,Error &e)
{
    Error::print(e);
}

void MainWindow::on_pushButton_clicked()
{
    learnMode = !learnMode;
    if(!learnMode){
        //qDebug() << "stop";
        learnTimer->stop();
    }
    else if(learnMode){
       // qDebug() << "start";
        learnTimer->start();
    }
}
void MainWindow::trainNet()
{
   // qDebug() << "trainNet start";
    net->set_input(trainingsSet[setPos]);
    net->run();
    outputList = net->get_output();
    net->set_expected(outputSet[setPos]);
    averageError += abs(net->get_netError());

    net->learn();

    //trainImage->setPixelColor(trainingsSet[setPos][0]+learnSize/2,trainingsSet[setPos][1]+learnSize/2,getValueColor(outputList[0]));
   // if(sycles % displayInterval == 0)
        trainImage->setPixelColor(int(mapD(trainingsSet[setPos][0],-1,1,0,learnSize-1)),int(mapD(trainingsSet[setPos][1],-1,1,learnSize-1,0)),getValueColor(outputList[0]));
        setPos++;
        sycles++;
    //qDebug() << setPos << trainingsSet.size();
    if(setPos >= trainingsSet.size())
    {
       // if(sycles % displayInterval == 0)
            trainPicLabel->setPixmap(QPixmap::fromImage(trainImage->scaled(300,300)));
        setPos = 0;
     //   qDebug() << "error: "<<averageError;
        ui->error_label->setText(QString::number(averageError));
        averageError = 0;
        testNet();
    }
   // qDebug() << "trainNet end";
}
void MainWindow::testNet()
{
    //qDebug() << "testNet start";
    double inputX;
    double inputY;
    for(int x=-testingSize/2; x<testingSize/2; x++)
    {
        for(int y=-testingSize/2; y<testingSize/2; y++)
        {
            double zoom = double(ui->zoom_slider->value())/double(1000);
            inputX = mapD(double(x),-testingSize/2,testingSize/2,-zoom,zoom);
            inputY = mapD(double(y),-testingSize/2,testingSize/2,-zoom,zoom);

            net->set_input({inputX,inputY});
            net->run();
            outputList = /*{inputX+inputY};*/net->get_output();
           // net->set_expected(outputSet[setPos]);
            testImage->setPixelColor(x+testingSize/2,(testingSize-1)-(y+testingSize/2),getValueColor(outputList[0]));

        }
    }
    testLabel->setPixmap(QPixmap::fromImage(testImage->scaled(600,600)));
   // qDebug() << "testNet end";
}
double MainWindow::mapD(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
QColor MainWindow::getValueColor(double value)
{
    value = (value-double(-ui->colorOffset_slider->value())/double(1000)-2 )*double(ui->color_slider->value())/double(1000);
    QColor color;
    if(value < -1)
        value = -1;
    else if(value > 1)
        value = 1;

    if(value < 0)
        color.setRed(int(mapD(-value,0,1,0,255)));
    else
        color.setGreen(int(mapD(value,0,1,0,255)));

    return color;
}

void MainWindow::on_mutation_slider_valueChanged(int value)
{
    net->set_mutationFactor(double(value)/1000);
    qDebug() << "mutFac: "<<net->get_mutationFactor();
}

void MainWindow::on_speed_slider_valueChanged(int value)
{
   // timer->setInterval(100-value);
    displayInterval = value;
}

void MainWindow::on_randomGenom_pushButton_clicked()
{
    net->set_randomGenom();
}

void MainWindow::on_hiddenX_spinBox_valueChanged(int arg1)
{

    hiddenX = arg1;
    setupNet();
}

void MainWindow::on_hiddenY_spinBox_valueChanged(int arg1)
{
    hiddenY = arg1;
    setupNet();
}
void MainWindow::setupNet()
{
   /* if(net != nullptr)
    {
        if(hiddenX == net->get_hiddenNeuronsX() &&
           hiddenY == net->get_hiddenNeuronsY() &&
           activation == net->get_activationFunction())
        {
            return;
        }
        delete net;
    }*/
    if(net == nullptr)
        return;
    sycles = 0;
    setPos = 0;
    averageError = 0;

    //net->set_seed(0);
   // net = new BackpropNet(0,input,hiddenX,hiddenY,output,true,false,activation); //Makes the Net object
    net->set_ID(0);
    net->set_inputNeurons(input);
    net->set_hiddenNeuronsX(hiddenX);
    net->set_hiddenNeuronsY(hiddenY);
    net->set_outputNeurons(output);
    net->set_bias(true);
    net->set_enableAverage(false);
    net->set_activationFunction(activation);
    net->set_mutationFactor(0.01);
    ui->mutation_slider->setValue(net->get_mutationFactor()*1000);
    net->updateNetConfiguration();
}
void MainWindow::setupTrainingSet()
{
    /*trainingsSet.push_back({-1,-1});outputSet.push_back({-1});
    trainingsSet.push_back({-1,1});outputSet.push_back({1});
    trainingsSet.push_back({1,-1});outputSet.push_back({1});
    trainingsSet.push_back({1,1});outputSet.push_back({-1});*/
    for(int a=0; a<314; a++)
    {
        trainingsSet.push_back({sin(double(a*2)/100),cos(double(a*2)/100)});
    }
    for(int a=0; a<200; a++)
    {
        trainingsSet.push_back({double(a-99)/100,0});
        trainingsSet.push_back({0,double(a-99)/100});
        trainingsSet.push_back({double(a-99)/100,double(a-99)/100});
        trainingsSet.push_back({double(a-99)/200,-double(a-99)/200});
    }


    for(unsigned int a=0; a<trainingsSet.size(); a++)
    {

        //outputSet.push_back({trainingsSet[a][0]*trainingsSet[a][1]/*qFunc(trainingsSet[a][0])+qFunc(trainingsSet[a][1])*/});
        outputSet.push_back({qFunc(trainingsSet[a][0])+qFunc(trainingsSet[a][1])});
        sollImage->setPixelColor((int)mapD(trainingsSet[a][0],-2,2,0,200),(int)mapD(trainingsSet[a][1],-2,2,200,0),getValueColor(outputSet[a][0]));
    }

    sollLabel->setPixmap(QPixmap::fromImage(sollImage->scaled(300,300)));
   // sollImage->setPixelColor(x+testingSize/2,(testingSize-1)-(y+testingSize/2),getValueColor(outputList[0]));
}
double MainWindow::qFunc(double x)
{
    double _a = -1;
    double _b = 0;
    double _c = 0.3;
    return _a*x*x + _b*x + _c;
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            activation = Activation::Gaussian;
            break;
        case 1:
            activation = Activation::Sigmoid;
            break;
        case 2:
            activation = Activation::Linear;
            break;
        case 3:
            activation = Activation::ReLu;
            break;
    }
    setupNet();
    //qDebug() << net->get_activationFunction() << " "<<activation;
}
