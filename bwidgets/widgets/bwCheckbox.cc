#include "bwEvent.h"
#include "bwPainter.h"
#include "bwStyle.h"

#include "bwCheckbox.h"

namespace bWidgets {

bwCheckbox::bwCheckbox(const std::string& text, unsigned int width_hint, unsigned int height_hint)
    : bwAbstractButton(text, Type::CHECKBOX, "bwCheckbox", width_hint, height_hint)
{
}

void bwCheckbox::draw(bwStyle& style)
{
  style.setWidgetStyle(*this);

  const bwRectanglePixel checkbox_rect = getCheckboxRectangle();
  const bwRectanglePixel text_rect = getTextRectangle(checkbox_rect);
  const bwGradient gradient{
      base_style.backgroundColor(), base_style.shadeTop(), base_style.shadeBottom()};
  bwPainter painter;

  painter.drawRoundboxWidgetBase(
      base_style, style, checkbox_rect, gradient, base_style.corner_radius);

  if (isChecked()) {
    painter.active_drawtype = bwPainter::DrawType::OUTLINE;
    painter.setActiveColor(base_style.decorationColor());
    painter.drawCheckMark(checkbox_rect);
  }

  painter.setContentMask(text_rect);  // Not sure if we should set this here.
  painter.setActiveColor(base_style.textColor());
  painter.drawText(text, text_rect, base_style.text_alignment);
}

bool bwCheckbox::isChecked() const
{
  return state == State::SUNKEN;
}

bwRectanglePixel bwCheckbox::getCheckboxRectangle() const
{
  bwRectanglePixel checkbox_rect{rectangle};
  const int delta = 1 + checkbox_rect.height() / 8;

  checkbox_rect.xmax = checkbox_rect.xmin + checkbox_rect.height();
  checkbox_rect.resize(-delta);

  return checkbox_rect;
}

bwRectanglePixel bwCheckbox::getTextRectangle(const bwRectanglePixel& checkbox_rectangle) const
{
  bwRectanglePixel text_rect{rectangle};
  text_rect.xmin = checkbox_rectangle.xmax - 1;  // XXX -1 is ugly. Specifically for demo app.
  return text_rect;
}

// ------------------ Handling ------------------

class bwCheckboxHandler : public bwAbstractButtonHandler {
 public:
  bwCheckboxHandler(bwCheckbox& checkbox);
  ~bwCheckboxHandler() = default;

  void onMousePress(bwMouseButtonEvent&) override;
  void onMouseRelease(bwMouseButtonEvent&) override;
};

bwPtr<bwScreenGraph::EventHandler> bwCheckbox::createHandler()
{
  return bwPtr_new<bwCheckboxHandler>(*this);
}

bwCheckboxHandler::bwCheckboxHandler(bwCheckbox& checkbox) : bwAbstractButtonHandler(checkbox)
{
}

void bwCheckboxHandler::onMousePress(bwMouseButtonEvent& event)
{
  if (event.button == bwMouseButtonEvent::BUTTON_LEFT) {
    button.state = (button.state == bwWidget::State::SUNKEN) ? bwWidget::State::HIGHLIGHTED :
                                                               bwWidget::State::SUNKEN;
    apply();
    event.swallow();
  }
}

void bwCheckboxHandler::onMouseRelease(bwMouseButtonEvent& event)
{
  event.swallow();

  // Don't change state at all.
}

}  // namespace bWidgets
