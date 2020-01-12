TEMPLATE = subdirs

SUBDIRS += \
    Mocks \
    TestModel \

TestModel.depends = Mocks
