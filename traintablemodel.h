#ifndef TRAINTABLEMODEL_H
#define TRAINTABLEMODEL_H

#include <QAbstractTableModel>

class TrainTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TrainTableModel(QObject *parent=nullptr);
    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);

    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

    int tSize();

    QVector<double> getTPos();
    QVector<double> getTCur();

    QString getSinData(int, int);

    bool setData0(const QModelIndex &index,int column, QString str);

signals:
    void rowChanged(int);
    void dataChange4G(int, int, QString);

private:
    QStringList tName;
    QStringList tPos;
    QStringList tCur;
};

#endif // TRAINTABLEMODEL_H
