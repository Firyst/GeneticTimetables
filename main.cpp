#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
 /*
    QTranslator translator;
    translator.load("QtLanguage_ru_RU",".");
    qApp->installTranslator(&translator);

*/
    // Определяем путь к директории с переводами
    QString translationsDir = ":/translations";

    // Получаем язык системы
    QLocale systemLocale = QLocale::system();
    QString localeName = systemLocale.name();

    // Загружаем перевод для выбранного языка
    //if (translator.load(localeName, translationsDir)) {
      //  app.installTranslator(&translator);
    //}


    MainWindow w;
    w.show();
    return a.exec();
}
