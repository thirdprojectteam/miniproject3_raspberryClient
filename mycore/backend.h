#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

class Backend:public QObject
{
    Q_OBJECT
public:
    static Backend& getInstance(){
        static Backend instance;
        return instance;
    }
    //getter setter
    QString getUID(){return m_UID;}
    QString getName(){return m_name;}
    //AGE 추가
    QString getAge(){return m_age;}
    long long getBudget(){return m_budget;}
    void setUID(QString UID){ m_UID = UID;}
    void setName(QString name){ m_name = name;}
    void setBudget(long long budget){m_budget = budget;}
    void setAge(QString age){m_age = age;}

    //next UI;
    int nxtUI = 0;
private:
    Backend(QObject* parent = nullptr) {}
    Backend(const Backend&) {}
    Backend& operator = (const Backend&) {}
    ~Backend() {}
    QString m_UID;
    QString m_name;
    long long m_budget;
    QString m_age="unknown";
};

#endif // BACKEND_H
