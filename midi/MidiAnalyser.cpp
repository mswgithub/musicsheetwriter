#include "MidiAnalyser.h"

clock_t t_start, t_end;

// Begin of test
std::list<AMusicElement *> tempChord; // List of note that represent a chord
// End of test

void setChord(AMusicElement* note, clock_t start)
{
    if (tempChord.empty())
    {
        tempChord.push_back(note->duplicate());
        t_start = start / 100;
    }
}

bool isPartOfChord(clock_t start)
{
    if (t_start == (start / 100))
        return true;
    return false;
}

void addToChord(AMusicElement* note)
{
    tempChord.push_back(note->duplicate());
}

Chord* getChord()
{
    Chord*  chord = new Chord();

    for (std::list<AMusicElement*>::iterator it = tempChord.begin(); it != tempChord.end(); ++it)
    {
        AMusicElement* note = *it;
        chord->addNote(note->duplicate());
    }
    return chord;
}

void mycallback( double deltatime, std::vector< unsigned char > *message, void *data)
{
    (double)deltatime;
  size_t nBytes = message->size();
  MidiAnalyser* midi = (MidiAnalyser*)data;
  //static char * nn[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
  static Note::NoteName const noteNames[] = { Note::NoteName::C,Note::NoteName::C,Note::NoteName::D,
                                              Note::NoteName::D,Note::NoteName::E,Note::NoteName::F,
                                              Note::NoteName::F,Note::NoteName::G,Note::NoteName::G,
                                              Note::NoteName::A,Note::NoteName::A,Note::NoteName::B};
  if (nBytes > 2){
      if ((int)message->at(2) > 0){//note on
          if (midi->noEvent() && midi->isSilence()){
              double duration = midi->endSilence();//silence duration in ms
              qDebug() << "Silence:" << duration*(1000) << "ms";
              Rest rest;
              // Find out the duration and the length-bpm rate
              midi->storeDuration(rest, duration*1000);
              if (rest.getNumDuration() != 0) {
                  midi->getEditor()->addMusicElement(rest.duplicate());
              }
          }
          midi->addNote((int)message->at(1));
      }
      else if ((int)message->at(2) == 0){
          MidiNote *m = midi->findNote((int)message->at(1));
          if (m == NULL){
              qDebug() << "Error : no note found";
              return ;
          }
          const double elapsed = (double)(m->end - m->start) / CLOCKS_PER_SEC;
          int octave = (m->note > 128) ? (m->note - 128) / 12 : m->note / 12;// récupération de l'octave
          if (octave > 2) octave -= 2; //Why? I don't know!!!
          int accidental = 0;
          const int nnote = (m->note > 128) ? (m->note - 128) % 12 : m->note % 12;
          if (nnote == 1 || nnote == 3 || nnote == 6 || nnote == 8 || nnote == 10)
            accidental = 1;

          const Note::NoteName note = noteNames[nnote];
          Note n;
          n.setNoteName(note);
          n.setAccidental(accidental);
          n.setOctave(octave);

          midi->storeDuration(n, (unsigned long)(elapsed*1000));
          if (n.getNumDuration() != 0)
          {
              if (tempChord.empty())
              {
                  setChord(n.duplicate(), m->start);
                  qDebug() << "first initialisation of chord";
              }
              else
              {
                  if (isPartOfChord(m->start))
                  {
                      addToChord(n.duplicate());
                      qDebug() << "Note added to chord";
                  }
                  else if (!isPartOfChord(m->start) && (tempChord.size() == 1))
                  {
                      AMusicElement* tempNote = tempChord.front();
                      midi->getEditor()->addMusicElement(tempNote->duplicate());
                      tempChord.clear();
                      setChord(n.duplicate(), m->start);
                      qDebug() << "Event not part of chord, displaying last note";
                  }
                  else if (!isPartOfChord(m->start) && (tempChord.size() > 1))
                  {
                      midi->getEditor()->addMusicElement(getChord()->duplicate());
                      tempChord.clear();
                      setChord(n.duplicate(), m->start);
                      qDebug() << "Event not part of chord, displaying chord";
                  }
              }
          }
          //qDebug() << "midi:" << m->note << "note:" << nn[nnote] << "octave:" << octave <<  "accidental:" << accidental << "duration:" << elapsed;
          delete m;
          if (midi->noEvent()){
              midi->startSilence();
          }
      }
  }
}

void MidiAnalyser::run(){

}

MidiAnalyser::MidiAnalyser(IScoreEditor* scoreEditor):
    ASoundAnalyzer(scoreEditor)
{
}

void    MidiAnalyser::init(){
    this->midiIn = new RtMidiIn();
    // Check available ports.
    if ( chooseMidiPort( this->midiIn ) == false )
    {
      qDebug() << "No ports available!!!!!!!\n";
      throw UserException("No input ports available!");
    }
}

void    MidiAnalyser::start(){
    this->silenceOn = false;
    this->midiIn->setCallback( &mycallback, this );
    this->midiIn->ignoreTypes( true, true, true);
    qDebug() << "Let's play";
}

void    MidiAnalyser::stop(){
    qDebug() << "Stop playing";
    delete this->midiIn;
}

bool MidiAnalyser::chooseMidiPort( RtMidiIn *rtmidi )
{

  unsigned int i = 0, nPorts = rtmidi->getPortCount();
  if ( nPorts == 0 ) {
    return false;
  }

  if ( nPorts == 1 ) {
    qDebug() << "Opening ";// << rtmidi->getPortName();
  }
  else {
      qDebug() << "To implement user has to choose a port!!!!!";// << rtmidi->getPortName();
    /*
     QString portName;
     for ( i=0; i<nPorts; i++ ) {
      portName = rtmidi->getPortName(i);
      QDebug() << "  Input port #" << i << ": " << portName << '\n';
    }

    do {
      QDebug() << "\nChoose a port number: ";
      std::cin >> i;
    } while ( i >= nPorts );*/
  }

  rtmidi->openPort( i );

  return true;
}

MidiAnalyser::~MidiAnalyser(){
}

void MidiAnalyser::addNote(const int note){
    MidiNote *n = new MidiNote;
    n->note = note;
    n->start = clock();
    this->tmp_notes.push_back(n);
}

MidiNote *MidiAnalyser::findNote(const int note){
    MidiNote *r;
    for (std::list<MidiNote *>::iterator it = this->tmp_notes.begin();it!=this->tmp_notes.end();it++){
        if ((*it)->note == note){
            r = *it;
            this->tmp_notes.erase(it);
            r->end = clock();
            return r;
        }
    }
    return NULL;
}

void MidiAnalyser::startSilence(){
    this->silence_start = clock();
    this->silenceOn = true;
}

double MidiAnalyser::endSilence(){
    if (this->silenceOn){
        this->silence_end = clock();
        this->silenceOn = false;
        return (double)(this->silence_end - this->silence_start)/CLOCKS_PER_SEC;
    }
    return 0.0;
}

bool MidiAnalyser::noEvent() const{
    return this->tmp_notes.empty();
}

bool MidiAnalyser::isSilence() const{
    return this->silenceOn;
}
