//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <vector>
#include <string>
#include "resource.hpp"
#include "combobox.hpp"
#include "math.hpp"
#include "../fonts.hpp"
#include "../math/vector2i.hpp"

namespace Editor {

Combobox::Combobox(const Rect& rect)
  : RectComponent(rect),
    sprite(Resource::load_sprite("core/editor/combobox")),
    current_item(-1),
    drop_down(false)
{
}

Combobox::~Combobox ()
{
  clear();
}

void
Combobox::add(int id, const std::string& str)
{
  item_list.push_back(ComboItem(id, str));
}

// Remove all items from this combobox
void
Combobox::clear()
{
  item_list.clear();
  current_item = -1;
  drop_down    = false;
}

// Returns whether or not the combobox is at this location
bool
Combobox::is_at(int x, int y)
{
  if (drop_down)
    return Rect(Vector2i(rect.left, rect.top + get_box_offset()),
                Size(rect.get_width(), rect.get_height() * (item_list.size()))).is_inside(Vector2i(x,y));
  else
    return RectComponent::is_at(x,y);
}

void
Combobox::on_primary_button_press(int x, int y)
{
  if (drop_down)
    {
      // Determine which item was selected, if any, and set the current item to it.
      drop_down = false;
      
      if (hover_item != -1)
        {
          current_item = hover_item;
          on_select(item_list[current_item]);
        }
    }
  else
    {
      drop_down = true;
      on_pointer_move(x,y);
    }
}

void
Combobox::draw(DrawingContext &gc)
{
  if (drop_down && !item_list.empty())
    {
      int y_offset = get_box_offset();

      Rect list_rect(Vector2i(rect.left, 
                              rect.top + y_offset),
                     Size(rect.get_width(),
                          item_list.size() * rect.get_height()));


      gc.draw_fillrect(list_rect, Color(255,255,255), 90);

      for (int i = 0; i < int(item_list.size()); ++i)
        {
          if (i == hover_item)
            gc.draw_fillrect(Rect(Vector2i(rect.left, list_rect.top + rect.get_height()*i),
                                  Size(rect.get_width(), rect.get_height())),
                             Color(150,200,255), 95);

          gc.print_left(Fonts::verdana11, 
                        list_rect.left + 5, 
                        list_rect.top + i * rect.get_height() + rect.get_height()/2 - Fonts::verdana11.get_height()/2,
                        item_list[i].label, 100);
        }

      gc.draw_rect(list_rect, Color(0,0,0), 100);
    }
  else
    {
      gc.draw_fillrect(rect, Color(255,255,255));
      gc.draw(sprite, Vector2i(rect.right - 12, rect.top));
      gc.draw_rect(rect, Color(0,0,0));

      if (current_item != -1)
        {
          gc.print_left(Fonts::verdana11, rect.left + 5, 
                        rect.top + rect.get_height()/2 - Fonts::verdana11.get_height()/2,
                        item_list[current_item].label);
        }
    }
}

int
Combobox::get_box_offset()
{
  return -(rect.get_height() * current_item);
}

bool
Combobox::set_selected_item(int id)
{
  for(int i = 0; i < int(item_list.size()); ++i)
    if (item_list[i].id == id)
      {
        current_item = i;
        return true;
      }
  return false;  
}

void
Combobox::on_pointer_move(int x, int y) 
{
  if (drop_down)
    {
      hover_item = (y - rect.top - get_box_offset()) / rect.get_height();
      if (hover_item < 0 || hover_item >= int(item_list.size()))
        hover_item = -1;
    }
}

} // namespace Editor 

/* EOF */
