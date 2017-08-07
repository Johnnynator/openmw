#ifndef DATAFOLDERSPAGE_H
#define DATAFOLDERSPAGE_H

#include "ui_datafolderspage.h"
#include <QWidget>


#include <QDir>
#include <QFile>

class QSortFilterProxyModel;
class QAbstractItemModel;
class QMenu;

namespace Files { struct ConfigurationManager; }
namespace ContentSelectorView { class ContentSelector; }
namespace Config { class GameSettings;
                   class LauncherSettings; }

namespace Launcher
{
    class TextInputDialog;
    class ProfilesComboBox;

    class DataFoldersPage : public QWidget
    {
        Q_OBJECT

        ContentSelectorView::ContentSelector *mSelector;
        Ui::DataFoldersPage ui;

    public:
        explicit DataFoldersPage (Files::ConfigurationManager &cfg, Config::GameSettings &gameSettings,
                                Config::LauncherSettings &launcherSettings, QWidget *parent = 0);

        QAbstractItemModel* profilesModel() const;

        int profilesIndex() const;

        //void writeConfig(QString profile = QString());
        void saveSettings(const QString &profile = "");
        bool loadSettings();

    signals:
        void signalProfileChanged (int index);

    public slots:
        void slotProfileChanged (int index);

    private slots:

        void slotProfileChangedByUser(const QString &previous, const QString &current);
        void slotProfileRenamed(const QString &previous, const QString &current);
        void slotProfileDeleted(const QString &item);

        void updateOkButton(const QString &text);

        void on_newProfileAction_triggered();
        void on_deleteProfileAction_triggered();

    public:
        /// Content List that is always present
        const static char *mDefaultContentListName;

    private:

        TextInputDialog *mProfileDialog;

        Files::ConfigurationManager &mCfgMgr;

        Config::GameSettings &mGameSettings;
        Config::LauncherSettings &mLauncherSettings;

        QString mPreviousProfile;

        QString mDataLocal;

        void setFolderCheckstates(Qt::CheckState state);

        void buildView();
        void setupConfig();
        void readConfig();
        void setProfile (int index, bool savePrevious);
        void setProfile (const QString &previous, const QString &current, bool savePrevious);
        void removeProfile (const QString &profile);
        bool showDeleteMessageBox (const QString &text);
        void addProfile (const QString &profile, bool setAsCurrent);
        void checkForDefaultProfile();
        void populateFileViews(const QString& contentModelName);

        QStringList filesInProfile(const QString& profileName);
    };
}
#endif
