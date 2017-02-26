#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QScrollBar>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Prepare UI
    ui->setupUi(this);

    // Prepare UI
    vm = new VM();
    vm->init("fs/challenge.bin");

    for (int var = 0; var < vm->m_memory.size(); ++var)
    {
         ui->listMemory->addItem(QString("%1: %2%3")
                                .arg(var)
                                .arg((vm->isRegister(vm->m_memory.at(var)) ? "R" : ""))
                                .arg(vm->value(vm->m_memory.at(var))));
    }

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));

    // Timer
    timerVm = new QTimer(this);
    connect(timerVm, SIGNAL(timeout()), this, SLOT(autostep()));
    timerVm->start();

    timerUi = new QTimer(this);
    connect(timerUi, SIGNAL(timeout()), this, SLOT(drawUI()));
    timerUi->start(100);

    ui->lineEdit->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawUI()
{
        // Labels
        ui->labelPC->setText(QString("%1").arg(vm->m_pc));
        ui->labelCount->setText(QString("%1").arg(vm->m_count));

        ui->textBrowser->clear();
        QString out;
        for (int var = 0; var < vm->m_outDeque.size(); ++var) {
            out += vm->m_outDeque.at(var);
        }
        ui->textBrowser->setText(out);

        auto sb = ui->textBrowser->verticalScrollBar();
        sb->setValue(sb->maximum());

        // Memory
        ui->listMemory->setCurrentRow(vm->m_pc);

        // Registers
        ui->listRegisters->clear();
        for (int var = 0; var < 8; ++var)
        {
            ui->listRegisters->addItem(QString("%1 : %2%3")
                                       .arg(var)
                                       .arg((vm->isRegister(vm->m_memory.at(var)) ? "R" : ""))
                                       .arg(vm->value(vm->getRegister(var)))
                                       );
        }

        // Stack
        ui->listStack->clear();
        auto stack = vm->m_stack;
        for (int var = 0; var < stack.size(); ++var) {
            ui->listStack->addItem(QString("%1")
                                   .arg(stack.top()));
            stack.pop();
}

}

void MainWindow::on_pushButton_clicked()
{
    vm->step();
    drawUI();
}

void MainWindow::autostep()
{
    if (vm->isRunning)
    {
        vm->step();
    }
}

void MainWindow::on_lineEdit_returnPressed()
{
    std::string contents = ui->lineEdit->text().toStdString();

    ui->lineEdit->clear();
    vm->sendInput(contents);
}

void MainWindow::on_pushButton_2_clicked()
{
    vm->isRunning = !vm->isRunning;
}

void MainWindow::on_pushButton_3_clicked()
{
    vm->hackTeleporter();

}

void MainWindow::on_pushButton_4_clicked()
{
     vm->autoPlay2();
}

void MainWindow::on_pushButton_5_clicked()
{
    vm->autoPlay1();
}

void MainWindow::on_pushButton_6_clicked()
{
    vm->sendInput("north");
}

void MainWindow::on_pushButton_7_clicked()
{
    vm->sendInput("west");
}

void MainWindow::on_pushButton_8_clicked()
{
    vm->sendInput("east");
}

void MainWindow::on_pushButton_9_clicked()
{
    vm->sendInput("south");
}

void MainWindow::on_pushButton_10_clicked()
{
    QString filename = "Data.txt";
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite))
    {
        file.resize(0);
        QTextStream stream(&file);
        stream << vm->m_outputBuffer.c_str() << endl;
    }
}
