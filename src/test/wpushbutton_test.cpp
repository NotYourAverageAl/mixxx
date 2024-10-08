#include <gtest/gtest.h>

#include <QTestEventList>
#include <QScopedPointer>

#include "mixxxtest.h"
#include "control/controlobject.h"
#include "control/controlproxy.h"
#include "control/controlpushbutton.h"
#include "widget/wpushbutton.h"
#include "widget/controlwidgetconnection.h"

class WPushButtonTest : public MixxxTest {
  public:
    WPushButtonTest()
          : m_pGroup("[Channel1]") {
    }

  protected:
    void SetUp() override {
        m_pTouchShift.reset(new ControlPushButton(ConfigKey("[Controls]", "touch_shift")));
        m_pButton.reset(new WPushButton());
        m_pButton->setStates(2);
    }

    QScopedPointer<WPushButton> m_pButton;
    QScopedPointer<ControlPushButton> m_pTouchShift;
    QTestEventList m_Events;
    const char* m_pGroup;
};

TEST_F(WPushButtonTest, QuickPressNoLatchTest) {
    QScopedPointer<ControlPushButton> pPushControl(
        new ControlPushButton(ConfigKey("[Test]", "push")));
    pPushControl->setButtonMode(mixxx::control::ButtonMode::LongPressLatching);

    m_pButton.reset(new WPushButton(NULL,
            mixxx::control::ButtonMode::LongPressLatching,
            mixxx::control::ButtonMode::Push));
    m_pButton->setStates(2);
    m_pButton->addLeftConnection(
        new ControlParameterWidgetConnection(
            m_pButton.data(),
            pPushControl->getKey(), NULL,
            ControlParameterWidgetConnection::DIR_FROM_AND_TO_WIDGET,
            ControlParameterWidgetConnection::EMIT_ON_PRESS_AND_RELEASE));

    // This test can be flaky if the event simulator takes too long to deliver
    // the event.
    m_Events.addMousePress(Qt::LeftButton);
    m_Events.addDelay(100);
    m_Events.addMouseRelease(Qt::LeftButton);

    m_Events.simulate(m_pButton.data());

    ASSERT_EQ(0.0, m_pButton->getControlParameterLeft());
}

TEST_F(WPushButtonTest, LongPressLatchTest) {
    QScopedPointer<ControlPushButton> pPushControl(
        new ControlPushButton(ConfigKey("[Test]", "push")));
    pPushControl->setButtonMode(mixxx::control::ButtonMode::LongPressLatching);

    m_pButton.reset(new WPushButton(NULL,
            mixxx::control::ButtonMode::LongPressLatching,
            mixxx::control::ButtonMode::Push));
    m_pButton->setStates(2);
    m_pButton->addLeftConnection(
        new ControlParameterWidgetConnection(
            m_pButton.data(),
            pPushControl->getKey(), NULL,
            ControlParameterWidgetConnection::DIR_FROM_AND_TO_WIDGET,
            ControlParameterWidgetConnection::EMIT_ON_PRESS_AND_RELEASE));

    m_Events.addMousePress(Qt::LeftButton);
    m_Events.addDelay(1000);
    m_Events.addMouseRelease(Qt::LeftButton);

    m_Events.simulate(m_pButton.data());

    ASSERT_EQ(1.0, m_pButton->getControlParameterLeft());
}
