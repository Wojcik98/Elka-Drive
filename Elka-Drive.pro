TEMPLATE = subdirs

SUBDIRS += \
    Backend \
    Elka-Drive-Desktop \
    ProcessResponse

Elka-Drive-Desktop.depends = Backend
ProcessResponse.depends = Backend
