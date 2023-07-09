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

    QgsLayoutItemShape * item = new QgsLayoutItemShape(layout);
    item->attemptSetSceneRect(QRectF(0, 0, 1000, 1000));
    QgsSimpleFillSymbolLayer* fill = new QgsSimpleFillSymbolLayer();
    QgsSymbolLayerList fill_list = {fill};
    QgsFillSymbol* fill_symbol = new QgsFillSymbol(fill_list);

    QgsFeatureIds featIds = moscowLayer->allFeatureIds();
    QgsRenderContext context = QgsRenderContext();
    QgsFeature feat = *(featIds.begin()+1);
    QgsPolygon* poly = new QgsPolygon();
    QgsConstWkbPtr wkbPtr = QgsConstWkbPtr(QgsCircle(100, 100).toCircularString()->asWkb());
    poly->fromWkb(wkbPtr);
    context.setGeometry(poly);
    fill_symbol->renderFeature(*(featIds.begin()+1), context);
    fill->setColor(QColor(127, 127, 127));
    item->setSymbol(fill_symbol);

    QgsLayoutExporter* exporter = new QgsLayoutExporter(layout);
    QgsLayoutExporter::ImageExportSettings exportSettings;
    exportSettings.imageSize = QSize(1200, 1200);
    exporter->exportToImage("../Image.png", exportSettings);
}

MainWindow::~MainWindow()
{
    delete ui;
}