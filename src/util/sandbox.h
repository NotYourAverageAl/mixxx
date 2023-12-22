#pragma once

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QSharedPointer>

#include "preferences/configobject.h"
#include "util/compatibility/qmutex.h"
#include "util/fileinfo.h"

#ifdef Q_OS_MAC
#include <CoreFoundation/CFURL.h>
#endif

struct SandboxSecurityToken {
    ~SandboxSecurityToken();
    QString m_path;
#ifdef Q_OS_MAC
    SandboxSecurityToken(const QString& path, CFURLRef url);
    CFURLRef m_url;
#endif
};

// Reference counted pointer to SandboxSecurityToken.
typedef QSharedPointer<SandboxSecurityToken> SecurityTokenPointer;
typedef QWeakPointer<SandboxSecurityToken> SecurityTokenWeakPointer;

class Sandbox {
  public:
    static void checkSandboxed();
    static void setPermissionsFilePath(const QString& permissionsFile);
    static void shutdown();

#ifdef __APPLE__
    static QString migrateOldSettings();
#endif

    // Returns true if we are in a sandbox.
    static bool enabled() {
        return s_bInSandbox;
    }

    // Prompt the user to give us access to the path with an open-file dialog.
    static bool askForAccess(mixxx::FileInfo* pFileInfo);

    static bool canAccess(mixxx::FileInfo* pFileInfo);
    static bool canAccessDir(const QDir& dir);

    static bool createSecurityToken(mixxx::FileInfo* pFileInfo);
    static bool createSecurityTokenForDir(const QDir& dir) {
        return createSecurityToken(dir.canonicalPath(), true);
    }

#if defined(__GNUC__) && __cplusplus < 201907L
#define RATIONALE
#else
#define RATIONALE                                                           \
    ("A new security token should be assigned to a variable, otherwise it " \
     "will be invalidated immediately.")
#endif

    [[nodiscard RATIONALE]] static SecurityTokenPointer openSecurityToken(
            mixxx::FileInfo* pFileInfo, bool create);
    [[nodiscard RATIONALE]] static SecurityTokenPointer openSecurityTokenForDir(
            const QDir& dir, bool create);

  private:
    Sandbox() = delete;

    static ConfigKey keyForCanonicalPath(const QString& canonicalPath);

    // Must hold s_mutex to call this.
    static SecurityTokenPointer openTokenFromBookmark(const QString& canonicalPath,
                                                      const QString& bookmarkBase64);

    // Creates a security token. s_mutex is not needed for this method.
    static bool createSecurityToken(const QString& canonicalPath, bool isDirectory);

    static QT_RECURSIVE_MUTEX s_mutex;
    static bool s_bInSandbox;
    static QSharedPointer<ConfigObject<ConfigValue>> s_pSandboxPermissions;
    static QHash<QString, SecurityTokenWeakPointer> s_activeTokens;
};
