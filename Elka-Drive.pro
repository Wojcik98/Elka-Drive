TEMPLATE = subdirs

SUBDIRS += \
    Backend \
    Elka-Drive-Desktop \
    tests \
    qamqp/src

Backend.depends = qamqp/src
Elka-Drive-Desktop.depends = Backend
tests.depends = Backend
