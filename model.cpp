#include "model.h"

#include <QDebug>

Model::Model()
{
}

/*!
 * \brief Model::readFilePic load the image file at the directory filename
 * \param filename directory of the file
 *
 * The file is opened as a QPixmap.
 */
bool Model::readFilePic(const QString &filename)
{
    if (!QFile::exists(filename)) {
        return false;
    }
    QFile f(filename);
    if (!f.open(QIODevice::ReadWrite)) {
        return false;
    }
    jonathan = QPixmap(filename);

    return true;
}

/*!
 * \brief Model::readFileVids load the video file at the directory filename
 * \param filename directory of the file.
 */
bool Model::readFileVids(const QString &filename)
{
    if (!QFile::exists(filename)) {
        return false;
    }
    QFile f(filename);
    if (!f.open(QIODevice::ReadWrite)) {
        return false;
    }
    vids = filename;

    return true;
}

/*!
 * \brief Model::picContent getter of the image file opened
 */
QPixmap Model::picContent()
{
    return jonathan;
}

/*!
 * \brief Model::vidsContent getter of the directory of the video file opened
 */
QString Model::vidsContent()
{
    return vids;
}

/*!
 * \brief Model::saveFile save the image at the directory filname
 * \param fileName directory to store the output file
 * \param image image to be saved
 */
void Model::saveFile(const QString &fileName, QPixmap *image)
{
    image->save(fileName + ".png");
}

/*!
 * \brief Model::removeFile remove a list of files
 * \param fileToRemove list of directory
 *
 * A counter cpt is created to control the right number of files deleted
 */
bool Model::removeFile(QList <QString> fileToRemove)
{
    int cpt = 0;
    for (int i = 0; i < fileToRemove.count(); i++)
    {
        //qDebug() << fileToRemove[i];
        QByteArray ba = fileToRemove[i].toLatin1();
        const char *cmd = ba.data();
        remove(cmd);
        if (QFile::exists(fileToRemove[i]))
            return false;
        else
            cpt++;
    }
    if (fileToRemove.count() == cpt)
        return true;
    else
        return false;
}

/*!
 * \brief Model::removeFile overloaded function
 * \param fileToRemove single file to be removed
 */
bool Model::removeFile(QString fileToRemove)
{
    QByteArray ba = fileToRemove.toLatin1();
    const char *cmd = ba.data();
    remove(cmd);
    if (QFile::exists(fileToRemove))
        return false;
    else
        return true;
}
