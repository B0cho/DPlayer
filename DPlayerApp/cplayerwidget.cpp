#include "cplayerwidget.h"

CPlayerWidget::CPlayerWidget(QWidget *parent):
    QWidget(parent),
    playPauseButton(new QPushButton(this)),
    stopButton(new QPushButton(this)),
    repeatButton(new QPushButton(this))
{
    // layout inits init
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->setAlignment(Qt::AlignHCenter);

    // widgets inits

    // adding widgets
    // add track slider (hide)
    // add fragment slider

    bottomLayout->addWidget(stopButton.get());
    bottomLayout->addWidget(playPauseButton.get());
    bottomLayout->addWidget(repeatButton.get());

    // connections

    // injecting layouts
    mainLayout->addLayout(bottomLayout);
    this->setLayout(mainLayout);
}
