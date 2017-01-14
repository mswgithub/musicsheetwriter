#include "ScoreEditor.h"

ScoreEditor::ScoreEditor()
{
    _score = NULL;
    _cursor.first = 0;
    _cursor.second = 0;
}

ScoreEditor::ScoreEditor(Score *score)
{
    _score = score;
    if (_score)
    {
        QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[_score->getVoices()->count() - 1]->getMusicElements();
        _cursor.first = _score->getVoices()->count() - 1;
        _cursor.second = tmp->count() - 1;
        tmp->at(_cursor.second)->select(AMusicElement::ElementColor::RED);
        emit signalRefreshScore();
    }
}

ScoreEditor::~ScoreEditor()
{

}

Score*  ScoreEditor::getScore() const
{
    return _score;
}

void    ScoreEditor::setScore(Score* score)
{
    _score = score;
    if (_score)
    {
        QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[_score->getVoices()->count() - 1]->getMusicElements();
        _cursor.first = _score->getVoices()->count() - 1;
        _cursor.second = tmp->count() - 1;
        tmp->at(_cursor.second)->select(AMusicElement::ElementColor::RED);
        emit signalRefreshScore();
    }
}

QString const&  ScoreEditor::getTitle() const
{
    return _score->getTitle();
}

void    ScoreEditor::setTitle(QString const& title)
{
    _score->setTitle(title);
    emit signalRefreshScore();
}

QString const&  ScoreEditor::getComposer() const
{
    return _score->getComposer();
}

void    ScoreEditor::setComposer(QString const& composer)
{
    _score->setComposer(composer);
    emit signalRefreshScore();
}

QPair<int, int> const&  ScoreEditor::getCursorIndex() const
{
    return _cursor;
}

void    ScoreEditor::setCursorIndex(int posvoice, int posnote)
{
    _cursor.first = posvoice;
    _cursor.second = posnote;
}

void    ScoreEditor::moveCursorNoteOf(int noteMoveOf)
{
    QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[_cursor.first]->getMusicElements();
    for (int it = 0; it < tmp->count(); it++)
    {
        tmp->at(it)->deselect();
    }
    if (( _cursor.second + noteMoveOf) == tmp->count())
    {
        _cursor.second = 0;
    }
    else if ((_cursor.second + noteMoveOf < 0))
    {
        _cursor.second = tmp->count() - 1;
    }
    else
    {
        _cursor.second += noteMoveOf;
    }
    tmp->at(_cursor.second)->select(AMusicElement::ElementColor::RED);
    emit signalRefreshScore();
}

void    ScoreEditor::moveCursorVoiceOf(int voiceMoveOf)
{
    QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[_cursor.first]->getMusicElements();
    tmp->at(_cursor.second)->deselect();
    if ((_cursor.first + voiceMoveOf) == _score->getVoices()->count())
    {
        _cursor.first = 0;
    }
    else if ((_cursor.first + voiceMoveOf < 0))
    {
        _cursor.first = _score->getVoices()->count() - 1;
    }
    else
    {
        _cursor.first += voiceMoveOf;
    }
    tmp = (*(_score->getVoices()))[_cursor.first]->getMusicElements();
    if (_cursor.second <= tmp->count() - 1)
    {
        tmp->at(_cursor.second)->select(AMusicElement::ElementColor::RED);
    }
    else
    {
        _cursor.second = tmp->count() - 1;
        tmp->at(_cursor.second)->select(AMusicElement::ElementColor::RED);
    }
    emit signalRefreshScore();
}

void    ScoreEditor::moveCursorTo(int voiceMoveTo, int noteMoveTo)
{
    QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[voiceMoveTo]->getMusicElements();
    for (int it = 0; it < tmp->count(); it++)
    {
        tmp->at(it)->deselect();
    }
    tmp->at(noteMoveTo)->select(AMusicElement::ElementColor::RED);
    _cursor.first = voiceMoveTo;
    _cursor.second = noteMoveTo;
    emit signalRefreshScore();
}

void    ScoreEditor::addMusicElement(AMusicElement* musicElement)
{
    addMusicElement(musicElement, _cursor.first, _cursor.second);
}

void    ScoreEditor::addMusicElement(AMusicElement* musicElement, int voiceIndex, int noteIndex)
{
    QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[voiceIndex]->getMusicElements();
    for (auto it = tmp->begin(); it != tmp->end(); ++it) {
        (*it)->deselect();
    }
    musicElement->select(AMusicElement::ElementColor::RED);
    if (tmp->count() > 0)
    {
        (*(_score->getVoices()))[voiceIndex]->insertElement(noteIndex + 1, musicElement);
        moveCursorTo(voiceIndex, noteIndex + 1);
    }
    else
    {
        (*(_score->getVoices()))[voiceIndex]->insertElement(noteIndex, musicElement);
        moveCursorTo(voiceIndex, noteIndex);
    }
    emit signalRefreshScore();
}

void    ScoreEditor::deleteMusicElement()
{
    deleteMusicElement(_cursor.first, _cursor.second);
}

