#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Map=ui->Map;
    canvas = new QgsMapCanvas(Map);

    canvas->enableAntiAliasing(true);
    canvas->setMapSettingsFlags(canvas->mapSettings().flags() | QgsMapSettings::RenderPreviewJob);
    canvas->setParallelRenderingEnabled(true);
    canvas->setCachingEnabled(true);
    canvas->setPreviewJobsEnabled(true);
    canvas->setMapUpdateInterval(500);

    //layers.push_back(baseWaterLayer);
    layers.push_back(moscowLayer);

    //start of water

    canvas->setLayers(layers);
    for(int i=0;i<layers.length();i++)
        canvas->setExtent(layers[i]->extent());
    canvas->refresh();

    QGridLayout* gl =new QGridLayout(Map);
    gl->addWidget(canvas);

    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    QPen pen(Qt::white);

    int CONST_SZ = 1000;

    QGraphicsScene* scene = new QGraphicsScene(0,0,10000000/CONST_SZ,10000000/CONST_SZ);
    

    QgsFeatureIds featIds = moscowLayer->allFeatureIds();
    for(int i = 0; i<featIds.size(); ++i){
        QgsFeature feat = moscowLayer->getFeature(*(featIds.begin()+i));
        if(feat.hasGeometry()){
            QgsPolygon* poly = new QgsPolygon();
            poly->fromWkt(feat.geometry().asWkt());
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(poly->asJson().toUtf8(), &parseError);
            QJsonObject obj = doc.object();
            QJsonValue val = obj["coordinates"];
            QJsonArray arr = val[0].toArray();
            QVector<QPointF> points;
            for(int i = 0; i<arr.size(); ++i){
                points << QPointF(((arr[i].toArray())[0].toDouble()-37)*10000000/CONST_SZ, ((arr[i].toArray())[1].toDouble()-55)*10000000/CONST_SZ);
            }
            QPolygonF polygon(points);
            //for(int i = 0; i<points.size(); ++i) qInfo() << points[i];
            scene->addPolygon(polygon, pen, brush);
        }
    }
    scene->clearSelection();                                                  // Selections would also render to the file
    //scene->setSceneRect(scene->itemsBoundingRect());                          // Re-shrink the scene to it's bounding contents
    QImage image(scene->sceneRect().size().toSize(), QImage::Format_Mono);  // Create the image with the exact size of the shrunk scene
    //image.fill(QColor(0,0,0));
    image.fill(0);                                       // Start all pixels transparent
    QPainter painter(&image);
    scene->render(&painter);
    image.save("../Output.png");
    qInfo() << "success";
}

MainWindow::~MainWindow()
{
    delete ui;
}