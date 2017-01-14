#include "Parse.h"

Parse::Parse(std::string tmp_FName)
{
    FName = tmp_FName;
    GmnFile.open(FName);
    std::ifstream ifs(FName);
    std::string content1( (std::istreambuf_iterator<char>(ifs) ),
                          (std::istreambuf_iterator<char>()    ) );
    _score = new Score();
    _lastNote = new Note();
    _lastRest = new Rest();
    int i = 0;

    content = content1;
    clean();
    //do while for each voices
    std::vector<std::string> VoiceStr;
    VoiceStr = CutPartVoice();

    while (i < VoiceStr.size()){
        _score->addVoice(ParseVoice(VoiceStr.at(i)));
        i++;
    }
    //rentrer la voice general dans score
    //fin du while des voices


    qDebug() << "title:" << _score->getTitle();
    qDebug() << "composer:" << _score->getComposer();

    QVector<Voice*>*    test = new QVector<Voice*>();
    test = _score->getVoices();

    Voice* voice = new Voice();
    voice = test->at(0);
    // egale a la premier voice

    int a = 0;

    QVector<AMusicElement*>* elem = new QVector<AMusicElement*>();
    elem = voice->getMusicElements();
}

void    Parse::clean(){
    content.erase(std::remove(content.begin(), content.end(), '\n'), content.end());

    int i = 0;
    while (i < content.size()){
        if (content.at(i) == ' ' && content.at(i + 1) == ' '){
            content.erase(i,1);
            i--;
        }
        i++;
    }

    while (content.find("*)") != std::string::npos){
        content.erase(content.begin() + content.find("(*"), content.begin() + content.find("*)")+2);
    }
}

std::vector<std::string>    Parse::CutPartVoice(){
    std::string                 NewContent;
    std::vector<std::string>    tab;
    bool                        endl = false;

    while (endl == false)//endl == 0
    {
        int start = content.find('[');
        int end = content.find("],", start);

        if (content.find("],", start) != std::string::npos){
            //multi voices
            NewContent = content.substr(start + 2, end - start - 1);
            content.erase(start, end - start);
        }
        else
        {
            //only one or last voice
            NewContent = content.substr(start + 2, content.find_last_of("]") - start - 1);
            content.erase(start + 1, content.find_last_of("]") - start - 1);
            endl = true;
        }
        tab.push_back(NewContent);
    }
    return tab;
}

