#ifndef CPLAYERWIDGET_H
#define CPLAYERWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
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
    std::unique_ptr<QPushButton> playPauseButton, stopButton, repeatButton;

signals:
    //
public slots:
    //
};

#endif // CPLAYERWIDGET_H
