#pragma once

enum class ShapeType {
  None = 0,
  Line = 1,
  Rect = 2,
  Circle = 3,
};

enum class ToolCmd {
  None = 0,
  CreateShape,
  Move,
  Size
};