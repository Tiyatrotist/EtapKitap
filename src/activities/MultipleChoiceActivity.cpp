#include "activities/MultipleChoiceActivity.h"
#include <QtCore/QJsonArray>

namespace EtapKitap {

MultipleChoiceActivity::MultipleChoiceActivity() = default;

QString MultipleChoiceActivity::id() const { return m_id; }
ActivityType MultipleChoiceActivity::type() const { return ActivityType::MultipleChoice; }
QString MultipleChoiceActivity::title() const { return m_title; }
QString MultipleChoiceActivity::questionText() const { return m_questionText; }
QString MultipleChoiceActivity::teacherHint() const { return m_teacherHint; }
QString MultipleChoiceActivity::solutionExplanation() const { return m_solutionExplanation; }

void MultipleChoiceActivity::reset() {
    m_selectedIndex = -1;
    m_validated = false;
}

bool MultipleChoiceActivity::isAnswered() const {
    return m_selectedIndex >= 0 && m_selectedIndex < m_options.size();
}

bool MultipleChoiceActivity::validate() {
    m_validated = true;
    return m_selectedIndex == m_correctIndex;
}

qreal MultipleChoiceActivity::score() const {
    if (!m_validated || m_correctIndex < 0) return 0.0;
    return (m_selectedIndex == m_correctIndex) ? 1.0 : 0.0;
}

QJsonObject MultipleChoiceActivity::serializeState() const {
    QJsonObject obj;
    obj[QStringLiteral("selectedIndex")] = m_selectedIndex;
    obj[QStringLiteral("validated")] = m_validated;
    return obj;
}

void MultipleChoiceActivity::restoreState(const QJsonObject& state) {
    m_selectedIndex = state.value(QStringLiteral("selectedIndex")).toInt(-1);
    m_validated = state.value(QStringLiteral("validated")).toBool(false);
}

void MultipleChoiceActivity::setId(const QString& id) { m_id = id; }
void MultipleChoiceActivity::setTitle(const QString& title) { m_title = title; }
void MultipleChoiceActivity::setQuestionText(const QString& text) { m_questionText = text; }
void MultipleChoiceActivity::setTeacherHint(const QString& hint) { m_teacherHint = hint; }
void MultipleChoiceActivity::setSolutionExplanation(const QString& sol) { m_solutionExplanation = sol; }

void MultipleChoiceActivity::setOptions(const QStringList& options) { m_options = options; }
const QStringList& MultipleChoiceActivity::options() const { return m_options; }

void MultipleChoiceActivity::setCorrectIndex(int idx) { m_correctIndex = idx; }
int MultipleChoiceActivity::correctIndex() const { return m_correctIndex; }

void MultipleChoiceActivity::selectOption(int idx) {
    if (idx >= 0 && idx < m_options.size()) {
        m_selectedIndex = idx;
    }
}
int MultipleChoiceActivity::selectedIndex() const { return m_selectedIndex; }

QJsonObject MultipleChoiceActivity::toJson() const {
    QJsonObject root;
    root[QStringLiteral("type")] = QStringLiteral("multiple_choice");
    root[QStringLiteral("id")] = m_id;
    root[QStringLiteral("title")] = m_title;
    root[QStringLiteral("question")] = m_questionText;
    root[QStringLiteral("hint")] = m_teacherHint;
    root[QStringLiteral("solution")] = m_solutionExplanation;
    root[QStringLiteral("correctIndex")] = m_correctIndex;

    QJsonArray optArr;
    for (const auto& opt : m_options) {
        optArr.append(opt);
    }
    root[QStringLiteral("options")] = optArr;
    return root;
}

std::unique_ptr<MultipleChoiceActivity> MultipleChoiceActivity::fromJson(const QJsonObject& json) {
    auto act = std::make_unique<MultipleChoiceActivity>();
    act->setId(json.value(QStringLiteral("id")).toString());
    act->setTitle(json.value(QStringLiteral("title")).toString());
    act->setQuestionText(json.value(QStringLiteral("question")).toString());
    act->setTeacherHint(json.value(QStringLiteral("hint")).toString());
    act->setSolutionExplanation(json.value(QStringLiteral("solution")).toString());
    act->setCorrectIndex(json.value(QStringLiteral("correctIndex")).toInt(-1));

    QStringList options;
    const QJsonArray arr = json.value(QStringLiteral("options")).toArray();
    for (const auto& v : arr) {
        options.append(v.toString());
    }
    act->setOptions(options);
    return act;
}

} // namespace EtapKitap
