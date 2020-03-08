#ifndef FILTERS_H
#define FILTERS_H

#include <QThread>
#include <QImage>
#include <QColor>
#include <QDebug>

/*!
 * \brief The filters class a class derived from QThread that performs
 *  image manipulation.
 *
 *  Each instantiation of 'filters' can be seen as a thread, whose arguments are given
 *  with the constructor.
 */

class filters : public QThread
{
public:
    filters(QImage* image, int filterType = 1);
    ~filters();
    void run();
    void setFilterSize(int filterSize);
    int setMatriceFiltre(int typeOfFilter);
    QRgb** getRGBData();

private:
    int filterSize;
    int filterType;
    qreal ** matriceFiltre;
    QImage* img;

    void grayScale();
    void convolution2D();
    int normalisation(int color);
};

#endif // FILTERS_H
