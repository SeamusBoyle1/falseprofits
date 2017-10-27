TEMPLATE = subdirs

SUBDIRS += \
    src \
    tests

OTHER_FILES += \
    README.md \
    $$files(dist/changes-*.md) \
    .appveyor.yml \
    .travis.yml
