// Autor: Félix Hernández

#include "../headers/XMLsEditorInteractiveNovels.hpp"

XMLsEditorInteractiveNovels::XMLsEditorInteractiveNovels() : QMainWindow(nullptr)
{
    ui.setupUi(this);

    model = new QStandardItemModel(0, 3, this);
    model->setHorizontalHeaderLabels(QStringList() << "Scene ID" << "Title" << "Content");

    // Botones del menu
    connect(ui.NewFileMenu, &QAction::triggered, this, &XMLsEditorInteractiveNovels::New);
    connect(ui.LoadFileMenu, &QAction::triggered, this, &XMLsEditorInteractiveNovels::Load);
    connect(ui.SaveFileMenu, &QAction::triggered, this, &XMLsEditorInteractiveNovels::Save);
    // Botones laterales
    connect(ui.AddNodeButton, &QPushButton::clicked, this, &XMLsEditorInteractiveNovels::AddNode);
    connect(ui.RemoveNodeButton, &QPushButton::clicked, this, &XMLsEditorInteractiveNovels::QuitNode);

    ui.treeView->setModel(model);
    ui.treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.treeView->model(), &QStandardItemModel::itemChanged, this, &XMLsEditorInteractiveNovels::UpdateXmlNode;

}

void XMLsEditorInteractiveNovels::New()
{
    // Cargamos la plantilla base para un nuevo archivo XML.
    std::string filePath = "../binaries/Base.xml";
    try {
        xmlEditorInstance.OpenFile(filePath);
        QMessageBox::information(this, "New File", "File template loaded. Please use the Save option to save the file once completed.");

        // Se carga en el árbol
        model->clear(); // limpia el model antes de llenarlo
        tinyxml2::XMLElement* root = xmlEditorInstance.GetRootNode();
        QStandardItem* rootItem = new QStandardItem(QString::fromStdString(root->Name()));
        model->appendRow(rootItem);
        buildTree(root, rootItem);
    }
    catch (std::runtime_error& e) {
        // Mostrar mensaje de error si no se puede abrir el archivo
        QMessageBox::critical(this, "Error", "Failed to load new file template. Check the file path and try again.");
    }
}

void XMLsEditorInteractiveNovels::Load()
{
    QString qFilePath = QFileDialog::getOpenFileName(this, tr("Open XML File"), "", tr("XML Files (*.xml)"));
    if (qFilePath.isEmpty()) {
        return;
    }

    std::string filePath = qFilePath.toStdString();
    try {
        xmlEditorInstance.OpenFile(filePath);

        // Se carga en el árbol
        model->clear(); // limpia el model antes de llenarlo
        tinyxml2::XMLElement* root = xmlEditorInstance.GetRootNode();
        QStandardItem* rootItem = new QStandardItem(QString::fromStdString(root->Name()));
        model->appendRow(rootItem);
        buildTree(root, rootItem);
    }
    catch (std::runtime_error& e) {
        // Mostrar mensaje de error si no se puede cargar el archivo
        QMessageBox::critical(this, "Error", "Failed to load XML file. Please check the file and try again.");
    }
}

void XMLsEditorInteractiveNovels::Save()
{
    // Solicitar al usuario la ruta del nuevo archivo
    QString qFilePath = QFileDialog::getSaveFileName(this, tr("Save XML File As"), "", tr("XML Files (*.xml)"));
    if (qFilePath.isEmpty()) {
        return;
    }

    std::string filePath = qFilePath.toStdString();

    // Actualizar la estructura XML en memoria según los cambios en la vista de árbol
    QModelIndex rootIndex = model->index(0, 0);
    UpdateXmlNode(xmlEditorInstance.GetRootNode(), model->itemFromIndex(rootIndex));

    // Guardar el archivo XML actualizado
    xmlEditorInstance.SaveFile(filePath);

    QMessageBox::information(this, "Save File", "File saved successfully.");
}

