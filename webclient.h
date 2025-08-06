// webclient.h
#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include <QObject>
#include <QNetworkAccessManager> // 네트워크 요청을 보낼 때 사용
#include <QNetworkReply>         // 네트워크 응답을 받을 때 사용
#include <QJsonObject>           // JSON 데이터 처리

class WebClient : public QObject
{
    Q_OBJECT
public:
    static WebClient &getInstance(){
        static WebClient s;
        return s;
    }

    bool initServer();
    void RequestGet();  // "Get 요청 보내기"
    void RequestPost(int idx); // "post 보내기" 버튼
    void RequestPut(long long amount,QString action,QString targetUID=QString()); // "예금" "출금" "송금"

signals:
    //restful 성공시
    void onGetSuccess();
    void onPutSuccess();
    void onPostSuccess();

    //restful실패시 -> 그냥 없던일 처리하기.
    void onFailure();

private slots:
    void onNetworkReplyFinished(QNetworkReply *reply); // 네트워크 요청 완료 시 호출될 슬롯


private:
    WebClient(QObject *parent = nullptr) {}
    ~WebClient(){}
    WebClient &operator=(const WebClient &ref){}
    QNetworkAccessManager *networkManager; // 네트워크 요청을 관리하는 객체
    const QString SERVER_GET_URL = "http://192.168.2.18:8081/api/atm"; // 웹서버의 API 엔드포인트
    const QString SERVER_PUT_URL = "http://192.168.2.18:8081/api/atm";     // 예금 출금 송금시 필요함.
    const QString SERVER_POST_URL = "http://192.168.2.18:8081/api/atm"; // 센서 로그 올리기
    // 참고: 웹서버의 포트가 다르면 여기를 수정하세요 (예: 8080)
};
#endif // WEBCLIENT_H