void    ScoreEditor::deleteMusicElement(int voiceIndex, int noteIndex)
{
    if ((*(_score->getVoices()))[voiceIndex]->getMusicElements()->count() > 0)
    {
        if (noteIndex + 1 < (*(_score->getVoices()))[voiceIndex]->getMusicElements()->count())
        {
            (*(_score->getVoices()))[voiceIndex]->removeElement(noteIndex);
            _cursor.second--;
            moveCursorNoteOf(1);
            emit signalRefreshScore();
        }
        else
        {
            moveCursorNoteOf(-1);
            (*(_score->getVoices()))[voiceIndex]->removeElement(noteIndex);
            emit signalRefreshScore();
        }
    }
    if (((*(_score->getVoices()))[voiceIndex]->getMusicElements()->count() == 0) && voiceIndex > 0)
    {
        this->deleteMeasure();
    }
}

void    ScoreEditor::addMeasure()
{
    Tag*    tag;
    Voice*  newVoice;

    newVoice = new Voice();
    tag = new Tag("clef");
    tag->addParam("g");
    newVoice->addElement(tag);
    tag = new Tag("meter");
    tag->addParam("4/4");
    newVoice->addElement(tag);
    _score->insertVoice(_cursor.first + 1, newVoice);
    QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[_cursor.first]->getMusicElements();
    tmp->at(_cursor.second)->deselect();
    moveCursorTo(_cursor.first + 1, 0);
    emit signalRefreshScore();
}

void    ScoreEditor::deleteMeasure()
{
    if (_cursor.first > 0)
    {
        (*(_score->getVoices())).remove(_cursor.first);
        _cursor.first--;
        moveCursorTo(_cursor.first, (*(_score->getVoices()))[_cursor.first]->getMusicElements()->count() - 1);
    }
    emit signalRefreshScore();
}

int ScoreEditor::getTimeSignatureNumerator() const
{
    int num = 4;
    QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[_cursor.first]->getMusicElements();

    for (int i = _cursor.second; i >= 0; i--)
    {
        Tag* tag = dynamic_cast<Tag*>(tmp->at (i));
        if (tag && tag->getName() == "meter")
        {
            QStringList list = tag->getParams()->at(0).first.split("/");
            bool ok;
            num = list.at(0).toInt(&ok, 10);
            return num;
        }
    }
    return num;
}

int ScoreEditor::getTimeSignatureNumerator(int voiceIndex, int noteIndex) const
{
    int num = 4;
    QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[voiceIndex]->getMusicElements();

    for (int i = noteIndex; i >= 0; i--)
    {
        Tag* tag = dynamic_cast<Tag*>(tmp->at (i));
        if (tag && tag->getName() == "meter")
        {
            QStringList list = tag->getParams()->at(0).first.split("/");
            bool ok;
            num = list.at(0).toInt(&ok, 10);
            return num;
        }
    }
    return num;
}

int ScoreEditor::getTimeSignatureDenominator() const
{
    int deno = 4;
    QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[_cursor.first]->getMusicElements();

    for (int i = _cursor.second; i >= 0; i--)
    {
        Tag* tag = dynamic_cast<Tag*>(tmp->at (i));
        if (tag && tag->getName() == "meter")
        {
            QStringList list = tag->getParams()->at(0).first.split("/");
            bool ok;
            deno = list.at(1).toInt(&ok, 10);
            return deno;
        }
    }
    return deno;
}

int ScoreEditor::getTimeSignatureDenominator(int voiceIndex, int noteIndex) const
{
    int deno = 4;
    QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[voiceIndex]->getMusicElements();

    for (int i = noteIndex; i >= 0; i--)
    {
        Tag* tag = dynamic_cast<Tag*>(tmp->at (i));
        if (tag && tag->getName() == "meter")
        {
            QStringList list = tag->getParams()->at(0).first.split("/");
            bool ok;
            deno = list.at(1).toInt(&ok, 10);
            return deno;
        }
    }
    return deno;
}

int ScoreEditor::getTempo() const
{
   int tempo = 120;
   QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[_cursor.first]->getMusicElements();

   for (int i = _cursor.second; i >= 0; i--)
   {
       Tag* tag = dynamic_cast<Tag*>(tmp->at (i));
       if (tag && tag->getName() == "tempo")
       {
           QStringList list = tag->getParams()->at(0).first.split("=");
           bool ok;
           tempo = list.at(1).toInt(&ok, 10);
           return tempo;
       }
   }
   return tempo;
}

int ScoreEditor::getTempo(int voiceIndex, int noteIndex) const
{
    int tempo = 120;
    QVector<AMusicElement*>* tmp = (*(_score->getVoices()))[voiceIndex]->getMusicElements();

    for (int i = noteIndex; i >= 0; i--)
    {
        Tag* tag = dynamic_cast<Tag*>(tmp->at (i));
        if (tag && tag->getName() == "tempo")
        {
            QStringList list = tag->getParams()->at(0).first.split("=");
            bool ok;
            tempo = list.at(1).toInt(&ok, 10);
            return tempo;
        }
    }
    return tempo;
}