void XMLsEditorInteractiveNovels::AddNode()
{
    // Primero, obten el elemento seleccionado en el árbol
    QModelIndex currentIndex = ui.treeView->currentIndex();
    QStandardItem* currentItem = model->itemFromIndex(currentIndex);

    if (currentItem == nullptr)
    {
        // No se seleccionó ningún elemento
        QMessageBox::information(this, tr("Error"), tr("No node selected."));
        return;
    }

    // Aquí se usa findNode para encontrar el nodo XML correspondiente
    tinyxml2::XMLElement* currentXMLNode = findNode(currentItem->text().toStdString(), xmlEditorInstance.GetRootNode());

    if (currentXMLNode == nullptr)
    {
        // No se encontró el nodo correspondiente en el XML
        QMessageBox::information(this, tr("Error"), tr("No corresponding XML node found."));
        return;
    }

    bool ok;
    QString nodeName = QInputDialog::getText(this, tr("Add Node"), tr("Node name:"), QLineEdit::Normal, "nodeName", &ok);
    if (ok && !nodeName.isEmpty())
    {
        QString nodeAttribute = QInputDialog::getText(this, tr("Add Node Attribute"), tr("Node attribute:"), QLineEdit::Normal, "attribute=value", &ok);
        if (ok)
        {
            tinyxml2::XMLElement* newNode = xmlEditorInstance.AddChildNode(currentXMLNode, nodeName.toStdString());
            QStringList attrList = nodeAttribute.split('=');
            if (attrList.size() == 2)
            {
                xmlEditorInstance.ModifyNodeAttribute(newNode, attrList[0].toStdString(), attrList[1].toStdString());
            }

            // Actualiza la vista
            model->clear();
            tinyxml2::XMLElement* root = xmlEditorInstance.GetRootNode();
            QStandardItem* rootItem = new QStandardItem(QString::fromStdString(root->Name()));
            model->appendRow(rootItem);
            buildTree(root, rootItem);

            // Encontramos el nuevo elemento en el árbol y lo seleccionamos
            QStandardItem* newItem = findItem(newNode, rootItem);
            if (newItem)
            {
                ui.treeView->setCurrentIndex(newItem->index());
            }
        }
    }
}

void XMLsEditorInteractiveNovels::QuitNode()
{
    // Primero, obten el elemento seleccionado en el árbol
    QModelIndex currentIndex = ui.treeView->currentIndex();
    QStandardItem* currentItem = model->itemFromIndex(currentIndex);

    if (currentItem == nullptr)
    {
        // No se seleccionó ningún elemento
        QMessageBox::information(this, tr("Error"), tr("No node selected."));
        return;
    }

    // Aquí se usa findNode para encontrar el nodo XML correspondiente
    tinyxml2::XMLElement* currentXMLNode = findNode(currentItem->text().toStdString(), xmlEditorInstance.GetRootNode());

    if (currentXMLNode == nullptr)
    {
        // No se encontró el nodo correspondiente en el XML
        QMessageBox::information(this, tr("Error"), tr("No corresponding XML node found."));
        return;
    }

    // Comprobando si currentXMLNode y su padre no son nullptr antes de proceder
    if (currentXMLNode->Parent())
    {
        tinyxml2::XMLElement* parentNode = currentXMLNode->Parent()->ToElement();
        xmlEditorInstance.RemoveChildNode(parentNode, currentXMLNode);

        // Actualizar el modelo de vista
        model->clear();
        tinyxml2::XMLElement* root = xmlEditorInstance.GetRootNode();
        QStandardItem* rootItem = new QStandardItem(QString::fromStdString(root->Name()));
        model->appendRow(rootItem);
        buildTree(root, rootItem);

        // Seleccionamos el elemento correspondiente al padre en el árbol
        QStandardItem* newCurrentItem = findItem(parentNode, rootItem);
        if (newCurrentItem)
        {
            ui.treeView->setCurrentIndex(newCurrentItem->index());
        }
    }
    else
    {
        QMessageBox::information(this, tr("Error"), tr("Cannot remove root node."));
    }
}

