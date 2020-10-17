#ifndef CPLAYERWIDGET_H
#define CPLAYERWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAbstractSlider>
#include <QPainter>
/// Represent a combined widget for track control.\n
/// Provides control of play/pause/stop, volume and track slider.
class CPlayerWidget: public QWidget
{
    Q_OBJECT
public:
    CPlayerWidget(QWidget* parent = 0);
private:
    /*
     * layout
     * track slider
     * fragment slider
     * volume slider
     * play button
     * pause button
     * stop slider
     * repeat button
     */
    class CChartSlider;
    class CScopeChartSlider;

    std::unique_ptr<CChartSlider> fragmentSlider;
    std::unique_ptr<CScopeChartSlider> trackSlider;
    std::unique_ptr<QPushButton> playPauseButton, stopButton, repeatButton;


signals:
    //
public slots:
    //
};

/// Internal class representing one-button slider.\n
/// Given set of values (e.g. sound amplitude over track timing) is drawed in background
class CPlayerWidget::CChartSlider: public QAbstractSlider
{
    Q_OBJECT
protected:
    void paintBackground() const;
    void paintEvent(QPaintEvent* event) override;

public:
    CChartSlider(QWidget* parent = nullptr);


};

/// Internal class extending chart slider for setting scope
class CPlayerWidget::CScopeChartSlider: public CChartSlider
{
    Q_OBJECT
private:
    void paintEvent(QPaintEvent* event) override;

public:
    CScopeChartSlider(QWidget* parent = nullptr);

};

#endif // CPLAYERWIDGET_H
