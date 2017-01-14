#include "AEditionManager.h"

AEditionManager::AEditionManager(QObject* parent) : QObject(parent)
{
    _score = NULL;
}

AEditionManager::~AEditionManager()
{

}

Score*  AEditionManager::getScore() const
{
    return _score;
}

void    AEditionManager::setScore(Score* score)
{
    _score = score;
}
