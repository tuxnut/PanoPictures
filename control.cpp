#include "control.h"
#include "model.h"
#include "view.h"
#include "filters.h"
#include "bucky.h"
#include "itemmovie.h"

/*!
 * \brief Control::Control constructor of the control class
 * \param model model of this application
 * \param view view of this application
 *
 * This is where default value for the type of pelemele and montage are set.
 */
Control::Control(Model *model, View &view) :
    view(view)
{
    this->model = model;
    view.setControl(this);
    nbImage = 0;
    choixPelemele = 1;
    choixSeq = 1;
    incPremierPlan = 0;
}

/*!
 * \brief Control::readFile reads the file using the model class, and passing it after
 * operations to the view
 * \param filename directory of the file
 * \param fileType whether it is a video or an image file
 *
 * There are actually two types of reading in this method depending on the type of file.
 * An image will be given as a QPixmap, then set into a QGraphicsPixmapItem and
 * finally sent to the view. As for a video, a .gif animated will be created to stand for
 * the video in the scene then add to the view.
 * A QListWidgetItem will also be created to list all the item in the view.
 */
void Control::readFile(const QString &filename, int fileType)
{
    switch (fileType)
    {
        case 1:
            if (!model->readFilePic(filename))
            {
                qDebug() << "Control error reading file" << filename;
            }
            else {
                QGraphicsPixmapItem *jonathan = new QGraphicsPixmapItem();
                jonathan->setPixmap(model->picContent());
                jonathan->setAcceptTouchEvents(true);
                jonathan->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |QGraphicsItem::ItemIsFocusable);
                jonathan->setZValue(++nbImage);

                QIcon iconItem = QIcon(model->picContent());
                QListWidgetItem *widgetItem = new QListWidgetItem(iconItem, "item image "+QString::number(nbImage), 0);
                widgetItem->setWhatsThis(QString::number(nbImage));
                view.printFile(jonathan, false, widgetItem);
            }
        break;
        case 2:
            if (!model->readFileVids(filename))
            {
                qDebug() << "Control error reading file" << filename;
            }
            else {
                QString cmd = "ffmpeg -i "+filename+" -t 3  -y "+filename+".gif";
                QByteArray ba = cmd.toLatin1();
                const char *c_str = ba.data();
                system(c_str);

                ItemMovie *jemamuse = new ItemMovie();
                jemamuse->setMovie(new QMovie(filename+".gif"));

                jemamuse->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);//  |QGraphicsItem::ItemIsFocusable);
                jemamuse->setToolTip(filename);
                jemamuse->setZValue(++nbImage);

                QIcon iconItem = QIcon(QPixmap(filename+".gif"));
                QListWidgetItem *widgetItem = new QListWidgetItem(iconItem, "item video "+QString::number(nbImage), 0);
                widgetItem->setWhatsThis(QString::number(nbImage));
                view.printFile(jemamuse, true, widgetItem);
            }
        break;
        default:
        break;
    }
}

/*!
 * \brief Control::saveFile save the Pelemele
 */
void Control::saveFile()
{
     model->saveFile(view.saveFile(), view.getPelemele());
     view.displayStatus("Le pelemele a ete sauvegarde");
}

/*!
 * \brief Control::getWidgetItem get the WidgetItem associated with the zValue
 * \param zValue parameter that allows a 1 to 1 correspondance between items
 * in the Scene and items in the List
 */
QListWidgetItem* Control::getWidgetItem(int zValue)
{
    for (int i = 0; i < view.getListWidget()->count(); i++)
    {
        if (view.getListWidget()->item(i)->whatsThis() == QString::number(zValue))
        {
            return view.getListWidget()->item(i);
        }
    }
    return NULL;
}

/*!
 * \brief Control::setZvalueItem Allows the selected item to be drawnon top of the others
 *
 * It is essential to keep the relation between the list and the scene, so we need to reassign
 * the 'whatsthis' of the listwidgetitem too.
 */
void Control::setZvalueItem()
{
    if (view.getSelectedItem() == NULL)
        return;
    else {
        QGraphicsItem* item = view.getSelectedItem();                     //récupère l'item en focus
        qreal z = item->zValue();
        qreal itemzmax = z;
        QList <QGraphicsItem*> listItem = view.getScene()->items();
        qDebug()<< item->parentItem();

        for (int i = 0; i < listItem.count(); i++)
            if (listItem[i]->zValue() > itemzmax)                       //compare les z-values
                itemzmax = listItem[i]->zValue();

        item->setZValue(++itemzmax);                                    //on le fait passer au premier plan

        if (getWidgetItem(z) != NULL)
        {
            QListWidgetItem *widgetItem = getWidgetItem(z);
            widgetItem->setWhatsThis(QString::number(itemzmax));
        }
        incPremierPlan++;
        view.displayStatus("L'item est passe au premier plan");
    }
}

