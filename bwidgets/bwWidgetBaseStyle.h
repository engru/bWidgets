#pragma once

#include "bwColor.h"

namespace bWidgets {

enum class TextAlignment;

/**
 * \class bwWidgetBaseStyle
 * \brief Properties for typical widgets with roundbox corners.
 */
class bwWidgetBaseStyle {
 public:
  bwWidgetBaseStyle();

  void registerProperties(class bwStyleProperties& style_properties);

  const bwColor& backgroundColor() const;
  const bwColor& textColor() const;
  const bwColor& borderColor() const;
  const bwColor& decorationColor() const;
  float shadeTop() const;
  float shadeBottom() const;

  bool isBorderVisible() const;

  bwColor background_color;
  bwColor text_color;
  bwColor border_color;
  bwColor decoration_color;

  int shade_top{0},
      shade_bottom{
          0};  // TODO could make this a char - or support gradient for background_color even.
  enum TextAlignment text_alignment;  // Default value set in constructor.
  unsigned int roundbox_corners;      // Default value set in constructor.
  float corner_radius{
      0.0f};  // TODO Utility function for calculating final radius (roundbox_radius * dpi_fac).
};

}  // namespace bWidgets
