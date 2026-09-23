#pragma once

#include <QtCore/QLoggingCategory>

namespace EtapKitap {

Q_DECLARE_LOGGING_CATEGORY(logCore)
Q_DECLARE_LOGGING_CATEGORY(logPdf)
Q_DECLARE_LOGGING_CATEGORY(logBook)
Q_DECLARE_LOGGING_CATEGORY(logStorage)
Q_DECLARE_LOGGING_CATEGORY(logTools)
Q_DECLARE_LOGGING_CATEGORY(logUi)

/**
 * @brief Initialize logging configuration for EtapKitap.
 * Configures filters, formatting, and file-based rotation if requested.
 */
void initializeLogging(bool verbose = false);

} // namespace EtapKitap
