/*
notepad - Simple text editor with tabs
Copyright (C) 2018  256Michael

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include"notepadapp.hpp"

NotepadApp::NotepadApp(int& argc, char**& argv)
  : QApplication::QApplication(argc, argv)
{
    QTranslator *qtTranslator = new QTranslator();
    qtTranslator->load("qt_" + QLocale::system().name(),
            QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    installTranslator(qtTranslator);

    QTranslator *notepadTranslator = new QTranslator();
    notepadTranslator->load(applicationDirPath()+"/translations/notepad_" + QLocale::system().name());
    installTranslator(notepadTranslator);
    setApplicationName(tr("Notepad"));
    setApplicationDisplayName(tr("Notepad"));
    setApplicationVersion("1.2");
    setWindowIcon(QIcon::fromTheme("accessories-text-editor"));
    QCommandLineParser parser;
    parser.setApplicationDescription(tr("Text editor with tabs"));
    parser.addHelpOption();
    parser.addVersionOption();
    //Add information to help page
    parser.addPositionalArgument(tr("files"), tr("Files to open."));
    parser.process(*this);
    const QStringList args = this->arguments();
    QStringList *files = new QStringList;
    for (int i = 1;i<args.size();i++)
    {   
        QFileInfo file(args.at(i));
        if (file.exists()) files->append(file.canonicalFilePath());
    }
    QFile file(applicationDirPath()+"/config");
    
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        QString a = stream.readLine();
        if (a != "" ) {
            font = new QFont();
            font->fromString(a);
        }
        a = stream.readLine();
        if (a=="true")
            menu = true;

        else menu = false;
        a = stream.readLine();
        if (a=="true")
            toolBarEnabled = true;
        else toolBarEnabled = false;
        file.close();
    } 
    MainWindow* newWindow = newWindowInstance();
    newWindow->openFiles(*files);
}

NotepadApp::~NotepadApp()
{
    QFile file(applicationDirPath()+"/config");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        if(font!=nullptr) stream << font->toString();
        stream << "\n";
        if(menu)  
        {
           stream<<"true";
        }
        else stream<<"false";
        stream << "\n";
        if(toolBarEnabled)  
        {
           stream<<"true";
        }
        else stream<<"false";
        file.close();
    }
}
void NotepadApp::about()
{
    QMessageBox::about(activeWindow(), tr("About notepad"),
            tr("Notepad-Text editor with tabs"));
}
MainWindow* NotepadApp::newWindowInstance()
{
    MainWindow *newWindow = new MainWindow(menu, toolBarEnabled, font);
    connect(newWindow, &MainWindow::newWindowRequest, this, &NotepadApp::newWindowInstance);
    connect(newWindow, &MainWindow::quitRequest, this, &QApplication::closeAllWindows);
    connect(newWindow, &MainWindow::font, this, &NotepadApp::changeFont);
    connect(newWindow, &MainWindow::menuChange, this, &NotepadApp::changeMenu);
    connect(newWindow, &MainWindow::about, this, &NotepadApp::about);
    connect(newWindow, &MainWindow::aboutQt, this, &NotepadApp::aboutQt);
    connect(newWindow, &MainWindow::toolBarChange, this, &NotepadApp::changeToolBar);
    connect(this, &NotepadApp::menuChanged, newWindow, &MainWindow::menu);
    connect(this, &NotepadApp::fontChanged, newWindow, &MainWindow::fontChanged);
    connect(this, &NotepadApp::toolBarChanged, newWindow, &MainWindow::changeToolBarVisibility);
    newWindow -> show();
    return newWindow;
}
void NotepadApp::changeMenu(bool newValue)
{
    menu = newValue;
    emit menuChanged(menu);
}
void NotepadApp::changeToolBar(bool newValue)
{
    toolBarEnabled = newValue;
    emit toolBarChanged(toolBarEnabled);
}
void NotepadApp::changeFont()
{
    bool ok;
    if (font ==  nullptr) font = new QFont();
    *font = QFontDialog::getFont(&ok, *font, activeWindow());
    if (ok) {
        emit fontChanged(font);
    }
}
