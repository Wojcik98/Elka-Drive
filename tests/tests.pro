TEMPLATE = subdirs

SUBDIRS += \
    Mocks \
    TestModel \
    TestUser

TestModel.depends = Mocks
TestUser.depends = Mocks
