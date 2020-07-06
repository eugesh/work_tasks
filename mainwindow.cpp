#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFile>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>

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

    // ToDo: do write after edit.
    // connect(standardModel, &QStandardItemModel::itemChanged);

    // ToDo: different source and ouput types.

    QItemSelectionModel * treeSelModel = ui->treeView->selectionModel();
    connect(treeSelModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::onSelectionChanged);
}

void
MainWindow::on_load() {

}

void
MainWindow::on_save() {

}

void
MainWindow::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    Q_UNUSED(deselected)

    QModelIndex parent_index = selected.indexes().first().parent();
    QModelIndex index = selected.indexes().first();

    if (! parent_index.isValid() || ! index.isValid())
        return;

    QString classname = parent_index.data().toString();
    QString name = index.data().toString();

    // QVariantMap obj = m_all_objects.values(name).first().toMap().values(classname).first().toMap();
    QVariantMap obj = m_all_objects.values(name).first().toMap();

    populateLayout(obj);

    /*for (int i = 0; object_list.size(); ++i) {
        QVariantMap obj = object_list[i].toMap();
        if (obj[""])
    }*/

    // QString key = map_classname_key[classname];

    //QVariantMap obj = object_list[index.row()].toMap();

    /*QMap<QString, uint32_t>::const_iterator iter = m_classname_row_map.constBegin();

    while (iter != m_classname_row_map.constEnd()) {
        //cout << iter.key() << ": " << i.value() << endl;
        ++iter;
    }*/
}

void
MainWindow::populateLayout(const QVariantMap & data) {
    QVBoxLayout * layout = new QVBoxLayout;

    QString classname = data.keys().first();
    QVariantMap obj = data.values(classname).first().toMap();

    // Clear content.
    if (ui->widget->layout() != nullptr) {
        for (int i = 0; ! widget_list.isEmpty(); ++i) {
            delete widget_list.takeLast();
        }
    }

    delete ui->widget->layout();

    // Show ID first.
    if (obj.contains("id")) {
        QLabel *lbl = new QLabel("ID :");
        layout->addWidget(lbl);
        widget_list.push_back(lbl);
        QWidget *wdgt = new QLabel(obj.find("id").value().toString());
        layout->addWidget(wdgt);
        widget_list.push_back(wdgt);
        obj.remove("id");
    }

    QMap<QString, QVariant>::const_iterator iter = obj.constBegin();

    while (iter != obj.constEnd()) {
        QLabel *lbl1 = new QLabel(iter.key() + ":");
        layout->addWidget(lbl1);
        widget_list.push_back(lbl1);

        QString key = iter.key();
        QVariant val = iter.value();
        QWidget *wdgt = widget_from_data(key, val);
        // QLabel *lbl2 = new QLabel(iter.value().toString());
        layout->addWidget(wdgt);
        widget_list.push_back(wdgt);
        ++iter;
    }

    ui->widget->setLayout(layout);
    ui->widget->update();
}

