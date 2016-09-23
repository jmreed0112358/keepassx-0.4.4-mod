/***************************************************************************
 *   Copyright (C) 2007 by Tarek Saidi                                     *
 *   tarek.saidi@arcor.de                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *

 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "ExpireAllEntriesDlg.h"

ExpireAllEntriesDialog::ExpireAllEntriesDialog(QWidget* parent):QDialog(parent){
	setupUi(this);
}

void ExpireAllEntriesDialog::expireEntries(IDatabase* database, QList<IEntryHandle*>& Entries) {
	int ret = 0;
	QMessageBox msgBox;
	msgBox.setInformativeText("This function will expire all entries!  Do you want this?");
	msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::No);
	ret = msgBox.exec();

	switch (ret) {
		case QMessageBox::Yes:
			this->executeExpireEntries(database, Entries);
			break;
		case QMessageBox::No:
			break;
		default:
			// Shouldn't get here...
			break;
	}
}

void ExpireAllEntriesDialog::executeExpireEntries(IDatabase* database, QList<IEntryHandle*>& Entries) {
	bool modified = false;

	for(int i=0;i<Entries.size();i++){
		if (Entries[i]->expire() != Date_Never) {
			Entries[i]->setExpire(QDate::currentDate().addDays(-1));
			modified = true;
		}

		QTreeWidgetItem* item=new QTreeWidgetItem(treeWidget);
		item->setData(0,Qt::UserRole,i);
		item->setText(0,Entries[i]->group()->title());
		item->setText(1,Entries[i]->title());
		item->setText(2,Entries[i]->username());
		item->setText(3,Entries[i]->expire().dateToString(Qt::SystemLocaleDate));
		item->setIcon(0,database->icon(Entries[i]->group()->image()));
		item->setIcon(1,database->icon(Entries[i]->image()));
	}
}

void ExpireAllEntriesDialog::paintEvent(QPaintEvent* event){
	QDialog::paintEvent(event);
	QPainter painter(this);
	painter.setClipRegion(event->region());
	painter.drawPixmap(QPoint(0,0),BannerPixmap);
}

void ExpireAllEntriesDialog::resizeEvent(QResizeEvent* event){
	createBanner(&BannerPixmap,getPixmap("alarmclock"),tr("All entries expired"),width());
	QDialog::resizeEvent(event);
}

///TODO 0.2.3 locale aware string/date compare for correct sorting
