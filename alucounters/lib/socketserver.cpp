#include "socketserver.h"
#include <QDebug>
#include "lib/dao.h"
SocketServer::SocketServer(QObject *parent) :
    QObject(parent)
{

    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(ReadData()));

    if(!server->listen(QHostAddress::Any, 4096))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started!";

    }


}

void SocketServer::ReadData(){

    QTcpSocket *socket = server->nextPendingConnection();
    QByteArray buffer;
    quint16 packet_size;

    while(1){

        socket->waitForReadyRead(250);

        packet_size=socket->bytesAvailable();//Read Count Bytes waiting on line
        //qDebug()<<"Packet Recived len: "<<QString::number(packet_size)<<"\n";
        //rcv += packet_size;


        if (packet_size>0)
        {

            buffer.append(socket->read(packet_size));//Read Packet
            //buffer.append(socket->readAll());
            //...
         }else{
            break;
        }
    }
    socket->close();
    qDebug() << "leggo" << buffer.trimmed();
    Dao d;
    QStringList values = QString(buffer.trimmed()).split(":");
    QHash<QString, QString> stat = d.singleRow("stations","name='"+values.at(0)+"'");
    QStringList val;
    val << "counters="+values.at(1);
    d.updateRow("days",val,"id_station="+stat.value("id")+" AND completed=0");




}
