//  $Id: particle_cache.cxx,v 1.4 2002/09/28 11:52:25 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include "pingu_particle.hxx"
#include "particle_cache.hxx"

ParticleCache::ParticleCache()
{
  allocate(10000);
  position = particles.begin();
}

ParticleCache::~ParticleCache()
{
  std::cout << "ParticleCache:~ParticleCache" << std::endl;
  clear();
}

Particle* 
ParticleCache::create()
{
  if (position != particles.end())
    {
      std::vector<Particle*>::iterator particle = position;
      
      ++position;
      return *particle;
    }
  else 
    {
      std::vector<Particle*>::iterator particle = position;
      
      std::cout << "Out of particles" << std::endl;
      particles.push_back(new PinguParticle());
      ++position;

      return *particle;
    }
}

void
ParticleCache::allocate(int num_objs)
{
  for(int i=0; i < num_objs; ++i)
    {
      particles.push_back(new PinguParticle());
    }
}

void
ParticleCache::clear()
{
  for(std::vector<Particle*>::iterator i = particles.begin(); i != particles.end(); ++i)
    {
      delete (*i);
    }  
}

/* EOF */