/*!
 * \brief Control::itemRotation performs a rotation of the selected item with the given angle
 * \param angle angle of rotation
 */
void Control::itemRotation(qreal angle)
{
    if (view.getSelectedItem() == NULL)
        return;
    else {
        if (view.getSelectedItem()->toolTip() == "")
        {
            QGraphicsItem* item = view.getSelectedItem();
            item->setTransformOriginPoint(item->boundingRect().center());
            item->setRotation(item->rotation() + angle);
        } else
            return;
    }
}

/*!
 * \brief Control::itemScale increase or decrease the size of the item
 * \param factor factor of scaling
 */
void Control::itemScale(qreal factor)
{
    if (view.getSelectedItem() == NULL)
        return;
    else {
        if (view.getSelectedItem()->toolTip() == "")
        {
            QGraphicsItem* item = view.getSelectedItem();
            item->setTransformOriginPoint(item->boundingRect().center());
            item->setScale(item->scale()*factor);
        } else
            return;
    }
}

/*!
 * \brief Control::itemDelete delete the item from the Scene AND from the list
 *
 * If the item is a video, we need to get rid of the .gif file created to represent it
 * in the scene.
 */
void Control::itemDelete()
{
    QGraphicsScene *scene = view.getScene();
    QListWidget *listWidget = view.getListWidget();

    if (view.getSelectedItem() == NULL)
        return;
    else {
        QGraphicsItem* item = view.getSelectedItem();
        scene->removeItem(item);

        if (item->toolTip() != "")
            model->removeFile(item->toolTip()+".gif");

        if (getWidgetItem(item->zValue()) != NULL)
        {
            QListWidgetItem *widgetItem = getWidgetItem(item->zValue());
            listWidget->setItemSelected(widgetItem, true);
            qDeleteAll(listWidget->selectedItems());
        } else
            return;
    }
    nbImage--;
    view.displayStatus("L'item a ete supprime");
}

/*!
 * \brief Control::itemSelectNext allows to explore all the item in the scene
 */
void Control::itemSelectNext()
{
    QGraphicsScene *scene = view.getScene();

    if (view.getSelectedItem() == NULL || scene->items().count() == 1)
        return;
    else {
        QList <QGraphicsItem*> listItem = scene->items(Qt::AscendingOrder);
        for (int i = 0; i < listItem.count(); i++)
        {
            if (listItem[i]->isSelected())
            {
                listItem[i]->setSelected(false);
                if (i == listItem.count() - 1)
                {
                    listItem[0]->setSelected(true);
                    return;
                } else {
                    listItem[i+1]->setSelected(true);
                    return;
                }
            }
        }
    }
}

/*!
 * \brief Control::selectImage set the image-items in real color and shades the video-item
 */
void Control::selectImage()
{
    QList<QGraphicsItem*> items = view.getScene()->items(Qt::AscendingOrder);

    if (items.isEmpty())
        return;

    for (int i = 0; i < items.count(); i ++)
    {
        if (!(items[i]->toolTip() == ""))
            items[i]->setOpacity(0.3);
        else
            items[i]->setOpacity(1);
    }
}

/*!
 * \brief Control::selectVideo set the video-items in real color and shades the image-item
 */
void Control::selectVideo()
{
    QList<QGraphicsItem*> items = view.getScene()->items(Qt::AscendingOrder);

    if (items.isEmpty())
        return;

    for (int i = 0; i < items.count(); i ++)
    {
        if (items[i]->toolTip() == "")
            items[i]->setOpacity(0.3);
        else
            items[i]->setOpacity(1);
    }
}

/*!
 * \brief Control::setChoixPelemele set the choice of pelemele to choix
 * \param choix choice of the pelemele output
 */
void Control::setChoixPelemele(int choix)
{
    choixPelemele = choix;
}

/*!
 * \brief Control::setChoixSeq set the choice of the montage to choix
 * \param choix choice of the montage 'direction'
 */
void Control::setChoixSeq(int choix)
{
    choixSeq = choix;
}

/*!
 * \brief Control::applySingle performs a transformation on a single image
 *
 * The type of the transformation is the same of choixPelemele.
 * There are 3 steps : first we 'cut' our scene matching our items and paint it on QImage
 * secondly we call filters that perform image manipulation over the image, and finally
 * we replace the old item by the new one at the same position.
 */
