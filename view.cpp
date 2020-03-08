#include "view.h"
#include "ui_view.h"
#include "control.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QLabel>
#include <QGraphicsItemGroup>

/*!
 * \brief View::View constructor of the view
 * \param parent
 *
 * We declare here oen of our main tool : the QGraphicsScene that will help us
 * keep track of every item.
 */
View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View)
{
    scene = new QGraphicsScene (this);
    ui->setupUi(this);
    ui->spinBox->setVisible(false);
    //videos = new QGraphicsItemGroup ();
}

/*!
 * \brief View::~View destructor
 */
View::~View()
{
    delete ui;
}

/*!
 * \brief View::setControl setter of the view for the controler
 * \param control
 */
void View::setControl(Control *control)
{
    this->control = control;
}

/*!
 * \brief View::printFile display a QGraphicsItem in the QGraphicsScene. The QGraphicsView
 * is updated.
 * \param pixmapItem Item to be displayed
 * \param video whether the item is a video or not : we perform of size reduction if the
 * .gif representative of the video is too big
 * \param widgetItem display the QListWidgetItem associated with the item
 */
void View::printFile(QGraphicsItem *pixmapItem, bool video, QListWidgetItem *widgetItem)
{
    if (!video)
    {
        scene->clearSelection();
        scene->addItem(pixmapItem);
        pixmapItem->setSelected(true);
        ui->graphicsView->setScene(scene);
        ui->listWidget->addItem(widgetItem);
        displayStatus("L'image a ete chargee");
    }
    else
    {
        scene->clearSelection();
        while (pixmapItem->sceneBoundingRect().width() > 200 || pixmapItem->sceneBoundingRect().height() > 200)
        {
            pixmapItem->setScale(pixmapItem->scale() * 0.9);
        }
        scene->addItem(pixmapItem);
        ui->graphicsView->setScene(scene);
        ui->listWidget->addItem(widgetItem);
        displayStatus("La video a ete chargee");
    }
}

/*!
 * \brief View::printFile overloaded function
 * \param movie
 */
void View::printFile(QMovie * movie)
{
}

/*!
 * \brief View::saveFile return the directory of the file we want to save
 */
QString View::saveFile()
{
    QFileDialog saveFile;
    QString fileName = saveFile.getSaveFileName(this, tr("Save File"), "/home/thierry", tr("Files (*.png *.jpg *.bmp *.gif *.mp4 *.mkv *.mpg *.flv *.avi *.mov)"));
    saveFile.close();
    return fileName;
}

/*!
 * \brief View::getScene getter of the QGraphicsScene
 */
QGraphicsScene* View::getScene()
{
    return this->scene;
}
/*
QGraphicsItemGroup* View::getVideos()
{
    return this->videos;
}
*/
/*!
 * \brief View::getSelectedItem getter of the item selected at the moment. If several items
 * are selected, will return only one.
 */
QGraphicsItem* View::getSelectedItem()
{
    if(scene->selectedItems().isEmpty())
        return NULL;
    else
        return this->scene->selectedItems().first();
}

/*!
 * \brief View::getListWidget getter of the ListWidget
 */
QListWidget* View::getListWidget()
{
    return ui->listWidget;
}

/*!
 * \brief View::getPelemele getter of the pelemele QPixmap
 */
QPixmap* View::getPelemele()
{
    return G_Blanc;
}

/*!
 * \brief View::getFilterSize getter of the size set by the user for the filter
 */
int View::getFilterSize()
{
    return ui->spinBox->value();
}

/*!
 * \brief View::closeEvent overloaded function : handle the event on closing to get rif of
 * the .gif we created
 * \param event
 */
void View::closeEvent (QCloseEvent * event)
{
    if (control->cleaner())
    {
        event->accept();
     } else {
        event->ignore();
     }
}

void View::mousePressEvent(QMouseEvent* event)
{
    //qDebug() << event->button();
}

/*!
 * \brief View::keyPressEvent overloaded function : handle the event of a key pressed
 * \param event
 */
void View::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_6:
            control->itemRotation(5);
        break;
    case Qt::Key_4:
            control->itemRotation(-5);
        break;
    case Qt::Key_8:
            control->itemScale(1.1);
        break;
    case Qt::Key_2:
            control->itemScale(0.9);
        break;
    case Qt::Key_Delete:
            control->itemDelete();
        break;
    case Qt::Key_Control:
            control->itemSelectNext();
        break;
    default:

        qDebug() << ui->listWidget->count();
      break;
    }
}

/*!
 * \brief View::timerEvent overloaded function : handle the event of a timer
 * \param event
 */
void View::timerEvent(QTimerEvent * event)
{
    killTimer(event->timerId());
    pelemeleChoice();
}

/*!
 * \brief View::displayPelemele display the pelemele in a QLabel
 * \param image
 */
