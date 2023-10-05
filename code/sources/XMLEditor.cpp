// Autor: Félix Hernández Muñoz-Yusta

#include <stdexcept>
#include <fstream>

#include "../headers/XMLEditor.hpp"

namespace xmlEditor
{
    XMLEditor::XMLEditor() { }

    XMLEditor::~XMLEditor() { }

    void XMLEditor::OpenFile(const std::string& filePath)
    {
        tinyxml2::XMLError eResult = xmlDoc.LoadFile(filePath.c_str());
        if (eResult != tinyxml2::XML_SUCCESS)
        {
            // Lanza un aviso en caso de error al abrir el archivo
            throw std::runtime_error("Failed to load file");
        }
    }

    tinyxml2::XMLElement* XMLEditor::GetRootNode()
    {
        return xmlDoc.RootElement();
    }

    tinyxml2::XMLElement* XMLEditor::GetChildNode(tinyxml2::XMLElement* parentNode, const std::string& nodeName)
    {
        if (parentNode == nullptr)
        {
            // Lanza un aviso en caso de que de error
            throw std::invalid_argument("Parent node is null");
        }
        return parentNode->FirstChildElement(nodeName.c_str());
    }

    tinyxml2::XMLElement* XMLEditor::AddChildNode(tinyxml2::XMLElement* parentNode, const std::string& nodeName)
    {
        if (parentNode == nullptr)
        {
            // Lanza un aviso en caso de que de error
            throw std::invalid_argument("Parent node is null");
        }
        tinyxml2::XMLElement* newChild = xmlDoc.NewElement(nodeName.c_str());
        parentNode->InsertEndChild(newChild);
        return newChild;
    }

    void XMLEditor::RemoveChildNode(tinyxml2::XMLElement* parentNode, tinyxml2::XMLElement* childNode)
    {
        if (parentNode == nullptr || childNode == nullptr)
        {
            // Lanza un aviso en caso de que de error
            throw std::invalid_argument("Parent node or child node is null");
        }
        parentNode->DeleteChild(childNode);
    }

    void XMLEditor::ModifyNodeValue(tinyxml2::XMLElement* node, const std::string& newValue)
    {
        if (node) // verifica que el nodo exista
        {
            node->SetText(newValue.c_str());
        }
    }

    void XMLEditor::ModifyNodeAttribute(tinyxml2::XMLElement* node, const std::string& attributeName, const std::string& attributeValue)
    {
        if (node) // verifica que el nodo exista
        {
            node->SetAttribute(attributeName.c_str(), attributeValue.c_str());
        }
    }

    void XMLEditor::SaveFile(const std::string& filePath)
    {
        tinyxml2::XMLError eResult = xmlDoc.SaveFile(filePath.c_str());
        if (eResult != tinyxml2::XML_SUCCESS)
        {
            // Lanza un aviso en caso de error al guardar el archivo
            throw std::runtime_error("Failed to save file");
        }
    }
    void XMLEditor::SaveFileAs(const std::string& newFilePath)
    {
        tinyxml2::XMLError eResult = xmlDoc.SaveFile(newFilePath.c_str());
        if (eResult != tinyxml2::XML_SUCCESS)
        {
            // Lanza un aviso en caso de error al guardar el archivo
            throw std::runtime_error("Failed to save file");
        }
    }


    void XMLEditor::CreateNew(const std::string& rootName)
    {
        // Limpiar el documento actual
        xmlDoc.Clear();

        // Crear la declaración XML
        tinyxml2::XMLDeclaration* decl = xmlDoc.NewDeclaration();
        xmlDoc.InsertFirstChild(decl);

        // Crear el nodo raíz
        tinyxml2::XMLElement* root = xmlDoc.NewElement(rootName.c_str());
        xmlDoc.InsertEndChild(root);
    }

    tinyxml2::XMLElement* XMLEditor::GetNodeByName(const std::string& nodeName)
    {
        return GetNodeByNameRecursive(xmlDoc.RootElement(), nodeName);
    }

    tinyxml2::XMLElement* XMLEditor::GetNodeByNameRecursive(tinyxml2::XMLElement* startNode, const std::string& nodeName)
    {
        // Primero, comprueba si el nodo actual tiene el nombre correcto
        if (startNode->Name() == nodeName)
        {
            return startNode;
        }

        // A continuación, busca en todos los nodos hijos del nodo actual
        for (tinyxml2::XMLElement* childNode = startNode->FirstChildElement(); childNode != nullptr; childNode = childNode->NextSiblingElement())
        {
            tinyxml2::XMLElement* result = GetNodeByNameRecursive(childNode, nodeName);
            if (result != nullptr)
            {
                return result;
            }
        }

        // Si no se encontró el nodo, devuelve nullptr
        return nullptr;
    }

}