Voice*    Parse::ParseVoice(std::string voice){
    int                 pos = 0;
    std::string         tmp;
    Voice*              voiceTmp = new Voice();
    Note*               noteTmp = new Note();
    Rest*               restTmp = new Rest();

    while (pos < voice.length()){
        // si jamais il y a un \ devant (donc un tag)
        if (voice.at(0) == ' ')
        {
            voice.erase(0, 1);
        }
        if (voice.at(0) == '[')
        {
            voice.erase(0, 1);
        }
        if (voice.at(pos) == '\\'){
            pos++;
            if (voice.substr(pos, 5) == "staff"){
                pos += 6;
                tmp = voice.substr(voice.find('<') + 1, voice.find('>') - voice.find('<') - 1);
                pos += tmp.length() + 1;
                //erase tout pour que title soit au debut puis remet pos a 0
                voice.erase(0, pos + 1);
                pos = 0;
            }
            else if (voice.substr(pos, 5) == "title"){
                pos += 6;
                tmp = voice.substr(voice.find('<') + 2, voice.find('>') - voice.find('<') - 4);
                _score->setTitle(QString::fromStdString(tmp));
                pos += tmp.length() + 4;
                voice.erase(0, pos + 1);
                pos = 0;
            }
            else if (voice.substr(pos, 8) == "composer"){
                pos += 8;
                tmp = voice.substr(voice.find('<') + 2, voice.find("\"", pos + 2) - voice.find('<') - 3);
                _score->setComposer(QString::fromStdString(tmp));
                pos += voice.find('>') - voice.find('<') + 1;
                voice.erase(0, pos + 1);
                pos = 0;
            }
            else if (voice.substr(pos, 5) == "instr"){
                pos += voice.find(">");
                voice.erase(0, pos + 1);
                pos = 0;
            }
            else if (voice.substr(pos, 12) == "systemFormat"){
                pos += voice.find(">");
                voice.erase(0, pos + 1);
                pos = 0;
            }
            else {
                Tag* tagTmp = new Tag();
                if ((tagTmp = dynamic_cast<Tag*>(ParseTag(voice))) == NULL)
                {
                    qDebug() << "erreur with Tag in ParceVoice() <---- Parce.cpp";
                    return NULL;
                }
                else{
                    //erase voice tag
                    if (voice.at(0) == '\\'){
                        if (voice.at(voice.find(' ') - 1) == '('){
                            voice.erase(0, count(voice, '(', voice.find('(')) + 1);
                        }
                        else if (voice.find('<') < voice.find(' '))
                        {
                            voice.erase(0, voice.find('>') + 1);
                        }
                        else
                        {
                            voice.erase(0, voice.find(' '));
                        }
                    }
                    pos = 0;
                    voiceTmp->addElement(tagTmp);
                }
            }
        }
        else if (voice.at(0) != ' ' && voice.at(0) != ']'){
            if (voice.at(0) == '_'){
                if ((restTmp = dynamic_cast<Rest*>(ParseRest(voice.substr(0, voice.find(' '))))) == NULL)
                {
                    qDebug() << "erreur with Rest in ParceVoice() <---- Parce.cpp";
                    return NULL;
                }
                else
                {
                    voiceTmp->addElement(restTmp);
                    _lastIsNote = false;
                    _lastRest = restTmp;
                    pos = 0;
                    voice.erase(0, voice.find(' ') + 1);
                }
            }
            else if (voice.at(0) == '{'){
                int i = 0;
                std::string tmp;
                Chord* chord = new Chord();

                tmp = voice.substr(voice.find('{'), voice.find('}') + 1);
                std::replace( tmp.begin(), tmp.end(), '{', '(');
                std::replace( tmp.begin(), tmp.end(), '}', ')');

                chord->setNote(ParseParenthese(tmp));

                pos = 0;
                voice.erase(0, voice.find('}') + 1);
            }
            else{
                // on envoi que la note et ses attribus, pas besoin de toute la voice
               int a = 0;
                if (voice.find(' ') == -1){
                    a = voice.size() - 1;
                }
                else{
                    a = voice.find(' ');
                }
                if ((noteTmp = dynamic_cast<Note*>(ParseNote(voice.substr(0, a)))) == NULL)
                {
                    qDebug() << "erreur with Note in ParceVoice() <---- Parce.cpp";
                    return NULL;
                }
                else{
                    voiceTmp->addElement(noteTmp);
                    pos = 0;
                    _lastIsNote = true;
                    _lastNote = noteTmp;
                    if (voice.find(' ') != -1){
                        voice.erase(0, voice.find(' ') + 1);
                    }
                    else
                    {
                       voice.erase(0, voice.find(']'));
                    }
                }
            }
        }
        else if (voice.at(0) == ']'){
            break;
        }
    }
    return voiceTmp;
}

