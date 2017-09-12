// Copyright 2017

#ifndef BSMI_REQUESTQUEUE_H
#define BSMI_REQUESTQUEUE_H

#include "iinvestorapiclient.h"

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

namespace bsmi {

class RequestQueue : public IRequestQueue
{
    Q_OBJECT
public:
    explicit RequestQueue(QNetworkAccessManager *network, QObject *parent = nullptr);

    QNetworkReply *get(const QNetworkRequest &request) override;
    QNetworkReply *post(const QNetworkRequest &request, const QJsonObject &jsonObject) override;
    QNetworkReply *deleteResource(const QNetworkRequest &request) override;

signals:

public slots:

private:
    QNetworkAccessManager *m_network;
};

} // namespace bsmi

#endif // BSMI_REQUESTQUEUE_H
