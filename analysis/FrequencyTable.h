#ifndef FREQUENCYTABLE_H
#define FREQUENCYTABLE_H

#include    <QMap>
#include    <stdexcept>
#include    "Note.h"

class FrequencyTable
{
public:
    FrequencyTable();
    virtual ~FrequencyTable();

    Note    getNote(double frequency) const;
    double   getFrequency(Note const& note) const;

private:
    QMap<double, Note>       _frequenciesTable;
};

#endif // FREQUENCYTABLE_H
