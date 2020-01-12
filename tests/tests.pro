TEMPLATE = subdirs

SUBDIRS += \
    Mocks \
    TestModel \
    TestUser \
    TestUploadItem

TestModel.depends = Mocks
TestUser.depends = Mocks
TestUploadItem.depends = Mocks
