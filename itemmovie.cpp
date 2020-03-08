#include "itemmovie.h"
#include <QDebug>

ItemMovie::ItemMovie(QObject *parent) : QObject(parent)
{
}

/*!
 * \brief ItemMovie::setMovie set a QMovie (a .gif) associated to this item
 * \param movie
 *
 * We connect the signal from the QMovie class to the slot of this class.
 */
void ItemMovie::setMovie(QMovie *movie)
{
    stackOverflowFTW = movie;
    connect(stackOverflowFTW, SIGNAL(frameChanged(int)), this, SLOT(changeFrame()));
    movie->start();
    changeFrame();
}

/*!
 * \brief ItemMovie::changeFrame pick up the current frame of the .gif to the item
 *
 *  It will allow us to pick up the actual frame of the .gif and display it as a QPixmap
 */
void ItemMovie::changeFrame()
{
    if (!stackOverflowFTW)
        return;

    setPixmap(stackOverflowFTW->currentPixmap());
}
