#ifndef SCOREEDITOR_H
#define SCOREEDITOR_H

#include    <QObject>
#include    "IScoreEditor.h"
#include    "Score.h"

class ScoreEditor : public QObject, public IScoreEditor
{
    Q_OBJECT
public:
    explicit ScoreEditor();
    explicit ScoreEditor(Score* score);
    virtual ~ScoreEditor();

    // IScoreEditor  implementation
    QString const&          getTitle() const;
    void                    setTitle(QString const& title);
    QString const&          getComposer() const;
    void                    setComposer(QString const& composer);
    QPair<int, int> const&  getCursorIndex() const;
    void                    setCursorIndex(int posvoice, int posnote);
    void                    moveCursorNoteOf(int noteMoveOf);
    void                    moveCursorVoiceOf(int voiceMoveOf);
    void                    moveCursorTo(int voiceMoveTo, int noteMoveTo);
    void                    addMusicElement(AMusicElement* musicElement);
    void                    addMusicElement(AMusicElement* musicElement, int voiceIndex, int noteIndex);
    void                    deleteMusicElement();
    void                    deleteMusicElement(int voiceIndex, int noteIndex);
    void                    addMeasure();
    void                    deleteMeasure();
    int                     getTimeSignatureNumerator() const;
    int                     getTimeSignatureNumerator(int voiceIndex, int noteIndex) const;
    int                     getTimeSignatureDenominator() const;
    int                     getTimeSignatureDenominator(int voiceIndex, int noteIndex) const;
    int                     getTempo() const;
    int                     getTempo(int voiceIndex, int noteIndex) const;

    // Score accessors
    Score*  getScore() const;
    void    setScore(Score* score);


signals:
    void    signalRefreshScore();

private:
    Score*          _score;
    // La premiere valeur pointe sur la voice
    // et la deuxieme sur l'élement musical.
    QPair<int, int> _cursor;
};

#endif // SCOREEDITOR_H
