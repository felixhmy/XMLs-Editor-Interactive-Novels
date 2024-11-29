// Autor: Félix Hernández

// Editor de XMLs de novelas interactivas
#include "../headers/XMLsEditorInteractiveNovels.hpp"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    XMLsEditorInteractiveNovels window;

    window.show();

    return application.exec();
}
