#ifndef MOCKVIEW_H
#define MOCKVIEW_H

#include "include/view.h"

class MockView : public View {
public:
    MockView();
    void showLogoutMsg() override;
    void showForbiddenMsg() override;
    void showDirectoryNotFound() override;
    void showUnknownErrorMsg() override;
    void showFileOpenError(const QString &filename) override;
    QString getNewGroupName(bool *ok) override;
    QString getNewFolderName(bool *ok) override;
    QStringList getUploadFileNames() override;
    QString getUploadFolderName() override;
    QString getSaveFilename(const QString &suggested) override;

private:
    void settingsButtonClicked() override;
    void downloadButtonClicked()  override;
    void deleteButtonClicked()  override;
};

#endif // MOCKVIEW_H
