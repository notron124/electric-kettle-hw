#ifndef ELKETWINDOW_H
#define ELKETWINDOW_H

#include <QMainWindow>

class KettleStatusDisplay;

class ElKetWindow : public QMainWindow
{
    Q_OBJECT

public:
    ElKetWindow(QWidget *parent = nullptr);
    ~ElKetWindow();

private:
    KettleStatusDisplay* m_ksd = nullptr;
};
#endif // ELKETWINDOW_H
