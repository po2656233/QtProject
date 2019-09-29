#ifndef UILABEL_H
#define UILABEL_H

#include <QLabel>

class UILabel : public QLabel
{
    Q_OBJECT
public:
    explicit UILabel(QWidget *parent = nullptr);

    void setNoStyle(bool haveStyle);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
signals:
    void clicked();
public slots:
    void onClicked();
private:
    QString m_styleSheet;
    bool m_bStyle;
};

#endif // UILABEL_H