AMusicElement*  Parse::ParseTag(std::string voice)
{
    int pos = 1;
    Tag* tagTmp = new Tag();

    std::string string;
    std::string voiceTmp;

    bool lol = false; //a delete \beam( a

    if (voice.at(0) == '\\'){
        if (voice.find(' ') != -1){
            if (voice.at(voice.find(' ') - 1) == '('){
                voiceTmp = voice.substr(pos, count(voice, '(', voice.find('(')));
            }
            else
            {
                if (voice.find('<') < voice.find(' ')){
                    voiceTmp = voice.substr(pos, voice.find('>'));
                }
                else
                    voiceTmp = voice.substr(pos -1, voice.find(' '));
            }
        }
        //parse voiceTmp pour avoir le tag et le contenu
        if (voiceTmp.find("<") != -1){
            tagTmp->setName(voiceTmp.substr(0, voiceTmp.find("<")).c_str());
            string = voiceTmp.substr(voiceTmp.find("<") + 1, voiceTmp.find(">") - voiceTmp.find("<") - 1);

            string.erase(std::remove(string.begin(), string.end(), '\"'), string.end()); // il faut voir si il vaut pas mieux prendre a l'interieur plutot que de les enlever e.g. tempo

            tagTmp->addParam(string.c_str());
            if (voiceTmp.find('(') != -1){
                //ex: \tag<0>( a b)
                //parser l'interieur des parentheses
                //=========================================
                std::string tmpParenthese;

                tmpParenthese = voiceTmp.substr(voiceTmp.find('('), count(voiceTmp, '(', voiceTmp.find('(')));

                tagTmp->setHasRange(true);
                tagTmp->setMusicElements(ParseParenthese(tmpParenthese));

            }
        }
        else if (voiceTmp.find(':') != -1)
        {
            tagTmp->setName(voiceTmp.substr(0, voiceTmp.find("<")).c_str());
            if (voiceTmp.find(' ')){
                tagTmp->addParam(voiceTmp.substr(voiceTmp.find(':') + 1, voiceTmp.find(' ') - voiceTmp.find(':') ).c_str());
            }
            else
            {
                tagTmp->addParam(voiceTmp.substr(voiceTmp.find(':') + 1, voiceTmp.length() - 1 - voiceTmp.find(':') - 1 ).c_str());
            }
        }
        else if (voiceTmp.find("(") != -1 && voiceTmp.find("<") == -1){
            tagTmp->setName(voiceTmp.substr(0, voiceTmp.find("(")).c_str());
            string = voiceTmp.substr(voiceTmp.find("(") + 1, count(voiceTmp, '(', voiceTmp.find('(')) - voiceTmp.find("(") - 1);
            //ex: \tag( a b)
            //parser l'interieur des parentheses donc string
            //=============================================
            std::string tmpParenthese;

            tmpParenthese = voiceTmp.substr(voiceTmp.find('('), count(voiceTmp, '(', voiceTmp.find('(')));

            tagTmp->setHasRange(true);
            tagTmp->setMusicElements(ParseParenthese(tmpParenthese));
            lol = true;
        }
        else{
            if (voiceTmp.find(' ') != -1){
                tagTmp->setName(voiceTmp.substr(0, voiceTmp.find(" ")).c_str());
            }
            else
            {
                //tagTmp->setName(voiceTmp.substr(0, voiceTmp.length() - 1).c_str());
                tagTmp->setName(voiceTmp.substr(0, voiceTmp.length()).c_str());
            }
            lol = true;
        }
    }
    return tagTmp;
}

