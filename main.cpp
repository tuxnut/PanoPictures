#include <QApplication>
#include <QDebug>

#include "view.h"
#include "model.h"
#include "control.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w;
    Model m;
    Control (&m, w);
    // 1868 lignes
    w.show();

    return a.exec();
}

// compte rendu valGrind
/*!
 * 24 (16 direct, 8 indirect) bytes in 1 blocks are definitely lost in loss record 3,823 of 13,796
  dans main dans /home/thierry/project/main.cpp:23
 * 41,296 (408 direct, 40,888 indirect) bytes in 1 blocks are definitely lost in loss record 13,784 of 13,796
  dans Control::readFile(QString const&amp;, int) dans /home/thierry/project/control.cpp:47
 * 42,912 (16 direct, 42,896 indirect) bytes in 1 blocks are definitely lost in loss record 13,786 of 13,796
  dans Control::applySingle() dans /home/thierry/project/control.cpp:336
* 43,060 (1,456 direct, 41,604 indirect) bytes in 1 blocks are definitely lost in loss record 13,787 of 13,796
  dans Control::applySingle() dans /home/thierry/project/control.cpp:325
* 43,716 (408 direct, 43,308 indirect) bytes in 1 blocks are definitely lost in loss record 13,788 of 13,796
  dans Control::applySingle() dans /home/thierry/project/control.cpp:336
* 44,040 (24 direct, 44,016 indirect) bytes in 1 blocks are definitely lost in loss record 13,789 of 13,796
  dans Control::applySingle() dans /home/thierry/project/control.cpp:324
* 3,739,947 (64 direct, 3,739,883 indirect) bytes in 1 blocks are definitely lost in loss record 13,796 of 13,796
  dans Control::readFile(QString const&amp;, int) dans /home/thierry/project/control.cpp:71

* Conditional jump or move depends on uninitialised value(s)
  dans filters::grayScale() dans /home/thierry/project/filters.cpp:236
* Conditional jump or move depends on uninitialised value(s)
  dans filters::grayScale() dans /home/thierry/project/filters.cpp:236
* Conditional jump or move depends on uninitialised value(s)
  dans filters::grayScale() dans /home/thierry/project/filters.cpp:236
* Conditional jump or move depends on uninitialised value(s)
  dans filters::grayScale() dans /home/thierry/project/filters.cpp:236
* Conditional jump or move depends on uninitialised value(s)
  dans Control::applySingle() dans /home/thierry/project/control.cpp:335
* Conditional jump or move depends on uninitialised value(s)
  dans Control::applySingle() dans /home/thierry/project/control.cpp:335
* Conditional jump or move depends on uninitialised value(s)
  dans filters::convolution2D() dans /home/thierry/project/filters.cpp:276
* Conditional jump or move depends on uninitialised value(s)
  dans filters::convolution2D() dans /home/thierry/project/filters.cpp:276
* Conditional jump or move depends on uninitialised value(s)
  dans filters::convolution2D() dans /home/thierry/project/filters.cpp:276
* Conditional jump or move depends on uninitialised value(s)
  dans filters::convolution2D() dans /home/thierry/project/filters.cpp:276
* Conditional jump or move depends on uninitialised value(s)
  dans filters::convolution2D() dans /home/thierry/project/filters.cpp:277
* Conditional jump or move depends on uninitialised value(s)
  dans filters::convolution2D() dans /home/thierry/project/filters.cpp:277
* Conditional jump or move depends on uninitialised value(s)
  dans filters::convolution2D() dans /home/thierry/project/filters.cpp:277
* Conditional jump or move depends on uninitialised value(s)
  dans filters::convolution2D() dans /home/thierry/project/filters.cpp:277
*/
