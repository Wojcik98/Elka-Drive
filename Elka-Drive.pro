TEMPLATE = subdirs

SUBDIRS += \
    Backend \
    Elka-Drive-Desktop \
    ProcessResponse \
    qamqp/src

Backend.depends = qamqp/src
Elka-Drive-Desktop.depends = Backend
ProcessResponse.depends = Backend
