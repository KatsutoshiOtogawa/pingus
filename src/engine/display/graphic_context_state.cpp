// Pingus - A free Lemmings clone
// Copyright (C) 2004 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/display/graphic_context_state.hpp"

#include "engine/display/display.hpp"

namespace pingus {

class GraphicContextStateImpl
{
public:
  Rect rect;
  geom::ioffset offset;

  bool have_limit;
  Rect limit;

  GraphicContextStateImpl() :
    rect(),
    offset(),
    have_limit(),
    limit()
  {}
};

GraphicContextState::GraphicContextState()
  : impl(new GraphicContextStateImpl())
{
  impl->rect       = Rect(geom::ipoint(0,0), Size(Display::get_width(), Display::get_height()));
  impl->offset     = geom::ioffset(0,0);
  impl->have_limit = false;
}

GraphicContextState::GraphicContextState(int w, int h)
  : impl(new GraphicContextStateImpl())
{
  impl->rect       = Rect(geom::ipoint(0,0), Size(w, h));
  impl->offset     = geom::ioffset(0,0);
  impl->have_limit = false;
}

GraphicContextState::GraphicContextState(const Rect& rect)
  : impl(new GraphicContextStateImpl())
{
  impl->rect       = rect;
  impl->offset     = geom::ioffset(0,0);
  impl->have_limit = false;
}

void
GraphicContextState::set_limit(const Rect& limit)
{
  impl->have_limit = true;
  impl->limit      = limit;
}

void
GraphicContextState::set_unlimited()
{
  impl->have_limit = false;
}

void
GraphicContextState::set_size(int w, int h)
{
  impl->rect = Rect(geom::ipoint(impl->rect.left(), impl->rect.top()), Size(w, h));
}

void
GraphicContextState::push(DrawingContext& gc)
{
  gc.push_modelview();

  gc.translate(impl->rect.left(), impl->rect.top());
  gc.translate(impl->offset.x(),  impl->offset.y());
}

void
GraphicContextState::push(SceneContext& gc)
{
  gc.push_modelview();

  gc.translate(impl->rect.left(), impl->rect.top());
  gc.translate(impl->offset.x(),  impl->offset.y());
}

void
GraphicContextState::pop (SceneContext& gc)
{
  gc.pop_modelview();
}

void
GraphicContextState::pop (DrawingContext& gc)
{
  gc.pop_modelview();
}

Rect
GraphicContextState::get_clip_rect()
{
  return Rect(geom::ipoint(0, 0) - impl->offset, impl->rect.size());
}

void
GraphicContextState::set_pos(const Vector2i& pos)
{
  impl->offset = geom::ioffset(-pos.x() + (get_width()/2),
                               -pos.y() + (get_height()/2));

  if (impl->have_limit)
  {
    if (-impl->offset.x() < impl->limit.left())
    {
      impl->offset = geom::ioffset(-(impl->limit.left()),
                                   impl->offset.y());
    }
    else if (-impl->offset.x() + get_width() > impl->limit.right())
    {
      if (impl->limit.right() - impl->limit.left() > get_width())
        impl->offset = geom::ioffset(-(impl->limit.right() - get_width()),
                                     impl->offset.y());
      else
        impl->offset = geom::ioffset(-(impl->limit.left()),
                                     impl->offset.y());
    }

    if (-impl->offset.y() < impl->limit.top())
    {
      impl->offset = geom::ioffset(impl->offset.x(),
                                   -(impl->limit.top()));
    }
    else if (-impl->offset.y() + get_height() > impl->limit.bottom())
    {
      if (impl->limit.bottom() - impl->limit.top() > get_height()) {
        impl->offset = geom::ioffset(impl->offset.x(),
                                     -(impl->limit.bottom() - get_height()));
      } else {
        impl->offset = geom::ioffset(impl->offset.x(),
                                     -(impl->limit.top()));
      }
    }
  }
}

Vector2i
GraphicContextState::get_pos() const
{
  return Vector2i(-impl->offset.x() + (get_width()/2),
                  -impl->offset.y() + (get_height()/2));
}

Vector2i
GraphicContextState::screen2world(const Vector2i& pos_) const
{
  Vector2i pos(pos_.x() - impl->rect.left(),
               pos_.y() - impl->rect.top());

  return pos
    - geom::ioffset(impl->rect.left(), impl->rect.top())
    - impl->offset;
}

int
GraphicContextState::get_width()  const
{
  return impl->rect.width();
}

int
GraphicContextState::get_height() const
{
  return impl->rect.height();
}

} // namespace pingus

/* EOF */
