#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QList>
#include <QDebug>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QBrush>
#include <QImage>
#include <QPainter>

#include <qgsmapcanvas.h>
#include <qgsvectorlayer.h>
#include <qgsdatasourceuri.h>
#include <qgsproject.h>
#include <qgslayout.h>
#include <qgslayoutexporter.h>

#include <qgslayoutitem.h>
#include <qgssymbol.h>
#include <qgssymbollayer.h>
#include <qgslayoutitemshape.h>
#include <qgsfillsymbollayer.h>
#include <qgsrendercontext.h>
#include <qgspolygon.h>
#include <qgswkbptr.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget *Map;
    QgsMapCanvas *canvas;
    QList<QgsMapLayer*> layers;
    QgsVectorLayer *baseWaterLayer = new QgsVectorLayer("../world-bathymetry-110-million.shp", "water", "ogr");
    QgsVectorLayer *baseEarthLayer = new QgsVectorLayer("../world-land-areas-110-million.shp", "earth", "ogr");
    QgsVectorLayer *moscowLayer = new QgsVectorLayer("../MoscowRiver.shp", "river", "ogr");
};
#endif // MAINWINDOW_H
