#ifndef METRONOMEPLAYER_H
#define METRONOMEPLAYER_H

#include <QWidget>

namespace Ui {
class MetronomePlayer;
}

class MetronomePlayer : public QWidget
{
    Q_OBJECT

public:
    explicit MetronomePlayer(QWidget *metronomeView);
    ~MetronomePlayer();
    void    setTranslation();

private:
    Ui::MetronomePlayer *_ui;
    bool                _state;
    QWidget*            _widgetTranslate;

private slots:
    void    slotButtonClicked();
    void    slotBmpChanged(QString value);
    void    slotSetBmpValue(int value);
    void    slotBtnAddBPMClicked();

signals:
    void    signalStartClicked(int bmp);
    void    signalStopClicked();
    void    signalButtonAddClicked(QString bmp);

};

#endif // METRONOMEPLAYER_H
