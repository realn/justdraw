#pragma once

namespace jd {
  enum class ShapeType {
    None = 0,
    Line = 1,
    Rect = 2,
    Circle = 3,
    Bezier = 4,
  };

  enum class ToolType {
    None = 0,
    CreateLine,
    CreateRect,
    CreateCircle,
    CreateBezier,
    Move,
    Size,
    Color,
    NewFile,
    SaveFile,
    OpenFile,
    FilterMedium,
    FilterMedian,
    FilterEdge,
    FilterMax,
    FilterMin
  };

  enum class Freedom {
    None = 0,
    AxisX,
    AxisY,
    Full
  };
}