void XMLsEditorInteractiveNovels::buildTree(tinyxml2::XMLElement* rootNode, QStandardItem* parentItem)
{
    if (!rootNode || !parentItem) return;

    for (tinyxml2::XMLElement* element = rootNode->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        QString elementName = QString::fromStdString(element->Name());
        QString elementText = QString::fromStdString(element->GetText() ? element->GetText() : "");

        QStandardItem* item = new QStandardItem(elementName);
        parentItem->appendRow(item);

        // Si hay texto dentro del nodo, lo agregamos como un hijo.
        if (!elementText.isEmpty()) {
            QStandardItem* textItem = new QStandardItem(elementText);
            item->appendRow(textItem);
        }

        // Manejo de los atributos del nodo
        for (const tinyxml2::XMLAttribute* attr = element->FirstAttribute(); attr; attr = attr->Next())
        {
            QString attrName = QString::fromStdString(attr->Name());
            QString attrValue = QString::fromStdString(attr->Value());
            QStandardItem* attrItem = new QStandardItem(attrName + " : " + attrValue);
            item->appendRow(attrItem);
        }

        // Agrega elementos hijos
        buildTree(element, item);
    }
}

void XMLsEditorInteractiveNovels::UpdateXmlNode(tinyxml2::XMLElement* xmlElement, QStandardItem* item)
{
    // Actualizar el contenido del nodo XML según el elemento de la vista de árbol
    QString itemName = item->text();

    // Actualizar los atributos del nodo y buscar posibles subnodos para actualizar
    QStandardItem* textItem = nullptr;
    for (int i = 0; i < item->rowCount(); i++) {
        QStandardItem* childItem = item->child(i);
        QString childText = childItem->text();
        QStringList attrList = childText.split(':');
        if (attrList.size() == 2) {
            // Este es un atributo
            QString attrName = attrList[0].trimmed();
            QString attrValue = attrList[1].trimmed();
            xmlEditorInstance.ModifyNodeAttribute(xmlElement, attrName.toStdString(), attrValue.toStdString());
        }
        else if (childItem->rowCount() > 0) {
            // Este es un subnodo
            tinyxml2::XMLElement* childElement = xmlElement->FirstChildElement(childItem->text().toStdString().c_str());
            if (childElement != nullptr) {
                UpdateXmlNode(childElement, childItem);
            }
        }
        else {
            // Este es un texto de nodo
            textItem = childItem;
        }
    }

    if (textItem != nullptr) {
        // Actualizar el texto del nodo
        xmlElement->SetText(textItem->text().toStdString().c_str());
    }
}

QStandardItem* XMLsEditorInteractiveNovels::findItem(tinyxml2::XMLElement* xmlElement, QStandardItem* parent)
{
    for (int i = 0; i < parent->rowCount(); i++)
    {
        QStandardItem* childItem = parent->child(i);
        // Si el elemento tiene hijos, es probable que sea un nombre de nodo.
        if (childItem->hasChildren() && childItem->text() == QString::fromStdString(xmlElement->Name()))
        {
            return childItem;
        }

        if (childItem->hasChildren())
        {
            QStandardItem* result = findItem(xmlElement, childItem);
            if (result) return result;
        }
    }

    return nullptr;
}

tinyxml2::XMLElement* XMLsEditorInteractiveNovels::findNode(const std::string& name, tinyxml2::XMLElement* parent)
{
    for (tinyxml2::XMLElement* child = parent->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (child->Name() == name)
        {
            return child;
        }

        if (child->FirstChildElement())
        {
            tinyxml2::XMLElement* result = findNode(name, child);
            if (result) return result;
        }
    }

    return nullptr;
}

