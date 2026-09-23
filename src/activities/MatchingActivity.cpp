#include "activities/MatchingActivity.h"
#include <QtCore/QJsonArray>
#include <algorithm>
#include <random>

namespace EtapKitap {

MatchingActivity::MatchingActivity() = default;

QString MatchingActivity::id() const { return m_id; }
ActivityType MatchingActivity::type() const { return ActivityType::Matching; }
QString MatchingActivity::title() const { return m_title; }
QString MatchingActivity::questionText() const { return m_questionText; }
QString MatchingActivity::teacherHint() const { return m_teacherHint; }
QString MatchingActivity::solutionExplanation() const { return m_solutionExplanation; }

void MatchingActivity::reset() {
    m_userMatches.clear();
    m_validated = false;
}

bool MatchingActivity::isAnswered() const {
    return m_userMatches.size() == m_pairs.size();
}

bool MatchingActivity::validate() {
    m_validated = true;
    if (m_pairs.isEmpty()) return true;

    int correct = 0;
    for (int i = 0; i < m_pairs.size(); ++i) {
        int rightIdx = m_userMatches.value(i, -1);
        if (rightIdx >= 0 && rightIdx < m_shuffledRight.size()) {
            if (m_shuffledRight[rightIdx] == m_pairs[i].right) {
                correct++;
            }
        }
    }
    return correct == m_pairs.size();
}

qreal MatchingActivity::score() const {
    if (!m_validated || m_pairs.isEmpty()) return 0.0;
    int correct = 0;
    for (int i = 0; i < m_pairs.size(); ++i) {
        int rightIdx = m_userMatches.value(i, -1);
        if (rightIdx >= 0 && rightIdx < m_shuffledRight.size()) {
            if (m_shuffledRight[rightIdx] == m_pairs[i].right) {
                correct++;
            }
        }
    }
    return static_cast<qreal>(correct) / static_cast<qreal>(m_pairs.size());
}

QJsonObject MatchingActivity::serializeState() const {
    QJsonObject obj;
    QJsonObject matchObj;
    for (auto it = m_userMatches.begin(); it != m_userMatches.end(); ++it) {
        matchObj[QString::number(it.key())] = it.value();
    }
    obj[QStringLiteral("matches")] = matchObj;
    obj[QStringLiteral("validated")] = m_validated;
    return obj;
}

void MatchingActivity::restoreState(const QJsonObject& state) {
    m_userMatches.clear();
    QJsonObject matchObj = state.value(QStringLiteral("matches")).toObject();
    for (auto it = matchObj.begin(); it != matchObj.end(); ++it) {
        m_userMatches[it.key().toInt()] = it.value().toInt();
    }
    m_validated = state.value(QStringLiteral("validated")).toBool(false);
}

void MatchingActivity::setId(const QString& id) { m_id = id; }
void MatchingActivity::setTitle(const QString& title) { m_title = title; }
void MatchingActivity::setQuestionText(const QString& text) { m_questionText = text; }
void MatchingActivity::setTeacherHint(const QString& hint) { m_teacherHint = hint; }
void MatchingActivity::setSolutionExplanation(const QString& sol) { m_solutionExplanation = sol; }

void MatchingActivity::addPair(const QString& left, const QString& right) {
    m_pairs.append(MatchingPair{left, right});
    m_shuffledRight.append(right);
}

QStringList MatchingActivity::leftItems() const {
    QStringList list;
    for (const auto& p : m_pairs) {
        list.append(p.left);
    }
    return list;
}

QStringList MatchingActivity::rightItems() const {
    return m_shuffledRight;
}

void MatchingActivity::match(int leftIndex, int rightIndex) {
    if (leftIndex >= 0 && leftIndex < m_pairs.size()) {
        m_userMatches[leftIndex] = rightIndex;
    }
}

int MatchingActivity::matchedRightForLeft(int leftIndex) const {
    return m_userMatches.value(leftIndex, -1);
}

QJsonObject MatchingActivity::toJson() const {
    QJsonObject root;
    root[QStringLiteral("type")] = QStringLiteral("matching");
    root[QStringLiteral("id")] = m_id;
    root[QStringLiteral("title")] = m_title;
    root[QStringLiteral("question")] = m_questionText;
    root[QStringLiteral("hint")] = m_teacherHint;
    root[QStringLiteral("solution")] = m_solutionExplanation;

    QJsonArray pairsArr;
    for (const auto& p : m_pairs) {
        QJsonObject o;
        o[QStringLiteral("left")] = p.left;
        o[QStringLiteral("right")] = p.right;
        pairsArr.append(o);
    }
    root[QStringLiteral("pairs")] = pairsArr;
    return root;
}

std::unique_ptr<MatchingActivity> MatchingActivity::fromJson(const QJsonObject& json) {
    auto act = std::make_unique<MatchingActivity>();
    act->setId(json.value(QStringLiteral("id")).toString());
    act->setTitle(json.value(QStringLiteral("title")).toString());
    act->setQuestionText(json.value(QStringLiteral("question")).toString());
    act->setTeacherHint(json.value(QStringLiteral("hint")).toString());
    act->setSolutionExplanation(json.value(QStringLiteral("solution")).toString());

    const QJsonArray arr = json.value(QStringLiteral("pairs")).toArray();
    for (const auto& val : arr) {
        if (val.isObject()) {
            QJsonObject o = val.toObject();
            act->addPair(o.value(QStringLiteral("left")).toString(), o.value(QStringLiteral("right")).toString());
        }
    }
    return act;
}

} // namespace EtapKitap
