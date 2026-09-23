#pragma once

#include "activities/IActivity.h"
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <memory>

namespace EtapKitap {

class ActivityViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isOpen READ isOpen WRITE setOpen NOTIFY openChanged)
    Q_PROPERTY(QString title READ title NOTIFY activityChanged)
    Q_PROPERTY(QString questionText READ questionText NOTIFY activityChanged)
    Q_PROPERTY(int activityType READ activityType NOTIFY activityChanged)
    Q_PROPERTY(QStringList options READ options NOTIFY activityChanged)
    Q_PROPERTY(int selectedIndex READ selectedIndex NOTIFY selectionChanged)
    Q_PROPERTY(bool isAnswered READ isAnswered NOTIFY stateChanged)
    Q_PROPERTY(bool isCorrect READ isCorrect NOTIFY stateChanged)
    Q_PROPERTY(bool showFeedback READ showFeedback NOTIFY stateChanged)
    Q_PROPERTY(bool showSolution READ showSolution NOTIFY stateChanged)
    Q_PROPERTY(QString solutionText READ solutionText NOTIFY stateChanged)
    Q_PROPERTY(QString hintText READ hintText NOTIFY stateChanged)
    Q_PROPERTY(qreal score READ score NOTIFY stateChanged)

public:
    explicit ActivityViewModel(QObject* parent = nullptr);
    ~ActivityViewModel() override = default;

    void setActivity(std::unique_ptr<IActivity> activity);

    [[nodiscard]] bool isOpen() const;
    void setOpen(bool open);

    [[nodiscard]] QString title() const;
    [[nodiscard]] QString questionText() const;
    [[nodiscard]] int activityType() const;
    [[nodiscard]] QStringList options() const;
    [[nodiscard]] int selectedIndex() const;

    [[nodiscard]] bool isAnswered() const;
    [[nodiscard]] bool isCorrect() const;
    [[nodiscard]] bool showFeedback() const;
    [[nodiscard]] bool showSolution() const;
    [[nodiscard]] QString solutionText() const;
    [[nodiscard]] QString hintText() const;
    [[nodiscard]] qreal score() const;

    Q_INVOKABLE void selectOption(int idx);
    Q_INVOKABLE void submitAnswer();
    Q_INVOKABLE void revealSolution();
    Q_INVOKABLE void revealHint();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void close();

signals:
    void openChanged(bool open);
    void activityChanged();
    void selectionChanged(int idx);
    void stateChanged();

private:
    std::unique_ptr<IActivity> m_activity;
    bool m_isOpen{false};
    bool m_showFeedback{false};
    bool m_showSolution{false};
    bool m_showHint{false};
    bool m_isCorrect{false};
};

} // namespace EtapKitap
