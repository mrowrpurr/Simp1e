#pragma once

#include <string_format.h>

#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>
#include <string>
#include <unordered_map>
#include <vector>

#include "../IApp.h"
#include "DataRecordBrowser/DataRecordBrowserFilterProxyModel.h"

namespace Simp1e::Editor::Windows {

    class DataRecordBrowserWindow : public QWidget {
        Q_OBJECT

        IApp* _app;

#pragma region Widget Variables
        QVBoxLayout*                                  _layout_Window;
        QStandardItemModel                            _model_DataRecords;
        DataRecordBrowser::DataRecordFilterProxyModel _model_DataRecords_SortFilterProxy;
        QTreeView                                     _tree_DataRecords;
        QLineEdit                                     _txt_FilterText;
#pragma endregion
    public:
        DataRecordBrowserWindow(IApp* app) : _app(app), QWidget(nullptr) {
            IDs();
            Layout();
            Configure();
            Events();
            ReloadRecords();
        }

    private:
#pragma region Widget Setup
        void IDs() { setObjectName("DataRecordBrowser"); }

        void Layout() {
            _layout_Window = new QVBoxLayout();
            _layout_Window->addWidget(&_txt_FilterText);
            _layout_Window->addWidget(&_tree_DataRecords);
            setLayout(_layout_Window);
        }

        void Configure() {
            setWindowTitle("Data Record Browser");
            setMinimumSize(800, 400);
            _model_DataRecords_SortFilterProxy.setSourceModel(&_model_DataRecords);
            _model_DataRecords_SortFilterProxy.setFilterKeyColumn(0);
            _tree_DataRecords.setModel(&_model_DataRecords_SortFilterProxy);
            _tree_DataRecords.setSortingEnabled(true);
            _tree_DataRecords.sortByColumn(0, Qt::SortOrder::AscendingOrder);
            _tree_DataRecords.setAlternatingRowColors(true);
            _tree_DataRecords.header()->setSectionResizeMode(
                QHeaderView::ResizeMode::ResizeToContents
            );
            _txt_FilterText.setPlaceholderText("Filter");
        }

        void Events() {
            connect(
                &_txt_FilterText, &QLineEdit::textChanged, &_model_DataRecords_SortFilterProxy,
                &QSortFilterProxyModel::setFilterFixedString
            );
            connect(
                &_tree_DataRecords, &QTreeView::doubleClicked, this,
                &DataRecordBrowserWindow::on_tree_DataRecords_doubleClicked
            );
            connect(
                _tree_DataRecords.selectionModel(), &QItemSelectionModel::selectionChanged, this,
                &DataRecordBrowserWindow::on_tree_DataRecords_selectionChanged
            );
        }
#pragma endregion
#pragma region Event Handlers
        void on_tree_DataRecords_doubleClicked(const QModelIndex& index) {
            auto  item = _model_DataRecords_SortFilterProxy.mapToSource(index);
            auto* record =
                _app->GetDataStore().GetRecord(item.data().toString().toStdString().c_str());
            if (record == nullptr) return;
            _app->ShowRecordWindow(record);
        }
        void on_tree_DataRecords_selectionChanged(
            const QItemSelection& selected, const QItemSelection& deselected
        ) {
            auto  item = _model_DataRecords_SortFilterProxy.mapToSource(selected.indexes()[0]);
            auto* record =
                _app->GetDataStore().GetRecord(item.data().toString().toStdString().c_str());
            if (record == nullptr) return;
            _app->ShowRecordPreview(record);
        }
#pragma endregion
#pragma region Private Functions
        void ReloadRecords() {
            _model_DataRecords.clear();

            std::unordered_map<std::string, std::vector<Data::Record*>> recordsByType;
            for (auto& record : _app->GetDataStore().GetAllRecords())
                recordsByType[record->GetType()].push_back(record);

            for (auto& [type, records] : recordsByType) {
                auto* groupItem         = new QStandardItem(type.c_str());
                auto* descriptionColumn = new QStandardItem("");
                groupItem->setEditable(false);
                descriptionColumn->setEditable(false);
                _model_DataRecords.appendRow({groupItem, descriptionColumn});

                for (auto& record : records) {
                    auto  description = record->GetData()->GetString("description");
                    auto* childId     = new QStandardItem(record->GetFullIdentifier());
                    auto* childDescription =
                        new QStandardItem(description.has_value() ? (*description).c_str() : "");
                    childId->setEditable(false);
                    childDescription->setEditable(false);
                    groupItem->appendRow({childId, childDescription});
                }
            }

            _model_DataRecords.setHeaderData(0, Qt::Orientation::Horizontal, "ID");
            _model_DataRecords.setHeaderData(1, Qt::Orientation::Horizontal, "Description");
        }
#pragma endregion
    };
}
