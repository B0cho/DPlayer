#ifndef CDROPSITELABEL_H
#define CDROPSITELABEL_H

#include <QObject>
#include <QLabel>
#include <QMimeData>
#include <QDragEnterEvent>

/*
 * Extenstion of QLabel to provide Drag and Drop
 */

class CdropSiteLabel : public QLabel
{
    Q_OBJECT
public:
    // ctors
    explicit CdropSiteLabel(QWidget* parent = nullptr);

    // drag and drop facilities
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent *event) override;

    // other
    void setFormats(const QStringList &value);

private:
    QStringList formats; // accepted drop

signals:
    void isDeleteAccepted(const QMimeData* mimeData, bool& flag) const;
    void droppedMimeData(const QMimeData* mimeData) const; // emitted when drop occurs on widget and sends qmime


};

#endif // CDROPSITELABEL_H