AMusicElement*  Parse::ParseNote(std::string voice){
    Note* note = new Note();
    int pos = 0;
    bool negatif = false;
    int tmp = 0;
    bool hasOctave = false;
    bool hasNumDuration = false;
    bool hasDenumDuration = false;

    if (voice.at(0) == 'a'){
        //note->setNoteName();
        // pas besoin, par defaut c'est a
    }
    else if (voice.at(0) == 'b'){
        note->setNoteName(Note::B);
    }
    else if (voice.at(0) == 'c'){
        note->setNoteName(Note::C);
    }
    else if (voice.at(0) == 'd'){
        note->setNoteName(Note::D);
    }
    else if (voice.at(0) == 'e'){
        note->setNoteName(Note::E);
    }
    else if (voice.at(0) == 'f'){
        note->setNoteName(Note::F);
    }
    else if (voice.at(0) == 'g'){
        note->setNoteName(Note::G);
    }
    if (pos < voice.length() - 1)
        pos++;
    if (voice.at(pos) == '#')
    {
        if (pos + 1 < voice.length() - 1){
            if (voice.at(pos + 1) == '#'){
                note->setAccidental(2);
                pos += 2;
            }
        }
        else{
            note->setAccidental(1);
            if (pos + 1 < voice.length() - 1)
                pos++;
        }
    }
    if (voice.at(pos) == '&')
    {
        if (pos + 1 < voice.length() - 1){
            if (voice.at(pos + 1) == '&'){
                note->setAccidental(-2);
                pos += 2;
            }
        }
        else{
            note->setAccidental(-1);
            if (pos + 1 < voice.length() - 1)
                pos++;
        }
    }
    if (voice.at(pos) == '-')
    {
        negatif = true;
    }
    if (voice.at(pos) >= 48 && voice.at(pos) <= 57){
        //choppe tous les chiffres pour faire un nombre et non juste un chiffre
        tmp = number(voice, pos);
        if (pos < voice.length() - 1){
            while (voice.at(pos) >= 48 && voice.at(pos) <= 57){
                pos++;
                if (pos >= voice.length() - 1){
                    break;
                }
            }
        }
        if (negatif == true)
            tmp = tmp * -1;
        note->setOctave(tmp);
        hasOctave = true;
    }

    while (true){
        if (voice.at(pos) == '*'){
            pos++;
            tmp = number(voice, pos);
            if (pos < voice.length() - 1){
                while (voice.at(pos) >= 48 && voice.at(pos) <= 57){
                    if (pos + 1 > voice.length() - 1){
                        break;
                    }
                    pos++;
                }
            }
            note->setNumDuration(tmp);
            hasNumDuration = true;
            hasDenumDuration = true;
        }
        else if (voice.at(pos) == '/'){
            pos++;
            tmp = number(voice, pos);

            if (pos < voice.length() - 1){
                while (voice.at(pos) >= 48 && voice.at(pos) <= 57){

                    if (pos + 1 > voice.length() - 1){
                        break;
                    }
                    pos++;
                }
            }
            note->setDenomDuration(tmp);
            hasDenumDuration = true;
            hasNumDuration = true;
        }
        else{
            break;
        }
    }
    if (pos <= voice.length() - 1)
    {
        if (voice.at(pos) == '.'){
            if (pos + 1 < voice.length()){
                if (voice.at(pos + 1) == '.'){
                 note->setDots(2);
                }
                else{
                    note->setDots(1);
                }
            }
            else{
                note->setDots(1);
            }
        }
    }
    if (hasOctave == false){
        if (_lastIsNote == true){
            if (_lastNote != NULL){
                note->setOctave(_lastNote->getOctave());
            }
        }
    }
    if (hasNumDuration == false){
        if (_lastIsNote == true){
            if (_lastNote != NULL){
                note->setNumDuration(_lastNote->getNumDuration());
            }
        }
        else{
            if (_lastRest != NULL){
                if (_lastRest != NULL){
                    note->setNumDuration(_lastRest->getNumDuration());
                }
            }
        }
    }
    if (hasDenumDuration == false){
        if (_lastIsNote == true){
            if (_lastNote != NULL){
                note->setDenomDuration(_lastNote->getDenomDuration());
            }
        }
        else{
            if (_lastRest != NULL){
                if (_lastRest != NULL){
                    note->setDenomDuration(_lastRest->getDenomDuration());
                }
            }
        }
    }
    return note;
}

