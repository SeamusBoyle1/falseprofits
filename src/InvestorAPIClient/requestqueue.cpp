// Copyright 2017

#include "requestqueue.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace bsmi {

RequestQueue::RequestQueue(QNetworkAccessManager *network, QObject *parent)
    : IRequestQueue(parent)
    , m_network{ network }
{
}

INetworkReply *RequestQueue::get(const QNetworkRequest &request)
{
    return new NetworkReply(m_network->get(request));
}

INetworkReply *RequestQueue::post(const QNetworkRequest &request, const QJsonObject &jsonObject)
{
    auto payload = QJsonDocument(jsonObject).toJson(QJsonDocument::Compact);
    return new NetworkReply(m_network->post(request, payload));
}

INetworkReply *RequestQueue::deleteResource(const QNetworkRequest &request)
{
    return new NetworkReply(m_network->deleteResource(request));
}

} // namespace bsmi
