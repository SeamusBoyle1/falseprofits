// Copyright 2017

#include "fptradingaccounts.h"

#include "fpaccountslistmodel.h"
#include "fpcore.h"

#include <InvestorAPIClient/jsonobjectwrappers.h>

#include <QJsonDocument>
#include <QJsonObject>

FpTradingAccounts::FpTradingAccounts(QObject *parent)
    : QObject(parent)
    , m_model{ new FpAccountsListModel(this) }
{
}

void FpTradingAccounts::updateAccounts()
{
    Q_ASSERT(m_fpCore);

    auto resp = m_fpCore->getUserProfile();
    connect(resp, &GetUserProfileResponse::finished, this, [resp, this] {
        onResponseReceived(resp, true);
        resp->deleteLater();
    });
}

void FpTradingAccounts::onResponseReceived(GetUserProfileResponse *reply, bool updateNotReplace)
{
    if (!reply->hasError()) {
        auto doc = QJsonDocument::fromJson(reply->payload());
        bsmi::JsonObjectWrappers::UserProfileResponse responseObj;
        responseObj.d = doc.object();
        auto jsonAccounts = responseObj.accounts();

        QVector<TradingAccountItem> accounts;
        accounts.reserve(jsonAccounts.size());
        for (auto i = 0, total = jsonAccounts.size(); i < total; ++i) {
            auto const e = jsonAccounts.at(i);

            TradingAccountItem r;
            auto id = e.id();
            if (!id) {
                continue;
            }
            r.id = *id;
            auto name = e.name();
            if (name) {
                r.name = *name;
            }
            auto balance = e.balance();
            if (balance) {
                r.balance = *balance;
            }

            accounts.append(r);
        }

        if (updateNotReplace) {
            m_model->updateOrRemoveData(accounts);
        } else {
            m_model->resetWithData(accounts);
        }
    } else {
        // TODO(seamus): Handle error case
    }
}
