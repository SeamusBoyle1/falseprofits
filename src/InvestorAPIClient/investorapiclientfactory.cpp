// Copyright 2017

#include "investorapiclient.h"

#include "requestqueue.h"

#include <QNetworkAccessManager>

namespace bsmi {

IInvestorAPIClient *InvestorAPIClientFactory::create(QObject *parent)
{
    auto nam = new QNetworkAccessManager;
    // investor-api service expects redirects available
    nam->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    auto requestQueue = new RequestQueue(nam);
    nam->setParent(requestQueue);

    return new InvestorAPIClient(requestQueue, QStringLiteral("https://investor-api.herokuapp.com"),
                                 parent);
}

} // namespace bsmi
