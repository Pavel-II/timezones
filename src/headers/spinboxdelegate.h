#ifndef SPINBOXDELEGATE_H
#define SPINBOXDELEGATE_H

#include <QWidget>
#include <QSpinBox>
#include <QItemDelegate>

class SpinBoxDelegate : public QItemDelegate
{
private:
    int min_v = -720;
    int max_v = 720;
public:
    SpinBoxDelegate(QObject *parent = 0);
    void set_minmax(int minv, int maxv);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // SPINBOXDELEGATE_H
