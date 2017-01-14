#include "FrequencyTable.h"

FrequencyTable::FrequencyTable()
{
    _frequenciesTable[16.35] = Note(Note::NoteName::C, 0, -1);
    _frequenciesTable[17.32] = Note(Note::NoteName::C, 1, -1);
    _frequenciesTable[18.35] = Note(Note::NoteName::D, 0, -1);
    _frequenciesTable[19.45] = Note(Note::NoteName::D, 1, -1);
    _frequenciesTable[20.6] = Note(Note::NoteName::E, 0, -1);
    _frequenciesTable[21.83] = Note(Note::NoteName::F, 0, -1);
    _frequenciesTable[23.12] = Note(Note::NoteName::F, 1, -1);
    _frequenciesTable[24.5] = Note(Note::NoteName::G, 0, -1);
    _frequenciesTable[25.96] = Note(Note::NoteName::G, 1, -1);
    _frequenciesTable[27.5] = Note(Note::NoteName::A, 0, -1);
    _frequenciesTable[29.1353] = Note(Note::NoteName::A, 1, -1);
    _frequenciesTable[30.8677] = Note(Note::NoteName::B, 0, -1);

    _frequenciesTable[32.7032] = Note(Note::NoteName::C, 0, 0);
    _frequenciesTable[34.6479] = Note(Note::NoteName::C, 1, 0);
    _frequenciesTable[36.7081] = Note(Note::NoteName::D, 0, 0);
    _frequenciesTable[38.8909] = Note(Note::NoteName::D, 1, 0);
    _frequenciesTable[41.2035] = Note(Note::NoteName::E, 0, 0);
    _frequenciesTable[43.6536] = Note(Note::NoteName::F, 0, 0);
    _frequenciesTable[46.2493] = Note(Note::NoteName::F, 1, 0);
    _frequenciesTable[48.9995] = Note(Note::NoteName::G, 0, 0);
    _frequenciesTable[51.9130] = Note(Note::NoteName::G, 1, 0);
    _frequenciesTable[55] = Note(Note::NoteName::A, 0, 0);
    _frequenciesTable[58.2705] = Note(Note::NoteName::A, 1, 0);
    _frequenciesTable[61.7354] = Note(Note::NoteName::B, 0, 0);

    _frequenciesTable[65.4064] = Note(Note::NoteName::C, 0, 1);
    _frequenciesTable[69.2957] = Note(Note::NoteName::C, 1, 1);
    _frequenciesTable[73.4162] = Note(Note::NoteName::D, 0, 1);
    _frequenciesTable[77.7817] = Note(Note::NoteName::D, 1, 1);
    _frequenciesTable[82.4069] = Note(Note::NoteName::E, 0, 1);
    _frequenciesTable[87.3071] = Note(Note::NoteName::F, 0, 1);
    _frequenciesTable[92.4986] = Note(Note::NoteName::F, 1, 1);
    _frequenciesTable[97.9989] = Note(Note::NoteName::G, 0, 1);
    _frequenciesTable[103.826] = Note(Note::NoteName::G, 1, 1);
    _frequenciesTable[110] = Note(Note::NoteName::A, 0, 1);//A2
    _frequenciesTable[116.541] = Note(Note::NoteName::A, 1, 1);
    _frequenciesTable[123.471] = Note(Note::NoteName::B, 0, 1);

    _frequenciesTable[130.813] = Note(Note::NoteName::C, 0, 2);
    _frequenciesTable[138.591] = Note(Note::NoteName::C, 1, 2);
    _frequenciesTable[146.832] = Note(Note::NoteName::D, 0, 2);
    _frequenciesTable[155.563] = Note(Note::NoteName::D, 1, 2);
    _frequenciesTable[164.814] = Note(Note::NoteName::E, 0, 2);
    _frequenciesTable[174.614] = Note(Note::NoteName::F, 0, 2);
    _frequenciesTable[184.997] = Note(Note::NoteName::F, 1, 2);
    _frequenciesTable[195.998] = Note(Note::NoteName::G, 0, 2);
    _frequenciesTable[207.652] = Note(Note::NoteName::G, 1, 2);
    _frequenciesTable[220] = Note(Note::NoteName::A, 0, 2);
    _frequenciesTable[233.082] = Note(Note::NoteName::A, 1, 2);
    _frequenciesTable[246.942] = Note(Note::NoteName::B, 0, 2);

    _frequenciesTable[261.626] = Note(Note::NoteName::C, 0, 3);
    _frequenciesTable[277.183] = Note(Note::NoteName::C, 1, 3);
    _frequenciesTable[293.665] = Note(Note::NoteName::D, 0, 3);
    _frequenciesTable[311.127] = Note(Note::NoteName::D, 1, 3);
    _frequenciesTable[329.628] = Note(Note::NoteName::E, 0, 3);
    _frequenciesTable[349.228] = Note(Note::NoteName::F, 0, 3);
    _frequenciesTable[369.994] = Note(Note::NoteName::F, 1, 3);
    _frequenciesTable[391.995] = Note(Note::NoteName::G, 0, 3);
    _frequenciesTable[415.305] = Note(Note::NoteName::G, 1, 3);
    _frequenciesTable[440] = Note(Note::NoteName::A, 0, 3);
    _frequenciesTable[466.164] = Note(Note::NoteName::A, 1, 3);
    _frequenciesTable[493.883] = Note(Note::NoteName::B, 0, 3);

    _frequenciesTable[523.251] = Note(Note::NoteName::C, 0, 4);
    _frequenciesTable[554.365] = Note(Note::NoteName::C, 1, 4);
    _frequenciesTable[587.33] = Note(Note::NoteName::D, 0, 4);
    _frequenciesTable[622.254] = Note(Note::NoteName::D, 1, 4);
    _frequenciesTable[659.255] = Note(Note::NoteName::E, 0, 4);
    _frequenciesTable[698.456] = Note(Note::NoteName::F, 0, 4);
    _frequenciesTable[739.989] = Note(Note::NoteName::F, 1, 4);
    _frequenciesTable[783.991] = Note(Note::NoteName::G, 0, 4);
    _frequenciesTable[830.609] = Note(Note::NoteName::G, 1, 4);
    _frequenciesTable[880] = Note(Note::NoteName::A, 0, 4);
    _frequenciesTable[932.328] = Note(Note::NoteName::A, 1, 4);
    _frequenciesTable[987.767] = Note(Note::NoteName::B, 0, 4);

    _frequenciesTable[1046.5] = Note(Note::NoteName::C, 0, 5);
    _frequenciesTable[1108.73] = Note(Note::NoteName::C, 1, 5);
    _frequenciesTable[1174.66] = Note(Note::NoteName::D, 0, 5);
    _frequenciesTable[1244.51] = Note(Note::NoteName::D, 1, 5);
    _frequenciesTable[1318.51] = Note(Note::NoteName::E, 0, 5);
    _frequenciesTable[1396.91] = Note(Note::NoteName::F, 0, 5);
    _frequenciesTable[1479.98] = Note(Note::NoteName::F, 1, 5);
    _frequenciesTable[1567.98] = Note(Note::NoteName::G, 0, 5);
    _frequenciesTable[1661.22] = Note(Note::NoteName::G, 1, 5);
    _frequenciesTable[1760] = Note(Note::NoteName::A, 0, 5);
    _frequenciesTable[1864.66] = Note(Note::NoteName::A, 1, 5);
    _frequenciesTable[1975.53] = Note(Note::NoteName::B, 1, 5);

    _frequenciesTable[2093] = Note(Note::NoteName::C, 0, 6);
    _frequenciesTable[2217.46] = Note(Note::NoteName::C, 0, 6);
    _frequenciesTable[2349.32] = Note(Note::NoteName::D, 1, 6);
    _frequenciesTable[2489.02] = Note(Note::NoteName::D, 0, 6);
    _frequenciesTable[2637.02] = Note(Note::NoteName::C, 0, 6);
    _frequenciesTable[2793.83] = Note(Note::NoteName::E, 0, 6);
    _frequenciesTable[2959.96] = Note(Note::NoteName::F, 1, 6);
    _frequenciesTable[3135.96] = Note(Note::NoteName::F, 0, 6);
    _frequenciesTable[3322.44] = Note(Note::NoteName::G, 1, 6);
    _frequenciesTable[3520] = Note(Note::NoteName::G, 0, 6);
    _frequenciesTable[3729.31] = Note(Note::NoteName::A, 1, 6);
    _frequenciesTable[3951.07] = Note(Note::NoteName::B, 0, 6);
    _frequenciesTable[4186.01] = Note(Note::NoteName::C, 0, 7);
}

FrequencyTable::~FrequencyTable()
{

}

/*
 * Retrieve the corresponding note whose fundamental frequency is the
 * closest one to the frequency passed as parameter.
 */
Note    FrequencyTable::getNote(double frequency) const
{
    double diff = 999999;
    double fundFrequency = 0;
    for (QMap<double, Note>::const_iterator it = _frequenciesTable.begin();
         it != _frequenciesTable.end();
         ++it)
    {
        double tmp = it.key() - frequency;
        if (tmp < 0) tmp = -tmp;

        if (tmp < diff)
        {
            diff = tmp;
            fundFrequency = it.key();
        }
    }

    if (!_frequenciesTable.count(fundFrequency))
    {
        throw std::range_error("No note for the frequency");
    }

    return _frequenciesTable[fundFrequency];
}

double   FrequencyTable::getFrequency(Note const& note) const
{
    double fundFrequency;

    if ((fundFrequency = _frequenciesTable.key(note, 0.f)) == 0.f)
    {
        throw std::range_error("No frequency for the note");
    }

    return fundFrequency;
}
