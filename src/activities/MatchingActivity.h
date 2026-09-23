#pragma once

#include "activities/IActivity.h"
#include <QtCore/QStringList>
#include <QtCore/QMap>

namespace EtapKitap {

struct MatchingPair {
    QString left;
    QString right;
};

class MatchingActivity : public IActivity {
public:
    MatchingActivity();
    ~MatchingActivity() override = default;

    [[nodiscard]] QString id() const override;
    [[nodiscard]] ActivityType type() const override;
    [[nodiscard]] QString title() const override;
    [[nodiscard]] QString questionText() const override;
    [[nodiscard]] QString teacherHint() const override;
    [[nodiscard]] QString solutionExplanation() const override;

    void reset() override;
    [[nodiscard]] bool isAnswered() const override;
    bool validate() override;
    [[nodiscard]] qreal score() const override;

    [[nodiscard]] QJsonObject serializeState() const override;
    void restoreState(const QJsonObject& state) override;
    [[nodiscard]] QJsonObject toJson() const override;
    static std::unique_ptr<MatchingActivity> fromJson(const QJsonObject& json);

    void setId(const QString& id);
    void setTitle(const QString& title);
    void setQuestionText(const QString& text);
    void setTeacherHint(const QString& hint);
    void setSolutionExplanation(const QString& sol);

    void addPair(const QString& left, const QString& right);
    [[nodiscard]] QStringList leftItems() const;
    [[nodiscard]] QStringList rightItems() const;

    void match(int leftIndex, int rightIndex);
    [[nodiscard]] int matchedRightForLeft(int leftIndex) const;

private:
    QString m_id;
    QString m_title;
    QString m_questionText;
    QString m_teacherHint;
    QString m_solutionExplanation;
    QVector<MatchingPair> m_pairs;
    QStringList m_shuffledRight;
    QMap<int, int> m_userMatches; // leftIndex -> rightIndex in m_shuffledRight
    bool m_validated{false};
};

} // namespace EtapKitap
