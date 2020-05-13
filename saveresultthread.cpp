#include "saveresultthread.h"
#include <QFile>
#include <QDir>
#include <QAxObject>
#include <QDebug>

SaveResultThread::SaveResultThread(QObject *parent) :
    QThread(parent),
    stopped(false)
{

}

void SaveResultThread::run()
{
    QString tempStr;
    if(flag == 0)
        tempStr = "钢轨电位(V)";
    else if(flag == 1)
        tempStr = "杂散电流(A)";
    if(fileName.right(5)==".xlsx" || fileName.right(5)==".XLSX")
    {
        QAxObject *excel = new QAxObject();//不是主线程，这里不能放this
        if(!excel->setControl("Excel.Application"))
        {
            emit saved("🤷 没能打开EXCEL，该功能需要EXCEL的控件，看看你是不是没装");
            return;//连接Excel控件
        }

        excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
        excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭时会出现“文件已修改，是否保存”的提示
        QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
        workbooks->dynamicCall("Add");//新建一个工作簿
        QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
        QAxObject *worksheets = workbook->querySubObject("Sheets");//获取工作表集合
        QAxObject *worksheet = worksheets->querySubObject("Item(int)",1);//获取工作表集合的工作表1，即sheet1

        worksheet->querySubObject("Cells(int,int)", 1 , 1)->setProperty("Value", "距离(km)");
        worksheet->querySubObject("Cells(int,int)", 1 , 2)->setProperty("Value", tempStr);

        QList<QVariant> allRowsData;//保存所有行数据
        allRowsData.clear();
        QVector<qreal>::iterator it1;
        QVector<qreal>::iterator it2;
        for(it1=distance.begin(),it2=current.begin(); it1 != distance.end(); ++it1,++it2)
        {
            QList<QVariant> aRowData;//保存一行数据
            aRowData.append(QVariant(*it1));
            aRowData.append(QVariant(*it2));
            allRowsData.append(QVariant(aRowData));
        }
        QAxObject *range = worksheet->querySubObject("Range(const QString )", "A2:B"+QString::number(distance.size()+1));
        range->dynamicCall("SetValue(const QVariant&)",QVariant(allRowsData));//存储所有数据到 excel

        workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));//保存
        workbook->dynamicCall("Close()");//关闭工作簿
        excel->dynamicCall("Quit()");//关闭excel
        delete excel;
        excel=NULL;

        goto finish;
    }
    if(fileName.right(4)==".txt" || fileName.right(4)==".TXT")
    {
        QFile file(fileName);
        //qDebug()<<fileName;
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
        {
            //QMessageBox::warning(this,tr("错误"),tr("打开文件失败\n未知错误"));
            return;
        }
        QTextStream out(&file);
        out<<"距离(km)  "+tempStr+"\n";
        QVector<qreal>::iterator it1;
        QVector<qreal>::iterator it2;
        for(it1=distance.begin(),it2=current.begin(); it1 != distance.end(); ++it1,++it2)
            out<<*it1<<"     "<<*it2<<endl;
        file.close();
    }

    finish:
    stopped = false;
    emit saved("👌 导出完成");
}

void SaveResultThread::stop()
{
    stopped = true;
    emit saved("已停止导出");
}
