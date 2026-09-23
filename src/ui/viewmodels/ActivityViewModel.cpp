#include "ui/viewmodels/ActivityViewModel.h"
#include "activities/MultipleChoiceActivity.h"

namespace EtapKitap {

ActivityViewModel::ActivityViewModel(QObject* parent)
    : QObject(parent)
{
}

void ActivityViewModel::setActivity(std::unique_ptr<IActivity> activity) {
    m_activity = std::move(activity);
    m_showFeedback = false;
    m_showSolution = false;
    m_showHint = false;
    m_isCorrect = false;

    if (m_activity) {
        m_isOpen = true;
        emit openChanged(true);
    }
    emit activityChanged();
    emit stateChanged();
}

bool ActivityViewModel::isOpen() const { return m_isOpen; }
void ActivityViewModel::setOpen(bool open) {
    if (m_isOpen != open) {
        m_isOpen = open;
        emit openChanged(m_isOpen);
    }
}

QString ActivityViewModel::title() const {
    return m_activity ? m_activity->title() : QString();
}

QString ActivityViewModel::questionText() const {
    return m_activity ? m_activity->questionText() : QString();
}

int ActivityViewModel::activityType() const {
    return m_activity ? static_cast<int>(m_activity->type()) : 0;
}

QStringList ActivityViewModel::options() const {
    if (auto mc = dynamic_cast<MultipleChoiceActivity*>(m_activity.get())) {
        return mc->options();
    }
    return QStringList();
}

int ActivityViewModel::selectedIndex() const {
    if (auto mc = dynamic_cast<MultipleChoiceActivity*>(m_activity.get())) {
        return mc->selectedIndex();
    }
    return -1;
}

bool ActivityViewModel::isAnswered() const {
    return m_activity ? m_activity->isAnswered() : false;
}

bool ActivityViewModel::isCorrect() const {
    return m_isCorrect;
}

bool ActivityViewModel::showFeedback() const {
    return m_showFeedback;
}

bool ActivityViewModel::showSolution() const {
    return m_showSolution;
}

QString ActivityViewModel::solutionText() const {
    return (m_activity && m_showSolution) ? m_activity->solutionExplanation() : QString();
}

QString ActivityViewModel::hintText() const {
    return (m_activity && m_showHint) ? m_activity->teacherHint() : QString();
}

qreal ActivityViewModel::score() const {
    return m_activity ? m_activity->score() : 0.0;
}

void ActivityViewModel::selectOption(int idx) {
    if (auto mc = dynamic_cast<MultipleChoiceActivity*>(m_activity.get())) {
        mc->selectOption(idx);
        emit selectionChanged(idx);
        emit stateChanged();
    }
}

void ActivityViewModel::submitAnswer() {
    if (m_activity) {
        m_isCorrect = m_activity->validate();
        m_showFeedback = true;
        emit stateChanged();
    }
}

void ActivityViewModel::revealSolution() {
    m_showSolution = true;
    emit stateChanged();
}

void ActivityViewModel::revealHint() {
    m_showHint = true;
    emit stateChanged();
}

void ActivityViewModel::reset() {
    if (m_activity) {
        m_activity->reset();
        m_showFeedback = false;
        m_showSolution = false;
        m_showHint = false;
        m_isCorrect = false;
        emit selectionChanged(-1);
        emit stateChanged();
    }
}

void ActivityViewModel::close() {
    setOpen(false);
}

} // namespace EtapKitap
