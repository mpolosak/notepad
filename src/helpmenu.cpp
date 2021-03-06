/*
notepad - Simple text editor with tabs
Copyright (C) 2018-2019  256Michael

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
#include "helpmenu.hpp"

HelpMenu::HelpMenu()
:aboutAct(QIcon::fromTheme("accessories-text-editor"), tr("&About Notepad")),
aboutQtAct(QIcon::fromTheme("accessories-text-editor"), tr("About &Qt"))
{
    setTitle(tr("&Help"));
    
	addAction(&aboutAct);
	
	addAction(&aboutQtAct);
	
    connect(&aboutAct, &QAction::triggered, this, &HelpMenu::about);
    connect(&aboutQtAct, &QAction::triggered, this, &HelpMenu::aboutQt);
}
HelpMenu::~HelpMenu()
{
}
