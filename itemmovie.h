#ifndef ITEMMOVIE_H
#define ITEMMOVIE_H

#include <QMovie>
#include <QGraphicsPixmapItem>

/*!
 * \brief The ItemMovie class is derived from QGraphicsItem and provides a way
 * to display animated image (like GIF) in a QGraphicsScene.
 *
 * A QMovie is loaded from a *.gif file, and calls the associated item in order to display
 * the current frame of the .gif using the signal 'frameChanged()'.
 */

class ItemMovie : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
private:
    QMovie* stackOverflowFTW;

public:
    explicit ItemMovie(QObject *parent = 0);
    void setMovie(QMovie* movie);

signals:

private slots:
    void changeFrame();
};

#endif // ITEMMOVIE_H
