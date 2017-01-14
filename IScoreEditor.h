#ifndef ISCOREEDITOR_H
#define ISCOREEDITOR_H

#include    <QList>
#include    <QPair>
#include    <QString>
#include    "AMusicElement.h"
#include    "Score.h"

class IScoreEditor
{
public:
    virtual QString const&          getTitle() const = 0;
    virtual void                    setTitle(QString const& title) = 0;
    virtual QString const&          getComposer() const = 0;
    virtual void                    setComposer(QString const& composer) = 0;
    virtual QPair<int, int> const&  getCursorIndex() const = 0;
    virtual void                    setCursorIndex(int posvoice, int posnote) = 0;
    virtual void                    moveCursorNoteOf(int noteMoveOf) = 0;
    virtual void                    moveCursorVoiceOf(int voiceMoveOf) = 0;
    virtual void                    moveCursorTo(int voiceMoveTo, int noteMoveTo) = 0;
    virtual void                    addMusicElement(AMusicElement* musicElement) = 0;
    virtual void                    addMusicElement(AMusicElement* musicElement, int voiceIndex, int noteIndex) = 0;
    virtual void                    deleteMusicElement() = 0;
    virtual void                    deleteMusicElement(int voiceIndex, int noteIndex) = 0;
    virtual void                    addMeasure() = 0;
    virtual void                    deleteMeasure() = 0;
    virtual int                     getTimeSignatureNumerator() const = 0;
    virtual int                     getTimeSignatureNumerator(int voiceIndex, int noteIndex) const = 0;
    virtual int                     getTimeSignatureDenominator() const = 0;
    virtual int                     getTimeSignatureDenominator(int voiceIndex, int noteIndex) const = 0;
    virtual int                     getTempo() const = 0;
    virtual int                     getTempo(int voiceIndex, int noteIndex) const = 0;
};

#endif // ISCOREEDITOR_H
