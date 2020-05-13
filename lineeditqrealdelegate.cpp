#include "lineeditqrealdelegate.h"
#include <QLineEdit>

LineEditQrealDelegate::LineEditQrealDelegate(QObject *parent, int flag) :
    QItemDelegate(parent)
{
    flag4I = flag;
}

// 创建编辑器
QWidget *LineEditQrealDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &/* option */,
                                       const QModelIndex &/* index */) const
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    if(flag4I == 0)
    {
        //正则表达式有莫名奇妙的bug
        QDoubleValidator *validator = new QDoubleValidator(0,99999999,20,parent);
        lineEdit->setValidator(validator);
    }
    else
    {
        QDoubleValidator *validator = new QDoubleValidator(parent);
        lineEdit->setValidator(validator);
    }
    return lineEdit;
}

// 为编辑器设置数据
void LineEditQrealDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(index.model()->data(index,Qt::EditRole).toString());
}

// 将数据写入到模型
void LineEditQrealDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    model->setData(index, lineEdit->text(), Qt::EditRole);
}

// 更新编辑器几何布局
void LineEditQrealDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

