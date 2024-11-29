// Autor: felixhmy

#pragma once

#include <string>
#include "..\headers\tinyxml2.h"

namespace xmlEditor
{
    class XMLEditor {

    public:
        // Constructor
        XMLEditor();

        // Destructor
        ~XMLEditor();

        // Abre el archivo XML y carga su contenido
        void OpenFile(const std::string& filePath);

        // Crear un nuevo archivo XML con el nombre de nodo raíz proporcionado
        void CreateNew(const std::string& rootName);

        // Obtener el nodo raíz
        tinyxml2::XMLElement* GetRootNode();

        // Obtener un nodo hijo por su nombre
        tinyxml2::XMLElement* GetChildNode(tinyxml2::XMLElement* parentNode, const std::string& nodeName);

        // Añadir un nuevo nodo hijo
        tinyxml2::XMLElement* AddChildNode(tinyxml2::XMLElement* parentNode, const std::string& nodeName);

        // Eliminar un nodo hijo
        void RemoveChildNode(tinyxml2::XMLElement* parentNode, tinyxml2::XMLElement* childNode);

        // Modificar el valor de un nodo
        void ModifyNodeValue(tinyxml2::XMLElement* node, const std::string& newValue);

        // Añadir o modificar un atributo a un nodo
        void ModifyNodeAttribute(tinyxml2::XMLElement* node, const std::string& attributeName, const std::string& attributeValue);

        // Guardar el archivo XML con los cambios
        void SaveFile(const std::string& filePath);
        void SaveFileAs(const std::string& newFilePath);

        // Obtener un nodo por su nombre
        tinyxml2::XMLElement* GetNodeByName(const std::string& nodeName);
        tinyxml2::XMLElement* GetNodeByNameRecursive(tinyxml2::XMLElement* startNode, const std::string& nodeName);

    private:
        // El documento XML en memoria
        tinyxml2::XMLDocument xmlDoc;
    };
}
