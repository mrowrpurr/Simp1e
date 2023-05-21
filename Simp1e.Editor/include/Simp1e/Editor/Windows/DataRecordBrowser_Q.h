#pragma once

#include <string_format.h>

#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
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
            Events();
            Configure();
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
            setMinimumSize(400, 400);
            _model_DataRecords_SortFilterProxy.setSourceModel(&_model_DataRecords);
            _model_DataRecords_SortFilterProxy.setFilterKeyColumn(0);
            // _model_DataRecords.setHeaderData(0, Qt::Orientation::Horizontal, "Data Records");
            _tree_DataRecords.setModel(&_model_DataRecords_SortFilterProxy);
            _tree_DataRecords.setSortingEnabled(true);
            _tree_DataRecords.sortByColumn(0, Qt::SortOrder::AscendingOrder);
            _tree_DataRecords.setAlternatingRowColors(true);
            _tree_DataRecords.header()->setStretchLastSection(true);
            _tree_DataRecords.header()->setSectionResizeMode(
                QHeaderView::ResizeMode::ResizeToContents
            );
            _tree_DataRecords.expandAll();
            _txt_FilterText.setPlaceholderText("Filter");
        }

        void Events() {
            connect(
                &_txt_FilterText, &QLineEdit::textChanged, &_model_DataRecords_SortFilterProxy,
                &QSortFilterProxyModel::setFilterFixedString
            );
        }
#pragma endregion

#pragma region Event Handlers
#pragma endregion

#pragma region Private Functions
        void ReloadRecords() {
            _model_DataRecords.clear();

            std::unordered_map<std::string, std::vector<Data::Record*>> recordsByType;
            for (auto& record : _app->GetDataStore().GetAllRecords())
                recordsByType[record->GetType()].push_back(record);

            for (auto& [type, records] : recordsByType) {
                auto* item = new QStandardItem(type.c_str());
                item->setEditable(false);
                _model_DataRecords.appendRow(item);

                for (auto& record : records) {
                    auto* child = new QStandardItem(record->GetFullIdentifier());
                    child->setEditable(false);
                    item->appendRow(child);
                }
            }

            _model_DataRecords.setHeaderData(0, Qt::Orientation::Horizontal, "Data Records");
        }
#pragma endregion
    };
}
