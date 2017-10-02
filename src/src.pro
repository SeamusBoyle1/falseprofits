TEMPLATE = subdirs

SUBDIRS += \
    FalseProfitsCore \
    FalseProfitsMobile \
    InvestorAPIClient

FalseProfitsMobile.depends += \
    FalseProfitsCore

FalseProfitsCore.depends += \
    InvestorAPIClient
