#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QFile>
#include <QPixmap>

/*! \class Model
 * \brief The model class is the class that performs loading & writing of files.
 * It is also called to remove files created temporarly.
 *
 */

class View;

class Model
{
private :
    QPixmap jonathan;
    QString vids;

public:
    Model();
    bool readFilePic(const QString &filename);
    bool readFileVids(const QString &filename);
    QPixmap picContent();
    QString vidsContent();
    void saveFile(const QString &fileName, QPixmap* image);
    bool removeFile(QList <QString> fileToRemove);
    bool removeFile(QString fileToRemove);
};

#endif // MODEL_H
