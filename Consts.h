#pragma once

namespace jd {
  enum class ShapeType {
    None = 0,
    Line = 1,
    Rect = 2,
    Circle = 3,
  };

  enum class ToolType {
    None = 0,
    CreateLine,
    CreateRect,
    CreateCircle,
    Move,
    Size,
    Color,
    NewFile,
    SaveFile,
    OpenFile
  };

  enum class Freedom {
    None = 0,
    AxisX,
    AxisY,
    Full
  };
}