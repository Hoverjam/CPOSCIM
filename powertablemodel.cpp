#include "powertablemodel.h"
#include <QDebug>

PowerTableModel::PowerTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    pName <<"在此开始输入";
    pPos <<"";
    pVol <<"";
    pRes <<"";
}

int PowerTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4; //四列
}

int PowerTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return pName.size();
}

QVariant PowerTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return QVariant();
    }
    if(role==Qt::DisplayRole||role==Qt::EditRole){
        switch(index.column()){
        case 0:
            return pName[index.row()];
            break;
        case 1:
            return pPos[index.row()];
            break;
        case 2:
            return pVol[index.row()];
            break;
        case 3:
            return pRes[index.row()];
            break;
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QVariant PowerTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole&&role!=Qt::EditRole)
        return QVariant();
    if(orientation==Qt::Horizontal)
        switch(section){
        case 0:
            return QString("牵引站名");
            break;
        case 1:
            return QString("牵引站位置\n(km)");
            break;
        case 2:
            return QString("电源电压\n(V)");
            break;
        case 3:
            return QString("等效电源内阻\n(Ω)");
            break;
        default:
            return QVariant();
        }
    else
        return QString("%1").arg(++section);
}

Qt::ItemFlags PowerTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool PowerTableModel::setData(const QModelIndex &index,
                              const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole){
        switch(index.column()){
        case 0:
            pName.replace(index.row(), value.toString());
            break;
        case 1:
            pPos.replace(index.row(), value.toString());
            break;
        case 2:
            pVol.replace(index.row(), value.toString());
            break;
        case 3:
            pRes.replace(index.row(), value.toString());
            break;
        default:
            return false;
        }
        emit dataChanged(index, index);
        emit dataChange4G(index.row(), index.column(), value.toString());
        return true;
    }
    return false;
}

bool PowerTableModel::setData0(const QModelIndex &index,int column, QString str)
{
    switch(column){
    case 1:
        pPos.replace(index.row(), str);
        break;
    case 2:
        pVol.replace(index.row(), str);
        break;
    case 3:
        pRes.replace(index.row(),str);
        break;
    default:
        return false;
    }
    emit dataChanged(index, index);
    return true;
}

QString PowerTableModel::getSinData(int column, int row)
{
    switch(column){
    case 1:
        return pPos[row];
        break;
    case 2:
        return pVol[row];
        break;
    case 3:
        return pRes[row];
        break;
    default:
        return "";
    }
}

int PowerTableModel::pSize()
{
    return pName.size();
}

bool PowerTableModel::insertRows(int position, int rows,
                                 const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), position, position+rows-1);
    for(int row=0; row<rows; ++row){
        pName.insert(position,"");
        pPos.insert(position,"");
        pVol.insert(position,"");
        pRes.insert(position,"");
    }
    endInsertRows();
    emit rowChanged(pSize());
    return true;
}

bool PowerTableModel::removeRows(int position, int rows,
                                 const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    for(int row=0; row<rows; ++row){
        pName.removeAt(position);
        pPos.removeAt(position);
        pVol.removeAt(position);
        pRes.removeAt(position);
    }
    endRemoveRows();
    emit rowChanged(pSize());
    return true;
}

//返回数据
QVector<double> PowerTableModel::getPPos()
{
    QVector<double> temp;
    QString tString;
    //qDebug()<<"done[2.1]";
    foreach(tString, pPos)
        temp<<tString.toDouble();
    return temp;
}

QVector<double> PowerTableModel::getPVol()
{
    QVector<double> temp;
    QString tString;
    foreach(tString, pVol)
        temp<<tString.toDouble();
    return temp;
}

QVector<double> PowerTableModel::getPRes()
{
    QVector<double> temp;
    QString tString;
    foreach(tString, pRes)
        temp<<tString.toDouble();
    return temp;
}
