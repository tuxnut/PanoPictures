#include "bucky.h"

/*!
 * \brief bucky::bucky is the first type of constructor : used for initializing a video manipulator thread
 * \param filename directory of the file
 * \param threadId number of the thread
 * \param sem semaphore given by the controler
 */
bucky::bucky(QString filename, int threadId, QSemaphore *sem)
{
    this->fichier = filename;
    this->threadId = threadId;
    this->sem = sem;
    qDebug() << "init thread gris video" << sem->available();
}

/*!
 * \brief bucky::bucky second type of constructor : used for initializing a concatenator thread
 * \param filename directory of the files separated by a space
 * \param threadId number of the thread
 * \param rolefiltreur role of the thread
 * \param filenameOut name of the output file
 * \param sem semaphore given by the controler
 */
bucky::bucky(QString filename, int threadId, bool rolefiltreur, QString filenameOut, QSemaphore *sem)
{
    this->fichier = filename;
    this->threadId = threadId;
    this->rolefiltreur = rolefiltreur;
    this->filenameOut = filenameOut;
    this->sem = sem;
    qDebug() << "init thread assembleur, la sem est  à" << sem->available();
}

bucky::~bucky()
{

}

/*!
 * \brief bucky::run called by 'start()' in the controler, this method is derived from
 * QThread::run().
 *
 * Depending on the role of the thread, it will either transform the colored video into
 * a grascale one exporting it as a .mpg video or concatenate the files given into a
 * single .mp4 file.
 * Every transformation is performed using ffmpeg in command line.
 */
void bucky::run()
{
    if (rolefiltreur)
    {
        QString fichierSortie = fichier + "output" + QString::number(threadId)+".mpg";

        QString cmd = "ffmpeg -i "+ fichier +" -filter_complex 'colorchannelmixer=.3:.4:.3:0:.3:.4:.3:0:.3:.4:.3' -qscale 0 -y "+fichierSortie;
        QByteArray ba = cmd.toLatin1();
        const char *c_str = ba.data();
        system(c_str);
    } else {
        qDebug() << "debut du thread assembleur";
        QString concat = "cat " + fichier;
        QString cmd = concat + "| ffmpeg -f mpeg -i - -strict -2 -q:v 0 -vcodec mpeg4 -y "+filenameOut+".mp4";
        QByteArray ba = cmd.toLatin1();
        const char *c_str = ba.data();
        system(c_str);
    }
    sem->acquire(1);
    qDebug() << "après thread " << threadId << " il reste : "<< sem->available();
    quit();
}
