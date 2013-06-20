#include "mvscene.h"
#include <cstdlib>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QtCore>
#include <QPalette>
#include <QPushButton>
#include <QProgressDialog>
#include <QColor>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlDriverCreator>
#include <QtSql/QSqlDriverCreatorBase>
#include <QtSql/QSqlDriverPlugin>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlField>
#include <QtSql/QSqlIndex>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRelationalTableModel>
#include <QMessageBox>

MvScene::MvScene(QObject *parent) : QGraphicsScene(parent)
{
    QBrush Brush;
    Brush.setColor(QColor(150,100,150));
    Brush.setStyle(Qt::DiagCrossPattern);
    setBackgroundBrush(Brush);

    QFont font;
    font.setPixelSize(16);
    lineEdit = new QLineEdit();
    QGraphicsProxyWidget * item = addWidget(lineEdit);
    lineEdit->setGeometry(-380, 300, 400, 35);
    lineEdit->setFont(font);
    item = addWidget(lineEdit);
    lineEdit->show();

    textEdit = new QTextEdit();
    item = addWidget(textEdit);
    textEdit->setGeometry(-380, 0, 400, 300);\
    textEdit->setFont(font);
    item = addWidget(textEdit);
    textEdit->show();

    QPushButton * button = new QPushButton(trUtf8("Speek!"), 0);
    item = addWidget(button);
    QTransform transform = item->transform();
    transform.translate(-500., 200.);
    transform.rotate(45.0);
    transform.scale(2., 2.);
    item->setTransform(transform);
    item = addWidget(button);
    button->show();

    connect(button,SIGNAL(clicked()),this,SLOT(btnClicked()));

    educationStatus = Free;

    UserName = "Master";
    BotName = "Slave";

    InitializeBase();
}

void MvScene::btnClicked()
{
    QString text;
    text = lineEdit->text();

    GetAnswer(text);
}

void MvScene::InitializeBase()
{
    InitializeSentenses();
    InitializeKeyWords();
    InitializeEducation();
    InitializeGetFromUser();
    InitializeSimpleWords();
}

void MvScene::InitializeSentenses()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.SDB");
    db.open();

    QSqlQuery query;
    query.exec("SELECT * FROM Answers");

    while(query.next())
    {
        Dictionary answer;

        answer.Key = query.value(0).toInt();
        answer.String = query.value(1).toString();

        speekerBase.Answers.append(answer);
    }

    QSqlQuery query1;
    query1.exec("SELECT * FROM Questions");

    while(query1.next())
    {
        Dictionary question;

        question.Key = query1.value(0).toInt();
        question.String = query1.value(1).toString();

        speekerBase.Questions.append(question);
    }

    QSqlQuery query2;
    query2.exec("SELECT * FROM Relations");

    while(query2.next())
    {
        Relation relation;

        relation.QIndex = query2.value(0).toInt();
        relation.AIndex = query2.value(1).toInt();

        speekerBase.Relations.append(relation);
    }

    db.close();
}

void MvScene::InitializeKeyWords()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.SDB");
    db.open();

    QSqlQuery query;
    query.exec("SELECT * FROM KeyWords");

    while(query.next())
    {
        QASentences sentences;

        sentences.KeyWord = query.value(0).toString();
        sentences.Answer = query.value(1).toString();

        speekerBase.KeyWords.append(sentences);
    }

    db.close();
}

void MvScene::InitializeEducation()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.SDB");
    db.open();

    QSqlQuery query;
    query.exec("SELECT * FROM Education");

    while(query.next())
    {
        speekerBase.UnnownSentences.append(query.value(0).toString());
    }

    db.close();
}

void MvScene::InitializeGetFromUser()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.SDB");
    db.open();

    QSqlQuery query;
    query.exec("SELECT * FROM GetFromUser");

    while(query.next())
    {
        QASentences sentences;

        sentences.KeyWord = query.value(0).toString();
        sentences.Answer = query.value(1).toString();

        speekerBase.GetFromUser.append(sentences);
    }

    db.close();
}