void View::displayPelemele(QPixmap * image)
{
    this->G_Blanc = image;
    display = new QLabel(this, Qt::Window | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    display->setPixmap(*image);
    display->show();
    this->startTimer(1500);
}

/*!
 * \brief View::displaySequencage display a QMessageBox to let the user decide if he wants
 * to watch the montage video
 * \param filenameout
 */
void View::displaySequencage(QString filenameout)
{
    int choice = QMessageBox::warning(this, "Sequencage", tr("Voulez-vous visualiser le sequencage cree ?"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
    if (choice == QMessageBox::Ok)
    {
        QString displayCMD = "audience " + filenameout+".mp4";
        QByteArray ba = displayCMD.toLatin1();
        const char *cmd = ba.data();
        system(cmd);
    }
}

/*!
 * \brief View::pelemeleChoice Let the user decide if he wants to save the pelemele after a short timer
 */
void View::pelemeleChoice()
{
    int choice = QMessageBox::warning(display, "Pelemele", tr("Voulez-vous enregistrer le pele-mele ?"), QMessageBox::Save | QMessageBox::Cancel, QMessageBox::Save);
    switch (choice) {
        case QMessageBox::Save:
            display->close();
            control->saveFile();
            return;
            break;
       case QMessageBox::Cancel:
            free(G_Blanc);
            display->close();
            return;
            break;
       default:
            return;
            break;
     }
    return; // au cas où
}

/*!
 * \brief View::displayProgress display a QProgressDialog
 * \param maximum
 */
void View::displayProgress(int maximum)
{
    progressDialog = new QProgressDialog ( "Le sequencage est en cours de realisation.", "Cancel", 0, maximum, this, Qt::Popup);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setAutoClose(false); // pour imiter windows :D (on arrive à 100 % et .... c'est pas fini)
}

/*!
 * \brief View::incProgress increment the QProgressBar in the QProgressDialog
 * \param step
 */
void View::incProgress(int step)
{
    if (progressDialog != NULL)
    {
        if (step <= progressDialog->maximum())
        {
            progressDialog->setValue(step);
        }
        else {
            progressDialog->close();
        }
    }
    else
        return;
}

/*!
 * \brief View::displayStatus display a brief message about the last action
 * \param status message to be displayed
 */
void View::displayStatus(QString status)
{
    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(status, 2000);
}

void View::on_pushButton_ouvrirImage_clicked()
{
    control->selectImage();
    QString dir = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/thierry/Images", tr("Image Files (*.png *.jpg *.bmp *.gif)"));
    control->readFile(dir);
}

void View::on_pushButton_setZValue_clicked()
{
    control->setZvalueItem();
}

void View::on_pushButton_pelemele_clicked()
{
    control->selectImage();
    control->pele_mele();
}

void View::on_pushButton_ouvrirVideo_clicked()
{
    control->selectVideo();
    QString dir = QFileDialog::getOpenFileName(this, tr("Open Vids"), "/home/thierry/Vidéos", tr("Movie Files (*.mp4 *.mkv *.mpg *.flv *.avi *.mov *.gif)"));
    control->readFile(dir, 2);
}

void View::on_pushButton_sequencage_clicked()
{
    control->selectVideo();
    control->seqVideos();
}

void View::on_ouvrirImage_triggered()
{
    QString dir = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/thierry/Images", tr("Image Files (*.png *.jpg *.bmp *.gif)"));
    control->readFile(dir);
}

void View::on_ouvrirVideo_triggered()
{
    QString dir = QFileDialog::getOpenFileName(this, tr("Open Vids"), "/home/thierry/Vidéos", tr("Movie Files (*.mp4 *.mkv *.mpg *.flv *.avi *.mov *.gif)"));
    control->readFile(dir, 2);
}

void View::on_focusImage_triggered()
{
    control->selectImage();
}

void View::on_focusVideo_triggered()
{
    control->selectVideo();
}

void View::on_pelemeleNB_triggered()
{
    ui->spinBox->setVisible(false);
    control->setChoixPelemele(1);
    QMessageBox::information(this, "Pele-mele", "Le pele-mele sera en niveau de gris.", QMessageBox::Ok, QMessageBox::Ok);
}

void View::on_pelemeleCouleur_triggered()
{
    ui->spinBox->setVisible(false);
    control->setChoixPelemele(2);
    QMessageBox::information(this, "Pele-mele", "Le pele-mele sera en couleurs.", QMessageBox::Ok, QMessageBox::Ok);
}

void View::on_pelemeleGauss_triggered()
{
    ui->spinBox->setVisible(true);
    control->setChoixPelemele(3);
    QMessageBox::information(this, "Pele-mele", "Le pele-mele sera floute. \nVous pouvez modifier la taille (le taux de floutage).", QMessageBox::Ok, QMessageBox::Ok);
}

void View::on_detection_de_contour_triggered()
{
    ui->spinBox->setVisible(false);
    control->setChoixPelemele(4);
    QMessageBox::information(this, "Pele-mele", "Le pele-mele detectera les contours.", QMessageBox::Ok, QMessageBox::Ok);
}

void View::on_seqX_triggered()
{
    control->setChoixSeq(1);
    QMessageBox::information(this, "Sequencage", "Le sequencage sera effectue en arrangeant les videos horizontalement.", QMessageBox::Ok, QMessageBox::Ok);
}

void View::on_seqY_triggered()
{
    control->setChoixSeq(2);
    QMessageBox::information(this, "Sequencage", "Le sequencage sera effectue en arrangeant les videos verticalement.", QMessageBox::Ok, QMessageBox::Ok);
}

void View::on_pushButton_clicked()
{
    control->applySingle();
}

void View::on_pushButton_selectImages_clicked()
{
    control->selectImage();
}

void View::on_pushButton_selectVideos_clicked()
{
    control->selectVideo();
}

