#ifndef USEREXCEPTION_H
#define USEREXCEPTION_H

#include <exception>
#include <QString>
#include <string.h>

using namespace std;

class UserException : public exception
{
public:
    UserException(const char*);
    ~UserException();
    virtual const char* what() const throw();
    QString UserException::getMessage() const;

private:
    char *_message;
};

#endif // USEREXCEPTION_H
