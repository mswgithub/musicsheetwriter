#ifndef AEDITIONMANAGER_H
#define AEDITIONMANAGER_H

#include    <QObject>
#include    <QKeyEvent>
#include    "GUIDOFactoryAdapter.h"
#include    "Score.h"

class AEditionManager : public QObject
{
    Q_OBJECT
public:
    explicit AEditionManager(QObject* parent = 0);
    virtual ~AEditionManager();

    virtual void    updateScore(Score* score) = 0;
    virtual void    keyReleaseEventHandler(QKeyEvent* event) = 0;
    virtual void    setBPMFromMetronome(QString bpm) = 0;
    virtual void    setTranslation() = 0;

protected:
    Score*              getScore() const;
    void                setScore(Score* score);

private:
    Score*              _score;

signals:

public slots:
    virtual void    slotRefreshScore() = 0;
};

#endif // AEDITIONMANAGER_H
