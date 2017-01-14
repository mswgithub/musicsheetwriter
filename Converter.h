#ifndef CONVERT_H
#define CONVERTER_H
#include <Windows.h>
#include <string>


/**
 * @brief The Converter class
 */
class MSWConverter{
    std::string guido2midiProgram;
public :
    MSWConverter();
    void guido2midi(std::string &guidoFile);
};

#endif
