#include "SpeekerBase.h"
#include <QGraphicsScene>
#include <QTextEdit>
#include <QLineEdit>

class MvScene : public QGraphicsScene
{
    Q_OBJECT
    public: MvScene(QObject *parent = 0);

    public slots:
                  void InitializeBase();
                  void InitializeSentenses();
                  void InitializeKeyWords();
                  void InitializeEducation();
                  void InitializeGetFromUser();
                  void InitializeSimpleWords();

                  QString Education();

                  void btnClicked();
                  void GetAnswer(QString);
                  void RememberUnnownSentences(QString);
                  void InsertNewFromUser(QString);

                  QString SearchFromUserBase(QString);
                  QString SearchInKeyWords(QString);
                  QList<QString> SearchInAll(QString);

    public: enum EducationStatus
    {
        Trained,
        WaitAnswer,
        Free
    };

    public: Speekerbase speekerBase;

            int answerCount;

            EducationStatus educationStatus;
            QString educationQuestion;

            QString UserName,
                    BotName;

            QTextEdit * textEdit;
            QLineEdit * lineEdit;
};

