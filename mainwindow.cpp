#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFile>


#include "mainwindow.h"
#include "modelloader.h"
#include "jsonobjectloader.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , standardModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    init_controls();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_controls() {
    load_data_model(data_model_path);
    load_data_file("example.json");
}

void
MainWindow::on_load() {

}

void
MainWindow::on_save() {

}

int
MainWindow::load_data_model(const QString & path) {
   ModelLoader ml;

   m_model_map = ml.loadAsMap(path);

   auto types = m_model_map["types"].toList();
   for (int i = 0; i < types.size(); ++i)
       m_types.push_back(types[i].toString());

   auto enums_list = m_model_map["enumerations"].toList();

   for (int i = 0; i < enums_list.size(); ++i) {
       auto enum_map = enums_list[i].toMap();
       QString name = enum_map["name"].toString();
       auto values = enum_map["values"].toList();
       /*QMapIterator<QString, QVariant> iter(values);
       while (iter.hasNext()) {
           iter.next();
           m_enums[name + iter.value()];
       }*/
       for (int j = 0; j < values.size(); ++j)
           m_enums.insertMulti(name, values[j].toString());
           // m_enums[values["name"]] = values.values()[j];
   }

//   auto classes0 = m_model_map["classes"];
//   auto classes1 = m_model_map["classes"].toMap();
//   auto classes2 = m_model_map["classes"].toList();
   m_model_classes = m_model_map["classes"].toList();

   return 0;
}

int
MainWindow::load_data_file(const QString & path) {
    JsonObjectLoader jsl;
    QVariantMap map = jsl.loadAsMap(path);

    QStandardItem *item = standardModel->invisibleRootItem();

    QVariantMap features = map["features"].toMap();

    QVariantList list = features["feature"].toList();

    for (int i = 0; i < list.size(); ++i) {
        QVariantMap obj = list[i].toMap();
        item->appendRow(new QStandardItem(obj.keys().first() + "-" + obj.values().first().toMap()["id"].toString()));
    }

    /*foreach (QVariant str, list) {
        item->appendRow(new QStandardItem(str.toString()));
    }*/

    ui->treeView->setModel(standardModel);
    ui->treeView->expandAll();

    return 0;
}

QList<QStandardItem *> MainWindow::prepareRow(const QString &first,
                                              const QString &second,
                                              const QString &third) const
{
    return {new QStandardItem(first),
            new QStandardItem(second),
            new QStandardItem(third)};
}

/*
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , treeView(new QTreeView(this))
    , standardModel(new QStandardItemModel(this))
{
    setCentralWidget(treeView);

    QList<QStandardItem *> preparedRow = prepareRow("first", "second", "third");
    QList<QStandardItem *> preparedRow1 = prepareRow("first1", "second1", "third1");
    QStandardItem *item = standardModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item->appendRow(preparedRow);

    QList<QStandardItem *> secondRow = prepareRow("111", "222", "333");
    QList<QStandardItem *> secondRow1 = prepareRow("111", "222", "334");
    QList<QStandardItem *> secondRow2 = prepareRow("111", "222", "335");
    // adding a row to an item starts a subtree
    preparedRow.first()->appendRow(secondRow);
    preparedRow.first()->appendRow(secondRow1);
    preparedRow.first()->appendRow(secondRow2);

    item->appendRow(preparedRow1);

    treeView->setModel(standardModel);
    treeView->expandAll();
}*/

/*bool Game::loadGame(Game::SaveFormat saveFormat)
{
    QFile loadFile(saveFormat == Json
        ? QStringLiteral("save.json")
        : QStringLiteral("save.dat"));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(saveFormat == Json
        ? QJsonDocument::fromJson(saveData)
        : QJsonDocument::fromBinaryData(saveData));

    read(loadDoc.object());

    QTextStream(stdout) << "Loaded save for "
                        << loadDoc["player"]["name"].toString()
                        << " using "
                        << (saveFormat != Json ? "binary " : "") << "JSON...\n";
    return true;
}*/
