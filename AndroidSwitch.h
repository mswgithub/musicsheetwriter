#ifndef ANDROIDSWITCH_H
#define ANDROIDSWITCH_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

class AndroidSwitch : public QWidget
{
    Q_OBJECT

public:
    explicit AndroidSwitch(QWidget *parent = 0);
    explicit AndroidSwitch(const QColor &color, QWidget *parent = 0);
    bool isToggled() const;
    void setToggle(bool set);
    void setDisable(bool set);
    bool isDisable() const;

private:
    bool _disabled;
    bool _switch;
    QColor _brush;
    qreal _opacity;
    qreal _radius;
    qint16 _x;
    qint16 _y;
    QTimer _timer;
    qint16 _height;
    qint16 _margin;
    QBrush _thumb;
    QBrush _track;

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *) Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

signals:
    void toggled();

    public slots:
    void timercall();
};

#endif // ANDROIDSWITCH_H
