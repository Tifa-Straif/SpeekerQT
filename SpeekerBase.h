#include <QtCore>
#include "QASentences.h"
#include "Dictionary.h"
#include "Relation.h"

class Speekerbase
{
    public:
    QList<QString> SimpleWords;
    QList<QString> UnnownSentences;
    QList<QASentences> GetFromUser;
    QList<QASentences> KeyWords;
    QList<Dictionary> Questions;
    QList<Dictionary> Answers;
    QList<Relation> Relations;
};
