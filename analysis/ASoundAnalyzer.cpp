#include "ASoundAnalyzer.h"

ASoundAnalyzer::ASoundAnalyzer(IScoreEditor* scoreEditor)
{
    _scoreEditor = scoreEditor;
}

ASoundAnalyzer::~ASoundAnalyzer()
{

}

IScoreEditor*   ASoundAnalyzer::getEditor() const
{
    return _scoreEditor;
}

/*
 * Calculate the duration of an event from the time (in millisecond)
 * for which the note has been played on, the BPM and the meter
 * (num and denom) of the measure.
 * The BPM and the meter is retrieved from the scoreEditor.
 *
 * The num, denom and the number of dots are stored in the event passed
 * as parameter.
 */
void    ASoundAnalyzer::storeDuration(AEvent& event, unsigned long time) const
{
    int bpm = this->getEditor()->getTempo();
    int numMeasure = this->getEditor()->getTimeSignatureNumerator();
    int denomMeasure = this->getEditor()->getTimeSignatureDenominator();

    // The duration of a single beat (in milliseconde)
    double beatDuration = 60000 / bpm;

    // if the measure denominator is in /4, /8, /12, etc.
    // TODO change to : If the signature is binary
    if ((denomMeasure * 4) / 4 == denomMeasure) {
        // round up to the nearest quarter of beat
        time = this->roundXToY(time, beatDuration / 4);
    }
    // TODO else the signature is ternary
    else {
        time = this->roundXToY(time, beatDuration / 6);
    }

    // The decimal number of beats of the event
    double numberOfBeats = time / beatDuration;

    QPair<int, int> fraction = this->decimalToFraction(numberOfBeats);
    fraction.second = fraction.second * denomMeasure;

    // if the time can be properly divided by 1.75
    // then, remove 1,75 from the fraction and add two dots
    if ((float) fraction.first / 7.f == (int) (fraction.first / 7) &&
            (float) fraction.second / 4.f == (int) (fraction.second / 4))
    {
        event.setNumDuration(fraction.first / 7);
        event.setDenomDuration(fraction.second / 4);
        event.setDots(2);
    }
    // same with 1,5
    else if ((float) fraction.first / 3.f == (int) (fraction.first / 3) &&
             (float) fraction.second / 2.f == (int) (fraction.second / 2))
    {
        event.setNumDuration(fraction.first / 3);
        event.setDenomDuration(fraction.second / 2);
        event.setDots(1);
    }
    // otherwise, set the numerator and denominator as calculated
    else
    {
        event.setNumDuration(fraction.first);
        event.setDenomDuration(fraction.second);
        event.setDots(0);
    }
}

QPair<int, int> ASoundAnalyzer::decimalToFraction(double decimal) const
{
    double integral = std::floor(decimal);
    double frac = decimal - integral;

    // This is the accuracy. Get the 4 digits after comma
    const long precision = 10000;

    long gcd_ = gcd(std::round(frac * precision), precision);

    long denominator = precision / gcd_;
    long numerator = std::round(frac * precision) / gcd_;

    return QPair<int, int>(integral*denominator+numerator, denominator);
}

double  ASoundAnalyzer::roundXToY(double x, double y) const
{
    return std::round(x / y) * y;
}

long    ASoundAnalyzer::gcd(long a, long b) const
{
    if (a == 0)
        return b;
    else if (b == 0)
        return a;

    if (a < b)
        return this->gcd(a, b % a);
    else
        return this->gcd(b, a % b);
}
