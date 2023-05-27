#include <QApplication>
#include <QPushButton>
#include <fstream>
#include <QFile>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

//    QString data;
//    float redData[256 * 256];
//    float restData[256 * 256] = {0};
//
//    QFile file("/home/trico/Documents/GithubProjects/32fp2hdr/256floats.txt");
//    file.open(QFile::ReadOnly);
//    data.append(file.readAll());
//    data.replace(",", ".");
//
//    auto list = data.split('\n');
//
//    for(int i = 0; i < 256 ; i++)
//    {
//        for(int j = 0; j < 256; j++)
//            redData[(i * 256) + j] = list[i].toFloat();
//    }
//
//    auto handle = CFIleSystem::createHandle(CFIleSystem::RLE_RGBE, 256, 256, redData, restData, restData );
//
//    CFIleSystem::rotate90Degrees(handle);
//
//    CFIleSystem::writeHDR("/home/trico/Documents/GithubProjects/32fp2hdr/256floats.hdr", handle);

    auto window = new MainWindow();
    window->show();

    return QApplication::exec();
}
