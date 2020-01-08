TEMPLATE = subdirs

SUBDIRS += \
    Backend \
    Elka-Drive-Desktop \
    ProcessResponse \
    qamqp

Backend.depends = qamqp
Elka-Drive-Desktop.depends = Backend
ProcessResponse.depends = Backend