void Control::applySingle()
{
    if (view.getSelectedItem()->toolTip() != "")
        return;

    if (choixPelemele == 1 || choixPelemele == 3 || choixPelemele == 4)
    {
        QGraphicsScene *scene = view.getScene();
        QGraphicsItem* clemence = view.getSelectedItem();
        int z = clemence->zValue();
        QListWidgetItem* widgetItem = getWidgetItem(z);

        scene->clearSelection();
        clemence->setZValue(clemence->zValue() + nbImage + incPremierPlan);

        QImage* img = new QImage(clemence->sceneBoundingRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(img);
        scene->render(&painter, img->rect(), clemence->mapRectToScene(clemence->boundingRect()));

        qDebug() << "0";
        filters* filtre = new filters(img, choixPelemele);
        if (choixPelemele == 3)
            filtre->setFilterSize(view.getFilterSize());
        QObject::connect(filtre, SIGNAL(finished()), filtre, SLOT(deleteLater()));
        filtre->start();
        while (filtre->isRunning()) {usleep(1000);}

        QPixmap pixmap = QPixmap::fromImage(*img);
        QGraphicsPixmapItem * pixmapItem = new QGraphicsPixmapItem(pixmap);
        pixmapItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);//  |QGraphicsItem::ItemIsFocusable);
        pixmapItem->setZValue(z);
        pixmapItem->setAcceptTouchEvents(true);
        pixmapItem->setPos(clemence->scenePos());

        scene->removeItem(clemence);
        widgetItem->setIcon(pixmap);
        view.printFile(pixmapItem, false);
    }
    view.displayStatus("L'image a ete transformee");
    return;
}

/*!
 * \brief Control::pele_mele apply a pelemele of the Scene. The user can change the output
 * pelemele by calling setChoixPelemele
 *
 * There are 5 steps : first we 'cut' our scene matching our items and paint it on QImage
 * secondly we call filters that perform image manipulation over the image, and then
 * we add the new items at the same position on a different Scene. We take a 'screenshot'
 * of this new scene and finally call a QLabel to show the output and ask the user to save it or not.
 */
