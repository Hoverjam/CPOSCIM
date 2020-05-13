#ifndef POWERTABLEMODEL_H
#define POWERTABLEMODEL_H

#include <QAbstractTableModel>

class PowerTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit PowerTableModel(QObject *parent=nullptr);
    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);

    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

    int pSize();

    QVector<double> getPPos();
    QVector<double> getPVol();
    QVector<double> getPRes();

    QString getSinData(int, int);
    bool setData0(const QModelIndex &index,int column, QString str);

signals:
    void rowChanged(int);
    void dataChange4G(int, int, QString);

private:
    QStringList pName;
    QStringList pPos;
    QStringList pVol;
    QStringList pRes;
};

#endif // POWERTABLEMODEL_H
