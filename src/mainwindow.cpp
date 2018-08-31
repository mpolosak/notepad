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
#include "mainwindow.hpp"
#include"notepadapp.hpp"
#include <QToolBar>

MainWindow::MainWindow(bool useMenu, bool toolBarEnabled, QFont *font, QStringList *files)
{
    mainToolBar = addToolBar(tr("Toolbar"));
    
	tabwidget = new TabWidget(font);

    connect(tabwidget,  &TabWidget::currentTextChanged, this,  &QMainWindow::setWindowTitle);
    
    fileMenu = new FileMenu();
    editMenu = new EditMenu();
    setsMenu = new SetsMenu(useMenu, mainToolBar->toggleViewAction());
    helpMenu = new HelpMenu();

    connect(fileMenu, &FileMenu::newFileclick, tabwidget, &TabWidget::newFileCreate);
    connect(fileMenu, &FileMenu::newWindowClick, (NotepadApp*) qApp, &NotepadApp::newWindowInstance);
    connect(fileMenu, &FileMenu::openclick, tabwidget, &TabWidget::openFilesClicked);
    connect(fileMenu, &FileMenu::saveclick, tabwidget, &TabWidget::saveclick);
    connect(fileMenu, &FileMenu::saveasclick, tabwidget, &TabWidget::saveas);
    connect(fileMenu, &FileMenu::saveAll, tabwidget, &TabWidget::saveAll);
    connect(fileMenu, &FileMenu::saveSession, tabwidget, &TabWidget::saveSession);
    connect(fileMenu, &FileMenu::closeclick, this, &MainWindow::close);
    connect(fileMenu, &FileMenu::quitclick, qApp, &QApplication::closeAllWindows);
    
    connect(editMenu, &EditMenu::undo, tabwidget, &TabWidget::undo);
    connect(editMenu, &EditMenu::redo, tabwidget, &TabWidget::redo);
    connect(editMenu, &EditMenu::cut, tabwidget, &TabWidget::cut);
    connect(editMenu, &EditMenu::copy, tabwidget, &TabWidget::copy);
    connect(editMenu, &EditMenu::paste, tabwidget, &TabWidget::paste);
    connect(editMenu, &EditMenu::find, tabwidget, &TabWidget::find);

    connect(setsMenu, &SetsMenu::font, (NotepadApp*) qApp, &NotepadApp::changeFont); 
    connect(setsMenu, &SetsMenu::menuChange, (NotepadApp*)qApp, &NotepadApp::changeMenu);
    
    connect(helpMenu, &HelpMenu::about, (NotepadApp*) qApp, &NotepadApp::about);
    connect(helpMenu, &HelpMenu::aboutQt, qApp, &QApplication::aboutQt);

    connect(tabwidget,  &TabWidget::undoAvailable, editMenu,  &EditMenu::undoAvailable);
    connect(tabwidget,  &TabWidget::redoAvailable, editMenu,  &EditMenu::redoAvailable);
    connect(tabwidget,  &TabWidget::copyAvailable, editMenu,  &EditMenu::copyAvailable);
    
    connect(mainToolBar->toggleViewAction(), &QAction::toggled, (NotepadApp*)qApp, &NotepadApp::changeToolBar);
    
    setToolBar(toolBarEnabled);
    
    setCentralWidget(tabwidget);
    if (files != nullptr) 
    {
        tabwidget->openFiles(*files);
        delete files;
    }
    else tabwidget->newFileCreate();
    menu(useMenu);
    connect((NotepadApp*)qApp, &NotepadApp::menuChanged, this, &MainWindow::menu);
    connect((NotepadApp*)qApp, &NotepadApp::fontChanged, tabwidget, &TabWidget::setFont);
    connect((NotepadApp*)qApp, &NotepadApp::toolBarChanged, mainToolBar->toggleViewAction(), &QAction::setChecked);
}
void MainWindow::menu(bool useMenuBar)
{
	if(useMenuBar)
	{
        menubar = new QMenuBar();
        menubar -> addMenu(fileMenu);
        menubar -> addMenu(editMenu);
        menubar -> addMenu(setsMenu);
        menubar -> addMenu(helpMenu);
        setMenuBar(menubar);
		tabwidget->setMenu();
    }
    else{
        if(menubar!=nullptr)
		{
            delete menubar;
            menubar = nullptr;
		}
        QMenu *menu = new QMenu();
        menu -> addMenu(fileMenu);
        menu -> addMenu(editMenu);
        menu -> addMenu(setsMenu);
        menu -> addMenu(helpMenu);
		tabwidget->setMenu(menu);
    }
}
void MainWindow::setToolBar(bool useToolBar)
{
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        auto newAct = mainToolBar->addAction(QIcon::fromTheme("document-new"), tr("New"));
        auto openAct = mainToolBar->addAction(QIcon::fromTheme("document-open"), tr("Open"));
        auto saveAct = mainToolBar->addAction(QIcon::fromTheme("document-save"), tr("Save"));
        auto saveAsAct = mainToolBar->addAction(QIcon::fromTheme("document-save-as"), tr("Save as..."));
        auto saveAllAct = mainToolBar->addAction(QIcon::fromTheme("document-save-all"), tr("Save all"));
        auto saveSessionAct = mainToolBar->addAction(QIcon::fromTheme("document-save"), tr("Save session"));
        auto undoAct = mainToolBar->addAction(QIcon::fromTheme("edit-undo"), tr("Undo"));
        auto redoAct = mainToolBar->addAction(QIcon::fromTheme("edit-redo"), tr("Redo"));
        connect(newAct, &QAction::triggered, tabwidget, &TabWidget::newFileCreate);
        connect(openAct, &QAction::triggered, tabwidget, &TabWidget::openFilesClicked);
        connect(saveAct, &QAction::triggered, tabwidget, &TabWidget::saveclick);
        connect(saveAsAct, &QAction::triggered, tabwidget, &TabWidget::saveas);
        connect(saveAllAct, &QAction::triggered, tabwidget, &TabWidget::saveAll);
        connect(saveSessionAct, &QAction::triggered, tabwidget, &TabWidget::saveSession);
        connect(undoAct, &QAction::triggered, tabwidget, &TabWidget::undo);
        connect(redoAct, &QAction::triggered, tabwidget, &TabWidget::redo);
        mainToolBar->toggleViewAction()->setChecked(useToolBar);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    tabwidget->closeEvent(event);
}
MainWindow::~MainWindow()
{
}
