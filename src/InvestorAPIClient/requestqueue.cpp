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

QNetworkReply *RequestQueue::get(const QNetworkRequest &request)
{
    return m_network->get(request);
}

QNetworkReply *RequestQueue::post(const QNetworkRequest &request, const QJsonObject &jsonObject)
{
    auto payload = QJsonDocument(jsonObject).toJson(QJsonDocument::Compact);
    return m_network->post(request, payload);
}

QNetworkReply *RequestQueue::deleteResource(const QNetworkRequest &request)
{
    return m_network->deleteResource(request);
}

} // namespace bsmi
