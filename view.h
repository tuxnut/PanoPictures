#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsScene>
#include <QFileDialog>
#include <QMainWindow>
#include <QLabel>
#include <QProgressDialog>
#include <QGraphicsProxyWidget>
#include <QListWidget>

/*! \class View
 * \brief The view class performs all the necessary to display the GUI.
 *
 * Its role is to adjust the files given by the controler to make a user-friendly interface.
 */

class Control;

namespace Ui {
class View;
}

class View : public QMainWindow
{
    Q_OBJECT

public:
    explicit View(QWidget *parent = 0);
    ~View();
    void setControl(Control *control);
    void printFile(QGraphicsItem *pixmapItem, bool video, QListWidgetItem *widgetItem = 0);
    void printFile(QMovie *movie);
    QString saveFile();
    QGraphicsScene *getScene();
    //QGraphicsItemGroup *getVideos();
    QGraphicsItem *getSelectedItem();
    QListWidget *getListWidget();
    QPixmap* getPelemele();
    int getFilterSize();
    void closeEvent ( QCloseEvent * event );
    void mousePressEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void timerEvent(QTimerEvent* event);
    void displayPelemele(QPixmap * image);
    void displaySequencage(QString filenameout);
    void pelemeleChoice();
    void displayProgress(int maximum);
    void incProgress(int step);
    void displayStatus(QString status);

private slots:
    void on_pushButton_ouvrirImage_clicked();
    void on_pushButton_setZValue_clicked();
    void on_pushButton_pelemele_clicked();
    void on_pushButton_ouvrirVideo_clicked();
    void on_pushButton_sequencage_clicked();

    void on_ouvrirImage_triggered();
    void on_ouvrirVideo_triggered();
    void on_focusImage_triggered();
    void on_focusVideo_triggered();
    void on_pelemeleNB_triggered();
    void on_pelemeleCouleur_triggered();
    void on_pelemeleGauss_triggered();
    void on_detection_de_contour_triggered();
    void on_seqX_triggered();
    void on_seqY_triggered();
    void on_pushButton_clicked(); //applySingle()
    void on_pushButton_selectImages_clicked();
    void on_pushButton_selectVideos_clicked();

private:
    Ui::View *ui;
    Control *control;
    QGraphicsScene *scene;
    QLabel* display;
    QProgressDialog* progressDialog;
    QPixmap* G_Blanc;
};

#endif // VIEW_H
