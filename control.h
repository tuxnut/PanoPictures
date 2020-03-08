#ifndef CONTROL_H
#define CONTROL_H

#include <QString>
#include <unistd.h>
#include <QImage>
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QSemaphore>
#include <QListWidgetItem>
#include <QMovie>
#include <QGraphicsProxyWidget>

/*! \class Control
*  \brief The control class is at the center of this application. It either calls the right
*  method or performs itself the operation.
*
* The controler is called at every action launched by the user. From changing the scale
* of an item to executing the video montage, the controler really is the 'brain' of the
* application. It knows its 'view' and its 'model' and makes the bridge if one wants to call
* the other. It contains the main methods called to perform image and video manipulation,
* creates and controls the threads.
*/

class Model;
class View;

class Control
{
private:
    Model *model;
    View &view;
    int nbImage;
    int incPremierPlan;
    int choixPelemele;
    int choixSeq;

public:
    Control(Model *model, View &view);
    void readFile(const QString &filename, int fileType = 1);
    void saveFile();
    QListWidgetItem* getWidgetItem(int zValue);
    void setZvalueItem();
    void itemRotation(qreal angle);
    void itemScale(qreal factor);
    void itemDelete();
    void itemSelectNext();
    void selectImage();
    void selectVideo();
    void setChoixPelemele(int choix);
    void setChoixSeq(int choix);
    void applySingle();
    void pele_mele();    
    void grayScale(QImage *image);
    QList <QGraphicsItem*> getVidsByOrder();
    void seqVideos();
    bool cleaner();
};

#endif // CONTROL_H
