/* iControlPadClient
 *
 * Copyright (c) 2011 Antti Pohjola, http://www.summeli.fi
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "icontrolpadclient.h"

#include <QThread>

#include <qbluetoothdeviceinfo.h>
#include <qbluetoothaddress.h>

#define KiControlPadPollInterval 25
//#define KiCPServiceName "iCP-SPP"
const QString KiCPServiceName("iCP-SPP");
#define CMD_GET_DIGITALS 0xA5
#define CMD_GET_ANALOGS 0x87

const quint32 getDigitalKeys = CMD_GET_DIGITALS;
iControlPadClient::iControlPadClient() :
    m_connected(false),
    m_discoveryAgent(new QBluetoothServiceDiscoveryAgent() ),
    m_socket( NULL )
{
    connect(m_discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
                 this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));

    connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(discoveryFinished()));

}

iControlPadClient::~iControlPadClient()
{
}


void iControlPadClient::run()
{
    char* buttonData = (char*) new quint32[2];
    while (m_connected) {
        //send get request to the iCP
        m_socket->write( (char*)&getDigitalKeys );
        qint64 bytesAvailable = m_socket->bytesAvailable ();
        if( bytesAvailable ){
            m_socket->read(buttonData, 2);
            qDebug() << "Button Data is " << buttonData;
        }
        QThread::msleep(KiControlPadPollInterval);
    }
}

void iControlPadClient::discoverAndConnect()
{
    if (m_discoveryAgent->isActive())
            m_discoveryAgent->stop();

     m_discoveryAgent->start();
}


void iControlPadClient::serviceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{
    qDebug() << "Discovered service on"
             << serviceInfo.device().name() << serviceInfo.device().address().toString();
    qDebug() << "\tService name:" << serviceInfo.serviceName();
    qDebug() << "\tDescription:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceDescription).toString();
    qDebug() << "\tProvider:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceProvider).toString();
    qDebug() << "\tL2CAP protocol service multiplexer:"
             << serviceInfo.protocolServiceMultiplexer();
    qDebug() << "\tRFCOMM server channel:" << serviceInfo.serverChannel();



    if(  QString::compare ( serviceInfo.serviceName(), KiCPServiceName, Qt::CaseInsensitive ) == 0 )
        connectToService( serviceInfo );
}

void iControlPadClient::connectToService(const QBluetoothServiceInfo &remoteService)
{
    if (m_socket)
        return;

    // Connect to service
    m_socket = new QBluetoothSocket(QBluetoothSocket::RfcommSocket);
    qDebug() << "Create socket";
    m_socket->connectToService(remoteService);
    qDebug() << "Connecte Service done";
    connect(m_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
}


void iControlPadClient::discoveryFinished()
{
}

void iControlPadClient::connected()
{
    m_connected = true;
    qDebug() << "Connected to the iControlPad, starting the polling loop";
    start(QThread::NormalPriority);
}

void iControlPadClient::disconnected()
{
    qDebug() << "disconnected, ending the polling loop";
    m_connected = false;
}

