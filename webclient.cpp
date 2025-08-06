// webclient.cpp
#include "webclient.h"
#include "backend.h"
#include <QDebug>
#include <QJsonDocument> // JSON 문서로 변환
#include <QJsonParseError> // JSON 파싱 에러 처리
#include <QUrl> // URL 생성
#include <QNetworkRequest> // 네트워크 요청 객체
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>
#include <QUrlQuery>

bool WebClient::initServer(){
    // QNetworkAccessManager 초기화
    networkManager = new QNetworkAccessManager(this);

    // networkManager의 finished 시그널을 onNetworkReplyFinished 슬롯에 연결
    // 이 시그널은 네트워크 요청이 완료될 때 (성공/실패 무관) 발생합니다
    // 네트워크 요청이 완료되었을때 안에서 어떤 명령이었는지 파싱하고 처리한다.
    connect(networkManager, &QNetworkAccessManager::finished, this, &WebClient::onNetworkReplyFinished);
    return true;
}

// "요청 보내기" 버튼 클릭 시 호출될 슬롯 -> 이게 선택문이네
void WebClient::RequestGet()
{
    qDebug()<<"request Get 잔고";
    QString uid = Backend::getInstance().getUID();
    QString name = Backend::getInstance().getName();

    // 요청할 URL 생성 (웹서버의 /api/users 경로)
    QUrl url(SERVER_GET_URL);
    QUrlQuery query;
    query.addQueryItem("uid",uid);
    query.addQueryItem("name",name);
    url.setQuery(query);

    QNetworkRequest request(url);
    qDebug()<<url;
    // GET 요청 보내기
    networkManager->get(request);
}

//하드코딩 되어있습니다. 현재시간, 센서타입 넣어줘야함.
void WebClient::RequestPost(int idx)
{
    qDebug()<<"request Post 잔고";
    // 요청할 URL 생성 (웹서버의 /api/users 경로)
    QUrl url(SERVER_POST_URL);
    QNetworkRequest request(url);

    auto now = QDateTime::currentDateTime().toString();

    QJsonObject obj;
    QJsonObject members;
    members["SensorType"] = idx;
    members["time"] = now;
    members["clientName"] = Backend::getInstance().getName();
    obj["data"] = members;
    QJsonDocument doc(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // Post 요청 보내기
    networkManager->post(request,postData);
}
//예금 출금 송금
void WebClient::RequestPut(long long amount,QString action,QString targetUID){
    qDebug()<<"request Put 잔고";
    QUrl url(SERVER_PUT_URL);
    QNetworkRequest request(url);

    QJsonObject obj;
    QJsonObject members;
    members["UID"] = Backend::getInstance().getUID();
    members["name"] = Backend::getInstance().getName();
    members["targetUID"] = targetUID;
    members["amount"] = QString::number(amount);
    members["action"] = action;
    obj["data"] = members;
    QJsonDocument doc(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // Post 요청 보내기
    networkManager->put(request,postData);
}

// 네트워크 요청 완료 시 호출될 슬롯
void WebClient::onNetworkReplyFinished(QNetworkReply *reply)
{
    // 1. 오류 확인
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Network error:" << reply->errorString();
        reply->deleteLater(); // 메모리 해제
        return;
    }

    // 2. 응답 데이터 읽기
    QByteArray responseData = reply->readAll();

    // 3. JSON 파싱
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);

    // 4. 데이터 처리.
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
    } else {
        if(reply->operation()==QNetworkAccessManager::GetOperation){
            qDebug()<<"get operation";
            if (jsonDoc.isObject()) {
                QJsonObject jsonObject = jsonDoc.object();
                if(jsonObject["data"].isObject()&&(!jsonObject.isEmpty())){
                    QJsonObject obj = jsonObject["data"].toObject();
                    //emit해서 rfid에 알려서 setBudget한후 넘어가야한다.
                    if(jsonObject["success"].toBool()){
                        Backend::getInstance().setBudget(obj["budget"].toVariant().toLongLong());
                        emit onGetSuccess();
                    } else {//실패시 처리
                        emit onFailure();
                    }
                }
            } else {
                qDebug("응답이 유효한 JSON 객체나 배열이 아닙니다.");
            }
        } else if(reply->operation()==QNetworkAccessManager::PostOperation){//로그 데이터 operation
            qDebug()<<"post operation";
            //qDebug() << "Client received JSON object:" << jsonObject;

        } else if(reply->operation()==QNetworkAccessManager::PutOperation){//수정 operation
            qDebug()<<"put operation";
            if (jsonDoc.isObject()) {
                QJsonObject jsonObject = jsonDoc.object();
                //put 성공 -> budget 최신화 get 불러야한다.
                if(jsonObject["success"].toBool()){
                    RequestGet();
                } else {//실패시 처리
                    emit onFailure();
                }
            } else {
                qDebug("응답이 유효한 JSON 객체나 배열이 아닙니다.");
            }
        }
    }

    reply->deleteLater();
}