void MvScene::InitializeSimpleWords()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.SDB");
    db.open();

    QSqlQuery query;
    query.exec("SELECT * FROM SimpleWords");

    while(query.next())
    {
        speekerBase.SimpleWords.append(query.value(0).toString());
    }

    db.close();
}

void MvScene::InsertNewFromUser(QString userText)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.SDB");
    db.open();

    QSqlQuery query;

    QString text;
    text = "INSERT INTO GetFromUser VALUES("
            + educationQuestion + ", "
            + userText + ")";

    query.exec("INSERT INTO GetFromUser VALUES('"
               + educationQuestion + "', '"
               + userText + "')");

    db.close();
}

QString MvScene::SearchFromUserBase(QString userText)
{
    foreach(QASentences sentences, speekerBase.GetFromUser)
    {
        if (sentences.KeyWord == userText)
            return sentences.Answer;
    }

    return QString();
}

void MvScene::RememberUnnownSentences(QString userText)
{
    speekerBase.UnnownSentences.append(userText);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.SDB");
    db.open();

    QSqlQuery query;

    query.exec("INSERT INTO Education VALUES('" + userText + "')");

    db.close();
}

QString MvScene::Education()
{
    QString educationSentences;

    if (speekerBase.UnnownSentences.count() != 0 && educationStatus == Free)
    {
        if ((answerCount % 5 == 0) || (answerCount % 9 == 0))
        {
            int index = qrand() % speekerBase.UnnownSentences.count();

            educationSentences = " Я тут подумал... "
                    + speekerBase.UnnownSentences[index];

            educationQuestion = speekerBase.UnnownSentences[index];

            educationStatus = WaitAnswer;

            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName("database.SDB");
            db.open();

            QSqlQuery query;

            int deleteIndex;
            deleteIndex = index + 1;

            query.exec("DELETE FROM Education WHERE rowid='" + (QString)deleteIndex + "'");
            db.close();

            speekerBase.UnnownSentences.removeAt(index);
        }
    }

    return educationSentences;
}

QList<QString> MvScene::SearchInAll(QString userText)
{
    int qIndex;

    foreach(Dictionary question, speekerBase.Questions)
    {
        if (question.String == userText)
            qIndex = question.Key;
    }

    if (qIndex == 0) return QList<QString>();

    QList<int> aIndexes;

    for (int i = 0; i < speekerBase.Relations.count(); i++)
    {
        if (speekerBase.Relations[i].QIndex == qIndex)
            aIndexes.append(speekerBase.Relations[i].AIndex);
    }

    QList<QString> result;

    foreach (Dictionary answer, speekerBase.Answers)
    {
        if (aIndexes.contains(answer.Key))
            result.append(answer.String);
    }

    return result;
}

QString MvScene::SearchInKeyWords(QString userText)
{
    QList<QString> keyWordsAnswers;

    QStringList words = userText.split(' ');

    foreach (QString word, words)
    {
        foreach(QASentences sentence, speekerBase.KeyWords)
        {
            if (word == sentence.KeyWord)
                keyWordsAnswers.append(sentence.Answer);
        }
    }

    if (keyWordsAnswers.count() > 0)
    {
        int index = qrand() % keyWordsAnswers.count();

        return keyWordsAnswers[index];
    }

    return QString();
}

void MvScene::GetAnswer(QString userText)
{
    if (educationStatus == WaitAnswer)
    {
        InsertNewFromUser(userText);
    }

    answerCount++;

    QList<QString> answers;

    QString answer;

    answers = SearchInAll(userText);

    if (answers.isEmpty())
    {
        answer = SearchFromUserBase(userText);

        if (answer.isEmpty())
            answer = SearchInKeyWords(userText);
    }
    else
    {
        int index = qrand() % answers.count();

        answer = answers[index];
    }

    if (answer.isEmpty())
    {
        if (educationStatus == Free)
            RememberUnnownSentences(userText);

        int index = qrand() % speekerBase.SimpleWords.count();

        answer = speekerBase.SimpleWords[index];
    }

    QString result;
    result = UserName + ": " + userText + "\n" + BotName + ": " + answer + Education() + "\n";

    textEdit->append(result);

    if (educationStatus == Trained)
    {
        educationStatus = Free;
    }

    lineEdit->clear();
}


