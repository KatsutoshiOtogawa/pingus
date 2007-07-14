//  $Id: editor_viewport.cxx,v 1.00 2005/11/09 23:41:12 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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
#include <iostream>
#include "../gui/gui_manager.hpp"
#include "../gui/display.hpp"
#include "../display/drawing_context.hpp"
#include "../math/vector3f.hpp"
#include "../graphic_context_state.hpp"
#include "editor_viewport.hpp"
#include "editor_screen.hpp"
#include "context_menu.hpp"
#include "xml_level.hpp"
#include "level_objs.hpp"

namespace Editor {

// Constructor
EditorViewport::EditorViewport(EditorScreen* e) :
	state(Display::get_width(), Display::get_height()),
	scene_context(new SceneContext()),
	editor(e),
	autoscroll(true),
	highlighted_area(0,0,0,0),
	context_menu(0),
	snap_to(false),
	current_action(NOTHING)
{
	// FIXME: Hardcoded values should be determined by level size
	state.set_limit(Rect(-30, -50, 1600, 1300));
	state.set_pos(Vector2f(0, 0));
}

// Destructor
EditorViewport::~EditorViewport ()
{
	delete scene_context;
}

// When someone right-clicks inside the viewport
void
EditorViewport::on_secondary_button_click(int x, int y)
{
	remove_context_menu();

	Vector3f mouse_pos(x - (state.get_width()/2 - state.get_pos().x),
		y - (state.get_height()/2 - state.get_pos().y));

	std::cout << "Right-click at " << mouse_pos.x << ", " 
		<< mouse_pos.y << std::endl;
	
//	LevelObj* obj = object_at((int)mouse_pos.x, (int)mouse_pos.y);
	if (!current_objs.empty())
	{
	//	std::vector<LevelObj*> objs;
	//	objs.push_back(obj);
		context_menu = new ContextMenu(current_objs, Vector3f((float)x, (float)y), this);
		editor->get_gui_manager()->add(context_menu, true);
	}
}

// Select 1 or more LevelObjs, or drag them.
void 
EditorViewport::on_primary_button_press(int x, int y)
{
	remove_context_menu();

	if (current_action == NOTHING)
	{
		LevelObj* obj = object_at(x - (state.get_width()/2 - (int)state.get_pos().x),
			y - (state.get_height()/2 - (int)state.get_pos().y));

		if (obj)
		{
			// If the currently selected object isn't selected, select it and deselect the rest
			if (!obj->is_selected())
			{
				for (unsigned i = 0; i < current_objs.size(); i++)
					current_objs[i]->unselect();
				current_objs.clear();
				obj->select();
				current_objs.push_back(obj);
			}
			// Allow dragging of the currently selected objects
			current_action = DRAGGING;
			drag_start_pos = mouse_at_world;
		}
		else
		{
			current_objs.clear();
			current_action = HIGHLIGHTING;
			highlighted_area.left = highlighted_area.right = x;
			highlighted_area.top = highlighted_area.bottom = y;
		}
	}
}


void 
EditorViewport::on_primary_button_release(int x, int y)
{
	if (current_action == HIGHLIGHTING)
	{
		// Make sure CL_Rect starts at the top left
		if (highlighted_area.right < highlighted_area.left)
			std::swap(highlighted_area.left, highlighted_area.right);
		if (highlighted_area.bottom < highlighted_area.top)
			std::swap(highlighted_area.top, highlighted_area.bottom);

		for (unsigned i = 0; i < objs.size(); i++)
		{
			// Calculate the object's position
			Vector2i obj_pos((int)objs[i]->get_pos().x + (state.get_width()/2 - 
				(int)state.get_pos().x), (int)objs[i]->get_pos().y + (state.get_height()/2 - 
				(int)state.get_pos().y));
			
			if (highlighted_area.is_inside(obj_pos))
			{
				current_objs.push_back(objs[i]);
				objs[i]->select();
			}
			else
				objs[i]->unselect();
		}
	}
	else if (current_action == DRAGGING)
	{
		// Set the objects' positions for good
		for (unsigned i = 0; i < objs.size(); i++)
			objs[i]->set_orig_pos(objs[i]->get_pos());
	}
	current_action = NOTHING;
}

void
EditorViewport::on_pointer_move(int x, int y)
{
	mouse_at = Vector3f(float(x), float(y));
	mouse_at_world = Vector3f(x - (state.get_width()/2 - state.get_pos().x),
		y - (state.get_height()/2 - state.get_pos().y));

	if (current_action == HIGHLIGHTING)
	{
		highlighted_area.right = x;
		highlighted_area.bottom = y;
	}
	else if (current_action == DRAGGING)
	{
		float new_x, new_y;

		for (unsigned i = 0; i < current_objs.size(); i++)
		{
			Vector3f orig_pos(current_objs[i]->get_orig_pos());
			float x_offset = mouse_at_world.x - drag_start_pos.x;
			float y_offset = mouse_at_world.y - drag_start_pos.y;
			if (snap_to)
			{
				// FIXME: May need to adjust the snap-to offset here.
				new_x = (float)((int)((x_offset + orig_pos.x) / 10) * 10);
				new_y = (float)((int)((y_offset + orig_pos.y) / 10) * 10);
			}
			else
			{
				new_x = x_offset + orig_pos.x;
				new_y = y_offset + orig_pos.y;
			}
			current_objs[i]->set_pos(Vector3f(new_x, new_y, orig_pos.z));
		}
	}
}


// Draws all of the objects in the viewport and the background (if any)
void
EditorViewport::draw(DrawingContext &gc)
{
	scene_context->clear();
	state.push(*scene_context);
	
	// Now, draw all of the objects

	// Draw the level objects
	for (unsigned i = 0; i < objs.size(); i++)
		objs[i]->draw(scene_context->color());

	if (current_action == HIGHLIGHTING)
		gc.draw_rect((float)highlighted_area.left, (float)highlighted_area.top, 
			(float)highlighted_area.right, (float)highlighted_area.bottom, 
			Color(255,255,255,150));

	state.pop(*scene_context);
	gc.draw(new SceneContextDrawingRequest(scene_context, Vector3f(0, 0, -5000)));
}

// Returns true if the viewport is at the x,y coordinate
bool
EditorViewport::is_at(int x, int y)
{
	// FIXME: Should return true everywhere except for on the panel
	return true;
}

void
EditorViewport::update(float delta)
{
	UNUSED_ARG(delta);

	// Autoscroll if necessary
	if (autoscroll)
	{
		const int autoscroll_border = 10;
		if (autoscroll)
		{
			if (mouse_at.x < autoscroll_border)
				state.set_pos(state.get_pos() - Vector2f(5, 0));
			else if ((float)Display::get_width() - mouse_at.x < autoscroll_border)
				state.set_pos(state.get_pos() + Vector2f(5, 0));
			else if (mouse_at.y < autoscroll_border)
				state.set_pos(state.get_pos() - Vector2f(0, 5));
			else if ((float)Display::get_height() - mouse_at.y < autoscroll_border)
				state.set_pos(state.get_pos() + Vector2f(0, 5));
		}
	}
}

LevelObj*
EditorViewport::object_at (int x, int y)
{
	// we travel reversly through the object list, so that we get the
	// top-most object
	for (std::vector<LevelObj*>::reverse_iterator i = objs.rbegin ();
		i != objs.rend (); ++i)
	{
		if ((*i)->is_at (x, y))
			return *i;
	}
	return 0;
}

// Delete the context menu if it exists.
void
EditorViewport::remove_context_menu()
{
	if (context_menu)
	{
		editor->get_gui_manager()->remove(context_menu);
		context_menu->display(false);
		context_menu = 0;
	}
}

void
EditorViewport::refresh()
{
	objs = editor->get_level()->get_objects();
}

void 
EditorViewport::add_object(LevelObj* obj)
{
	objs.push_back(obj);
}

} // Editor namespace

/* EOF */
