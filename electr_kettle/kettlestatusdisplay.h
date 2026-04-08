#ifndef KETTLESTATUSDISPLAY_H
#define KETTLESTATUSDISPLAY_H

#include <QWidget>
#include <functional>

class QPaintEvent;

class KettleState;

class KettleControllerBase;

class KettleStatusDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KettleStatusDisplay(QWidget *parent = nullptr);
    virtual ~KettleStatusDisplay();

    ///Запустить симуляцию
    void run();
    ///Остановить симуляцию
    void stop();

    ///принудительный нагрев (с чекбокса)
    void setHeating(bool iHeating);

    ///Включение программы автоматики
    void setAutomatic(bool iAutomatic);

    ///Включение генерации случайных событий
    void setRandomEvents(bool iRE);

signals:

    void heatingChanged(bool iHeating);

protected:
    void paintEvent(QPaintEvent *event) final;

private:

    void updateStatus();

    KettleState* m_ketState = nullptr;
    KettleControllerBase* m_ketCntrl = nullptr;

    static constexpr qint64 m_refreshPeriod_ms = 250;
    bool m_simulate = false;///Влючение симуляции работы
    bool m_automatic = false;///Включение автоматической программы управления
    bool m_randomEvents = false;///Генерация случайных событий
    QTimer* m_updTimer = nullptr;
    std::vector<double> m_tempPoints;///точки температуры воды для рисования графика
    std::vector<double> m_tempSteamPoints;///точки температуры пара для рисования графика

    void drawGraph(std::vector<double>  graph, QPen pen, QPainter &painter, std::function<double(double)> mapY);
};

#endif // KETTLESTATUSDISPLAY_H
