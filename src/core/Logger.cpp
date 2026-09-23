#include "core/Logger.h"
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <iostream>

namespace EtapKitap {

Q_LOGGING_CATEGORY(logCore, "etapkitap.core")
Q_LOGGING_CATEGORY(logPdf, "etapkitap.pdf")
Q_LOGGING_CATEGORY(logBook, "etapkitap.book")
Q_LOGGING_CATEGORY(logStorage, "etapkitap.storage")
Q_LOGGING_CATEGORY(logTools, "etapkitap.tools")
Q_LOGGING_CATEGORY(logUi, "etapkitap.ui")

void initializeLogging(bool verbose) {
    if (verbose) {
        QLoggingCategory::setFilterRules(QStringLiteral("etapkitap.*.debug=true"));
    } else {
        QLoggingCategory::setFilterRules(QStringLiteral("etapkitap.*.debug=false\netapkitap.*.info=true"));
    }
}

} // namespace EtapKitap
