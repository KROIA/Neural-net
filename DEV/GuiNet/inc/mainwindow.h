#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <net.h>
#include <geneticnet.h>
#include <backpropnet.h>
#include <netvisu.h>

#include <QDebug>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        void sinusTestForWeights(std::vector<double*> &genom);

    private slots:
        void loop();

    private:

        //Global Params
        unsigned int sinTest_weightIndex;
        double       sinTest_angle;


        Ui::MainWindow *ui;

        // Standard Net
        unsigned int net_ID;
        unsigned int net_inputs;
        unsigned int net_hiddenX;
        unsigned int net_hiddenY;
        unsigned int net_outputs;
        bool         net_enableBias;
        bool         net_enableAverage;
        Activation   net_activation;

        Net         *ptr_net;
        NetVisu     *ptr_net_visu;
        std::vector<double*> *ptr_net_genom;

        // Backprop Net
        BackpropNet *ptr_backpropNet;
        NetVisu     *ptr_backpropNet_visu;

        // Genetic Net
        GeneticNet  *ptr_geneticNet;
        NetVisu     *ptr_geneticNet_visu;

        QTimer *ptr_timer_loop;
        unsigned int timer_loop_interval;

};

#endif // MAINWINDOW_H
