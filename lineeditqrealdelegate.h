#ifndef EDITLINEQREALDELEGATE_H
#define EDITLINEQREALDELEGATE_H
#include <QItemDelegate>

class LineEditQrealDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit LineEditQrealDelegate(QObject *parent = 0,int flag = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
private:
    int flag4I;
};

#endif // EDITLINEQREALDELEGATE_H