void Control::pele_mele()
{
    /*** Initialisation de variables dont on aura besoin ***/
    view.displayStatus("Pelemele en cours de realisation ...");
    QGraphicsScene *scene = view.getScene();
    QGraphicsScene *chloe = new QGraphicsScene();
    scene->clearSelection();

    QList<QGraphicsItem*> listItemTotal = scene->items(Qt::AscendingOrder); // item 0 : le tout premier et ainsi de suite
    QList<QGraphicsItem*> listItem;

    /*** On récupère seuelement les items images, celles qui ne contiennent pas de tooTip***/
    for (int i = 0; i < listItemTotal.count(); i++)
    {
        if (listItemTotal[i]->toolTip() == "")
        {
            listItem.append(listItemTotal[i]);
        }
    }

    /*** Si il n'y a pas d'image, on s'arrête***/
    if (listItem.isEmpty())
       return;

    /*** Choix du pêlemêle couleur, ici pas de filtres à faire, on récupère seulement la scène ***/
    if (choixPelemele == 2 )
    {
        for (int i = 0; i < listItemTotal.count(); i++)
        {
            /*** On élimine les items vidéos ***/
            if (listItemTotal[i]->toolTip() != "") // = si ce sot des items 'videos'
            {
                listItemTotal[i]->setVisible(false);
            } else {
                /*** On peint chaque item dans une QPixmap ***/
                QPixmap* img = new QPixmap(listItemTotal[i]->sceneBoundingRect().size().toSize());
                img->fill(Qt::transparent);
                QPainter painter(img);
                scene->render(&painter, img->rect(), listItemTotal[i]->mapRectToScene(listItemTotal[i]->boundingRect())); // #rekt !!
                /*** On replace les QPixmap dans une nouvelle scène, en gardant les mêmes positions***/
                QGraphicsPixmapItem * pixmapItem = new QGraphicsPixmapItem(*img);
                chloe->addItem((QGraphicsItem *) pixmapItem);
                chloe->items(Qt::AscendingOrder).last()->setPos(listItem[i]->scenePos());
           }
        }
        /*** On remet en place les vidéos ***/
        for (int i = 0; i < listItemTotal.count(); i++)
        {
            if (listItemTotal[i]->toolTip() != "")
            {
                listItemTotal[i]->setVisible(true);
            }
        }
        /*** Enfin, on peint la scene numéro 2 ***/
        QPixmap* christopher = new QPixmap(chloe->sceneRect().size().toSize());
        christopher->fill(Qt::transparent);
        QPainter painter(christopher);
        chloe->render(&painter);
        view.displayPelemele(christopher);
        view.displayStatus("Pelemele realise");
        return;
    }

    QList<QImage*> listImage;
    QList<filters*> thread;

    for (int i = 0; i < listItem.count(); i++)
    {
            // *** on decoupe notre scene pour avoir nos images une par une *** //
        int zvalue = listItem[i]->zValue();

        if (!(listItem[i]->collidingItems().isEmpty()))
        {
            listItem[i]->setZValue(nbImage + incPremierPlan); //premier plan
        }

        QImage* img = new QImage(listItem[i]->sceneBoundingRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(img);
        scene->render(&painter, img->rect(), listItem[i]->mapRectToScene(listItem[i]->boundingRect())); // #rekt !!
        listImage.append(img);
        listItem[i]->setZValue(zvalue);

            // *** on attribue un thread (= filtre) à chacune d'elle, avec si besion les détails du filtre *** //
        thread.append(new filters(listImage[i], choixPelemele));
        if (choixPelemele == 3)
            thread[i]->setFilterSize(view.getFilterSize());
        QObject::connect(thread[i], SIGNAL(finished()), thread[i], SLOT(deleteLater()));
    }
            // *** on lance les threads *** //
    for (int i = 0; i < listItem.count(); i++)
    {
         thread[i]->start();
    }
            // *** on attend qu'ils aient fini *** //
    for (int i = 0; i < listItem.count(); i++)
    {
         while(thread[i]->isRunning()) //polling
         {usleep(1000);}
    }

            // *** on récupère les images traitées en les "affichant" dans une autre scene *** //
    for (int i =0; i < listItem.count(); i ++)
    {
        QPixmap pixmap = QPixmap::fromImage(*listImage[i]);
        QGraphicsPixmapItem * pixmapItem = new QGraphicsPixmapItem(pixmap);

        chloe->addItem((QGraphicsItem *) pixmapItem);
        chloe->items(Qt::AscendingOrder).last()->setPos(listItem[i]->scenePos());
    }

            // *** on enregistre le tout *** //
    chloe->setSceneRect(chloe->itemsBoundingRect());

    QPixmap* christopher = new QPixmap(chloe->sceneRect().size().toSize());
    christopher->fill(Qt::transparent);
    QPainter painter(christopher);
    chloe->render(&painter);

    view.displayPelemele(christopher);

    for (int i = 0; i < listItem.count(); i++)
    {
        free(listImage[i]); listImage[i] = NULL;
    }
    //free(chloe); chloe = NULL; // je comprends pas le free
    view.displayStatus("Pelemele realise");
}

/*!
 * \brief Control::grayScale the grayscale function in control is never called
 * \param image file we want to work on
 */
void Control::grayScale(QImage *image)
{
    for (int x = 0; x < image->height(); x++) {
        uchar  *index = image->scanLine(x);
        int depth = 4; //puisque c'est du aRGB
        for (int y = 0; y < image->width(); y++) {

            QRgb *pixel = reinterpret_cast <QRgb*> (index + y * depth);
            int gray = qGray(*pixel);
            *pixel = QColor(gray, gray, gray, qAlpha(*pixel)).rgba();
        }
    }
}

/*!
 * \brief Control::getVidsByOrder get the video-item in the order defined by the user
 * (following the x-axis or the y-axis)
 */
QList <QGraphicsItem*> Control::getVidsByOrder() // selo x OU y
{
    QGraphicsScene *scene = view.getScene();
    scene->clearSelection();

    QList<QGraphicsItem*> listItemTotal = scene->items(Qt::AscendingOrder); // item 0 : le tout premier et ainsi de suite
    QList<QGraphicsItem*> listItemVid;

    for (int i = 0; i < listItemTotal.count(); i++)
    {
        if (!(listItemTotal[i]->toolTip() == ""))
        {
            listItemVid.append(listItemTotal[i]);
        }
    }

    if (listItemVid.count() <=1)
        return listItemVid;

    //int * ref = (int*) malloc (listItemVid.count() * sizeof (int));
    //int cursor = 0;
    if (choixSeq == 1)
    {
        for (int i = 0; i < listItemVid.count() - 1; i++)
        {
            qreal pos = listItemVid[i]->scenePos().x();
            for (int j = i + 1; j < listItemVid.count(); j++)
            {
                if (listItemVid[j]->scenePos().x() < pos)
                {
                    pos = listItemVid[j]->scenePos().x();
                    listItemVid.swap(i, j);
                }
            }
        }
    } else {
        for (int i = 0; i < listItemVid.count() - 1; i++)
        {
            qreal pos = listItemVid[i]->scenePos().y();
            for (int j = i + 1; j < listItemVid.count(); j++)
            {
                if (listItemVid[j]->scenePos().y() < pos)
                {
                    pos = listItemVid[j]->scenePos().y();
                    listItemVid.swap(i, j);
                }
            }
        }
    }
    return listItemVid;
}

/*!
 * \brief Control::seqVideos performs the montage of the videos
 *
 * We first get the video item by order (x or y axis)
 * Secondly, we prepare n+1 threads for n video (the last thread will be in charge of
 * the concatenation) and 1 semaphore for the controler to know when the threads have
 * finished. A QProgressDialog is also initialized to let the user know about the progression
 * of the montage.
 */
void Control::seqVideos()
{
    /*** On récupère les items videos, s'il n'y en a pas, on s'arrete là***/
    QList<QGraphicsItem*> listItemVid = getVidsByOrder();

    if (listItemVid.isEmpty())
       return;

    view.displayStatus("Montage video en cours ...");
    QList<bucky*> thread;
    QList <QString> grayTranscodedFile;
    QString grayFile = "";
    /*** ffmepg a besoin du fichier de sortie, donc on le demande des le début***/
    QString filenameOut = view.saveFile();

    if (filenameOut == "") // En fait, c'est si on a appuyé sur 'cancel'
        return;

    QSemaphore sem (listItemVid.count() + 1); // 1 thread pour chaque videos + 1 thread 'assembleur de sequence'

    for (int i = 0; i <= listItemVid.count(); i++)
    {
        if (i < listItemVid.count()) // les threads video ont besoin du nom du fichier (dans le tooTip), de leur Id et de la semaphore, le controleur garde le fichier sortie : tooTip + ".mpg"
        {
            grayFile.append(listItemVid[i]->toolTip()+ "output" + QString::number(i+1) +".mpg ");
            grayTranscodedFile.append(listItemVid[i]->toolTip()+ "output" + QString::number(i+1) +".mpg");
            thread.append(new bucky(listItemVid[i]->toolTip(), i+1, &sem));
        }
        else {
            thread.append(new bucky(grayFile, i+1, false, filenameOut, &sem)); // le thread concatenate
        }
        QObject::connect(thread[i], SIGNAL(finished()), thread[i], SLOT(deleteLater()));
    }

    view.displayProgress(listItemVid.count() + 1); // Normalement, une QProgressDialog s'ouvre, mais on la voit pas au début :(

    /*** On lance les traitements videos***/
    for (int i = 0; i < listItemVid.count(); i++) //tous sauf le dernier (qui réunit toutes les vidéos)
    {
        thread[i]->start();
    }

    while(listItemVid.count() + 1 - sem.available() < listItemVid.count())
    {
        view.incProgress(listItemVid.count() + 1 - sem.available());
        usleep(100000);
    }

    view.incProgress(listItemVid.count() + 1 - sem.available()); //si jamais le thread finit dans le sleep et la barre ne semet donc pas à jour

    /*** La concaténation***/
    thread[listItemVid.count()]->start();
    view.displayStatus("Passage en vieau de gris realise. Montage final en cours ...");
    while(thread[listItemVid.count()]->isRunning())
    {
        usleep(250000);
    }
    view.incProgress(listItemVid.count() + 1); // On devrait  arriver à 100%
    model->removeFile(grayTranscodedFile); // On supprime tous les fichiers 'mpg' "intermédiaires".
    view.incProgress(listItemVid.count() + 2); // ProgressBar Windows : on va à + de 100 %
    view.displaySequencage(filenameOut); // Popup pour voir la video
    view.displayStatus("Montage video realise");
}

/*!
 * \brief Control::cleaner remove all the temporary files created (essentially the .gif)
 * at the closure of the application
 */
bool Control::cleaner()
{
    QList <QGraphicsItem*> listVids = getVidsByOrder();
    QList <QString> fileNames;

    for (int i = 0; i < listVids.count(); i ++)
    {
        fileNames.append(listVids[i]->toolTip()+".gif");
    }

    if (fileNames.isEmpty())
        return true;

    if (model->removeFile(fileNames))
    {
        return true;
    } else {
        return false;
    }
}