AMusicElement*  Parse::ParseRest(std::string voice){
    Rest* rest = new Rest();
    int pos = 0;
    bool negatif = false;
    int tmp = 0;
    bool hasOctave = false;
    bool hasNumDuration = false;
    bool hasDenumDuration = false;

    if (pos < voice.length() - 1)
        pos++;
    
    while (true){
        if (voice.at(pos) == '*'){
            pos++;
            rest->setNumDuration((int)voice.at(pos));
            hasNumDuration = true;
            hasDenumDuration = true;
            if (pos < voice.length() - 1)
                pos++;
        }
        else if (voice.at(pos) == '/'){
            pos++;
            rest->setDenomDuration((int)voice.at(pos));
            hasDenumDuration = true;
            hasNumDuration = true;
            if (pos < voice.length() - 1)
                pos++;
        }
        else{
            break;
        }
    }
    if (pos <= voice.length() - 1)
    {
        if (voice.at(pos) == '.'){
            if (pos + 1 < voice.length()){
                if (voice.at(pos + 1) == '.'){
                 rest->setDots(2);
                }
                else{
                    rest->setDots(1);
                }
            }
            else{
                rest->setDots(1);
            }
        }
    }

    if (hasNumDuration == false){
        if (_lastIsNote == true){
            if (_lastNote != NULL){
                rest->setNumDuration(_lastNote->getNumDuration());
            }
        }
        else{
            if (_lastRest != NULL){
                if (_lastRest != NULL){
                    rest->setNumDuration(_lastRest->getNumDuration());
                }
            }
        }
    }
    if (hasDenumDuration == false){
        if (_lastIsNote == true){
            if (_lastNote != NULL){
                rest->setDenomDuration(_lastNote->getDenomDuration());
            }
        }
        else{
            if (_lastRest != NULL){
                if (_lastRest != NULL){
                    rest->setDenomDuration(_lastRest->getDenomDuration());
                }
            }
        }
    }
    return rest;
}

int Parse::count(std::string string, char delim, int pos) // const
{
    int i = pos + 1;
    int count = 1;

    if (delim == '('){
        while (count != 0){
            if (string.at(i) == '('){
                count++;
            }
            else if (string.at(i + 1) == ')'){
                count --;
            }
            i++;
        }
    }
    else if (delim == '{'){
        while (count != 0){
            if (string.at(i) == '{'){
                count++;
            }
            if (string.at(i + 1) == '}'){
                count --;
            }
            i++;
        }
    }
    return i;
}

QVector<AMusicElement*>*        Parse::ParseParenthese(std::string voice){
    QVector<AMusicElement*>*    musicElements = new QVector<AMusicElement*>();
    Voice*                      voiceTmp = new Voice();
    int i = 0;
    std::string tmpString;
    //ex:( a a a \tag(a a) a)

    voice[voice.find('(')] = '[';
    voice[count(voice, '(', voice.find('('))] = ']';
    //-------------------------------
    //build zone
    //probleme avec les tags dans les tags donc je les deletes
    int a = 0;
    //\tag( a a \tag( b b) c c \tag d d)
    while (a < voice.length()){
        if (voice.at(a) == '\\'){
            if (voice.at(voice.find(' ', a) - 1) == '(' ){
                voice.erase(a, count(voice, '(', voice.find('(', a)) + 1 - a);
            }
            else if (voice.find('<', a) < voice.find(' ', a))
            {
                voice.erase(a, voice.find('>', a) + 1 - a);
            }
            else
            {
                voice.erase(a, voice.find(' ', a) - a);
            }
        }
    a++;
    }

    //-------------------------------
    voiceTmp = ParseVoice(voice);    
    musicElements = voiceTmp->getMusicElements();



    while (i < musicElements->size() - 1){
        if (dynamic_cast<Tag*>(musicElements->at(i)) != NULL){
            tmpString = dynamic_cast<Tag*>(musicElements->at(i))->getName().toStdString();

            if (tmpString.find('\\') != -1){
                dynamic_cast<Tag*>(musicElements->at(i))->setName(tmpString.substr(1, tmpString.size()).c_str());
            }
        }
        i++;
    }
    return musicElements;
}

int                         Parse::number(std::string voice, int pos){
    std::string numTmp;
    int i = pos;
    int numReturn = 0;

    while (voice.at(i) >= 48 && voice.at(i) <= 57){
        i++;
        if (i > voice.length() - 1)
            break;
    }
    numTmp = voice.substr(pos, i - pos);
    numReturn = std::stoi(numTmp);

    return numReturn;
}

Score*  Parse::getScoreFromImport(){
    return _score;
}
