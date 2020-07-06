#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

#include "modelloader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QStandardItemModel;
class QStandardItem;
class QItemSelection;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_load();
    void on_save();

private slots:
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    int load_data_model(const QString & path);
    int load_data_file(const QString & path);
    void init_controls();
    QList<QStandardItem *> prepareRow(const QString &first, const QString &second, const QString &third) const;
    void populateLayout(const QVariantMap & obj);
    QWidget *widget_from_data(const QString &key, const QVariant &obj)const;

private:
    Ui::MainWindow * ui;
    QStandardItemModel * standardModel;
    const QString data_model_path = "model.json";
    const QString data_source_folder = "data";
    DataModel  * m_data_model;
    QVariantMap  m_model_map;
    QMultiMap<QString, QVariant>  m_all_objects;

    // Parsed data model for convenience of indexation.
    // Enum name, enum values.
    QMultiMap<QString, QString> m_enums;
    QStringList  m_types;
    QVariantList m_model_classes;

    // Class name, row in the Tree
    QMap<QString, uint32_t> m_classname_row_map;

    QMap<QString, QString> m_map_name_type;

    QList<QWidget *> widget_list;
};
#endif // MAINWINDOW_H
