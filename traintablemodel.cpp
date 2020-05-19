#include "traintablemodel.h"

TrainTableModel::TrainTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    tName<<"在此开始输入";
    tPos<<"";
    tCur<<"";
}

int TrainTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3; //三列
}

int TrainTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return tName.size();
}

QVariant TrainTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return QVariant();
    }
    if(role==Qt::DisplayRole||role==Qt::EditRole){
        switch(index.column()){
        case 0:
            return tName[index.row()];
            break;
        case 1:
            return tPos[index.row()];
            break;
        case 2:
            return tCur[index.row()];
            break;
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QVariant TrainTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole&&role!=Qt::EditRole)
        return QVariant();
    if(orientation==Qt::Horizontal)
        switch(section){
        case 0:
            return QString("机车编号");
            break;
        case 1:
            return QString("机车位置\n(km)");
            break;
        case 2:
            return QString("牵引电流\n(A)");
            break;
        default:
            return QVariant();
        }
    else
        return QString("%1").arg(++section);
}

Qt::ItemFlags TrainTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool TrainTableModel::setData(const QModelIndex &index,
                              const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole){
        switch(index.column()){
        case 0:
            tName.replace(index.row(), value.toString());
            break;
        case 1:
            tPos.replace(index.row(), value.toString());
            break;
        case 2:
            tCur.replace(index.row(), value.toString());
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

bool TrainTableModel::setData0(const QModelIndex &index,int column, QString str)
{
    switch(column){
    case 1:
        tPos.replace(index.row(), str);
        break;
    case 2:
        tCur.replace(index.row(), str);
        break;
    default:
        return false;
    }
    emit dataChanged(index, index);
    return true;
}

int TrainTableModel::tSize()
{
    return tName.size();
}

bool TrainTableModel::insertRows(int position, int rows,
                                 const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), position, position+rows-1);
    for(int row=0; row<rows; ++row){
        tName.insert(position,"");
        tPos.insert(position,"");
        tCur.insert(position,"");
    }
    endInsertRows();
    emit rowChanged(tSize());
    return true;
}

bool TrainTableModel::removeRows(int position, int rows,
                                 const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    for(int row=0; row<rows; ++row){
        tName.removeAt(position);
        tPos.removeAt(position);
        tCur.removeAt(position);
    }
    endRemoveRows();
    emit rowChanged(tSize());
    return true;
}

//返回数据
QVector<double> TrainTableModel::getTPos()
{
    QVector<double> temp;
    QString tString;
    foreach(tString, tPos)
        temp<<tString.toDouble();
    return temp;
}

QVector<double> TrainTableModel::getTCur()
{
    QVector<double> temp;
    QString tString;
    foreach(tString, tCur)
        temp<<tString.toDouble();
    return temp;
}

QString TrainTableModel::getSinData(int column, int row)
{
    switch(column){
    case 1:
        return tPos[row];
        break;
    case 2:
        return tCur[row];
        break;
    default:
        return "";
    }
}
