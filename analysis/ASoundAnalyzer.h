#ifndef ASOUNDANALYZER_H
#define ASOUNDANALYZER_H

#include    <QThread>
#include    <cmath>
#include    "IScoreEditor.h"

class ASoundAnalyzer : public QThread
{
    Q_OBJECT
public:
    ASoundAnalyzer(IScoreEditor* scoreEditor);
    virtual ~ASoundAnalyzer();

    virtual void start() = 0;
    virtual void stop() = 0;
    IScoreEditor*   getEditor() const;
    void            storeDuration(AEvent& event, unsigned long time) const;

private:
    QPair<int, int> decimalToFraction(double decimal) const;
    double          roundXToY(double x, double y) const;
    long            gcd(long a, long b) const;

private:
    IScoreEditor*    _scoreEditor;
};

#endif // ASOUNDANALYZER_H
