#include "filters.h"
#include <QtCore/qmath.h>

/*!
 * \brief filters::filters constructor of filters, used for initializing an image manipulator thread
 * \param image file we'll be working on
 * \param filterType type of filter
 */
filters::filters(QImage* image, int filterType)
{
    this->img = image;
    this->filterType = filterType;
    matriceFiltre = NULL;
}

/*!
 * \brief filters::~filters destructor of filters
 */
filters::~filters()
{
    //donc je peux pas "free" un truc NON nul ??? free(): invalid pointer: 0x00007ffe6ac92238

    qDebug() << "destr. filters";
    img = NULL;
    if (matriceFiltre != NULL)
    {
        for (int line = 0; line < filterSize; line++)
        {
            free(matriceFiltre[line]);
            matriceFiltre[line] = NULL;
        }
        free(matriceFiltre);
        matriceFiltre = NULL;
    }
}

/*!
 * \brief filters::run run called by 'start()' in the controler, this method is derived from
 * QThread::run().
 *
 * Each type of filter has its own function. Some are basic as we just need to replace
 * the ARGB value of the pixels, others need a kernel matrice to operate.
 */
void filters::run()
{
    switch (filterType) // = choixPelemele
    {
    case 1:
        grayScale();
        break;
    case 3:
        if (setMatriceFiltre(1) != 1)
            quit();
        else
        {
            convolution2D();
        }
        break;
    case 4:
        qDebug() << "1";
        if (setMatriceFiltre(2) != 1)
            quit();
        else
        {
            convolution2D();
        }
        break;
        break;
    default:
        break;
    }
    quit();
}

/*!
 * \brief filters::setFilterSize set the size of the filter to filterSize
 * \param filterSize size of the filter
 */
void filters::setFilterSize(int filterSize)
{
    this->filterSize = filterSize;
}

/*!
 * \brief filters::getRGBData get a complete table of the pixel of an image
 *
 * The value return is actually even bigger than the image. To operate a convolution
 * onthe borders of the image, we need value 'outside' the image.
 */
QRgb** filters::getRGBData()
{
    int marge = filterSize-1;

    QRgb ** pixel = (QRgb **) malloc((img->width() + marge) * sizeof(QRgb*));
    if (pixel == NULL)
        quit();
    else
    {
        for (int line = 0; line < img->width() + marge; line++)
        {
            pixel[line] = (QRgb*) malloc((img->height() + marge) * sizeof(QRgb));
        }
        if (pixel[0] == NULL)
            quit();
    }
    //memset(pixel, QColor(0, 0, 0, 0).rgba(), (img->height()+marge)*(img->width()+marge));

    for (int i = 0 ; i < (img->width() + marge); i++)
    {
        for (int j = 0; j < (img->height() + marge); j++)
        {
            if ( i >= marge/2 && i < img->width() + marge/2 && j >= marge/2 && j < img->height() + marge/2)
                pixel[i][j] = img->pixel(i - marge/2, j - marge/2);
            else
                pixel[i][j] = QColor(0, 0, 0, 0).rgba();
        }
    }
    return pixel;
}

/*!
 * \brief filters::setMatriceFiltre calculates the kernel matrice if a convolution is to be made
 * \param typeOfFilter type of filter
 */
