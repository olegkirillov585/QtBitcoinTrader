//  This file is part of Qt Bitcoin Trader
//      https://github.com/JulyIGHOR/QtBitcoinTrader
//  Copyright (C) 2013-2016 July IGHOR <julyighor@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  In addition, as a special exception, the copyright holders give
//  permission to link the code of portions of this program with the
//  OpenSSL library under certain conditions as described in each
//  individual source file, and distribute linked combinations including
//  the two.
//
//  You must obey the GNU General Public License in all respects for all
//  of the code used other than OpenSSL. If you modify file(s) with this
//  exception, you may extend this exception to your version of the
//  file(s), but you are not obligated to do so. If you do not wish to do
//  so, delete this exception statement from your version. If you delete
//  this exception statement from all source files in the program, then
//  also delete it here.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "news/newsmodel.h"

NewsModel::NewsModel() : QObject()
{
    runningJulyHttp=false;

    downloadThread=new QThread;
    connect(downloadThread,SIGNAL(started()),this,SLOT(runThread()));
    moveToThread(downloadThread);
    downloadThread->start();
}

NewsModel::~NewsModel()
{
    julyHttp->destroyClass=true;
    julyHttp->deleteLater();

    /*for(int i=0;runningJulyHttp&&i<2000;i++)
    {
        QThread::msleep(2);
    }

    downloadThread->exit(0);*/
    downloadThread->deleteLater();
}

void NewsModel::destroyedJulyHttp()
{
    runningJulyHttp=false;
}

void NewsModel::runThread()
{
    julyHttp=new JulyHttp("centrabit.com","",this);
    connect(julyHttp,SIGNAL(dataReceived(QByteArray,int)),this,SLOT(dataReceived(QByteArray,int)));
    connect(julyHttp,SIGNAL(destroyed()),this,SLOT(destroyedJulyHttp()));
    runningJulyHttp=true;
}

void NewsModel::loadData()
{
    julyHttp->sendData(100, "GET /TraderNews/");
}

void NewsModel::dataReceived(QByteArray data,int reqType)
{
    if(reqType==100){
        emit setHtmlData(data);
    }
}
