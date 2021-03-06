#include <QApplication>
#include <QFile>
#include <QDir>
#include <QTextCodec>
#include <QTranslator>
#include <QFontDatabase>
#include <QDebug>
#include "mainwindow.h"

void loadTranslations(QApplication* a, QString lang)
 {
  QDir dir("i18n");
  foreach (QString s, dir.entryList(QStringList("*" + lang + ".qm")))
   {
    QTranslator* t = new QTranslator;
    qDebug() << "load translation file" << s << ":"
                 << (t->load(dir.absolutePath() + '/' + s) ? "success" : "failed");
    a->installTranslator(t);
   }
 }

#if (QT_VERSION < QT_VERSION_CHECK(5, 2, 0))
void emptyOutput ( QtMsgType type, const char *msg )
#else
void emptyOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
#endif
 {
 }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Zodiac");
    a.setApplicationVersion("v0.7.1 (build 2014-06-30)");

#if (QT_VERSION < QT_VERSION_CHECK(5, 2, 0))
    QTextCodec* codec = QTextCodec::codecForName ( "UTF-8" );
    QTextCodec::setCodecForCStrings ( codec );
    QTextCodec::setCodecForTr ( codec );
    qInstallMsgHandler (emptyOutput);
#else
    qInstallMessageHandler(emptyOutput);
#endif

    QDir::setCurrent(a.applicationDirPath());
    QString lang = "";
    if (!a.arguments().contains("nolocale"))
     {
      if (QLocale::system().name().contains("RU", Qt::CaseInsensitive))
        lang = "ru";
      else
        lang = "en";

      loadTranslations(&a, lang);
     }

    QFontDatabase::addApplicationFont("fonts/Almagest.ttf");
    A::load(lang);
    MainWindow w;

    QFile cssfile ( "style/style.css" );
    cssfile.open  ( QIODevice::ReadOnly | QIODevice::Text );
    w.setStyleSheet  ( cssfile.readAll() );

    w.show();
    return a.exec();
}
