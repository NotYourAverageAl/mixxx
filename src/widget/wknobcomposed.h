#pragma once

#include <optional>

#include "skin/legacy/skincontext.h"
#include "widget/knobeventhandler.h"
#include "widget/wpixmapstore.h"
#include "widget/wwidget.h"

// This is used for knobs, if the knob value can be displayed
// by rotating a single SVG image.
// For more complex transitions you may consider to use
// WEffectParameterKnob, which displays one of e.g. 64
// pixmaps.
class WKnobComposed : public WWidget {
    Q_OBJECT
  public:
    explicit WKnobComposed(QWidget* pParent=nullptr);

    virtual void setup(const QDomNode& node, const SkinContext& context);

    void onConnectedControlChanged(double dParameter, double dValue) override;

  protected:
    void wheelEvent(QWheelEvent *e) override;
    void leaveEvent(QEvent* e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void resizeEvent(QResizeEvent* /*unused*/) override;
    void paintEvent(QPaintEvent* /*unused*/) override;

    std::optional<double> m_defaultAngle;
    void setDefaultAngleFromParameterOrReset(std::optional<double> parameter);

  private:
    void inputActivity();
    void clear();
    void setPixmapBackground(
            const PixmapSource& source,
            Paintable::DrawMode mode,
            double scaleFactor);
    void setPixmapKnob(
            const PixmapSource& source,
            Paintable::DrawMode mode,
            double scaleFactor);
    void drawArc(QPainter* pPainter);

    PaintablePointer m_pKnob;
    PaintablePointer m_pPixmapBack;
    KnobEventHandler<WKnobComposed> m_handler;
    double m_dCurrentAngle;
    double m_dMinAngle;
    double m_dMaxAngle;
    double m_dKnobCenterXOffset;
    double m_dKnobCenterYOffset;
    double m_dArcRadius;
    double m_dArcThickness;
    double m_dArcBgThickness;
    QColor m_arcColor;
    QColor m_arcBgColor;
    bool m_arcUnipolar;
    bool m_arcReversed;
    Qt::PenCapStyle m_arcPenCap;
    QRectF m_rect;

    friend class KnobEventHandler<WKnobComposed>;
};
