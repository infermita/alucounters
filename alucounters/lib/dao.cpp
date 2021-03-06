#include "dao.h"
#include <QDir>
#include <iostream>
#include <typeinfo>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

Dao::Dao()
{
    QStringList drv = QSqlDatabase::drivers();

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("alucount");
    db.setUserName("root");
    if(QString(getenv("USER"))!="alberto"){
        db.setPassword("Medea123");
    }else{
        db.setPassword("am1c0m10");
    }
    if(db.open()){

    }else{
        qDebug() << "Error Open database";
    }


}
Dao::~Dao(){

    close();

}
QHash<QString, QString> Dao::singleRow(QString table,QString where)
{

    QHash<QString,QString> tmpHash;

    QSqlQuery query(db);

    QString sql = "SELECT * FROM "+table;

    if(where.length())
        sql += " WHERE "+where;

    //qDebug() << sql;
    query.exec(sql);
    QSqlRecord record = query.record();
    int fieldcount = record.count();


    for( int r=0; query.next(); r++ ){
        for (int i = 0; i < fieldcount; i++) {

            tmpHash.insert(record.fieldName(i),query.value(i).toString());

        }
    }


    //close();
    return tmpHash;

}
QList< QHash<QString, QString> > Dao::listRow(QString table,QString select,QString where){

    QList< QHash<QString, QString> > list;
    QSqlRecord record;
    int fieldcount;
    //QSqlRecord record = db.record(table);

    if(where.length())
        where = " WHERE "+where;

    QSqlQuery query(db);
    QString queryS = "SELECT "+select+" FROM "+table+" "+where+";";
    //qDebug() << "Query" << queryS;
    query.exec(queryS);

    record = query.record();
    fieldcount = record.count();

    while ( query.next() ) {


        QHash<QString,QString> tmpHash;

        for (int i = 0; i < fieldcount; i++) {

            tmpHash.insert(record.fieldName(i),query.value(i).toString());

        }

        list.append(tmpHash);


    }
    //close();
    return list;

}
bool Dao::updateRow(QString table,QStringList values,QString where){

    bool ret = false;
    QSqlQuery query(db);

    QString sql = "UPDATE "+table+" SET "+values.join(",");

    if(where!=NULL)
        sql += " WHERE "+where;

    if(query.exec(sql))
        ret = true;

    //close();

    return ret;

}
bool Dao::insertRow(QString table,QHash<QString, QString> fieldsVals){

    bool ret = false;
    int i;
    QSqlQuery query(db);
    QString val;

    QStringList fields,values;
    foreach (QString k, fieldsVals.keys()) {

        fields << k;
        val = fieldsVals.value(k);
        val.replace("'","''");
        values << val;

    }
    QString sql = "INSERT INTO "+table+" ("+fields.join(",")+") VALUES('"+values.join("','")+"')";
    //qDebug() << sql;

    if(query.exec(sql))
        ret = true;


    //close();

    return ret;

}
QHash<QString, QString> Dao::boolVal(QString select){

    QHash<QString,QString> tmpHash;

    QSqlQuery query(db);

    QString sql = "SELECT "+select;

    //qDebug() << sql;
    query.exec(sql);
    QSqlRecord record = query.record();
    int fieldcount = record.count();


    for( int r=0; query.next(); r++ ){
        for (int i = 0; i < fieldcount; i++) {

            tmpHash.insert(record.fieldName(i),query.value(i).toString());

        }
    }


    return tmpHash;

}
bool Dao::replaceRow(QString table,QHash<QString, QString> fieldsVals){

    bool ret = false;
    int i;
    QSqlQuery query(db);

    QStringList fields,values;
    foreach (QString k, fieldsVals.keys()) {

        fields << k;
        values << fieldsVals.value(k);

    }
    QString sql = "REPLACE INTO "+table+" ("+fields.join(",")+") VALUES('"+values.join("','")+"');";
    //qDebug() << sql;

    if(query.exec(sql))
        ret = true;

    return ret;

}

bool Dao::deleteRow(QString table, QString where){

    bool ret = false;
    QSqlQuery query(db);

    QString sql = "DELETE FROM "+table+" WHERE "+where;
    //qDebug() << sql;

    if(query.exec(sql))
        ret = true;


    //close();

    return ret;

}

void Dao::close()
{

    QString connection;
    connection = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(connection);


}
