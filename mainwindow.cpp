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
    QgsLayout* layout = new QgsLayout(QgsProject::instance());
    layout->initializeDefaults();

    QgsSimpleFillSymbolLayer* fill = new QgsSimpleFillSymbolLayer();
    fill->setColor(QColor(0, 0, 0));
    QgsSymbolLayerList fill_list = {fill};

    QgsFeatureIds featIds = moscowLayer->allFeatureIds();
    for(int i = 0; i<1; ++i){
        QgsFeature feat = moscowLayer->getFeature(*(featIds.begin()+i));
        if(feat.hasGeometry()){
            QgsLayoutItemShape * item = new QgsLayoutItemShape(layout);
            item->attemptSetSceneRect(QRectF(0, 0, 1000, 1000));
            QgsPolygon* poly = new QgsPolygon();
            poly->fromWkt(feat.geometry().asWkt());
            QgsRenderContext context = QgsRenderContext();
            context.setGeometry(poly);
            qInfo() << poly->asJson();
            QgsFillSymbol* fill_symbol = new QgsFillSymbol(fill_list);
            fill_symbol->startRender(context);
            fill_symbol->renderPolygon(feat.geometry().asQPolygonF(), nullptr, feat, context);
            fill_symbol->stopRender(context);
            fill_symbol->setColor(QColor(127, 127, 127));
            fill_symbol->changeSymbolLayer(0, fill);
            fill->setStrokeStyle(Qt::NoPen);
            item->setSymbol(fill_symbol);
            layout->addItem(item);
        }
    }
    QgsLayoutExporter* exporter = new QgsLayoutExporter(layout);
    layout->renderContext().setDpi(40);
    QImage image = exporter->renderRegionToImage(QRectF(0, 0, 100, 100));
    image.save("../Image.png", "PNG");
}

MainWindow::~MainWindow()
{
    delete ui;
}