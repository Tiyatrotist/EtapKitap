#pragma once

#include "document/IDocument.h"
#include "book/BookPackage.h"
#include "book/InteractionRegistry.h"
#include "pdf/IPdfBackend.h"
#include <memory>

namespace EtapKitap {

class InteractiveBook : public IDocument {
public:
    explicit InteractiveBook(std::unique_ptr<BookPackage> package, IPdfBackend* backend);
    ~InteractiveBook() override = default;

    [[nodiscard]] QString id() const override;
    [[nodiscard]] QString title() const override;
    [[nodiscard]] QString filePath() const override;
    [[nodiscard]] int pageCount() const override;
    [[nodiscard]] QSizeF pageSize(int pageNumber) const override;
    [[nodiscard]] bool isInteractiveBook() const override;
    [[nodiscard]] QString pageText(int pageNumber) const override;

    [[nodiscard]] BookPackage* package() const;
    [[nodiscard]] InteractionRegistry* interactions() const;

private:
    std::unique_ptr<BookPackage> m_package;
    IPdfBackend* m_backend{nullptr};
    std::unique_ptr<InteractionRegistry> m_interactions;
};

} // namespace EtapKitap
