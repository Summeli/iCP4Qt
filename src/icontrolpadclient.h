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

#ifndef ICONTROLPADCLIENT_H
#define ICONTROLPADCLIENT_H

#include <QObject>
#include <Qthread>


#include <qbluetoothuuid.h>
#include <qbluetoothserviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <qbluetoothsocket.h>

QTM_USE_NAMESPACE

class iControlPadClient : public QThread
{
    Q_OBJECT

public:
    iControlPadClient();
    ~iControlPadClient();
    virtual void run();

public:
    void discoverAndConnect();
    void subscribeKeyEvent(QObject* aObject );

signals:
    void connectedToiControlPad();

private slots:
     void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
     void discoveryFinished();
     void connected();
     void disconnected();

public slots:

private:
     void connectToService(const QBluetoothServiceInfo &remoteService);
     void parseDigitalKeyData();
protected:
    bool m_connected;
    QObject* m_receiver;

    QBluetoothServiceDiscoveryAgent *m_discoveryAgent;
    QBluetoothServiceInfo m_service;
    QBluetoothSocket* m_socket;

    quint16 m_DigitalButtons;
};

#endif // ICONTROLPADCLIENT_H
