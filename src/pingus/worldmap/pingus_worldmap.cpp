// Pingus - A free Lemmings clone
// Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/worldmap/pingus_worldmap.hpp"

#include <stdexcept>

#include "util/pathname.hpp"
#include "util/raise_exception.hpp"

using namespace WorldmapNS;

class PingusWorldmapImpl
{
public:
  ReaderDocument doc;
  std::string filename;

  std::string name;
  std::string short_name;
  std::string description;
  std::string music;
  std::string author;
  std::string email;
  int width;
  int height;

  std::string default_node;
  std::string final_node;

  ReaderDocument intro_story;
  ReaderDocument end_story;
  ReaderMapping path_graph;

  ReaderCollection objects;

  PingusWorldmapImpl() :
    doc(),
    filename(),
    name(),
    short_name(),
    description(),
    music(),
    author(),
    email(),
    width(),
    height(),
    default_node(),
    final_node(),
    intro_story(),
    end_story(),
    path_graph(),
    objects()
  {}
};

PingusWorldmap::PingusWorldmap() :
  impl()
{
}

PingusWorldmap::PingusWorldmap(const Pathname& pathname) :
  impl(new PingusWorldmapImpl())
{
  impl->doc = ReaderDocument::from_file(pathname.get_sys_path());
  parse_file(impl->doc.get_root());
}

void
PingusWorldmap::parse_file(const ReaderObject& reader_object)
{
  if (reader_object.get_name() != "pingus-worldmap")
  {
    raise_exception(std::runtime_error, "Worldmap:" << impl->filename << ": not a Worldmap file");
  }
  else
  {
    ReaderMapping reader = reader_object.get_mapping();

    if (!reader.read("graph", impl->path_graph))
    {
      raise_exception(std::runtime_error, "Worldmap: " << impl->filename << " is missed 'graph' section");
    }

    impl->objects = reader.get<ReaderCollection>("objects");

    parse_properties(reader.get<ReaderMapping>("head"));

    std::string intro_story;
    std::string end_story;

    if (reader.read("intro-story", intro_story))
    {
      impl->intro_story = ReaderDocument::from_file(Pathname(intro_story, Pathname::DATA_PATH).get_sys_path());
    }

    if (reader.read("end-story", end_story))
    {
      impl->end_story = ReaderDocument::from_file(Pathname(end_story, Pathname::DATA_PATH).get_sys_path());
    }
  }
}

void
PingusWorldmap::parse_properties(const ReaderMapping& reader)
{
  reader.read("music",  impl->music);
  reader.read("author", impl->author);
  reader.read("name",   impl->name);
  reader.read("short-name", impl->short_name);
  reader.read("email",  impl->email);
  reader.read("width",     impl->width);
  reader.read("height",    impl->height);

  reader.read("default-node", impl->default_node);
  reader.read("final-node",   impl->final_node);
}

std::string
PingusWorldmap::get_name() const
{
  return impl->name;
}

std::string
PingusWorldmap::get_short_name() const
{
  return impl->short_name;
}

std::string
PingusWorldmap::get_description() const
{
  return impl->description;
}

std::string
PingusWorldmap::get_music() const
{
  return impl->music;
}

std::string
PingusWorldmap::get_author() const
{
  return impl->author;
}

std::string
PingusWorldmap::get_email() const
{
  return impl->email;
}

int
PingusWorldmap::get_width() const
{
  return impl->width;
}

int
PingusWorldmap::get_height() const
{
  return impl->height;
}

std::string
PingusWorldmap::get_default_node() const
{
  return impl->default_node;
}

std::string
PingusWorldmap::get_final_node() const
{
  return impl->final_node;
}

ReaderMapping const&
PingusWorldmap::get_graph() const
{
  return impl->path_graph;
}

ReaderCollection const&
PingusWorldmap::get_objects() const
{
  return impl->objects;
}

ReaderDocument const&
PingusWorldmap::get_intro_story() const
{
  return impl->intro_story;
}

ReaderDocument const&
PingusWorldmap::get_end_story() const
{
  return impl->end_story;
}

/* EOF */
