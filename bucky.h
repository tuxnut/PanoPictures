#ifndef BUCKY_H
#define BUCKY_H

#include <QThread>
#include <QDebug>
#include <QSemaphore>

/*!
 * \brief The bucky class is a class derived from QThread that performs
 *  video manipulation.
 *
 * Each instantiation of 'bucky' can be seen as a thread, whose arguments are given
 * with the constructor. There are typically two types of threads for the video :
 * the first kind changes the color of the video to grayscale colors with exporting the video
 * as a mpeg video, which facilitates the second kind that performs a concatenation of
 * the exported video as a single file.
 */

class bucky : public QThread
{
public:
    bucky(QString filename, int threadId, QSemaphore *sem);
    bucky(QString filename, int threadId, bool rolefiltreur, QString filenameOut, QSemaphore *sem);
    ~bucky();
    void run();

private:
    QSemaphore* sem;
    bool rolefiltreur;
    int threadId;
    QString fichier;
    QString filenameOut;
};

#endif // BUCKY_H
