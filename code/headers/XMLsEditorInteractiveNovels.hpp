#pragma once

#include <QtWidgets/QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include "ui_XMLsEditorInteractiveNovels.h"
#include "XMLEditor.hpp"
#include <map>

class XMLsEditorInteractiveNovels : public QMainWindow
{
    Q_OBJECT

public:
    //Constructor
    XMLsEditorInteractiveNovels();

private slots:
    //Funciones para los botones
    void New();
    void Load();
    void Save();

    void AddNode();
    void QuitNode();

private:
    //Funciones que manejan los cambios en el xml
    void buildTree(tinyxml2::XMLElement* rootNode, QStandardItem* parentItem);
    void UpdateXmlNode(tinyxml2::XMLElement* xmlElement, QStandardItem* item);

    //Declaraciones
    QStandardItem* findItem(tinyxml2::XMLElement* xmlElement, QStandardItem* parent);
    tinyxml2::XMLElement* findNode(const std::string& name, tinyxml2::XMLElement* parent);
    Ui::XMLsEditorInteractiveNovelsClass ui;
    QStandardItemModel* model;
    xmlEditor::XMLEditor xmlEditorInstance;
};
