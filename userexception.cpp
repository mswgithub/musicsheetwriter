#include "userexception.h"

UserException::UserException(const char* message)
{
    _message = strdup(message);
}

const char* UserException::what() const throw()
  {
    return _message;
  }

QString UserException::getMessage() const{
    return _message;
}

UserException::~UserException(){
    delete _message;
}