int filters::setMatriceFiltre(int typeOfFilter)
{
    qreal ** matrice;

    if (typeOfFilter == 1)
    {
        if ( ! (filterSize % 2))       // s'assurer d'un filtre d'une taille impaire
            filterSize++;

        //allocation (on pourrait faire un filtre 1D mais après c'est chaud (passer vert. puis horiz))
        matrice = (qreal **) malloc (filterSize * sizeof(qreal*));
        if (matrice == NULL)
        {
            return 0;
        }
        else
        {
            for (int line = 0; line < filterSize; line++)
                matrice[line] = (qreal*) malloc (filterSize * sizeof(qreal));
        }

        if (matrice[0] == NULL)
        {
            return 0;
        }

        //début des calculs
        qreal radius = filterSize / 2.;        //radius = 2.5
        qreal sigma = filterSize / 3.;
        qreal norm = 0;

        for (int i = 0; i < radius; i++)    // i (0, 1, 2)
        {
            for (int j = 0; j < filterSize; j++)
            {
                if (j > radius)
                {
                    matrice[i][j] = matrice[i][filterSize-1-j];
                    norm += matrice[i][j];
                }
                else
                {
                    matrice[i][j] = qExp(-(qPow(radius-i, 2) + qPow(radius-j, 2)) / (2 * qPow(sigma, 2))) / (2 * M_PI * qPow(sigma, 2));
                    norm += matrice[i][j];
                }
            }
        }
        for (int i = radius + 1; i < filterSize; i++)
        {
            for (int j = 0; j < filterSize; j++)
            {
                matrice[i][j] = matrice[filterSize-1-i][j];
                norm += matrice[i][j];
            }
        }

        for (int i = 0 ; i < filterSize; i ++)
        {
            for (int j = 0 ; j < filterSize; j ++)
            {
                matrice[i][j] /= norm;      //on normalise le filtre
            }
        }

        matriceFiltre = matrice;
        matrice = NULL;
        return 1;

    } else if (typeOfFilter == 2)    {

        //allocation (on pourrait faire un filtre 1D mais après c'est chaud (passer vert. puis horiz))
        matrice = (qreal **) malloc (filterSize * sizeof(qreal*));
        if (matrice == NULL)
        {
            return 0;
        }
        else
        {
            for (int line = 0; line < filterSize; line++)
                matrice[line] = (qreal*) malloc (filterSize * sizeof(qreal));
        }

        if (matrice[0] == NULL)
        {
            return 0;
        }

        matrice[0][0] = -1; matrice[0][1] = -1; matrice[0][2] = -1;
        matrice[1][0] = -1; matrice[1][1] = 8; matrice[1][2] = -1;
        matrice[2][0] = -1; matrice[2][1] = -1; matrice[2][2] = -1;
        qDebug() << "2";
        matriceFiltre = matrice;
        matrice = NULL;
        return 1;
    }
    return 0;
}

/*!
 * \brief filters::grayScale performs a grayscale on the image
 *
 * We use a 'QImage::scanLine' as it is faster than a simple for-loop
 */
void filters::grayScale()
{
    for (int x = 0; x < img->height(); x++) {
        uchar  *index = img->scanLine(x);
        int depth = 4; //puisque c'est du aRGB
        for (int y = 0; y < img->width(); y++) {

            QRgb *pixel = reinterpret_cast <QRgb*> (index + y * depth);
            int gray = qGray(*pixel);
            *pixel = QColor(gray, gray, gray, qAlpha(*pixel)).rgba();
        }
    }
}

/*!
 * \brief filters::convolution2D performs a convolution between the matrice and the image
 */
void filters::convolution2D()
{
    QRgb ** data = getRGBData();

    //int marge = filterSize - 1;
    int radius = filterSize/2;

    for (int x = radius; x < img->width() + radius; x++)
    {
        for (int y = radius; y < img->height() + radius; y++)
        {
            int red = 0;
            int green = 0;
            int blue = 0;
            int alpha = 0;
            for (int i = 0; i < filterSize; i++)
            {
                for (int j = 0; j < filterSize; j++)
                {
                    red += qRed(data[x+i - radius][y+j - radius]) * matriceFiltre[i][j];
                    green += qGreen(data[x+i - radius][y+j - radius]) * matriceFiltre[i][j];
                    blue += qBlue(data[x+i - radius][y+j - radius]) * matriceFiltre[i][j];
                    alpha+= qAlpha(data[x+i - radius][y+j - radius]) * matriceFiltre[i][j];
                }
            }/*
            if (filterType == 4)
            {
                red = normalisation(red);
                green = normalisation(green);
                blue = normalisation(blue);
                alpha = normalisation(alpha);
            }*/
            data[x][y] = QColor(red, green, blue, alpha).rgba();
            img->setPixel(x - radius, y - radius, QColor(red, green, blue, alpha).rgba());
        }
    }

    for (int line = 0; line < img->height() + filterSize - 1; line++)
    {
        free(data[line]); data[line] = NULL;
    }
    free(data);
    data = NULL;
}

/*!
 * \brief filters::normalisation normalize the color to an aRGB value (0-255)
 * \param color color to be normalized
 */
int filters::normalisation(int color)
{
    if (color > 0 && color < 2550)
        color = (int) 255 * qLn(1+qFabs(color)) / qLn(1+255);
    return color;
}
