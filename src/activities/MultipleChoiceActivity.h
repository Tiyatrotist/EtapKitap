#pragma once

#include "activities/IActivity.h"
#include <QtCore/QStringList>

namespace EtapKitap {

class MultipleChoiceActivity : public IActivity {
public:
    MultipleChoiceActivity();
    ~MultipleChoiceActivity() override = default;

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
    static std::unique_ptr<MultipleChoiceActivity> fromJson(const QJsonObject& json);

    void setId(const QString& id);
    void setTitle(const QString& title);
    void setQuestionText(const QString& text);
    void setTeacherHint(const QString& hint);
    void setSolutionExplanation(const QString& sol);

    void setOptions(const QStringList& options);
    [[nodiscard]] const QStringList& options() const;

    void setCorrectIndex(int idx);
    [[nodiscard]] int correctIndex() const;

    void selectOption(int idx);
    [[nodiscard]] int selectedIndex() const;

private:
    QString m_id;
    QString m_title;
    QString m_questionText;
    QString m_teacherHint;
    QString m_solutionExplanation;
    QStringList m_options;
    int m_correctIndex{-1};
    int m_selectedIndex{-1};
    bool m_validated{false};
};

} // namespace EtapKitap
