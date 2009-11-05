//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "sound/sound_res_mgr.hpp"

#include "pingus/path_manager.hpp"
#include "pingus/globals.hpp"
#include "pingus/debug.hpp"

SoundResMgr::SoundMap SoundResMgr::sound_map;

SoundHandle
SoundResMgr::load(const std::string& name)
{
  SoundMap::iterator i = sound_map.find(name);

  if (i == sound_map.end())
    {
      std::string filename = path_manager.complete("sounds/" + name + ".wav");
      Mix_Chunk* chunk = Mix_LoadWAV(filename.c_str());
      pout(PINGUS_DEBUG_LOADING) << "SoundResMgr: Loading sound from disk: "
                                 << name << " -> " << filename << std::endl;
      if (!chunk)
        pout(PINGUS_DEBUG_LOADING) << "Error: " << Mix_GetError() << std::endl;

      sound_map[name] = chunk;
      return chunk;
    }
  else
    {
      pout(PINGUS_DEBUG_LOADING) << "SoundResMgr: Loading sound from cache: " << name << std::endl;
      return i->second;
    }

  return 0;
}

void SoundResMgr::free_sound_map()
{
  for (SoundMap::iterator i = sound_map.begin(); i != sound_map.end(); ++i)
    {
      Mix_FreeChunk(i->second);
    }
}

/* EOF */
