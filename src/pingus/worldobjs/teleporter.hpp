// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_TELEPORTER_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_TELEPORTER_HPP

#include "math/vector2f.hpp"
#include "pingus/worldobj.hpp"

namespace pingus::worldobjs {

class TeleporterTarget;

class Teleporter : public WorldObj
{
private:
  Vector2f pos;
  float m_z_index;
  Sprite sprite;
  std::string target_id;
  TeleporterTarget* target;

public:
  Teleporter(ReaderMapping const& reader);

  void  draw(SceneContext& gc) override;
  void  update() override;

  float z_index() const override;
  void set_z_index(float z_index) override { m_z_index = z_index; }
  void set_pos(Vector2f const& p) override { pos = p; }
  Vector2f get_pos() const override { return pos; }

  void  on_startup() override;

private:
  Teleporter (Teleporter const&);
  Teleporter& operator= (Teleporter const&);
};

} // namespace pingus::worldobjs

#endif

/* EOF */
