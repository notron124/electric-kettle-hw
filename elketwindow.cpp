#include "elketwindow.h"
#include <QMenuBar>
#include "kettlestatusdisplay.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>

ElKetWindow::ElKetWindow(QWidget *parent)
    : QMainWindow(parent)
{
//    QMenuBar& bar = *new QMenuBar(this);
//    bar.addMenu("Start simulation");
//    setMenuBar(&bar);
    {
        setCentralWidget(new QWidget(this));
        QHBoxLayout* mLay = new QHBoxLayout(centralWidget());
        centralWidget()->setLayout(mLay);
        m_ksd = new KettleStatusDisplay(this);
        centralWidget()->layout()->addWidget(m_ksd);
        {
            QWidget* rwid = new QWidget(centralWidget());///правый виджет
            centralWidget()->layout()->addWidget(rwid);
            auto parent = rwid;
            parent->setLayout(new QVBoxLayout(parent));

            auto btStart = new QPushButton(tr("Начать"), parent);
            connect(btStart, &QPushButton::clicked, m_ksd, &KettleStatusDisplay::run);
            parent->layout()->addWidget(btStart);

            auto btStop = new QPushButton(tr("Астанавить!"), parent);
            connect(btStop, &QPushButton::clicked, m_ksd, &KettleStatusDisplay::stop);
            parent->layout()->addWidget(btStop);

            auto cbHeat = new QCheckBox(tr("Нагрев!"), parent);
            connect(cbHeat, &QCheckBox::stateChanged, m_ksd, &KettleStatusDisplay::setHeating);
            connect(m_ksd, &KettleStatusDisplay::heatingChanged, cbHeat, &QCheckBox::setChecked);
            parent->layout()->addWidget(cbHeat);
            ///TODO наладить нагрев

            auto cbRandomEvents = new QCheckBox(tr("Случайные события"), parent);

            auto cbAutomatic = new QCheckBox(tr("Автоматика"), parent);
            connect(cbAutomatic, &QCheckBox::stateChanged, this, [this, cbHeat,cbRandomEvents](bool stat)
            {
                cbHeat->setEnabled(!stat);
                cbRandomEvents->setEnabled(stat);
                m_ksd->setAutomatic(stat);
            });
            parent->layout()->addWidget(cbAutomatic);


            connect(cbRandomEvents, &QCheckBox::stateChanged, m_ksd, &KettleStatusDisplay::setRandomEvents);
            parent->layout()->addWidget(cbRandomEvents);

            parent->layout()->addWidget(new QWidget(parent));
        }
    }
}

ElKetWindow::~ElKetWindow()
{
}

