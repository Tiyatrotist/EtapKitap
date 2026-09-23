#pragma once

#include <QtSql/QSqlDatabase>

namespace EtapKitap {

class Migrations {
public:
    static constexpr int CURRENT_SCHEMA_VERSION = 1;

    static bool apply(QSqlDatabase& db);

private:
    static int getCurrentVersion(QSqlDatabase& db);
    static bool applyVersion1(QSqlDatabase& db);
};

} // namespace EtapKitap