QWidget *
MainWindow::widget_from_data(const QString &key, const QVariant & obj) const {
    QString type_str = m_map_name_type[key];

    if (key == "id")
        return new QLabel(obj.toString());

    if (type_str == "text") {
        return new QLineEdit(obj.toString());
    } else if (type_str == "id") {
        return new QLabel(obj.toString());
    } else if (type_str == "integer") {
        QSpinBox *spb = new QSpinBox();
        spb->setValue(obj.toInt());
        return spb;
    } else if (type_str == "lat_lon") {
        QWidget *wgt = new QWidget;
        QHBoxLayout *lay = new QHBoxLayout;
        QDoubleSpinBox *spb1 = new QDoubleSpinBox();
        QDoubleSpinBox *spb2 = new QDoubleSpinBox();
        QString str = obj.toMap().values().first().toString();
        QStringList str_list = str.split(" ");
        spb1->setValue(str_list.first().toDouble());
        spb2->setValue(str_list.last().toDouble());
        lay->addWidget(spb1);
        lay->addWidget(spb2);
        wgt->setLayout(lay);
        return  wgt;
    } else if (type_str == "lat_lon_list") {
        QWidget *wgt = new QWidget;
        QVBoxLayout *v_lay = new QVBoxLayout;
        QVariantList list_tmp = obj.toMap().values();
        QVariantList list = list_tmp.first().toMap().values().first().toList();
        for (int i = 0; i < list.size(); ++i) {
            QDoubleSpinBox *spb1 = new QDoubleSpinBox();
            QDoubleSpinBox *spb2 = new QDoubleSpinBox();
            QString str = list[i].toString();
            QStringList str_list = str.split(" ");
            spb1->setValue(str_list.first().toDouble());
            spb2->setValue(str_list.last().toDouble());
            QHBoxLayout *h_lay = new QHBoxLayout;
            h_lay->addWidget(spb1);
            h_lay->addWidget(spb2);
            v_lay->addItem(h_lay);
        }
        wgt->setLayout(v_lay);
        return wgt;
    } else if (type_str == "EnumAirspaceType" ||
               type_str == "EnumRouteDirection") {
        QComboBox *box = new QComboBox;
        box->insertItems(0, m_enums.values(type_str));
        box->setCurrentText(obj.toString());
        return box;
    }
    else {
        return new QLabel(obj.toString());
    }

    /*if (m_model_map.contains(key)) {
        /*if (m_model_map[key].toString()) {

        }*/
    /*} else {
        return new QLabel(obj.toString());
    }

    if (key == "name") {
        return new QLineEdit(obj.toString());
    } else if (key == "id") {
        return new QLabel(obj.toString());
    } else if (key == "city") {
        return new QLineEdit(obj.toString());
    } else if (key == "line") {

    } else if (key == "arp") {

    }
    else {
        return new QLabel(obj.toString());
    }*/

    return nullptr;
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

        for (int j = 0; j < values.size(); ++j)
            m_enums.insertMulti(name, values[j].toString());
    }

    m_model_classes = m_model_map["classes"].toList();

    // Parse types.
    QList<QVariant>::const_iterator iter = m_model_map["classes"].toList().constBegin();
    while (iter != m_model_map["classes"].toList().constEnd()) {
        QVariantList name_type_list = iter->toMap()["properties"].toList();

        for (int i = 0; i < name_type_list.size(); ++i) {
            QString name = name_type_list[i].toMap()["name"].toString();
            QString type = name_type_list[i].toMap()["type"].toString();
            if (! m_map_name_type.contains(name)) {
                m_map_name_type.insert(name, type);
            }
        }

        /*QList<QVariant>::const_iterator iter2 = name_type_list.constBegin();
        while (iter2 != name_type_list.constEnd()) {
            if (! m_map_name_type.contains(iter2->)) {

            }

            ++iter2;
        }*/

        ++iter;
    }

    // Show Classes in the Tree.
    QStandardItem *item = standardModel->invisibleRootItem();
    QList<QVariant> rootData;
    rootData << "Features";
    item->setData(rootData);

    for (int i = 0; i < m_model_classes.size(); ++i) {
        QString class_name = m_model_classes[i].toMap()["name"].toString();
        item->appendRow(new QStandardItem(class_name));
        m_classname_row_map[class_name] = uint32_t(i);
    }

    ui->treeView->setModel(standardModel);
    ui->treeView->expandAll();

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
        QString classname = obj.keys().first();
        uint32_t row =  m_classname_row_map[classname];

        QStandardItem *item_child = item->child(row);

        QString name;

        if (obj.values().first().toMap().contains("name")) {
            name = obj.values().first().toMap()["name"].toString();
        } else if (obj.values().first().toMap().contains("annotation")) {
            name = obj.values().first().toMap()["annotation"].toString();
        } else {
            name = "#" + obj.values().first().toMap()["id"].toString();
        }

        item_child->appendRow(new QStandardItem(name));
        m_all_objects.insertMulti(name, obj);
    }

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
