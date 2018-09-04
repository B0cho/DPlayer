#include "cdropsitelabel.h"

CdropSiteLabel::CdropSiteLabel(QWidget *parent)
    : QLabel(parent)
{
    setAcceptDrops(true);
    setAlignment(Qt::AlignHCenter);
    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    setLineWidth(1);
}

void CdropSiteLabel::dragEnterEvent(QDragEnterEvent *event)
{
    // getting formats
    const QStringList eventFormats = event->mimeData()->formats();
    // changing if deletion of given data is accepted
    bool flag;
    emit isDeleteAccepted(event->mimeData(), flag);
    if(flag && std::any_of(eventFormats.cbegin(), eventFormats.cend(), [this](const QString format){ return formats.contains(format); }))
        event->acceptProposedAction();
}

void CdropSiteLabel::dropEvent(QDropEvent *event)
{
    emit droppedMimeData(event->mimeData());
    event->acceptProposedAction();
}

void CdropSiteLabel::setFormats(const QStringList &value)
{
    formats = value;
}
