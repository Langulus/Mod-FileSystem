///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"
#include <Flow/Factory.hpp>
#include <Flow/Verbs/Create.hpp>
#include <Flow/Verbs/Select.hpp>


///                                                                           
///   A folder                                                                
///                                                                           
struct Folder final : A::Folder, ProducedFrom<FileSystem> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) FileSystem;
   LANGULUS_BASES(A::Folder);
   LANGULUS_VERBS(Verbs::Create, Verbs::Select);

private:
   Path mFolderPath;

   // Information about the folder, if it exists                        
   PHYSFS_Stat mFolderInfo {};

public:
   Folder(FileSystem*, const Neat&);

   void Detach();
   void Refresh();
   void Create(Verb&);
   void Select(Verb&);

   NOD() Ref<A::File>   RelativeFile(const Path&) const;
   NOD() Ref<A::Folder> RelativeFolder(const Path&) const;
};